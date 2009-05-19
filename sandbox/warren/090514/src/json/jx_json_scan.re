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

#include <stdio.h>
#include <string.h>

#include "jx_json_private.h"

#define jx_os_read read
#define jx_os_sscanf sscanf
#define jx_os_fprintf fprintf
#define jx_os_strncpy strncpy

typedef jx_uint32 jx_size;
typedef jx_uint32 jx_uword;
typedef jx_int32  jx_word;

#define JX_JSON_SCANNER_MODE_STDIN  0
#define JX_JSON_SCANNER_MODE_STRING 1

typedef struct {
  jx_char *bot, *tok, *ptr, *cur, *pos, *lim, *top, *eof;
  jx_word line;
  jx_int mode; 
  jx_json_parse_context context;
  jx_int n_tok_parsed;
} jx_json_scanner_state;

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
static void *jx__json_alloc(jx_size bytes)
{
  jx_char *ptr = NULL;
  ptr = jx_calloc(1,bytes);
  return (void*)ptr;
}

static void jx__json_free(void *ptr)
{
  if(ptr) 
    jx_free(ptr);
}

static jx_size jx_read_stdin(jx_char *buf, jx_size buf_size)
{
  return fread((char*)buf, 1, buf_size, stdin);
}

static jx_char *jx_fill(jx_json_scanner_state *s, jx_char *cursor)
{
  switch(s->mode) {
  case JX_JSON_SCANNER_MODE_STDIN:
    if(!s->eof) {  
      jx_uword cnt = s->tok - s->bot; /* amount of open space in buffer */
      if(cnt) {
        if(s->lim > s->tok) { /* any remaining characters? */
          memcpy(s->bot, s->tok, s->lim - s->tok); /* move to start of buffer */
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
          memcpy(buf, s->tok, s->lim - s->tok);
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
        cnt = jx_read_stdin(s->lim, BSIZE);
        if(cnt != BSIZE) {
          jx_size cc = cnt;
          while(cc<BSIZE) {
            s->lim[cc] = 0;
            cc++;
          }
          s->eof = &s->lim[cnt]; 
          if(cnt) { 
            *(s->eof)++ = '\n';
          } else {
            *(s->eof)++ = 0;
          }
        }
        s->lim += cnt;
      }
    }
    break;
  }
  return cursor;
}
  
static int jx_scan(jx_json_scanner_state *s)
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
    LP  = L (L|D)*;
    H   = [a-fA-F0-9];
    E   = [Ee] [+-]? D+;
    FS   = [fFlL];
    IS   = [uUlL]*;
    ESC  = [\\] ([abfnrtv?'"\\] | "x" H+ | O+);

    ([+\-]? D+ E FS?) | ([+\-]? D* "." D+ E? FS?) | ([+\-]? D+ "." D* E? FS?)  { RET(JX_JSON_FCON); }

    (["] (ESC|any\[\n\\"])* ["]) { RET(JX_JSON_SCON); }

    (['] (ESC|any\[\n\\'])* [']) { RET(JX_JSON_SCON); }
    
    "["         { RET(JX_JSON_OPEN_RECT_BRACE); }
    "]"         { RET(JX_JSON_CLOSE_RECT_BRACE); }

    "{"         { RET(JX_JSON_OPEN_CURLY_BRACE); }
    "}"         { RET(JX_JSON_CLOSE_CURLY_BRACE); }

    ","         { RET(JX_JSON_COMMA); }
    ":"         { RET(JX_JSON_COLON); }
    "true"      { RET(JX_JSON_TRUE); }
    "false"     { RET(JX_JSON_FALSE); }
    "null"      { RET(JX_JSON_NULL); }
    
    LP ("." LP)* { RET(JX_JSON_IDENT); }

    ("0" [xX] H+ IS?) | ("0" D+ IS?) | ([+\-]? D+ IS?) { RET(JX_JSON_ICON); }
    
    ";"         { RET(JX_JSON_SEMICOLON); }

    [ \t\v\f]+      { goto std; }
    
    null_char { 
      if(cursor == s->eof) RET(JX_JSON_EOI);
    }

    "#"         { goto comment; }

    "\n" {
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std;
    }
    
    any {
#ifdef JX_JSON_PARSER_DEBUG
      printf("unexpected character: %c\n", *s->tok);
#endif
       RET(JX_JSON_ERROR);
    }

  */

  /* not used:
    "("         { RET(JX_JSON_OPENPAR); }
    ")"         { RET(JX_JSON_CLOSEPAR); }
    "."         { RET(JX_JSON_DOT); }
    "`"         { RET(JX_JSON_BACKAPOSTROPHE); }

    "-"         { RET(JX_JSON_MINUS); }
    "+"         { RET(JX_JSON_PLUS); }
    (['] (ESC|any\[\n\\'])* [']) { RET(JX_JSON_SCON); }

    "="         { RET(JX_JSON_EQUALS); }
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

#define SSCANF_BUFSIZE 32

/* anticipated use cases:
   (1) parsing a string buffer from memory
   (2) parsing from a file or file descriptor (e.g. stdin) 
   (3) parsing from a character stream 
*/

void jx_json_scan_input(jx_json_scanner_state *state)
{
  /* allocate parser */

  void *jx_Parser = jx_json_Alloc( (void *(*)(size_t))jx__json_alloc);

  jx_word tok_type;
  jx_char stack_buffer[SSCANF_BUFSIZE];
  
  state->context.status = 0;
  state->n_tok_parsed = 0;
  while(!state->context.exhausted) {
    tok_type = jx_scan(state);
    {
      jx_ob token = JX_OB_NULL;
      jx_size st_len = state->cur - state->tok;
#ifdef JX_JSON_PARSER_DEBUG

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
      case JX_JSON_ICON:
      case JX_JSON_FCON:
      case JX_JSON_SCON:
      case JX_JSON_IDENT:
        {
          char *buffer = stack_buffer;
          if(st_len >= SSCANF_BUFSIZE) {
            buffer = jx_malloc(st_len+1);
          }
          if(buffer) {
            jx_os_strncpy(buffer, state->tok, st_len);                              
            buffer[st_len] = 0;
            switch(tok_type) {
            case JX_JSON_ICON:
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
            case JX_JSON_FCON:
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
            case JX_JSON_SCON:
              buffer[st_len-1]=0;
              token = jx_ob_from_str(buffer+1);
              break;
            case JX_JSON_IDENT:
              token = jx_ob_from_ident(buffer);
              break;
            }
            if(buffer != stack_buffer)
              jx_free(buffer);
          }
          break;
        case JX_JSON_TRUE:
          token = jx_ob_from_bool(true);
          break;
        case JX_JSON_FALSE:
          token = jx_ob_from_bool(false);
          break;
        case JX_JSON_NULL:
          token = jx_ob_from_null();
          break;
        case JX_JSON_OPEN_RECT_BRACE:
        case JX_JSON_CLOSE_RECT_BRACE:
        case JX_JSON_OPEN_CURLY_BRACE:
        case JX_JSON_CLOSE_CURLY_BRACE:
        case JX_JSON_COMMA:
        case JX_JSON_EOI:
        case JX_JSON_SEMICOLON: 
          /* do nothing */
          break;
          break;
        case JX_JSON_ERROR:
          state->context.status = JX_FAILURE;
          break;
        }
      }
      jx_json_(jx_Parser, (int)tok_type, token, &state->context);

      if(!jx_ok(state->context.status)) /* something bad happened */
        break;

      switch(tok_type) {
      case JX_JSON_SEMICOLON:
      case JX_JSON_EOI:
        {/* parse a null token to enable acceptance */
          jx_ob ob = JX_OB_NULL;
          jx_json_(jx_Parser, 0, ob, &state->context);
        }
        break;
      }

      if(state->context.status) /* accepted or complete */
        break;
      else
        state->n_tok_parsed++;
    }
  }
  
  if(tok_type == JX_JSON_EOI) {
    state->context.exhausted = JX_TRUE;
  }
  
  /* free the parser instance */
  jx_json_Free(jx_Parser, jx__json_free);
}

static jx_status jx_json_scanner_state_init(jx_json_scanner_state *state)
{
  memset(state,0,sizeof(jx_json_scanner_state));
  return JX_SUCCESS;
}

static jx_status jx_json_scanner_state_purge(jx_json_scanner_state *state)
{
  /* free last object */
  jx_ob_free(state->context.result);
  /* free the input buffer (if using stdin) */
  switch(state->mode) {
  case JX_JSON_SCANNER_MODE_STDIN:
    if(state->bot) 
      jx_free(state->bot);
    break;
  }
  return JX_SUCCESS;
}

jx_ob jx_ob_from_json_str(jx_char *st)
{
  jx_json_scanner_state state;
  jx_json_scanner_state_init(&state);
  jx_ob result = JX_OB_NULL;
  state.cur = st;
  state.eof = st + strlen(st) + 1;
  state.mode = JX_JSON_SCANNER_MODE_STRING;
  jx_json_scan_input(&state);
  if(state.context.status == JX_YES) {
    if(state.n_tok_parsed) /* non-blank string? */
      result = state.context.result;
    state.context.result = jx_ob_from_null();
  }
  jx_json_scanner_state_purge(&state);
  return result;
}

void jx_json_echo_stdin(void)
{
  jx_json_scanner_state state;
  jx_json_scanner_state_init(&state);
  while(1) {    
    jx_json_scan_input(&state);
    switch(state.context.status) {
    case JX_FAILURE:
      printf("syntax error.\n");
      break;
    case JX_YES: /* accepted */
      {
        jx_ob json = jx_ob_to_json(state.context.result);
        if(state.n_tok_parsed)
          printf("%s;\n",jx_ob_as_str(&json));
        jx_ob_free(json);
      }
      state.context.result = jx_null_with_ob( state.context.result);
      break;
    }
    if(state.context.exhausted)
      break;
  }
  jx_json_scanner_state_purge(&state);
}
