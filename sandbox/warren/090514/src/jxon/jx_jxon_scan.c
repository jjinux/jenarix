/* Generated by re2c 0.12.3 on Tue May 26 21:52:43 2009 */
#line 1 "jx_jxon_scan.re"
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

#include "jx_jxon_private.h"

#define JX_JXON_SCANNER_MODE_FILE    0 
#define JX_JXON_SCANNER_MODE_STRING  1
#define JX_JXON_SCANNER_MODE_CONSOLE 2

typedef struct {
  jx_char *bot, *tok, *ptr, *cur, *pos, *lim, *top, *eof;
  jx_int line;
  jx_int n_tok_parsed;

  jx_int mode;
  FILE *file;
  jx_jxon_parse_context context;
} jx_jxon_scanner_state;

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
  case JX_JXON_SCANNER_MODE_FILE:
    return fread((char*)buf, 1, buf_size, stdin);
    break;
  case JX_JXON_SCANNER_MODE_CONSOLE:
    if(fgets((char*)buf, buf_size, stdin)) {
      return jx_strlen(buf);
    } else 
      return 0;
    break;
  }
  return 0;
}

static jx_char *jx_fill(jx_jxon_scanner_state *s, jx_char *cursor)
{
  switch(s->mode) {
  case JX_JXON_SCANNER_MODE_FILE:
  case JX_JXON_SCANNER_MODE_CONSOLE:
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
  
static int jx_scan(jx_jxon_scanner_state *s)
{
  jx_char *cursor = s->cur;
 std:
  s->tok = cursor;
  
  
#line 170 "<stdout>"
{
	YYCTYPE yych;
	unsigned int yyaccept = 0;

	if((YYLIMIT - YYCURSOR) < 6) YYFILL(6);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:	goto yy40;
	case 0x09:
	case 0x0B:
	case 0x0C:
	case ' ':	goto yy34;
	case 0x0A:	goto yy38;
	case '"':	goto yy8;
	case '#':	goto yy36;
	case '\'':	goto yy9;
	case '(':	goto yy19;
	case ')':	goto yy21;
	case '+':
	case '-':	goto yy2;
	case ',':	goto yy23;
	case '.':	goto yy7;
	case '0':	goto yy4;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy6;
	case ':':	goto yy25;
	case ';':	goto yy32;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy31;
	case '[':	goto yy11;
	case ']':	goto yy13;
	case '`':	goto yy10;
	case 'f':	goto yy29;
	case 'n':	goto yy30;
	case 't':	goto yy27;
	case '{':	goto yy15;
	case '}':	goto yy17;
	default:	goto yy42;
	}
yy2:
	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case '.':	goto yy123;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy97;
	default:	goto yy3;
	}
yy3:
#line 225 "jx_jxon_scan.re"
	{
#ifdef JX_JXON_PARSER_DEBUG
      printf("unexpected character: %c\n", *s->tok);
#endif
       RET(JX_JXON_ERROR);
    }
#line 290 "<stdout>"
yy4:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy95;
	case 'X':
	case 'x':	goto yy116;
	default:	goto yy115;
	}
yy5:
#line 205 "jx_jxon_scan.re"
	{ RET(JX_JXON_ICON); }
#line 306 "<stdout>"
yy6:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	goto yy98;
yy7:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy87;
	default:	goto yy3;
	}
yy8:
	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy3;
	default:	goto yy78;
	}
yy9:
	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy3;
	default:	goto yy68;
	}
yy10:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy64;
	default:	goto yy3;
	}
yy11:
	++YYCURSOR;
#line 188 "jx_jxon_scan.re"
	{ RET(JX_JXON_OPEN_RECT_BRACE); }
#line 415 "<stdout>"
yy13:
	++YYCURSOR;
#line 189 "jx_jxon_scan.re"
	{ RET(JX_JXON_CLOSE_RECT_BRACE); }
#line 420 "<stdout>"
yy15:
	++YYCURSOR;
#line 191 "jx_jxon_scan.re"
	{ RET(JX_JXON_OPEN_CURLY_BRACE); }
#line 425 "<stdout>"
yy17:
	++YYCURSOR;
#line 192 "jx_jxon_scan.re"
	{ RET(JX_JXON_CLOSE_CURLY_BRACE); }
#line 430 "<stdout>"
yy19:
	++YYCURSOR;
#line 194 "jx_jxon_scan.re"
	{ RET(JX_JXON_OPEN_PAR); }
#line 435 "<stdout>"
yy21:
	++YYCURSOR;
#line 195 "jx_jxon_scan.re"
	{ RET(JX_JXON_CLOSE_PAR); }
#line 440 "<stdout>"
yy23:
	++YYCURSOR;
#line 197 "jx_jxon_scan.re"
	{ RET(JX_JXON_COMMA); }
#line 445 "<stdout>"
yy25:
	++YYCURSOR;
#line 198 "jx_jxon_scan.re"
	{ RET(JX_JXON_COLON); }
#line 450 "<stdout>"
yy27:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'r':	goto yy60;
	default:	goto yy46;
	}
yy28:
#line 203 "jx_jxon_scan.re"
	{ RET(JX_JXON_IDENT); }
#line 461 "<stdout>"
yy29:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'a':	goto yy55;
	default:	goto yy46;
	}
yy30:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'u':	goto yy51;
	default:	goto yy46;
	}
yy31:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	goto yy46;
yy32:
	++YYCURSOR;
#line 207 "jx_jxon_scan.re"
	{ RET(JX_JXON_SEMICOLON); }
#line 484 "<stdout>"
yy34:
	++YYCURSOR;
	yych = *YYCURSOR;
	goto yy44;
yy35:
#line 210 "jx_jxon_scan.re"
	{ goto std; }
#line 492 "<stdout>"
yy36:
	++YYCURSOR;
#line 212 "jx_jxon_scan.re"
	{ goto comment; }
#line 497 "<stdout>"
yy38:
	++YYCURSOR;
#line 214 "jx_jxon_scan.re"
	{
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std;
    }
#line 506 "<stdout>"
yy40:
	++YYCURSOR;
#line 220 "jx_jxon_scan.re"
	{ 
      if(cursor == s->eof) RET(JX_JXON_EOI);
      RET(JX_JXON_EOI); 
    }
#line 514 "<stdout>"
yy42:
	yych = *++YYCURSOR;
	goto yy3;
yy43:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy44:
	switch(yych) {
	case 0x09:
	case 0x0B:
	case 0x0C:
	case ' ':	goto yy43;
	default:	goto yy35;
	}
yy45:
	yyaccept = 2;
	YYMARKER = ++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy46:
	switch(yych) {
	case '.':	goto yy47;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy45;
	default:	goto yy28;
	}
yy47:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy49;
	default:	goto yy48;
	}
yy48:
	YYCURSOR = YYMARKER;
	switch(yyaccept) {
	case 0: 	goto yy3;
	case 1: 	goto yy5;
	case 2: 	goto yy28;
	case 3: 	goto yy54;
	case 4: 	goto yy59;
	case 5: 	goto yy63;
	case 6: 	goto yy89;
	}
yy49:
	yyaccept = 2;
	YYMARKER = ++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '.':	goto yy47;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy49;
	default:	goto yy28;
	}
yy51:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'l':	goto yy52;
	default:	goto yy46;
	}
yy52:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'l':	goto yy53;
	default:	goto yy46;
	}
yy53:
	yyaccept = 3;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case '.':	goto yy47;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy45;
	default:	goto yy54;
	}
yy54:
#line 201 "jx_jxon_scan.re"
	{ RET(JX_JXON_NULL); }
#line 833 "<stdout>"
yy55:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'l':	goto yy56;
	default:	goto yy46;
	}
yy56:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 's':	goto yy57;
	default:	goto yy46;
	}
yy57:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'e':	goto yy58;
	default:	goto yy46;
	}
yy58:
	yyaccept = 4;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case '.':	goto yy47;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy45;
	default:	goto yy59;
	}
yy59:
#line 200 "jx_jxon_scan.re"
	{ RET(JX_JXON_FALSE); }
#line 928 "<stdout>"
yy60:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'u':	goto yy61;
	default:	goto yy46;
	}
yy61:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'e':	goto yy62;
	default:	goto yy46;
	}
yy62:
	yyaccept = 5;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case '.':	goto yy47;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy45;
	default:	goto yy63;
	}
yy63:
#line 199 "jx_jxon_scan.re"
	{ RET(JX_JXON_TRUE); }
#line 1016 "<stdout>"
yy64:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy64;
	default:	goto yy66;
	}
yy66:
#line 186 "jx_jxon_scan.re"
	{ RET(JX_JXON_BUILTIN); }
#line 1091 "<stdout>"
yy67:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy68:
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy48;
	case '\'':	goto yy70;
	case '\\':	goto yy69;
	default:	goto yy67;
	}
yy69:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '"':
	case '\'':
	case '?':
	case '\\':
	case 'a':
	case 'b':
	case 'f':
	case 'n':
	case 'r':
	case 't':
	case 'v':	goto yy67;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy73;
	case 'x':	goto yy72;
	default:	goto yy48;
	}
yy70:
	++YYCURSOR;
#line 184 "jx_jxon_scan.re"
	{ RET(JX_JXON_SCON); }
#line 1135 "<stdout>"
yy72:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy75;
	default:	goto yy48;
	}
yy73:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy48;
	case '\'':	goto yy70;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy73;
	case '\\':	goto yy69;
	default:	goto yy67;
	}
yy75:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy48;
	case '\'':	goto yy70;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy75;
	case '\\':	goto yy69;
	default:	goto yy67;
	}
yy77:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy78:
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy48;
	case '"':	goto yy80;
	case '\\':	goto yy79;
	default:	goto yy77;
	}
yy79:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '"':
	case '\'':
	case '?':
	case '\\':
	case 'a':
	case 'b':
	case 'f':
	case 'n':
	case 'r':
	case 't':
	case 'v':	goto yy77;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy83;
	case 'x':	goto yy82;
	default:	goto yy48;
	}
yy80:
	++YYCURSOR;
#line 182 "jx_jxon_scan.re"
	{ RET(JX_JXON_SCON); }
#line 1260 "<stdout>"
yy82:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy85;
	default:	goto yy48;
	}
yy83:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy48;
	case '"':	goto yy80;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy83;
	case '\\':	goto yy79;
	default:	goto yy77;
	}
yy85:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy48;
	case '"':	goto yy80;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy85;
	case '\\':	goto yy79;
	default:	goto yy77;
	}
yy87:
	yyaccept = 6;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy87;
	case 'E':
	case 'e':	goto yy90;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy91;
	default:	goto yy89;
	}
yy89:
#line 180 "jx_jxon_scan.re"
	{ RET(JX_JXON_FCON); }
#line 1369 "<stdout>"
yy90:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy92;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy93;
	default:	goto yy48;
	}
yy91:
	yych = *++YYCURSOR;
	goto yy89;
yy92:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy93;
	default:	goto yy48;
	}
yy93:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy93;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy91;
	default:	goto yy89;
	}
yy95:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy95;
	default:	goto yy5;
	}
yy97:
	yyaccept = 1;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
yy98:
	switch(yych) {
	case '.':	goto yy99;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy97;
	case 'E':
	case 'e':	goto yy100;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy95;
	default:	goto yy5;
	}
yy99:
	yyaccept = 6;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'E':
	case 'e':	goto yy106;
	default:	goto yy105;
	}
yy100:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy101;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy102;
	default:	goto yy48;
	}
yy101:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy102;
	default:	goto yy48;
	}
yy102:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy102;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy91;
	default:	goto yy89;
	}
yy104:
	yyaccept = 6;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
yy105:
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy104;
	case 'E':
	case 'e':	goto yy110;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy91;
	default:	goto yy89;
	}
yy106:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy107;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy108;
	default:	goto yy48;
	}
yy107:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy108;
	default:	goto yy48;
	}
yy108:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy108;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy91;
	default:	goto yy89;
	}
yy110:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy111;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy112;
	default:	goto yy48;
	}
yy111:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy112;
	default:	goto yy48;
	}
yy112:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy112;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy91;
	default:	goto yy89;
	}
yy114:
	yyaccept = 1;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
yy115:
	switch(yych) {
	case '.':	goto yy99;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy114;
	case 'E':
	case 'e':	goto yy100;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy121;
	default:	goto yy5;
	}
yy116:
	yych = *++YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy117;
	default:	goto yy48;
	}
yy117:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy117;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy119;
	default:	goto yy5;
	}
yy119:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy119;
	default:	goto yy5;
	}
yy121:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy121;
	default:	goto yy5;
	}
yy123:
	++YYCURSOR;
	switch((yych = *YYCURSOR)) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy87;
	default:	goto yy48;
	}
}
#line 232 "jx_jxon_scan.re"


comment:
  
#line 1784 "<stdout>"
{
	YYCTYPE yych;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:	goto yy126;
	case 0x0A:	goto yy127;
	default:	goto yy129;
	}
yy126:
yy127:
	++YYCURSOR;
#line 237 "jx_jxon_scan.re"
	{ 
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std; 
    }
#line 1803 "<stdout>"
yy129:
	++YYCURSOR;
#line 244 "jx_jxon_scan.re"
	{ goto comment; }
#line 1808 "<stdout>"
}
#line 245 "jx_jxon_scan.re"

  
}


  /* not used:
    "."         { RET(JX_JXON_DOT); }
    "`"         { RET(JX_JXON_BACKAPOSTROPHE); }

    "<"         { RET(JX_JXON_LESS_THAN); }
    "<="        { RET(JX_JXON_LESS_THAN_OR_EQ); }
    ">"         { RET(JX_JXON_GREATER_THAN); }
    ">="        { RET(JX_JXON_GREATER_THAN_OR_EQ); }
    "-"         { RET(JX_JXON_MINUS); }
    "+"         { RET(JX_JXON_PLUS); }
    (['] (ESC|any\[\n\\'])* [']) { RET(JX_JXON_SCON); }

    "="         { RET(JX_JXON_EQUALS); }
   */
#define SSCANF_BUFSIZE 32

/* anticipated use cases:
   (1) parsing a string buffer from memory
   (2) parsing from a file or file descriptor (e.g. stdin) 
   (3) parsing from a character stream 
*/

static void jx_jxon_scan_input(jx_jxon_scanner_state *state)
{
  /* allocate parser */

  void *jx_Parser = jx_jxon_Alloc( (void *(*)(size_t))jx__jxon_alloc);
  jx_int tok_type = 0;
  jx_char stack_buffer[SSCANF_BUFSIZE];
  
  state->context.status = 0;
  state->n_tok_parsed = 0;

  while(!state->context.exhausted) {
    tok_type = jx_scan(state);
    {
      jx_ob token = JX_OB_NULL;
      jx_size st_len = state->cur - state->tok;
#ifdef JX_JXON_PARSER_DEBUG

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
      case JX_JXON_ICON:
      case JX_JXON_FCON:
      case JX_JXON_SCON:
      case JX_JXON_IDENT:
        {
          char *buffer = stack_buffer;
          if(st_len >= SSCANF_BUFSIZE) {
            buffer = jx_malloc(st_len+1);
          }
          if(buffer) {
            jx_os_strncpy(buffer, state->tok, st_len);                              
            buffer[st_len] = 0;
            switch(tok_type) {
            case JX_JXON_ICON:
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
            case JX_JXON_FCON:
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
            case JX_JXON_SCON:
              buffer[st_len-1]=0;
              token = jx_ob_from_str(buffer+1);
              break;
            case JX_JXON_IDENT:
              token = jx_ob_from_ident(buffer);
              break;
            }
            if(buffer != stack_buffer)
              jx_free(buffer);
          }
          break;
        case JX_JXON_TRUE:
          token = jx_ob_from_bool(true);
          break;
        case JX_JXON_FALSE:
          token = jx_ob_from_bool(false);
          break;
        case JX_JXON_NULL:
          token = jx_ob_from_null();
          break;
        case JX_JXON_BUILTIN:
          token = jx_builtin_new_from_selector(0);
          break;
        case JX_JXON_OPEN_RECT_BRACE:
        case JX_JXON_CLOSE_RECT_BRACE:
        case JX_JXON_OPEN_CURLY_BRACE:
        case JX_JXON_CLOSE_CURLY_BRACE:
        case JX_JXON_COMMA:
        case JX_JXON_EOI:
        case JX_JXON_SEMICOLON: 
          /* do nothing */
          break;
          break;
        case JX_JXON_ERROR:
          state->context.status = JX_STATUS_SYNTAX_ERROR;
          break;
        }
      }
      jx_jxon_(jx_Parser, (int)tok_type, token, &state->context);

      if(!jx_ok(state->context.status)) /* something bad happened */
        break;

      switch(tok_type) {
      case JX_JXON_SEMICOLON:
      case JX_JXON_EOI:
        {/* parse a null token to enable acceptance */
          jx_ob ob = JX_OB_NULL;
          jx_jxon_(jx_Parser, 0, ob, &state->context);
        }
        break;
      }

      if(state->context.status) /* accepted or complete */
        break;
      else
        state->n_tok_parsed++;
    }
  }
  
  if(tok_type == JX_JXON_EOI) {
    state->context.exhausted = JX_TRUE;
  }
  
  /* free the parser instance */
  jx_jxon_Free(jx_Parser, jx__jxon_free);
}

static jx_status jx_jxon_scanner_state_init(jx_jxon_scanner_state *state)
{
  memset(state,0,sizeof(jx_jxon_scanner_state));
  return JX_SUCCESS;
}

static jx_status jx_jxon_scanner_state_reset(jx_jxon_scanner_state *state)
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

static jx_status jx_jxon_scanner_state_purge(jx_jxon_scanner_state *state)
{
  /* free last object */
  jx_ob_free(state->context.result);
  /* free the input buffer (if using stdin) */
  switch(state->mode) {
  case JX_JXON_SCANNER_MODE_FILE:
  case JX_JXON_SCANNER_MODE_CONSOLE:
    if(state->bot) 
      jx_free(state->bot);
    break;
  }
  return JX_SUCCESS;
}

jx_ob jx_ob_from_jxon_str(jx_char *st)
{
  jx_jxon_scanner_state state;
  jx_jxon_scanner_state_init(&state);
  jx_ob result = JX_OB_NULL;
  state.cur = st;
  state.eof = st + jx_strlen(st) + 1;
  state.mode = JX_JXON_SCANNER_MODE_STRING;
  jx_jxon_scan_input(&state);
  if(state.context.status == JX_YES) {
    if(state.n_tok_parsed) /* non-blank string? */
      result = state.context.result;
    state.context.result = jx_ob_from_null();
  }
  jx_jxon_scanner_state_purge(&state);
  return result;
}

void jx_jxon_echo_stdin(void)
{
  jx_jxon_scanner_state state;
  jx_jxon_scanner_state_init(&state);
  jx_bool done = JX_FALSE;
  while(!done) {    
    jx_jxon_scan_input(&state);
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
  jx_jxon_scanner_state_purge(&state);
}

#define JX_JXON_SCANNER_MAGIC "jxJSc"

typedef struct {
  jx_opaque_ob opaque;
  jx_jxon_scanner_state state;
} jx_jxon_scanner;

static jx_status jx_jxon_scanner_free(jx_jxon_scanner *scanner) 
{
  if(jx_ob_equal(scanner->opaque.magic,
                 jx_ob_from_str(JX_JXON_SCANNER_MAGIC))) {
    jx_jxon_scanner_state_purge(&scanner->state);
    jx_ob_free(scanner->opaque.magic);
    jx_free(scanner);
    return JX_SUCCESS;
  }
  return JX_FAILURE;
}

jx_ob jx_jxon_scanner_new_with_file(FILE *file)
{
  jx_ob result = JX_OB_NULL;
  jx_jxon_scanner *scanner = (jx_jxon_scanner*)jx_calloc(1,sizeof(jx_jxon_scanner));
  if(scanner) {
    if(jx_ok(jx_jxon_scanner_state_init(&scanner->state))) {
      scanner->opaque.magic = jx_ob_from_str(JX_JXON_SCANNER_MAGIC);
      scanner->opaque.free_fn = (jx_opaque_free_fn)jx_jxon_scanner_free;
      scanner->state.file = file;
      if (isatty((int)fileno(file))) {      
        scanner->state.mode = JX_JXON_SCANNER_MODE_CONSOLE;
      } else {
        scanner->state.mode = JX_JXON_SCANNER_MODE_FILE;
      }
      result = jx_builtin_new_with_opaque_ob(&scanner->opaque);
    } else 
      jx_free(scanner);
  }
  return result;
}

jx_ob jx_jxon_scanner_get_error_message(jx_ob scanner_ob)
{
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_jxon_scanner *scanner = (jx_jxon_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,
                   jx_ob_from_str(JX_JXON_SCANNER_MAGIC))) {
      jx_jxon_scanner_state *state = &scanner->state;
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

jx_status jx_jxon_scanner_next_ob(jx_ob *result, jx_ob scanner_ob)
{
  jx_status status = JX_FAILURE;
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_jxon_scanner *scanner = (jx_jxon_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,
                   jx_ob_from_str(JX_JXON_SCANNER_MAGIC))) {
      jx_jxon_scanner_state *state = &scanner->state;
      state->context.status = JX_SUCCESS;
      while(!state->context.status) {    
        jx_jxon_scan_input(state);
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

jx_status jx_jxon_scanner_purge_input(jx_ob scanner_ob)
{
  jx_status status = JX_FAILURE;
  if(jx_builtin_opaque_ob_check(scanner_ob)) {
    jx_jxon_scanner *scanner = (jx_jxon_scanner*)scanner_ob.data.io.opaque_ob;
    if(jx_ob_equal(scanner->opaque.magic,
                   jx_ob_from_str(JX_JXON_SCANNER_MAGIC))) {
      jx_jxon_scanner_state *state = &scanner->state;
      jx_jxon_scanner_state_reset(state);
      status = JX_SUCCESS;
    }
  }
  return status;
}
