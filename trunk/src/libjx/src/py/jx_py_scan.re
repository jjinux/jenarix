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

#include "jx_py_private.h"

#define JX_PY_SCANNER_MODE_FILE    0 
#define JX_PY_SCANNER_MODE_STRING  1
#define JX_PY_SCANNER_MODE_CONSOLE 2

typedef struct {
  jx_char *bot, *tok, *ptr, *cur, *pos, *lim, *top, *eof;
  jx_int line;
  jx_int n_tok_parsed;
  jx_int mode,flags;
  FILE *file;
  jx_py_parse_context context;
  jx_ob indent_stack;
  jx_bool ignore_whitesp;
  jx_int current_indent;
  jx_bool newline_just_seen;
  jx_bool colon_just_seen;
  jx_bool saved_token;
} jx_py_scanner_state;

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

#ifndef JX_PY_EOI
#define JX_PY_EOI -1
#endif

#ifndef JX_PY_DEDENT_NEWLINE
#define JX_PY_DEDENT_NEWLINE -2
#endif

/* parser allocator */

static void *jx__py_alloc(jx_size bytes)
{
  jx_char *ptr = NULL;
  ptr = jx_calloc(1,bytes);
  return (void*)ptr;
}

static void jx__py_free(void *ptr)
{
  if(ptr) 
    jx_free(ptr);
}

static jx_size jx_read_file(FILE *file, jx_char *buf, jx_size buf_size, jx_int mode)
{
  switch(mode) {
  case JX_PY_SCANNER_MODE_FILE:
    return fread((char*)buf, 1, buf_size, file);
    break;
  case JX_PY_SCANNER_MODE_CONSOLE:
    if(fgets((char*)buf, buf_size, file)) {
      return jx_strlen(buf);
    } else 
      return 0;
    break;
  }
  return 0;
}

static jx_char *jx_fill(jx_py_scanner_state *s, jx_char *cursor)
{
  switch(s->mode) {
  case JX_PY_SCANNER_MODE_FILE:
  case JX_PY_SCANNER_MODE_CONSOLE:
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
  
static int jx_scan(jx_py_scanner_state *s)
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

    "if"     { RET(JX_PY_IF); }
    "elif"   { RET(JX_PY_ELIF); }
    "else"   { RET(JX_PY_ELSE); }
    "in"     { RET(JX_PY_IN);}
    "not"    { RET(JX_PY_NOT);}
    "print"  { RET(JX_PY_PRINT); }
    "for"    { RET(JX_PY_FOR); }
    "while"  { RET(JX_PY_WHILE); }
    "del"    { RET(JX_PY_DEL); }
    "pass"   { RET(JX_PY_PASS); }
    "def"    { RET(JX_PY_DEF); }
    "lambda" { RET(JX_PY_LAMBDA);}
    "return" { RET(JX_PY_RETURN); }
    "class"  { RET(JX_PY_CLASS); }

    ("0" [xX] H+ IS?) | ("0" D+ IS?) | ( D+ IS?) { RET(JX_PY_ICON); }
    
    (D+ E FS?) | ( D* "." D+ E? FS?) | (D+ "." D* E? FS?)  { RET(JX_PY_FCON); }

    (["] (ESC|any\[\n\\"])* ["]) { RET(JX_PY_SCON); }

    (['] (ESC|any\[\n\\'])* [']) { RET(JX_PY_SCON); }

    LD ("." "."? LD)* "."? { RET(JX_PY_NAME); }

    "["         { s->ignore_whitesp++; RET(JX_PY_OPEN_RECT_BRACE); }
    "]"         { s->ignore_whitesp--; RET(JX_PY_CLOSE_RECT_BRACE); }

    "("         { s->ignore_whitesp++; RET(JX_PY_OPEN_PAR); }
    ")"         { s->ignore_whitesp--; RET(JX_PY_CLOSE_PAR); }

    "{"         {  s->ignore_whitesp++; RET(JX_PY_OPEN_CURLY_BRACE); }
    "}"         {  s->ignore_whitesp--; RET(JX_PY_CLOSE_CURLY_BRACE); }

    "~"         { RET(JX_PY_TILDE); }
    "+"         { RET(JX_PY_PLUS); }
    "-"         { RET(JX_PY_MINUS); }

    "**"        { RET(JX_PY_DOUBLE_ASTERISK); }
    "*"         { RET(JX_PY_ASTERISK); }
    "/"         { RET(JX_PY_SLASH); }
    "//"        { RET(JX_PY_SLASHSLASH); }
    "%"         { RET(JX_PY_PERCENT); }

    "<"         { RET(JX_PY_LESS_THAN); }
    "<="        { RET(JX_PY_LESS_THAN_OR_EQ); }
    "=="        { RET(JX_PY_EQ_EQ); }
    "!="        { RET(JX_PY_NOT_EQ); }
    "<>"        { RET(JX_PY_NOT_EQ); }
    ">"         { RET(JX_PY_GREATER_THAN); }
    ">="        { RET(JX_PY_GREATER_THAN_OR_EQ); }

    "="         { RET(JX_PY_EQUALS); }

    "+="        { RET(JX_PY_PLUS_EQUALS); }
    "-="        { RET(JX_PY_MINUS_EQUALS); }

    ":"         { RET(JX_PY_COLON); }

    ";"         { RET(JX_PY_SEMICOLON); }

    ","         { RET(JX_PY_COMMA); }

    "."         { RET(JX_PY_DOT); }

    [ \t\v\f]+   { if(s->pos == s->tok) 
                    s->current_indent = cursor - s->pos;
                  goto std; }

    "#"         { goto comment; }

    "\n" {
    s->line++;
    s->pos = cursor;
    s->eof = NULL;
    if(!s->ignore_whitesp) {
       RET(JX_PY_NEWLINE);
    } else {
       goto std;
    }
  }

    null_char { 
      if(cursor == s->eof) RET(JX_PY_EOI);
      RET(JX_PY_EOI); 
    }
    
    any {
#ifdef JX_PY_PARSER_DEBUG
      printf("unexpected character: %c\n", *s->tok);
#endif
       RET(JX_PY_ERROR);
    }

  */

comment:
  /*!re2c
    "\n" 
    { 
      if((s->flags & JX_SCANNER_FLAG_ECHO_COMMENTS) &&
         (cursor > s->tok)) {
         fwrite(s->tok,(cursor-s->tok),1,stdout);
       }
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std; 
    }

    any 
    { goto comment; }
*/
  
}


  /* not used:


#if 0
    "and"    { RET(JX_PY_AND); }
    "break"  { RET(JX_PY_BREAK); }

    "exec"   { RET(JX_PY_EXEC); }
    "from"   { RET(JX_PY_FROM); }
    "import" { RET(JX_PY_IMPORT); }
    "is"     { RET(JX_PY_IS);}
    "or"     { RET(JX_PY_OR);}
#endif

    ("0" [xX] H+ IS?) | ("0" D+ IS?) | ([+\-]? D+ IS?) { RET(JX_PY_ICON); }

    ([+\-]? D+ E FS?) | ([+\-]? D* "." D+ E? FS?) | ([+\-]? D+ "." D* E? FS?)  { RET(JX_PY_FCON); }

    "`" (L|D|":")+  { RET(JX_PY_BUILTIN); }
    

    "-="        { RET(JX_PY_MINUS_EQUALS); }
    "*="        { RET(JX_PY_ASTERISK_EQUALS); }
    "/="        { RET(JX_PY_SLASH_EQUALS); } 
    "%="        { RET(JX_PY_PERCENT_EQUALS); } 
    "&="        { RET(JX_PY_AND_EQUALS); } 
    "|="        { RET(JX_PY_OR_EQUALS); }
    "^="        { RET(JX_PY_CARAT_EQUALS); }
    "<<="       { RET(JX_PY_LSHFT_EQUALS); }
    ">>="       { RET(JX_PY_RSHFT_QUALS); }
    "**="       { RET(JX_PY_STARSTAR_EQUALS); } 
    "//="       { RET(JX_PY_SLASHSLASH_EQUALS); } 


    "true"      { RET(JX_PY_TRUE); }
    "false"     { RET(JX_PY_FALSE); }
    "null"      { RET(JX_PY_NULL); }
    
    "`"         { RET(JX_PY_BACKAPOSTROPHE); }



    "*"         { RET(JX_PY_ASTERISK); }
    "/"         { RET(JX_PY_SLASH); }
    "%"         { RET(JX_PY_PERCENT); }
    (['] (ESC|any\[\n\\'])* [']) { RET(JX_PY_SCON); }

   */
#define SSCANF_BUFSIZE 32

/* anticipated use cases:
   (1) parsing a string buffer from memory
   (2) parsing from a file or file descriptor (e.g. stdin) 
   (3) parsing from a character stream 
*/

JX_INLINE void jx_py_send(void *parser, int token, jx_ob ob, jx_py_parse_context *context)
{
#ifdef JX_PY_PARSER_DEBUG
  printf("scanner: sending %d\n",token);
#endif
  jx_py_(parser,token, ob, context);
}


static void jx_py_scan_input(jx_py_scanner_state *s)
{
  /* allocate parser */

  void *jx_Parser = jx_py_Alloc( (void *(*)(size_t))jx__py_alloc);
  jx_int tok_type = 0;
  jx_char stack_buffer[SSCANF_BUFSIZE];

  s->context.status = 0;
  s->n_tok_parsed = 0;
  
  while(!s->context.exhausted) {
    jx_bool skip_token = JX_FALSE;
    jx_bool indent_just_changed = JX_FALSE;
    if(!s->saved_token) 
      tok_type = jx_scan(s);
    else {
      tok_type = s->saved_token;
      s->saved_token = 0;
#ifdef JX_PY_PARSER_DEBUG
    printf("scanner: saved token index %d: \n",tok_type);
#endif
    }
#ifdef JX_PY_PARSER_DEBUG
    printf("scanner: token index %d: \n",tok_type);
    jx_jxon_dump(stdout,"scanner: indent_stack",s->indent_stack);
    printf("scanner: current_indent %d\n",s->current_indent);
    printf("scanner: newline_just_seen %d\n",s->newline_just_seen);
    printf("scanner: colon_just_seen %d\n",s->colon_just_seen);
    printf("scanner: expected indent %d\n",jx_ob_as_int(jx_list_borrow(s->indent_stack,-1)));
#endif
    /* Python / block indentation handling - UGH - COMPLEX!!! */

    if(s->newline_just_seen) { /* previous token was a newline... */
      jx_int expected_indent = jx_ob_as_int(jx_list_borrow(s->indent_stack,-1));
      switch(tok_type) {
      case JX_PY_NEWLINE:
        /* current token is also a newline --  what are the implications? */
        switch(s->mode) {
        case JX_PY_SCANNER_MODE_CONSOLE:
          { /* on the console, changes in indentation are always significant */
            if(!s->ignore_whitesp) {
              if(expected_indent != s->current_indent) {
                if(s->current_indent > expected_indent) {
                  jx_list_push(s->indent_stack,jx_ob_from_int(s->current_indent));
                  jx_py_send(jx_Parser, JX_PY_INDENT, jx_ob_from_null(),&s->context);
                  indent_just_changed = JX_TRUE;
		} else if(expected_indent > s->current_indent) {
		  while(expected_indent > s->current_indent) {
		    jx_list_pop(s->indent_stack);
		    expected_indent = jx_ob_as_int(jx_list_borrow(s->indent_stack,-1));
		    if(expected_indent < s->current_indent) {
		      s->context.status = JX_STATUS_SYNTAX_ERROR;
		      break;
		    } else {
		      jx_py_send(jx_Parser, JX_PY_DEDENT, jx_ob_from_null(),&s->context); 
		      jx_py_send(jx_Parser, JX_PY_NEWLINE, jx_ob_from_null(), &s->context);        
		      indent_just_changed = JX_TRUE;
		    }
		  }
                }
              }
            }
          }
          break;
        default:
          
          /* in files, changed to invisible indentation in front of
             newlines doesn't mean anything, so just ignore indentation for now */
          s->current_indent = 0; 
          break;
        }
        break;
      case JX_PY_EOI: /* end of input reached -- back-out the indents... */
        while(jx_list_size(s->indent_stack)) {
          jx_py_send(jx_Parser, JX_PY_DEDENT, jx_ob_from_null(), &s->context);        
          jx_py_send(jx_Parser, JX_PY_NEWLINE, jx_ob_from_null(), &s->context);        
          jx_list_pop(s->indent_stack);
          indent_just_changed = JX_TRUE;
        }
        break;
      default: 
        {
          /* current token not a newline, so this is for real in all cases  */
          if(!s->ignore_whitesp) {
            if(expected_indent != s->current_indent) {
              if(s->current_indent > expected_indent) {
                jx_list_push(s->indent_stack,jx_ob_from_int(s->current_indent));
                jx_py_send(jx_Parser, JX_PY_INDENT, jx_ob_from_null(), &s->context);
                indent_just_changed = JX_TRUE;
              } else {
                while(s->current_indent < expected_indent) {
                  jx_list_pop(s->indent_stack);
                  expected_indent = jx_ob_as_int(jx_list_borrow(s->indent_stack,-1));
                  jx_py_send(jx_Parser, JX_PY_DEDENT, jx_ob_from_null(), &s->context); 
                  switch(tok_type) {
                  case JX_PY_ELIF:
                  case JX_PY_ELSE:
                    break;
                  default:
                    jx_py_send(jx_Parser, JX_PY_NEWLINE, jx_ob_from_null(), &s->context);
                    break;
                  }
                  indent_just_changed = JX_TRUE;
                  if(expected_indent == s->current_indent) {
                    break;
                  }
                }
                if(expected_indent != s->current_indent) {
                  s->context.status = JX_STATUS_SYNTAX_ERROR;
                  break;
                } else if(!s->current_indent) {
                  switch(tok_type) {
                  case JX_PY_ELIF:
                  case JX_PY_ELSE:
                    break;
                  default:
                    jx_py_send(jx_Parser, JX_PY_NEWLINE, jx_ob_from_null(), &s->context); 
                    break;
                  }
                }
              }
            }
          }
        }
        break;
      }
    } else { /* previous token was not a newline */
      switch(tok_type) {
      case JX_PY_EOI: /* end of input reached -- back-out the indents... */
        while(jx_list_size(s->indent_stack)) {
          jx_py_send(jx_Parser, JX_PY_DEDENT, jx_ob_from_null(), &s->context);        
          jx_py_send(jx_Parser, JX_PY_NEWLINE, jx_ob_from_null(), &s->context);        
          jx_list_pop(s->indent_stack);
          indent_just_changed = JX_TRUE;
        }
        break;
      }
    }

    /* address newline subtleties... */

    if(tok_type == JX_PY_NEWLINE) {
      if(s->newline_just_seen && (!indent_just_changed) && (!s->colon_just_seen)) {
        /* suppress extra newlines which don't change the indent level */
        skip_token = true;
      } else if (!s->n_tok_parsed++) {
        /* skip leading newlines in parse block */
        skip_token = true;
      } else if((!s->colon_just_seen) && 
                (!jx_list_size(s->indent_stack) || (s->newline_just_seen))) {
        /* help parser along */                             
        jx_py_send(jx_Parser, JX_PY_NEWLINE, jx_ob_from_null(), &s->context);                
      }
    }
    s->newline_just_seen = (tok_type == JX_PY_NEWLINE); /* for next time */

    if(s->newline_just_seen)
      s->current_indent = 0;

    if(s->context.status) {/* accepted or complete */
      s->saved_token = tok_type;
      break;
    }

    {
      jx_ob token = JX_OB_NULL;
      jx_size st_len = s->cur - s->tok;
      
#ifdef JX_PY_PARSER_DEBUG
      {
        jx_size i;
        jx_char *c = s->tok;
        
        printf("scanner: token: '");
        if(st_len) {
          for(i=0;i<st_len;i++) {
            printf("%c",*(c++));
          }
        }
        printf("'\n");
      }
#endif

      switch(tok_type) {
      case JX_PY_ICON:
      case JX_PY_FCON:
      case JX_PY_SCON:
      case JX_PY_NAME:
        {
          char *buffer = stack_buffer;
          if(st_len >= SSCANF_BUFSIZE) {
            buffer = jx_malloc(st_len+1);
          }
          if(buffer) {
            jx_os_strncpy(buffer, s->tok, st_len);                              
            buffer[st_len] = 0;
            switch(tok_type) {
            case JX_PY_ICON:
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
            case JX_PY_FCON:
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
            case JX_PY_SCON:
              buffer[st_len-1]=0;
              token = jx_ob_from_str(buffer+1);
              break;
            case JX_PY_NAME:
              token = jx_ob_from_ident(buffer);
              break;
            }
            if(buffer != stack_buffer)
              jx_free(buffer);
          }
        }
        break;
#if 0
      case JX_PY_TRUE:
        token = jx_ob_from_bool(true);
        break;
      case JX_PY_FALSE:
        token = jx_ob_from_bool(false);
        break;
      case JX_PY_NULL:
        token = jx_ob_from_null();
        break;
      case JX_PY_BUILTIN:
        token = jx_builtin_new_from_selector(0);
        break;
#endif
      case JX_PY_ERROR:
        s->context.status = JX_STATUS_SYNTAX_ERROR;
        break;
      }
    
      if(tok_type == JX_PY_EOI) {
        s->context.exhausted = JX_TRUE;
      }

      if(!skip_token) {

        s->colon_just_seen = (tok_type == JX_PY_COLON);

        jx_py_send(jx_Parser, (int)tok_type, token, &s->context);

        if(!jx_ok(s->context.status)) /* something bad happened */
          break;
        
        switch(tok_type) { 
        case JX_PY_EOI:
          {/* parse a null token to enable acceptance */
            jx_ob ob = JX_OB_NULL;
            jx_py_send(jx_Parser, 0, ob, &s->context);
          }
          break;
        }
      }
      if(s->context.status) /* accepted or complete */
        break;
      else /* we need more */
        s->n_tok_parsed++;
    }
  }
  /* free the parser instance */
  jx_py_Free(jx_Parser, jx__py_free);
}

#if 0
static void jx_py_scan_input(jx_py_scanner_state *state)
{
  /* allocate parser */

  void *jx_Parser = jx_py_Alloc( (void *(*)(size_t))jx__py_alloc);
  jx_int tok_type = 0;
  jx_char stack_buffer[SSCANF_BUFSIZE];
  jx_int colon_seen = JX_FALSE;
  jx_int newline_seen = JX_FALSE;
  s->context.status = 0;
  s->n_tok_parsed = 0;

  while(!s->context.exhausted) {
    tok_type = jx_scan(state);
    {
      jx_ob token = JX_OB_NULL;
      jx_size st_len = s->cur - s->tok;
      jx_bool skip = JX_FALSE;
      
#ifdef JX_PY_PARSER_DEBUG
      
      if(1) {
        jx_size i;
        jx_char *c = s->tok;

        printf("token type %d: ",tok_type);
        
        if(st_len) {
          for(i=0;i<st_len;i++) {
            printf("%c",*(c++));
          }
          printf("\n");
        }
      }
#endif

      switch(tok_type) {
      case JX_PY_INDENT:
      case JX_PY_DEDENT:
      case JX_PY_NEWLINE:
        break;
      default:
        if(newline_seen && 
           (s->indent_level)) {
          int new_indent = (s->tok - s->pos);
          if(new_indent != s->indent_level) {
            jx_ob ob = jx_ob_from_null();
            jx_py_(jx_Parser, JX_PY_DEDENT, ob, &s->context);        
            s->indent_level = new_indent;
          }
        }
      }

      switch(tok_type) {
      case JX_PY_ICON:
      case JX_PY_FCON:
      case JX_PY_SCON:
      case JX_PY_NAME:
        {
          char *buffer = stack_buffer;
          if(st_len >= SSCANF_BUFSIZE) {
            buffer = jx_malloc(st_len+1);
          }
          if(buffer) {
            jx_os_strncpy(buffer, s->tok, st_len);                              
            buffer[st_len] = 0;
            switch(tok_type) {
            case JX_PY_ICON:
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
            case JX_PY_FCON:
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
            case JX_PY_SCON:
              buffer[st_len-1]=0;
              token = jx_ob_from_str(buffer+1);
              break;
            case JX_PY_NAME:
              token = jx_ob_from_ident(buffer);
              break;
            }
            if(buffer != stack_buffer)
              jx_free(buffer);
          }
        }
        break;
#if 0
      case JX_PY_TRUE:
        token = jx_ob_from_bool(true);
        break;
      case JX_PY_FALSE:
        token = jx_ob_from_bool(false);
        break;
      case JX_PY_NULL:
        token = jx_ob_from_null();
        break;
      case JX_PY_BUILTIN:
        token = jx_builtin_new_from_selector(0);
        break;
#endif
      case JX_PY_DEDENT_NEWLINE:
        {
          jx_ob ob = jx_ob_from_null();
          jx_py_(jx_Parser, JX_PY_DEDENT, ob, &s->context);        
          tok_type = JX_PY_NEWLINE;
          jx_py_(jx_Parser, JX_PY_NEWLINE, ob, &s->context);        
          tok_type = JX_PY_NEWLINE;
        }
        break;
      case JX_PY_NEWLINE:
        if((!colon_seen)&&(!s->indent_level)) { /* send double newline */
          jx_ob ob = jx_ob_from_null();
          jx_py_(jx_Parser, JX_PY_NEWLINE, ob, &s->context);        
          tok_type = JX_PY_NEWLINE;
        }
        break;
      case JX_PY_ERROR:
        s->context.status = JX_STATUS_SYNTAX_ERROR;
        break;
      case JX_PY_EOI:
        skip = JX_TRUE;
        break;
      }
      
      newline_seen = (tok_type == JX_PY_NEWLINE);

      if(!skip) {
        colon_seen = (tok_type == JX_PY_COLON);
        jx_py_(jx_Parser, (int)tok_type, token, &s->context);
        
        if(!jx_ok(s->context.status)) /* something bad happened */
          break;
      }
      switch(tok_type) {
      case JX_PY_EOI:
        {/* parse a null token to enable acceptance */
          jx_ob ob = JX_OB_NULL;
          jx_py_(jx_Parser, 0, ob, &s->context);
        }
        break;
      }
      
      if(s->context.status) /* accepted or complete */
        break;
      else
        s->n_tok_parsed++;
    }
  
    if(tok_type == JX_PY_EOI) {
      s->context.exhausted = JX_TRUE;
    }
  }
  
  /* free the parser instance */
  jx_py_Free(jx_Parser, jx__py_free);
}
#endif

static jx_status jx_py_scanner_state_init(jx_py_scanner_state *state)
{
  memset(state,0,sizeof(jx_py_scanner_state));
  state->indent_stack = jx_list_new();
  return JX_SUCCESS;
}

static jx_status jx_py_scanner_state_reset(jx_py_scanner_state *state)
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
  state->current_indent = 0;
  state->colon_just_seen = JX_FALSE;
  state->newline_just_seen = JX_FALSE;
  jx_list_reset(state->indent_stack);
  return JX_SUCCESS;
}

static jx_status jx_py_scanner_state_purge(jx_py_scanner_state *state)
{
  jx_ob_free(state->indent_stack);
  /* free last object */
  jx_ob_free(state->context.result);
  /* free the input buffer (if using stdin) */
  switch(state->mode) {
  case JX_PY_SCANNER_MODE_FILE:
  case JX_PY_SCANNER_MODE_CONSOLE:
    if(state->bot) 
      jx_free(state->bot);
    break;
  }
  return JX_SUCCESS;
}

void jx_py_echo_stdin(void)
{
  jx_py_scanner_state state;
  jx_py_scanner_state_init(&state);
  jx_bool done = JX_FALSE;
  while(!done) {    
    jx_py_scan_input(&state);
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
  jx_py_scanner_state_purge(&state);
}

#define JX_PY_SCANNER_MAGIC "jxJSc"

typedef struct {
  jx_opaque_ob opaque;
  jx_py_scanner_state state;
} jx_py_scanner;

static jx_status jx_py_scanner_free(jx_py_scanner *scanner) 
{
  jx_status result = JX_FAILURE;
  jx_ob magic = jx_ob_from_str(JX_PY_SCANNER_MAGIC);
  if(jx_ob_equal(scanner->opaque.magic,magic)) {
    jx_py_scanner_state_purge(&scanner->state);
    jx_ob_free(scanner->opaque.magic);
    jx_free(scanner);
    result = JX_SUCCESS;
  }
  jx_ob_free(magic);
  return result;
}

jx_ob jx_py_scanner_new_with_file(FILE *file,jx_int flags)
{
  jx_ob result = jx_ob_from_null();
  jx_py_scanner *scanner = (jx_py_scanner*)jx_calloc(1,sizeof(jx_py_scanner));
  if(scanner) {
    if(jx_ok(jx_py_scanner_state_init(&scanner->state))) {
      scanner->opaque.magic = jx_ob_from_str(JX_PY_SCANNER_MAGIC);
      scanner->opaque.free_fn = (jx_opaque_free_fn)jx_py_scanner_free;
      scanner->state.file = file;
      scanner->state.flags = flags;
      if (isatty((int)fileno(file))) {      
        scanner->state.mode = JX_PY_SCANNER_MODE_CONSOLE;
      } else {
        scanner->state.mode = JX_PY_SCANNER_MODE_FILE;
      }
      result = jx_builtin_new_with_opaque_ob(&scanner->opaque);
    } else 
      jx_free(scanner);
  }
  return result;
}

jx_ob jx_py_scanner_get_error_message(jx_ob scanner_ob)
{
  jx_ob result = jx_ob_from_null();
  jx_ob magic = jx_ob_from_str(JX_PY_SCANNER_MAGIC);
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_py_scanner *scanner = (jx_py_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,magic)) {
      jx_py_scanner_state *state = &scanner->state;
      jx_ob list = jx_list_new();
      if(JX_OK(state->context.status)) {
        jx_char buffer[50];
        sprintf(buffer,"Error: failure on or before line %d\n",(int)state->line+1);
        jx_list_append(list, jx_ob_from_str(buffer));
      } else {
        jx_char buffer[50];
        sprintf(buffer,"Error: invalid syntax on line %d\n",(int)state->line+1);
        jx_list_append(list, jx_ob_from_str(buffer));
      }
      if(state->bot != state->lim) {
        if(state->tok >= state->pos) {
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
      result = jx_str_join_with_list(list);
    }
  }
  jx_ob_free(magic);
  return result;
}

jx_status jx_py_scanner_next_ob(jx_ob *result, jx_ob scanner_ob)
{
  jx_ob magic = jx_ob_from_str(JX_PY_SCANNER_MAGIC);
  jx_status status = JX_FAILURE;
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_py_scanner *scanner = (jx_py_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,magic)) {
      jx_py_scanner_state *state = &scanner->state;
      state->context.status = JX_SUCCESS;
      while(!state->context.status) {    
        jx_py_scan_input(state);
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
  jx_ob_free(magic);
  return status;
}

jx_status jx_py_scanner_purge_input(jx_ob scanner_ob)
{
  jx_ob magic = jx_ob_from_str(JX_PY_SCANNER_MAGIC);
  jx_status status = JX_FAILURE;
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_py_scanner *scanner = (jx_py_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,magic)) {
      jx_py_scanner_state *state = &scanner->state;
      jx_py_scanner_state_reset(state);
      status = JX_SUCCESS;
    }
  }
  jx_ob_free(magic);
  return status;
}
