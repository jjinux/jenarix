/* Generated by re2c 0.12.3 on Thu May 14 19:17:56 2009 */
#line 1 "jx_json_scan.re"
/* RE2C-based tokenizer */

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
  if(!fgets((char*)buf, buf_size, stdin))
    buf[0] = 0;
  return strlen((char*)buf);
}

static jx_char *jx_fill(jx_json_scanner_state *s, jx_char *cursor)
{
  switch(s->mode) {
  case JX_JSON_SCANNER_MODE_STDIN:
    if(!s->eof) {  
      jx_uword cnt = s->tok - s->bot;
      if(cnt) {
        if(s->lim > s->tok) {
          memcpy(s->bot, s->tok, s->lim - s->tok);
        }
        s->tok = s->bot;
        s->ptr -= cnt;
        cursor -= cnt;
        s->pos -= cnt;
        s->lim -= cnt;
      }
      if((s->top - s->lim) < BSIZE){
        jx_char *buf = (jx_char*) jx_calloc(1,((s->lim - s->bot) +
                                                   BSIZE)*sizeof(jx_char));
        memcpy(buf, s->tok, s->lim - s->tok);
        s->tok = buf;
        s->ptr = &buf[s->ptr - s->bot];
        cursor = &buf[cursor - s->bot];
        s->pos = &buf[s->pos - s->bot];
        s->lim = &buf[s->lim - s->bot];
        s->top = &s->lim[BSIZE];
        if(s->bot) {
          jx_free(s->bot);
        }
        s->bot = buf;
      }
      if((cnt = jx_read_stdin(s->lim, BSIZE)) != BSIZE) {
        {
          jx_size cc = cnt;
          while(cc<BSIZE) {
            s->lim[cc] = 0;
            cc++;
          }
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
    break;
  }
  return cursor;
}
  
static int jx_scan(jx_json_scanner_state *s)
{
  jx_char *cursor = s->cur;
 std:
  s->tok = cursor;
  
  
#line 129 "<stdout>"
{
	YYCTYPE yych;
	unsigned int yyaccept = 0;

	if((YYLIMIT - YYCURSOR) < 5) YYFILL(5);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:	goto yy25;
	case 0x09:
	case 0x0B:
	case 0x0C:
	case ' ':	goto yy23;
	case 0x0A:	goto yy29;
	case '"':	goto yy7;
	case '#':	goto yy27;
	case ',':	goto yy16;
	case '.':	goto yy5;
	case '0':	goto yy2;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy4;
	case ':':	goto yy18;
	case '[':	goto yy8;
	case ']':	goto yy10;
	case 'f':	goto yy21;
	case 'n':	goto yy22;
	case 't':	goto yy20;
	case '{':	goto yy12;
	case '}':	goto yy14;
	default:	goto yy31;
	}
yy2:
	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy66;
	case 'X':
	case 'x':	goto yy87;
	default:	goto yy86;
	}
yy3:
#line 155 "jx_json_scan.re"
	{ RET(JX_JSON_ICON); }
#line 182 "<stdout>"
yy4:
	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	goto yy69;
yy5:
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
	case '9':	goto yy58;
	default:	goto yy6;
	}
yy6:
#line 171 "jx_json_scan.re"
	{
#ifdef JX_JSON_PARSER_DEBUG
      printf("unexpected character: %c\n", *s->tok);
#endif
       RET(JX_JSON_ERROR);
    }
#line 210 "<stdout>"
yy7:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy6;
	default:	goto yy49;
	}
yy8:
	++YYCURSOR;
#line 143 "jx_json_scan.re"
	{ RET(JX_JSON_OPEN_RECT_BRACE); }
#line 223 "<stdout>"
yy10:
	++YYCURSOR;
#line 144 "jx_json_scan.re"
	{ RET(JX_JSON_CLOSE_RECT_BRACE); }
#line 228 "<stdout>"
yy12:
	++YYCURSOR;
#line 146 "jx_json_scan.re"
	{ RET(JX_JSON_OPEN_CURLY_BRACE); }
#line 233 "<stdout>"
yy14:
	++YYCURSOR;
#line 147 "jx_json_scan.re"
	{ RET(JX_JSON_CLOSE_CURLY_BRACE); }
#line 238 "<stdout>"
yy16:
	++YYCURSOR;
#line 149 "jx_json_scan.re"
	{ RET(JX_JSON_COMMA); }
#line 243 "<stdout>"
yy18:
	++YYCURSOR;
#line 150 "jx_json_scan.re"
	{ RET(JX_JSON_COLON); }
#line 248 "<stdout>"
yy20:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'r':	goto yy44;
	default:	goto yy6;
	}
yy21:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'a':	goto yy39;
	default:	goto yy6;
	}
yy22:
	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'u':	goto yy34;
	default:	goto yy6;
	}
yy23:
	++YYCURSOR;
	yych = *YYCURSOR;
	goto yy33;
yy24:
#line 157 "jx_json_scan.re"
	{ goto std; }
#line 277 "<stdout>"
yy25:
	++YYCURSOR;
#line 159 "jx_json_scan.re"
	{ 
      if(cursor == s->eof) RET(JX_JSON_EOI);
    }
#line 284 "<stdout>"
yy27:
	++YYCURSOR;
#line 163 "jx_json_scan.re"
	{ goto comment; }
#line 289 "<stdout>"
yy29:
	++YYCURSOR;
#line 165 "jx_json_scan.re"
	{
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std;
    }
#line 298 "<stdout>"
yy31:
	yych = *++YYCURSOR;
	goto yy6;
yy32:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy33:
	switch(yych) {
	case 0x09:
	case 0x0B:
	case 0x0C:
	case ' ':	goto yy32;
	default:	goto yy24;
	}
yy34:
	yych = *++YYCURSOR;
	switch(yych) {
	case 'l':	goto yy36;
	default:	goto yy35;
	}
yy35:
	YYCURSOR = YYMARKER;
	switch(yyaccept) {
	case 0: 	goto yy3;
	case 1: 	goto yy6;
	case 2: 	goto yy60;
	}
yy36:
	yych = *++YYCURSOR;
	switch(yych) {
	case 'l':	goto yy37;
	default:	goto yy35;
	}
yy37:
	++YYCURSOR;
#line 153 "jx_json_scan.re"
	{ RET(JX_JSON_NULL); }
#line 337 "<stdout>"
yy39:
	yych = *++YYCURSOR;
	switch(yych) {
	case 'l':	goto yy40;
	default:	goto yy35;
	}
yy40:
	yych = *++YYCURSOR;
	switch(yych) {
	case 's':	goto yy41;
	default:	goto yy35;
	}
yy41:
	yych = *++YYCURSOR;
	switch(yych) {
	case 'e':	goto yy42;
	default:	goto yy35;
	}
yy42:
	++YYCURSOR;
#line 152 "jx_json_scan.re"
	{ RET(JX_JSON_FALSE); }
#line 360 "<stdout>"
yy44:
	yych = *++YYCURSOR;
	switch(yych) {
	case 'u':	goto yy45;
	default:	goto yy35;
	}
yy45:
	yych = *++YYCURSOR;
	switch(yych) {
	case 'e':	goto yy46;
	default:	goto yy35;
	}
yy46:
	++YYCURSOR;
#line 151 "jx_json_scan.re"
	{ RET(JX_JSON_TRUE); }
#line 377 "<stdout>"
yy48:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy49:
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy35;
	case '"':	goto yy51;
	case '\\':	goto yy50;
	default:	goto yy48;
	}
yy50:
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
	case 'v':	goto yy48;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy54;
	case 'x':	goto yy53;
	default:	goto yy35;
	}
yy51:
	++YYCURSOR;
#line 141 "jx_json_scan.re"
	{ RET(JX_JSON_SCON); }
#line 421 "<stdout>"
yy53:
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
	case 'f':	goto yy56;
	default:	goto yy35;
	}
yy54:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy35;
	case '"':	goto yy51;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy54;
	case '\\':	goto yy50;
	default:	goto yy48;
	}
yy56:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:
	case 0x0A:	goto yy35;
	case '"':	goto yy51;
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
	case 'f':	goto yy56;
	case '\\':	goto yy50;
	default:	goto yy48;
	}
yy58:
	yyaccept = 2;
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
	case '9':	goto yy58;
	case 'E':
	case 'e':	goto yy61;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy62;
	default:	goto yy60;
	}
yy60:
#line 139 "jx_json_scan.re"
	{ RET(JX_JSON_FCON); }
#line 530 "<stdout>"
yy61:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy63;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy64;
	default:	goto yy35;
	}
yy62:
	yych = *++YYCURSOR;
	goto yy60;
yy63:
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
	case '9':	goto yy64;
	default:	goto yy35;
	}
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
	case '9':	goto yy64;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy62;
	default:	goto yy60;
	}
yy66:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy66;
	default:	goto yy3;
	}
yy68:
	yyaccept = 0;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
yy69:
	switch(yych) {
	case '.':	goto yy70;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy68;
	case 'E':
	case 'e':	goto yy71;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy66;
	default:	goto yy3;
	}
yy70:
	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych) {
	case 'E':
	case 'e':	goto yy77;
	default:	goto yy76;
	}
yy71:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy72;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy73;
	default:	goto yy35;
	}
yy72:
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
	case '9':	goto yy73;
	default:	goto yy35;
	}
yy73:
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
	case '9':	goto yy73;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy62;
	default:	goto yy60;
	}
yy75:
	yyaccept = 2;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
yy76:
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
	case '9':	goto yy75;
	case 'E':
	case 'e':	goto yy81;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy62;
	default:	goto yy60;
	}
yy77:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy78;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy79;
	default:	goto yy35;
	}
yy78:
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
	case '9':	goto yy79;
	default:	goto yy35;
	}
yy79:
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
	case '9':	goto yy79;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy62;
	default:	goto yy60;
	}
yy81:
	yych = *++YYCURSOR;
	switch(yych) {
	case '+':
	case '-':	goto yy82;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy83;
	default:	goto yy35;
	}
yy82:
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
	case '9':	goto yy83;
	default:	goto yy35;
	}
yy83:
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
	case '9':	goto yy83;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy62;
	default:	goto yy60;
	}
yy85:
	yyaccept = 0;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
yy86:
	switch(yych) {
	case '.':	goto yy70;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy85;
	case 'E':
	case 'e':	goto yy71;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy92;
	default:	goto yy3;
	}
yy87:
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
	case 'f':	goto yy88;
	default:	goto yy35;
	}
yy88:
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
	case 'f':	goto yy88;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy90;
	default:	goto yy3;
	}
yy90:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy90;
	default:	goto yy3;
	}
yy92:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy92;
	default:	goto yy3;
	}
}
#line 178 "jx_json_scan.re"


  /* not used:
    "("         { RET(JX_JSON_OPENPAR); }
    ")"         { RET(JX_JSON_CLOSEPAR); }
    "."         { RET(JX_JSON_DOT); }
    L (L|D)*      { RET(JX_JSON_IDENT); }
    "`"         { RET(JX_JSON_BACKAPOSTROPHE); }

    ";"         { RET(JX_JSON_SEMICOLON); }

    "-"         { RET(JX_JSON_MINUS); }
    "+"         { RET(JX_JSON_PLUS); }
    (['] (ESC|any\[\n\\'])* [']) { RET(JX_JSON_SCON); }

    "="         { RET(JX_JSON_EQUALS); }
   */

comment:
  
#line 946 "<stdout>"
{
	YYCTYPE yych;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych) {
	case 0x00:	goto yy96;
	case 0x0A:	goto yy97;
	default:	goto yy99;
	}
yy96:
yy97:
	++YYCURSOR;
#line 199 "jx_json_scan.re"
	{ 
      s->pos = cursor; s->line++;
      s->eof = NULL;
      goto std; 
    }
#line 965 "<stdout>"
yy99:
	++YYCURSOR;
#line 206 "jx_json_scan.re"
	{ goto comment; }
#line 970 "<stdout>"
}
#line 207 "jx_json_scan.re"

  
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

  while((tok_type = jx_scan(state)) != JX_JSON_EOI) {
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
              int icon;/* eventually need to handle int32 vs. int64 */
              if( jx_os_sscanf(buffer, "%i", &icon) != 1) { /* use strtol instead? */
                icon = 0;
              }
              token = jx_ob_from_int(icon);            
            }
            break;
          case JX_JSON_FCON:
            {
              float fcon; /* eventually need to handle float64 vs. float32 */
              if( jx_os_sscanf(buffer, "%f", &fcon) != 1) { /* use strtof instead? */
                fcon = 0.0F;
              }
              token = jx_ob_from_float(fcon);
            }
            break;
          case JX_JSON_SCON:
            buffer[st_len-1]=0;
            token = jx_ob_from_str(buffer+1);
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
        /* do nothing */
        break;
      case JX_JSON_ERROR:
        state->context.status = -1;
        break;
      }
    }
    jx_json_(jx_Parser, (int)tok_type, token, &state->context);
    if(state->context.status<0) /* error */
      break;
  }

  /* parse end of input token */
  if(tok_type == JX_JSON_EOI) {
    jx_ob ob = JX_OB_NULL;
    jx_json_(jx_Parser, JX_JSON_EOI, ob, &state->context);
    jx_json_(jx_Parser, 0, ob, &state->context);
  }

  /* free the input buffer (if using stdin) */
  switch(state->mode) {
  case JX_JSON_SCANNER_MODE_STDIN:
    if(state->bot) 
      jx_free(state->bot);
    break;
  }
  /* free the parser instance */
  jx_json_Free(jx_Parser, jx__json_free);
}


jx_ob jx_ob_from_json_str(jx_char *st)
{
  jx_json_scanner_state state;
  jx_ob result = JX_OB_NULL;
  memset(&state,0,sizeof(state));
  state.cur = st;
  state.eof = st + strlen(st) + 1;
  state.mode = JX_JSON_SCANNER_MODE_STRING;
  jx_json_scan_input(&state);
  if(state.context.status==1) {
    result = state.context.result;
  }
  return result;
}

void jx_json_echo_stdin(void)
{
  jx_json_scanner_state state;
  memset(&state,0,sizeof(state));
  jx_json_scan_input(&state);
  if(state.context.status<0) {
    printf("syntax error.\n");
  } else {
    {
      jx_ob json = jx_ob_to_json(state.context.result);
      printf("%s\n",jx_ob_as_str(&json));
      jx_ob_free(json);
    }
    jx_ob_free(state.context.result);
  }
}
