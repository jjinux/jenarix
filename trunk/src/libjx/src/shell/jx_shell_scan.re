/* RE2C-based tokenizer */

/* 
Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

* Neither the name of the DeLano Scientific LLC nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "jx_shell_private.h"

#define JX_SHELL_SCANNER_MODE_FILE    0 
#define JX_SHELL_SCANNER_MODE_STRING  1
#define JX_SHELL_SCANNER_MODE_CONSOLE 2

typedef struct {
  jx_char *bot, *tok, *ptr, *cur, *pos, *lim, *top, *eof;
  jx_int line;
  jx_int n_tok_parsed;

  jx_int mode;
  FILE *file;
  jx_shell_parse_context context;
} jx_shell_scanner_state;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define   BSIZE   8192

#define   YYCTYPE     jx_char
#define   YYCURSOR    cursor
#define   YYLIMIT     s->lim
#define   YYMARKER    s->ptr
#define   YYFILL(n)   {cursor = jx_fill(s, cursor);}

#define   RET(i)   {s->cur = cursor; return i;}

/* parser allocator */

static void *jx__jxon_alloc(jx_size bytes)
{
  jx_char *ptr = NULL;
  ptr = jx_calloc(1,bytes);
  return (void*)ptr;
}

static void jx__jxon_free(void *ptr)
{
  if(ptr) 
    jx_free(ptr);
}

static jx_size jx_read_file(FILE *file, jx_char *buf, jx_size buf_size, jx_int mode)
{
  switch(mode) {
  case JX_SHELL_SCANNER_MODE_FILE:
    return fread((char*)buf, 1, buf_size, stdin);
    break;
  case JX_SHELL_SCANNER_MODE_CONSOLE:
    if(fgets((char*)buf, buf_size, stdin)) {
      return jx_strlen(buf);
    } else 
      return 0;
    break;
  }
  return 0;
}

static jx_char *jx_fill(jx_shell_scanner_state *s, jx_char *cursor)
{
  switch(s->mode) {
  case JX_SHELL_SCANNER_MODE_FILE:
  case JX_SHELL_SCANNER_MODE_CONSOLE:
    if(!s->eof) { /* only fill if eof is not set (known) */
      jx_diff cnt = s->tok - s->bot; /* amount of open space in buffer */
      if(cnt) {
        if(s->lim > s->tok) { /* any remaining characters? */
          jx_os_memcpy(s->bot, s->tok, s->lim - s->tok); /* move to start of buffer */
        }
        s->tok = s->bot;  /* shift pointers accordingly */
        s->ptr -= cnt; 
        cursor -= cnt;
        s->pos -= cnt;
        s->lim -= cnt;
      }
      if((s->top - s->lim) < BSIZE) { /* open space less than read quantum? */
        /* allocate a larger buffer */
        jx_char *buf = (jx_char*) jx_calloc(1,((s->lim - s->bot) +
                                               BSIZE)*sizeof(jx_char));
        if(buf) { /* and copy contents to new buffer */
          jx_os_memcpy(buf, s->tok, s->lim - s->tok);
          s->tok = buf; /* adjust pointers accordingly */
          s->ptr = &buf[s->ptr - s->bot];
          cursor = &buf[cursor - s->bot];
          s->pos = &buf[s->pos - s->bot];
          s->lim = &buf[s->lim - s->bot];
          s->top = &s->lim[BSIZE];
          if(s->bot) { /* free old buffer (if exists) */
            jx_free(s->bot);
          }
          s->bot = buf; /* use the new buffer from now on */
        }
      }
      if((s->top - s->lim) < BSIZE) {
        /* unhandled error condition */
      } else {
        cnt = jx_read_file(s->file, s->lim, BSIZE, s->mode);
        if(cnt != BSIZE) {
          jx_size cc = cnt;
          while(cc<BSIZE) {
            s->lim[cc] = 0;
            cc++;
          }
          s->eof = &s->lim[cnt]; 
          if(cnt) { /* short read ...there may be more to come */
            s->eof++; 
          } else { /* nothing read, so set the null sentinel */
            *(s->eof) = 0;
          }
        }
        s->lim += cnt;
      }
    }
    break;
  }
  return cursor;
}
  
static int jx_scan(jx_shell_scanner_state *s)
{
  jx_char *cursor = s->cur;
 std:
  s->tok = cursor;
  
  /*!re2c

    null_char = [\000];
    any  = [\001-\377];
    O   = [0-7];
    D   = [0-9];
    L   = [a-zA-Z_];
    LD  = L (L|D)*;
    H   = [a-fA-F0-9];
    E   = [Ee] [+-]? D+;
    FS   = [fFlL];
    IS   = [uUlL]*;
    ESC  = [\\] ([abfnrtv?'"\\] | "x" H+ | O+);

    ([+\-]? D+ E FS?) | ([+\-]? D* "." D+ E? FS?) | ([+\-]? D+ "." D* E? FS?)  { RET(JX_SHELL_FCON); }

    (["] (ESC|any\[\n\\"])* ["]) { RET(JX_SHELL_SCON); }

    (['] (ESC|any\[\n\\'])* [']) { RET(JX_SHELL_SCON); }

    "`" (L|D|":")+  { RET(JX_SHELL_BUILTIN); }
    
    "["         { RET(JX_SHELL_OPEN_RECT_BRACE); }
    "]"         { RET(JX_SHELL_CLOSE_RECT_BRACE); }

    "{"         { RET(JX_SHELL_OPEN_CURLY_BRACE); }
    "}"         { RET(JX_SHELL_CLOSE_CURLY_BRACE); }
    ":"         { RET(JX_SHELL_COLON); }

    "="         { RET(JX_SHELL_EQUALS); }

    "("         { RET(JX_SHELL_OPEN_PAR); }
    ")"         { RET(JX_SHELL_CLOSE_PAR); }

    ","         { RET(JX_SHELL_COMMA); }
    "true"      { RET(JX_SHELL_TRUE); }
    "false"     { RET(JX_SHELL_FALSE); }
    "null"      { RET(JX_SHELL_NULL); }
    
    LD ("." LD)* { RET(JX_SHELL_IDENT); }

    ("0" [xX] H+ IS?) | ("0" D+ IS?) | ([+\-]? D+ IS?) { RET(JX_SHELL_ICON); }
    
    ";"         { RET(JX_SHELL_SEMICOLON); }

    [ \t\v\f]+      { goto std; }

    "#"         { goto comment; }

    "\n"  { 
      s->line++;
      s->eof = NULL;
      RET(JX_SHELL_EOL);
      }


    null_char { 
      if(cursor == s->eof) RET(JX_SHELL_EOI);
      RET(JX_SHELL_EOI); 
    }
    
    any {
#ifdef JX_SHELL_PARSER_DEBUG
      printf("unexpected character: %c\n", *s->tok);
#endif
       RET(JX_SHELL_ERROR);
    }

  */

comment:
  /*!re2c
    "\n" 
    { 
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std; 
    }

    any 
    { goto comment; }
*/
  
}


  /* not used:


    "."         { RET(JX_SHELL_DOT); }
    "`"         { RET(JX_SHELL_BACKAPOSTROPHE); }

    "<"         { RET(JX_SHELL_LESS_THAN); }
    "<="        { RET(JX_SHELL_LESS_THAN_OR_EQ); }
    ">"         { RET(JX_SHELL_GREATER_THAN); }
    ">="        { RET(JX_SHELL_GREATER_THAN_OR_EQ); }
    "-"         { RET(JX_SHELL_MINUS); }
    "+"         { RET(JX_SHELL_PLUS); }
    (['] (ESC|any\[\n\\'])* [']) { RET(JX_SHELL_SCON); }

   */
#define SSCANF_BUFSIZE 32

/* anticipated use cases:
   (1) parsing a string buffer from memory
   (2) parsing from a file or file descriptor (e.g. stdin) 
   (3) parsing from a character stream 
*/

static void jx_shell_scan_input(jx_shell_scanner_state *state)
{
  /* allocate parser */

  void *jx_Parser = jx_shell_Alloc( (void *(*)(size_t))jx__jxon_alloc);
  jx_int tok_type = 0;
  jx_char stack_buffer[SSCANF_BUFSIZE];
  
  state->context.status = 0;
  state->n_tok_parsed = 0;

  while(!state->context.exhausted) {
    tok_type = jx_scan(state);
    {
      jx_ob token = JX_OB_NULL;
      jx_size st_len = state->cur - state->tok;
      jx_bool skip = JX_FALSE;
#ifdef JX_SHELL_PARSER_DEBUG

      if(1) {
        jx_size i;
        jx_char *c = state->tok;
        
        if(st_len) {
          printf(" ");
          for(i=0;i<st_len;i++) {
            printf("%c",*(c++));
          }
          printf("\n");
        }
      }
#endif

      switch(tok_type) {
      case JX_SHELL_ICON:
      case JX_SHELL_FCON:
      case JX_SHELL_SCON:
      case JX_SHELL_IDENT:
        {
          char *buffer = stack_buffer;
          if(st_len >= SSCANF_BUFSIZE) {
            buffer = jx_malloc(st_len+1);
          }
          if(buffer) {
            jx_os_strncpy(buffer, state->tok, st_len);                              
            buffer[st_len] = 0;
            switch(tok_type) {
            case JX_SHELL_ICON:
              {
#ifdef JX_64_BIT
                jx_int icon;
                if( jx_os_sscanf(buffer, "%lli", &icon) != 1) { /* use strtol instead? */
                  icon = 0;
                }
                token = jx_ob_from_int(icon);            
#else
                int icon;
                if( jx_os_sscanf(buffer, "%i", &icon) != 1) { /* use strtol instead? */
                  icon = 0;
                }
#endif
                token = jx_ob_from_int(icon);            
              }
              break;
            case JX_SHELL_FCON:
              {
#ifdef JX_64_BIT
                double fcon;
                if( jx_os_sscanf(buffer, "%lf", &fcon) != 1) { /* use strtof instead? */
                  fcon = 0.0;
                }
#else
                float fcon; 
                if( jx_os_sscanf(buffer, "%f", &fcon) != 1) { /* use strtof instead? */
                  fcon = 0.0F;
                }
#endif
                token = jx_ob_from_float(fcon);
              }
              break;
            case JX_SHELL_SCON:
              buffer[st_len-1]=0;
              token = jx_ob_from_str(buffer+1);
              break;
            case JX_SHELL_IDENT:
              token = jx_ob_from_ident(buffer);
              break;
            }
            if(buffer != stack_buffer)
              jx_free(buffer);
          }
          break;
        case JX_SHELL_TRUE:
          token = jx_ob_from_bool(true);
          break;
        case JX_SHELL_FALSE:
          token = jx_ob_from_bool(false);
          break;
        case JX_SHELL_NULL:
          token = jx_ob_from_null();
          break;
        case JX_SHELL_BUILTIN:
          token = jx_builtin_new_from_selector(0);
          break;
        case JX_SHELL_EOL:
          {
            jx_ob ob = JX_OB_NULL;
            jx_shell_(jx_Parser, JX_SHELL_EOI, ob, &state->context);
            state->n_tok_parsed++;
          }
          break;
        case JX_SHELL_OPEN_RECT_BRACE:
        case JX_SHELL_CLOSE_RECT_BRACE:
        case JX_SHELL_OPEN_CURLY_BRACE:
        case JX_SHELL_CLOSE_CURLY_BRACE:
          /* do nothing */
          break;
        case JX_SHELL_EOI:
          break;
        case JX_SHELL_ERROR:
          state->context.status = JX_STATUS_SYNTAX_ERROR;
          break;
        case JX_SHELL_COMMA:
          break;
        case JX_SHELL_SEMICOLON:
          break;
        }
      }
      if(!skip) {
        jx_shell_(jx_Parser, (int)tok_type, token, &state->context);
        
        if(!jx_ok(state->context.status)) /* something bad happened */
          break;
        
        switch(tok_type) {
        case JX_SHELL_EOI:
          {/* parse a null token to enable acceptance */
            jx_ob ob = JX_OB_NULL;
            jx_shell_(jx_Parser, 0, ob, &state->context);
          }
          break;
        }
      }
      
      if(state->context.status) /* accepted or complete */
        break;
      else
        state->n_tok_parsed++;
    }
  }
  
  if(tok_type == JX_SHELL_EOI) {
    state->context.exhausted = JX_TRUE;
  }
  
  /* free the parser instance */
  jx_shell_Free(jx_Parser, jx__jxon_free);
}

static jx_status jx_shell_scanner_state_init(jx_shell_scanner_state *state)
{
  memset(state,0,sizeof(jx_shell_scanner_state));
  return JX_SUCCESS;
}

static jx_status jx_shell_scanner_state_reset(jx_shell_scanner_state *state)
{
  if(state->bot) 
    jx_free(state->bot);
  state->bot = NULL;
  state->tok = NULL;
  state->ptr = NULL;
  state->cur = NULL;
  state->pos = NULL;
  state->lim = NULL;
  state->top = NULL;
  state->eof = NULL;
  state->line = 0;
  state->n_tok_parsed = 0;
  return JX_SUCCESS;
}

static jx_status jx_shell_scanner_state_purge(jx_shell_scanner_state *state)
{
  /* free last object */
  jx_ob_free(state->context.result);
  /* free the input buffer (if using stdin) */
  switch(state->mode) {
  case JX_SHELL_SCANNER_MODE_FILE:
  case JX_SHELL_SCANNER_MODE_CONSOLE:
    if(state->bot) 
      jx_free(state->bot);
    break;
  }
  return JX_SUCCESS;
}

void jx_shell_echo_stdin(void)
{
  jx_shell_scanner_state state;
  jx_shell_scanner_state_init(&state);
  jx_bool done = JX_FALSE;
  while(!done) {    
    jx_shell_scan_input(&state);
    switch(state.context.status) {
    case JX_STATUS_SYNTAX_ERROR:
      printf("syntax error.\n");
      break;
    case JX_YES: /* accepted */
      {
        jx_ob jxon = jx_ob_to_jxon(state.context.result);
        if(state.n_tok_parsed)
          printf("%s;\n",jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      state.context.result = jx_null_with_ob(state.context.result);
      break;
    case JX_FAILURE:
      done = JX_TRUE;
      break;
    }
    if(state.context.exhausted)
      done = JX_TRUE;
  }
  jx_shell_scanner_state_purge(&state);
}

#define JX_SHELL_SCANNER_MAGIC "jxJSc"

typedef struct {
  jx_opaque_ob opaque;
  jx_shell_scanner_state state;
} jx_shell_scanner;

static jx_status jx_shell_scanner_free(jx_shell_scanner *scanner) 
{
  if(jx_ob_equal(scanner->opaque.magic,
                 jx_ob_from_str(JX_SHELL_SCANNER_MAGIC))) {
    jx_shell_scanner_state_purge(&scanner->state);
    jx_ob_free(scanner->opaque.magic);
    jx_free(scanner);
    return JX_SUCCESS;
  }
  return JX_FAILURE;
}

jx_ob jx_shell_scanner_new_with_file(FILE *file)
{
  jx_ob result = JX_OB_NULL;
  jx_shell_scanner *scanner = (jx_shell_scanner*)jx_calloc(1,sizeof(jx_shell_scanner));
  if(scanner) {
    if(jx_ok(jx_shell_scanner_state_init(&scanner->state))) {
      scanner->opaque.magic = jx_ob_from_str(JX_SHELL_SCANNER_MAGIC);
      scanner->opaque.free_fn = (jx_opaque_free_fn)jx_shell_scanner_free;
      scanner->state.file = file;
      if (isatty((int)fileno(file))) {      
        scanner->state.mode = JX_SHELL_SCANNER_MODE_CONSOLE;
      } else {
        scanner->state.mode = JX_SHELL_SCANNER_MODE_FILE;
      }
      result = jx_builtin_new_with_opaque_ob(&scanner->opaque);
    } else 
      jx_free(scanner);
  }
  return result;
}

jx_ob jx_shell_scanner_get_error_message(jx_ob scanner_ob)
{
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_shell_scanner *scanner = (jx_shell_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,
                   jx_ob_from_str(JX_SHELL_SCANNER_MAGIC))) {
      jx_shell_scanner_state *state = &scanner->state;
      jx_ob list = jx_list_new();
      {
        jx_char buffer[50];
        sprintf(buffer,"Error: invalid syntax on line %d\n",state->line+1);
        jx_list_append(list, jx_ob_from_str(buffer));
      }
      if(state->bot != state->lim) {
        if(state->tok > state->pos) {
          jx_list_append(list, jx_ob_from_str_with_len(state->pos, state->tok - state->pos));
        } else if(state->tok > state->bot) {
          jx_list_append(list, jx_ob_from_str_with_len(state->bot, state->tok - state->bot));
        }
        if((state->cur > state->tok) && state->tok && state->tok[0]) {
          jx_list_append(list, jx_ob_from_str(" -->"));
          jx_list_append(list, jx_ob_from_str_with_len(state->tok, state->cur - state->tok));
          jx_list_append(list, jx_ob_from_str("<-- "));
          if(state->lim > state->cur) {
            jx_char *ch = state->cur;
            while( (ch<state->lim) && (*ch!='\n') &&(*ch!='\r'))
              ch++;
            jx_list_append(list, jx_ob_from_str_with_len(state->cur, ch - state->cur));
          }
        } else {
          jx_list_append(list, jx_ob_from_str("<-- (at end of input)"));
        }
      } else {
        jx_list_append(list, jx_ob_from_str("<-- (at end of input)"));
      }
      return jx_str_join_with_list(list);
    }
  }
  return jx_ob_from_null();
}

jx_status jx_shell_scanner_next_ob(jx_ob *result, jx_ob scanner_ob)
{
  jx_status status = JX_FAILURE;
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_shell_scanner *scanner = (jx_shell_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,
                   jx_ob_from_str(JX_SHELL_SCANNER_MAGIC))) {
      jx_shell_scanner_state *state = &scanner->state;
      state->context.status = JX_SUCCESS;
      while(!state->context.status) {    
        jx_shell_scan_input(state);
        switch(state->context.status) {
        case JX_YES: /* accept called */
          jx_ob_replace(result, state->context.result);
          state->context.result = jx_ob_from_null();
          status = state->n_tok_parsed ? JX_YES : JX_NO; /* but were any tokens parsed */
          break;
        default:
          status = state->context.status;
          break;
        }
        if(state->context.exhausted) {
          if(!status) 
            status = JX_STATUS_EXHAUSTED;
          break;
        }
      }
    }
  }
  return status;
}

jx_status jx_shell_scanner_purge_input(jx_ob scanner_ob)
{
  jx_status status = JX_FAILURE;
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_shell_scanner *scanner = (jx_shell_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,
                   jx_ob_from_str(JX_SHELL_SCANNER_MAGIC))) {
      jx_shell_scanner_state *state = &scanner->state;
      jx_shell_scanner_state_reset(state);
      status = JX_SUCCESS;
    }
  }
  return status;
}
