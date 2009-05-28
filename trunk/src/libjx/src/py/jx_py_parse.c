/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 37 "jx_py_parse.lem"


#include <stdio.h>

#include "jx_py_private.h"

#line 16 "jx_py_parse.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    jx_py_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is jx_py_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    jx_py_ARG_SDECL     A static variable declaration for the %extra_argument
**    jx_py_ARG_PDECL     A parameter declaration for the %extra_argument
**    jx_py_ARG_STORE     Code to store %extra_argument into yypParser
**    jx_py_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 95
#define YYACTIONTYPE unsigned short int
#define jx_py_TOKENTYPE jx_ob
typedef union {
  jx_py_TOKENTYPE yy0;
  jx_ob yy67;
  int yy189;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_py_ARG_SDECL jx_py_parse_context *context;
#define jx_py_ARG_PDECL ,jx_py_parse_context *context
#define jx_py_ARG_FETCH jx_py_parse_context *context = yypParser->context
#define jx_py_ARG_STORE yypParser->context = context
#define YYNSTATE 184
#define YYNRULE 108
#define YYERRORSYMBOL 46
#define YYERRSYMDT yy189
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* Next are that tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    95,   64,   80,   72,   54,  293,   76,  175,   72,   54,
 /*    10 */    72,   54,   57,  101,  155,  156,    4,   85,  172,  173,
 /*    20 */   174,   61,   94,  103,  104,  105,  106,  108,   56,  110,
 /*    30 */   111,  112,  113,   63,   58,  116,   78,   67,  114,   58,
 /*    40 */   116,  146,  116,   71,  165,   91,   64,   80,   72,   54,
 /*    50 */    27,   92,  175,   72,   54,   72,   54,   57,  101,  155,
 /*    60 */   156,   11,   85,  172,  173,  174,   61,    2,  103,  104,
 /*    70 */   105,  106,  108,   56,  110,  111,  112,  113,   63,   58,
 /*    80 */   116,  122,   19,  149,   58,  116,  119,  116,   71,  165,
 /*    90 */    96,  182,   64,   80,   72,   54,    5,  183,  175,   51,
 /*   100 */    46,   47,   50,   57,  101,  155,  156,   20,   85,  172,
 /*   110 */   173,  174,   61,  142,  103,  104,  105,  106,  108,   56,
 /*   120 */   110,  111,  112,  113,   63,   58,  116,  171,   64,   80,
 /*   130 */    72,   54,   43,   44,   71,  165,  177,   72,   54,   57,
 /*   140 */   101,  155,  156,  127,   85,    6,   26,   13,   10,   12,
 /*   150 */   103,  104,  105,  106,  108,   56,  110,  111,  112,  113,
 /*   160 */    63,   58,  116,  171,   64,   80,   72,   54,  147,  116,
 /*   170 */    71,  165,   97,   72,   54,   57,  101,  155,  156,  124,
 /*   180 */    85,   87,   67,   14,   90,   30,  103,  104,  105,  106,
 /*   190 */   108,   56,  110,  111,  112,  113,   63,   58,  116,  171,
 /*   200 */    64,   80,   72,   54,  118,  116,   71,  165,  180,   72,
 /*   210 */    54,   57,  101,  155,  156,   15,   85,  138,   18,   22,
 /*   220 */   100,    3,  103,  104,  105,  106,  108,   56,  110,  111,
 /*   230 */   112,  113,   63,   58,  116,  171,   64,   80,   72,   54,
 /*   240 */   117,  116,   71,  165,  178,   72,   54,   57,  101,  155,
 /*   250 */   156,   41,   85,   86,   75,   53,  120,  129,  103,  104,
 /*   260 */   105,  106,  108,   56,  110,  111,  112,  113,   63,   58,
 /*   270 */   116,  171,   64,   80,   72,   54,  148,  116,   71,  165,
 /*   280 */   176,   72,   54,   57,  101,  155,  156,    7,   85,    8,
 /*   290 */    23,   42,  128,  170,  103,  104,  105,  106,  108,   56,
 /*   300 */   110,  111,  112,  113,   63,   58,  116,  171,   64,   80,
 /*   310 */    72,   54,  115,  116,   71,  165,  179,   24,   98,   57,
 /*   320 */   101,  155,  156,   31,   85,    9,    1,   55,   25,  294,
 /*   330 */   103,  104,  105,  106,  108,   56,  110,  111,  112,  113,
 /*   340 */    63,   58,  116,   70,   80,   72,   54,  294,  294,  294,
 /*   350 */    71,  165,  294,  294,   59,  101,  155,  156,  294,   85,
 /*   360 */   294,  294,  294,  294,  294,  103,  104,  105,  106,  108,
 /*   370 */    56,  110,  111,  112,  113,   63,   58,  116,  294,  294,
 /*   380 */   294,   72,   54,  294,  294,   71,  165,  294,  294,  294,
 /*   390 */   167,  294,  294,  294,  294,   85,  294,  294,  294,  294,
 /*   400 */   294,  103,  104,  105,  106,  108,   56,  110,  111,  112,
 /*   410 */   113,   63,   58,  116,  294,  294,   72,   54,  294,  294,
 /*   420 */   294,   71,  165,  294,  294,  162,  294,  294,  294,  294,
 /*   430 */    85,  294,  294,  294,  294,  294,  103,  104,  105,  106,
 /*   440 */   108,   56,  110,  111,  112,  113,   63,   58,  116,  294,
 /*   450 */    93,   81,  132,  294,  294,  294,   71,  165,   32,  161,
 /*   460 */   294,  294,  294,   21,  294,  294,   29,   28,   33,  294,
 /*   470 */   294,  132,   34,  294,  294,  294,  294,  294,  294,   49,
 /*   480 */    48,  294,  294,  294,  294,   45,   16,  294,   17,  294,
 /*   490 */   125,  126,   52,  131,   72,   54,  294,  294,   49,   48,
 /*   500 */   294,  294,  294,   79,   45,   16,  294,   17,   85,  125,
 /*   510 */   126,  294,  131,  294,  103,  104,  105,  106,  108,   56,
 /*   520 */   110,  111,  112,  113,   63,   58,  116,  294,  294,  294,
 /*   530 */    72,   54,  294,  294,   71,  165,  294,  294,  294,  166,
 /*   540 */   294,  294,  294,  294,   85,  294,  294,  294,  294,  294,
 /*   550 */   103,  104,  105,  106,  108,   56,  110,  111,  112,  113,
 /*   560 */    63,   58,  116,  294,  294,   81,  132,  294,  294,  294,
 /*   570 */    71,  165,   32,  161,  294,  294,  294,   21,  294,  294,
 /*   580 */    29,   28,   33,  294,  294,  181,   34,  294,  294,  294,
 /*   590 */   294,  294,  294,   49,   48,   72,   54,  294,  294,   45,
 /*   600 */    16,  294,   17,  294,  125,  126,   52,  131,  294,  144,
 /*   610 */   294,  294,  294,  294,  132,  103,  104,  105,  106,  108,
 /*   620 */    56,  110,  111,  112,  113,   63,   58,  116,   81,  132,
 /*   630 */   294,  294,   66,  145,   34,   32,  161,  294,  294,  294,
 /*   640 */    21,   49,   48,   29,   28,   33,  294,   45,   16,   34,
 /*   650 */    17,  123,  125,  126,   52,  131,   49,   48,   72,   54,
 /*   660 */   294,  294,   45,   16,  294,   17,  294,  125,  126,   52,
 /*   670 */   131,  294,  140,  294,  294,  294,  294,  294,  103,  104,
 /*   680 */   105,  106,  108,   56,  110,  111,  112,  113,   63,   58,
 /*   690 */   116,   72,   54,  294,   62,  294,  294,  141,   35,   36,
 /*   700 */    37,   38,   39,   40,  294,  136,  294,  294,   72,   54,
 /*   710 */   294,  103,  104,  105,  106,  108,   56,  110,  111,  112,
 /*   720 */   113,   63,   58,  116,  294,   60,  294,  294,  294,   72,
 /*   730 */    54,  107,  108,   56,  110,  111,  112,  113,   63,   58,
 /*   740 */   116,  294,  294,  140,  294,  294,  294,  294,  294,  103,
 /*   750 */   104,  105,  106,  108,   56,  110,  111,  112,  113,   63,
 /*   760 */    58,  116,   72,   54,  294,  294,  294,  294,  139,  294,
 /*   770 */   294,  294,  294,  294,  294,  294,   83,  294,  294,  294,
 /*   780 */    72,   54,  103,  104,  105,  106,  108,   56,  110,  111,
 /*   790 */   112,  113,   63,   58,  116,  294,  294,   65,  294,  294,
 /*   800 */    72,   54,  294,  294,  294,  153,  110,  111,  112,  113,
 /*   810 */    63,   58,  116,  294,  144,  294,  294,  294,  294,  294,
 /*   820 */   103,  104,  105,  106,  108,   56,  110,  111,  112,  113,
 /*   830 */    63,   58,  116,   72,   54,  294,  294,  294,  143,  294,
 /*   840 */   294,  294,  294,  294,  294,  294,  294,   69,   72,   54,
 /*   850 */   294,  294,  294,  103,  104,  105,  106,  108,   56,  110,
 /*   860 */   111,  112,  113,   63,   58,  116,  294,  294,  294,  294,
 /*   870 */   294,   72,   54,  158,  110,  111,  112,  113,   63,   58,
 /*   880 */   116,   72,   54,  294,  294,   74,  294,  294,  294,  294,
 /*   890 */   294,  103,  104,  105,  106,  108,   56,  110,  111,  112,
 /*   900 */   113,   63,   58,  116,   72,   54,  159,  110,  111,  112,
 /*   910 */   113,   63,   58,  116,  294,  294,  294,  294,  168,  294,
 /*   920 */   294,  294,   72,   54,  103,  104,  105,  106,  108,   56,
 /*   930 */   110,  111,  112,  113,   63,   58,  116,  294,  294,  294,
 /*   940 */   294,  294,   72,   54,  294,  294,  294,  151,  110,  111,
 /*   950 */   112,  113,   63,   58,  116,  294,   73,  294,  294,  294,
 /*   960 */   294,  294,  103,  104,  105,  106,  108,   56,  110,  111,
 /*   970 */   112,  113,   63,   58,  116,   72,   54,  294,  294,  294,
 /*   980 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  130,
 /*   990 */   294,  294,  294,   72,   54,  103,  104,  105,  106,  108,
 /*  1000 */    56,  110,  111,  112,  113,   63,   58,  116,  294,  294,
 /*  1010 */   294,  294,  294,   72,   54,  294,  294,  294,  152,  110,
 /*  1020 */   111,  112,  113,   63,   58,  116,  294,   89,  294,  294,
 /*  1030 */   294,  294,  294,  103,  104,  105,  106,  108,   56,  110,
 /*  1040 */   111,  112,  113,   63,   58,  116,   72,   54,  294,  294,
 /*  1050 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*  1060 */   133,  294,  294,  294,   72,   54,  103,  104,  105,  106,
 /*  1070 */   108,   56,  110,  111,  112,  113,   63,   58,  116,  294,
 /*  1080 */   294,  294,  294,  294,   72,   54,  294,  294,  294,  154,
 /*  1090 */   110,  111,  112,  113,   63,   58,  116,  294,  102,  294,
 /*  1100 */   294,  294,  294,  294,  103,  104,  105,  106,  108,   56,
 /*  1110 */   110,  111,  112,  113,   63,   58,  116,   72,   54,  294,
 /*  1120 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*  1130 */   294,  164,  294,  294,  294,   72,   54,  103,  104,  105,
 /*  1140 */   106,  108,   56,  110,  111,  112,  113,   63,   58,  116,
 /*  1150 */   294,  294,  294,  294,  294,   72,   54,  294,  294,  294,
 /*  1160 */   150,  110,  111,  112,  113,   63,   58,  116,  294,  134,
 /*  1170 */   294,  294,  294,  294,  294,  103,  104,  105,  106,  108,
 /*  1180 */    56,  110,  111,  112,  113,   63,   58,  116,   72,   54,
 /*  1190 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*  1200 */   294,  294,  163,  294,  294,  294,   72,   54,  103,  104,
 /*  1210 */   105,  106,  108,   56,  110,  111,  112,  113,   63,   58,
 /*  1220 */   116,  294,  294,  294,  294,  294,   72,   54,  294,  294,
 /*  1230 */   294,  109,  110,  111,  112,  113,   63,   58,  116,  294,
 /*  1240 */   135,  294,  294,  294,  294,  294,  103,  104,  105,  106,
 /*  1250 */   108,   56,  110,  111,  112,  113,   63,   58,  116,   82,
 /*  1260 */   294,  132,  294,  294,  169,  294,  294,   32,  161,  294,
 /*  1270 */   294,   84,   21,  294,   13,   10,   12,  294,  294,  294,
 /*  1280 */   294,   34,  294,  294,  294,  294,  294,  294,   49,   48,
 /*  1290 */   294,  294,  294,  294,   45,   16,  294,   17,  294,  125,
 /*  1300 */   126,   52,  131,   99,  294,  132,  294,  294,  294,  294,
 /*  1310 */   294,   32,  161,  294,  294,  294,   21,  294,  294,  294,
 /*  1320 */   294,  294,  294,   72,   54,   34,  294,  294,  294,  294,
 /*  1330 */   294,  294,   49,   48,  294,  294,  157,  294,   45,   16,
 /*  1340 */   294,   17,  294,  125,  126,   52,  131,  294,   88,  110,
 /*  1350 */   111,  112,  113,   63,   58,  116,   72,   54,  294,  294,
 /*  1360 */   294,  294,  294,  294,  294,   68,  160,  294,  294,   77,
 /*  1370 */   294,  294,  294,  294,  132,  294,  294,  137,  294,  294,
 /*  1380 */   294,   88,  110,  111,  112,  113,   63,   58,  116,  132,
 /*  1390 */   294,  294,  294,  294,   34,  294,  294,  294,   68,  160,
 /*  1400 */   294,   49,   48,  294,  294,  294,  132,   45,   16,   34,
 /*  1410 */    17,  294,  125,  126,   52,  131,   49,   48,  294,  294,
 /*  1420 */   294,  132,   45,   16,  121,   17,   34,  125,  126,   52,
 /*  1430 */   131,  294,  294,   49,   48,  294,  294,  294,  294,   45,
 /*  1440 */    16,   34,   17,  294,  125,  126,   52,  131,   49,   48,
 /*  1450 */   294,  294,  294,  294,   45,   16,  294,   17,  294,  125,
 /*  1460 */   126,  294,  131,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    47,   48,   49,   50,   51,   52,   53,   54,   50,   51,
 /*    10 */    50,   51,   59,   60,   61,   62,    5,   64,   65,   66,
 /*    20 */    67,   68,    7,   70,   71,   72,   73,   74,   75,   76,
 /*    30 */    77,   78,   79,   80,   81,   82,   57,   58,   80,   81,
 /*    40 */    82,   81,   82,   90,   91,   47,   48,   49,   50,   51,
 /*    50 */     8,   53,   54,   50,   51,   50,   51,   59,   60,   61,
 /*    60 */    62,   21,   64,   65,   66,   67,   68,   69,   70,   71,
 /*    70 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*    80 */    82,   39,    8,   80,   81,   82,   81,   82,   90,   91,
 /*    90 */     2,   47,   48,   49,   50,   51,    5,   53,   54,   33,
 /*   100 */    34,   35,   36,   59,   60,   61,   62,    5,   64,   65,
 /*   110 */    66,   67,   68,   39,   70,   71,   72,   73,   74,   75,
 /*   120 */    76,   77,   78,   79,   80,   81,   82,   47,   48,   49,
 /*   130 */    50,   51,   31,   32,   90,   91,   56,   50,   51,   59,
 /*   140 */    60,   61,   62,   45,   64,    5,    8,   12,   13,   14,
 /*   150 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   160 */    80,   81,   82,   47,   48,   49,   50,   51,   81,   82,
 /*   170 */    90,   91,   56,   50,   51,   59,   60,   61,   62,   41,
 /*   180 */    64,   57,   58,    6,   16,   17,   70,   71,   72,   73,
 /*   190 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   47,
 /*   200 */    48,   49,   50,   51,   81,   82,   90,   91,   56,   50,
 /*   210 */    51,   59,   60,   61,   62,   38,   64,    7,    8,    8,
 /*   220 */     2,    5,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   230 */    78,   79,   80,   81,   82,   47,   48,   49,   50,   51,
 /*   240 */    81,   82,   90,   91,   56,   50,   51,   59,   60,   61,
 /*   250 */    62,    8,   64,    8,   55,    6,   83,    4,   70,   71,
 /*   260 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   270 */    82,   47,   48,   49,   50,   51,   81,   82,   90,   91,
 /*   280 */    56,   50,   51,   59,   60,   61,   62,    5,   64,    5,
 /*   290 */     5,    8,    4,    2,   70,   71,   72,   73,   74,   75,
 /*   300 */    76,   77,   78,   79,   80,   81,   82,   47,   48,   49,
 /*   310 */    50,   51,   81,   82,   90,   91,   56,    8,    2,   59,
 /*   320 */    60,   61,   62,    5,   64,    9,   22,    4,    8,   94,
 /*   330 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   340 */    80,   81,   82,   48,   49,   50,   51,   94,   94,   94,
 /*   350 */    90,   91,   94,   94,   59,   60,   61,   62,   94,   64,
 /*   360 */    94,   94,   94,   94,   94,   70,   71,   72,   73,   74,
 /*   370 */    75,   76,   77,   78,   79,   80,   81,   82,   94,   94,
 /*   380 */    94,   50,   51,   94,   94,   90,   91,   94,   94,   94,
 /*   390 */    59,   94,   94,   94,   94,   64,   94,   94,   94,   94,
 /*   400 */    94,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   410 */    79,   80,   81,   82,   94,   94,   50,   51,   94,   94,
 /*   420 */    94,   90,   91,   94,   94,   59,   94,   94,   94,   94,
 /*   430 */    64,   94,   94,   94,   94,   94,   70,   71,   72,   73,
 /*   440 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   94,
 /*   450 */     2,    3,    4,   94,   94,   94,   90,   91,   10,   11,
 /*   460 */    94,   94,   94,   15,   94,   94,   18,   19,   20,   94,
 /*   470 */    94,    4,   24,   94,   94,   94,   94,   94,   94,   31,
 /*   480 */    32,   94,   94,   94,   94,   37,   38,   94,   40,   94,
 /*   490 */    42,   43,   44,   45,   50,   51,   94,   94,   31,   32,
 /*   500 */    94,   94,   94,   59,   37,   38,   94,   40,   64,   42,
 /*   510 */    43,   94,   45,   94,   70,   71,   72,   73,   74,   75,
 /*   520 */    76,   77,   78,   79,   80,   81,   82,   94,   94,   94,
 /*   530 */    50,   51,   94,   94,   90,   91,   94,   94,   94,   59,
 /*   540 */    94,   94,   94,   94,   64,   94,   94,   94,   94,   94,
 /*   550 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   560 */    80,   81,   82,   94,   94,    3,    4,   94,   94,   94,
 /*   570 */    90,   91,   10,   11,   94,   94,   94,   15,   94,   94,
 /*   580 */    18,   19,   20,   94,   94,   23,   24,   94,   94,   94,
 /*   590 */    94,   94,   94,   31,   32,   50,   51,   94,   94,   37,
 /*   600 */    38,   94,   40,   94,   42,   43,   44,   45,   94,   64,
 /*   610 */    94,   94,   94,   94,    4,   70,   71,   72,   73,   74,
 /*   620 */    75,   76,   77,   78,   79,   80,   81,   82,    3,    4,
 /*   630 */    94,   94,   87,   88,   24,   10,   11,   94,   94,   94,
 /*   640 */    15,   31,   32,   18,   19,   20,   94,   37,   38,   24,
 /*   650 */    40,   41,   42,   43,   44,   45,   31,   32,   50,   51,
 /*   660 */    94,   94,   37,   38,   94,   40,   94,   42,   43,   44,
 /*   670 */    45,   94,   64,   94,   94,   94,   94,   94,   70,   71,
 /*   680 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   690 */    82,   50,   51,   94,   86,   94,   94,   89,   25,   26,
 /*   700 */    27,   28,   29,   30,   94,   64,   94,   94,   50,   51,
 /*   710 */    94,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   720 */    79,   80,   81,   82,   94,   84,   94,   94,   94,   50,
 /*   730 */    51,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   740 */    82,   94,   94,   64,   94,   94,   94,   94,   94,   70,
 /*   750 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   760 */    81,   82,   50,   51,   94,   94,   94,   94,   89,   94,
 /*   770 */    94,   94,   94,   94,   94,   94,   64,   94,   94,   94,
 /*   780 */    50,   51,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   790 */    78,   79,   80,   81,   82,   94,   94,   85,   94,   94,
 /*   800 */    50,   51,   94,   94,   94,   75,   76,   77,   78,   79,
 /*   810 */    80,   81,   82,   94,   64,   94,   94,   94,   94,   94,
 /*   820 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   830 */    80,   81,   82,   50,   51,   94,   94,   94,   88,   94,
 /*   840 */    94,   94,   94,   94,   94,   94,   94,   64,   50,   51,
 /*   850 */    94,   94,   94,   70,   71,   72,   73,   74,   75,   76,
 /*   860 */    77,   78,   79,   80,   81,   82,   94,   94,   94,   94,
 /*   870 */    94,   50,   51,   75,   76,   77,   78,   79,   80,   81,
 /*   880 */    82,   50,   51,   94,   94,   64,   94,   94,   94,   94,
 /*   890 */    94,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   900 */    79,   80,   81,   82,   50,   51,   75,   76,   77,   78,
 /*   910 */    79,   80,   81,   82,   94,   94,   94,   94,   64,   94,
 /*   920 */    94,   94,   50,   51,   70,   71,   72,   73,   74,   75,
 /*   930 */    76,   77,   78,   79,   80,   81,   82,   94,   94,   94,
 /*   940 */    94,   94,   50,   51,   94,   94,   94,   75,   76,   77,
 /*   950 */    78,   79,   80,   81,   82,   94,   64,   94,   94,   94,
 /*   960 */    94,   94,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   970 */    78,   79,   80,   81,   82,   50,   51,   94,   94,   94,
 /*   980 */    94,   94,   94,   94,   94,   94,   94,   94,   94,   64,
 /*   990 */    94,   94,   94,   50,   51,   70,   71,   72,   73,   74,
 /*  1000 */    75,   76,   77,   78,   79,   80,   81,   82,   94,   94,
 /*  1010 */    94,   94,   94,   50,   51,   94,   94,   94,   75,   76,
 /*  1020 */    77,   78,   79,   80,   81,   82,   94,   64,   94,   94,
 /*  1030 */    94,   94,   94,   70,   71,   72,   73,   74,   75,   76,
 /*  1040 */    77,   78,   79,   80,   81,   82,   50,   51,   94,   94,
 /*  1050 */    94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
 /*  1060 */    64,   94,   94,   94,   50,   51,   70,   71,   72,   73,
 /*  1070 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   94,
 /*  1080 */    94,   94,   94,   94,   50,   51,   94,   94,   94,   75,
 /*  1090 */    76,   77,   78,   79,   80,   81,   82,   94,   64,   94,
 /*  1100 */    94,   94,   94,   94,   70,   71,   72,   73,   74,   75,
 /*  1110 */    76,   77,   78,   79,   80,   81,   82,   50,   51,   94,
 /*  1120 */    94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
 /*  1130 */    94,   64,   94,   94,   94,   50,   51,   70,   71,   72,
 /*  1140 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*  1150 */    94,   94,   94,   94,   94,   50,   51,   94,   94,   94,
 /*  1160 */    75,   76,   77,   78,   79,   80,   81,   82,   94,   64,
 /*  1170 */    94,   94,   94,   94,   94,   70,   71,   72,   73,   74,
 /*  1180 */    75,   76,   77,   78,   79,   80,   81,   82,   50,   51,
 /*  1190 */    94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
 /*  1200 */    94,   94,   64,   94,   94,   94,   50,   51,   70,   71,
 /*  1210 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*  1220 */    82,   94,   94,   94,   94,   94,   50,   51,   94,   94,
 /*  1230 */    94,   75,   76,   77,   78,   79,   80,   81,   82,   94,
 /*  1240 */    64,   94,   94,   94,   94,   94,   70,   71,   72,   73,
 /*  1250 */    74,   75,   76,   77,   78,   79,   80,   81,   82,    2,
 /*  1260 */    94,    4,   94,   94,    2,   94,   94,   10,   11,   94,
 /*  1270 */    94,    9,   15,   94,   12,   13,   14,   94,   94,   94,
 /*  1280 */    94,   24,   94,   94,   94,   94,   94,   94,   31,   32,
 /*  1290 */    94,   94,   94,   94,   37,   38,   94,   40,   94,   42,
 /*  1300 */    43,   44,   45,    2,   94,    4,   94,   94,   94,   94,
 /*  1310 */    94,   10,   11,   94,   94,   94,   15,   94,   94,   94,
 /*  1320 */    94,   94,   94,   50,   51,   24,   94,   94,   94,   94,
 /*  1330 */    94,   94,   31,   32,   94,   94,   63,   94,   37,   38,
 /*  1340 */    94,   40,   94,   42,   43,   44,   45,   94,   75,   76,
 /*  1350 */    77,   78,   79,   80,   81,   82,   50,   51,   94,   94,
 /*  1360 */    94,   94,   94,   94,   94,   92,   93,   94,   94,   63,
 /*  1370 */    94,   94,   94,   94,    4,   94,   94,    7,   94,   94,
 /*  1380 */    94,   75,   76,   77,   78,   79,   80,   81,   82,    4,
 /*  1390 */    94,   94,   94,   94,   24,   94,   94,   94,   92,   93,
 /*  1400 */    94,   31,   32,   94,   94,   94,    4,   37,   38,   24,
 /*  1410 */    40,   94,   42,   43,   44,   45,   31,   32,   94,   94,
 /*  1420 */    94,    4,   37,   38,   39,   40,   24,   42,   43,   44,
 /*  1430 */    45,   94,   94,   31,   32,   94,   94,   94,   94,   37,
 /*  1440 */    38,   24,   40,   94,   42,   43,   44,   45,   31,   32,
 /*  1450 */    94,   94,   94,   94,   37,   38,   94,   40,   94,   42,
 /*  1460 */    43,   94,   45,
};
#define YY_SHIFT_USE_DFLT (-1)
#define YY_SHIFT_MAX 90
static const short yy_shift_ofst[] = {
 /*     0 */   448,  625,  562, 1257, 1257, 1257, 1257, 1257, 1257, 1301,
 /*    10 */  1402, 1402, 1402, 1402, 1370, 1402, 1385,  610, 1402, 1402,
 /*    20 */  1402, 1402, 1402, 1402, 1402, 1402, 1402, 1402, 1402, 1402,
 /*    30 */  1402, 1402,  467,  467, 1417,  467,  467,  467,  467,  467,
 /*    40 */   467,  467,  467,  467,  467,  467,  467,  467,  467,  467,
 /*    50 */   467,  467,  253,  253,  177,  249,  673, 1262,   66,  135,
 /*    60 */    42,  168,  210,  101,  316,  138,   74,  245,  243,  216,
 /*    70 */   218,  211,   98,  140,  102,   91,   88,   40,   15,   11,
 /*    80 */   320,  323,  304,  318,  291,  309,  288,  285,  283,  284,
 /*    90 */   282,
};
#define YY_REDUCE_USE_DFLT (-48)
#define YY_REDUCE_MAX 55
static const short yy_reduce_ofst[] = {
 /*     0 */   -47,   -2,   44,  188,  152,  116,  260,  224,   80,  295,
 /*    10 */   480,  444,  331,  366,  608,  545,  641,  712,  679,  750,
 /*    20 */   996,  854, 1138,  925, 1067, 1034,  821, 1176,  892,  783,
 /*    30 */   963, 1105, 1273, 1306,  658, 1156, 1085,  872,  943,  730,
 /*    40 */  1014,  798,  831,  -42,    3,    5,  -40,   87,  123,  159,
 /*    50 */   195,  231,  124,  -21,  173,  199,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   292,  292,  292,  292,  292,  292,  292,  292,  292,  292,
 /*    10 */   292,  292,  292,  292,  292,  292,  292,  292,  292,  292,
 /*    20 */   292,  206,  292,  292,  282,  292,  292,  292,  292,  292,
 /*    30 */   292,  292,  292,  292,  292,  292,  292,  292,  292,  292,
 /*    40 */   292,  292,  288,  292,  292,  292,  292,  292,  292,  292,
 /*    50 */   292,  292,  292,  292,  255,  292,  231,  292,  245,  292,
 /*    60 */   292,  213,  292,  242,  292,  292,  292,  189,  290,  292,
 /*    70 */   292,  284,  262,  292,  292,  292,  292,  292,  292,  292,
 /*    80 */   198,  292,  292,  292,  292,  283,  292,  292,  289,  292,
 /*    90 */   292,  223,  224,  184,  188,  185,  186,  187,  192,  195,
 /*   100 */   196,  197,  208,  225,  226,  227,  228,  229,  230,  232,
 /*   110 */   238,  239,  240,  241,  243,  246,  250,  251,  252,  253,
 /*   120 */   254,  256,  257,  258,  259,  260,  261,  278,  191,  190,
 /*   130 */   265,  277,  279,  273,  272,  264,  263,  266,  267,  275,
 /*   140 */   276,  274,  268,  270,  271,  269,  247,  248,  249,  244,
 /*   150 */   233,  234,  235,  236,  237,  199,  200,  201,  286,  287,
 /*   160 */   291,  202,  203,  280,  281,  285,  204,  205,  207,  193,
 /*   170 */   194,  219,  209,  210,  211,  212,  214,  215,  216,  217,
 /*   180 */   218,  220,  221,  222,
};
#define YY_SZ_ACTTAB (sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  jx_py_ARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void jx_py_Trace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *yyTokenName[] = { 
  "$",             "ERROR",         "NEWLINE",       "DEF",         
  "NAME",          "COLON",         "OPEN_PAR",      "CLOSE_PAR",   
  "COMMA",         "SEMICOLON",     "DEL",           "PASS",        
  "PLUS_EQUALS",   "MINUS_EQUALS",  "EQUALS",        "PRINT",       
  "ELSE",          "ELIF",          "IF",            "WHILE",       
  "FOR",           "IN",            "INDENT",        "DEDENT",      
  "NOT",           "LESS_THAN",     "LESS_THAN_OR_EQ",  "EQ_EQ",       
  "NOT_EQ",        "GREATER_THAN",  "GREATER_THAN_OR_EQ",  "PLUS",        
  "MINUS",         "ASTERISK",      "SLASH",         "SLASHSLASH",  
  "PERCENT",       "TILDE",         "OPEN_RECT_BRACE",  "CLOSE_RECT_BRACE",
  "OPEN_CURLY_BRACE",  "CLOSE_CURLY_BRACE",  "ICON",          "FCON",        
  "LAMBDA",        "SCON",          "error",         "simple_stmt", 
  "small_stmt",    "print_stmt",    "string",        "atom",        
  "single_input",  "compound_stmt",  "funcdef",       "parameters",  
  "suite",         "varargslist",   "fpdefs",        "testlist",    
  "expr_stmt",     "del_stmt",      "pass_stmt",     "exprlist",    
  "test",          "if_stmt",       "while_stmt",    "for_stmt",    
  "if_head",       "stmts",         "or_test",       "lambdef",     
  "and_test",      "not_test",      "comparison",    "expr",        
  "xor_expr",      "and_expr",      "shift_expr",    "arith_expr",  
  "term",          "factor",        "power",         "trailer",     
  "listmaker",     "dictmaker",     "arglist",       "subscriptlist",
  "subscript",     "argument",      "testlist_2",    "testlist_3",  
  "exprlist_2",    "exprlist_3",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "single_input ::= NEWLINE",
 /*   1 */ "single_input ::= simple_stmt",
 /*   2 */ "single_input ::= compound_stmt NEWLINE",
 /*   3 */ "funcdef ::= DEF NAME parameters COLON suite",
 /*   4 */ "parameters ::= OPEN_PAR varargslist CLOSE_PAR",
 /*   5 */ "varargslist ::= fpdefs",
 /*   6 */ "fpdefs ::= NAME",
 /*   7 */ "fpdefs ::= fpdefs COMMA NAME",
 /*   8 */ "simple_stmt ::= small_stmt NEWLINE",
 /*   9 */ "simple_stmt ::= testlist NEWLINE",
 /*  10 */ "simple_stmt ::= testlist SEMICOLON NEWLINE",
 /*  11 */ "simple_stmt ::= small_stmt SEMICOLON NEWLINE",
 /*  12 */ "simple_stmt ::= small_stmt SEMICOLON small_stmt NEWLINE",
 /*  13 */ "small_stmt ::= expr_stmt",
 /*  14 */ "small_stmt ::= print_stmt",
 /*  15 */ "small_stmt ::= del_stmt",
 /*  16 */ "small_stmt ::= pass_stmt",
 /*  17 */ "del_stmt ::= DEL exprlist",
 /*  18 */ "pass_stmt ::= PASS",
 /*  19 */ "expr_stmt ::= testlist PLUS_EQUALS testlist",
 /*  20 */ "expr_stmt ::= testlist MINUS_EQUALS testlist",
 /*  21 */ "expr_stmt ::= testlist EQUALS testlist",
 /*  22 */ "print_stmt ::= PRINT",
 /*  23 */ "print_stmt ::= PRINT test",
 /*  24 */ "print_stmt ::= print_stmt COMMA test",
 /*  25 */ "compound_stmt ::= if_stmt",
 /*  26 */ "compound_stmt ::= while_stmt",
 /*  27 */ "compound_stmt ::= for_stmt",
 /*  28 */ "compound_stmt ::= funcdef",
 /*  29 */ "if_stmt ::= if_head",
 /*  30 */ "if_stmt ::= if_head ELSE COLON suite",
 /*  31 */ "if_head ::= if_head ELIF test COLON suite",
 /*  32 */ "if_head ::= IF test COLON suite",
 /*  33 */ "while_stmt ::= WHILE test COLON suite",
 /*  34 */ "for_stmt ::= FOR exprlist IN testlist COLON suite",
 /*  35 */ "suite ::= simple_stmt",
 /*  36 */ "suite ::= NEWLINE INDENT stmts DEDENT",
 /*  37 */ "stmts ::= stmts simple_stmt",
 /*  38 */ "stmts ::= stmts compound_stmt",
 /*  39 */ "stmts ::= simple_stmt",
 /*  40 */ "stmts ::= compound_stmt",
 /*  41 */ "test ::= or_test",
 /*  42 */ "test ::= lambdef",
 /*  43 */ "or_test ::= and_test",
 /*  44 */ "and_test ::= not_test",
 /*  45 */ "not_test ::= NOT not_test",
 /*  46 */ "not_test ::= comparison",
 /*  47 */ "comparison ::= expr",
 /*  48 */ "comparison ::= expr LESS_THAN expr",
 /*  49 */ "comparison ::= expr LESS_THAN_OR_EQ expr",
 /*  50 */ "comparison ::= expr EQ_EQ expr",
 /*  51 */ "comparison ::= expr NOT_EQ expr",
 /*  52 */ "comparison ::= expr GREATER_THAN expr",
 /*  53 */ "comparison ::= expr GREATER_THAN_OR_EQ expr",
 /*  54 */ "expr ::= xor_expr",
 /*  55 */ "xor_expr ::= and_expr",
 /*  56 */ "and_expr ::= shift_expr",
 /*  57 */ "shift_expr ::= arith_expr",
 /*  58 */ "arith_expr ::= term",
 /*  59 */ "arith_expr ::= term PLUS term",
 /*  60 */ "arith_expr ::= term MINUS term",
 /*  61 */ "term ::= factor",
 /*  62 */ "term ::= factor ASTERISK factor",
 /*  63 */ "term ::= factor SLASH factor",
 /*  64 */ "term ::= factor SLASHSLASH factor",
 /*  65 */ "term ::= factor PERCENT factor",
 /*  66 */ "factor ::= power",
 /*  67 */ "factor ::= PLUS factor",
 /*  68 */ "factor ::= MINUS factor",
 /*  69 */ "factor ::= TILDE factor",
 /*  70 */ "power ::= atom trailer",
 /*  71 */ "power ::= atom",
 /*  72 */ "atom ::= OPEN_RECT_BRACE CLOSE_RECT_BRACE",
 /*  73 */ "atom ::= OPEN_RECT_BRACE listmaker CLOSE_RECT_BRACE",
 /*  74 */ "atom ::= OPEN_CURLY_BRACE CLOSE_CURLY_BRACE",
 /*  75 */ "atom ::= OPEN_CURLY_BRACE dictmaker CLOSE_CURLY_BRACE",
 /*  76 */ "atom ::= ICON",
 /*  77 */ "atom ::= FCON",
 /*  78 */ "atom ::= string",
 /*  79 */ "listmaker ::= test",
 /*  80 */ "listmaker ::= listmaker COMMA test",
 /*  81 */ "lambdef ::= LAMBDA varargslist COLON test",
 /*  82 */ "trailer ::= OPEN_PAR CLOSE_PAR",
 /*  83 */ "trailer ::= OPEN_PAR arglist CLOSE_PAR",
 /*  84 */ "trailer ::= OPEN_RECT_BRACE subscriptlist CLOSE_RECT_BRACE",
 /*  85 */ "subscriptlist ::= subscript",
 /*  86 */ "subscriptlist ::= subscriptlist COMMA subscript",
 /*  87 */ "subscript ::= test",
 /*  88 */ "dictmaker ::= test COLON test",
 /*  89 */ "dictmaker ::= dictmaker COMMA test COLON test",
 /*  90 */ "arglist ::= argument",
 /*  91 */ "arglist ::= arglist COMMA argument",
 /*  92 */ "argument ::= test",
 /*  93 */ "string ::= SCON",
 /*  94 */ "string ::= string SCON",
 /*  95 */ "atom ::= NAME",
 /*  96 */ "testlist_2 ::= testlist_2 COMMA test",
 /*  97 */ "testlist_2 ::= test COMMA test",
 /*  98 */ "testlist_3 ::= test COMMA",
 /*  99 */ "testlist ::= test",
 /* 100 */ "testlist ::= testlist_2",
 /* 101 */ "testlist ::= testlist_3",
 /* 102 */ "exprlist_2 ::= exprlist_2 COMMA expr",
 /* 103 */ "exprlist_2 ::= expr COMMA expr",
 /* 104 */ "exprlist_3 ::= expr COMMA",
 /* 105 */ "exprlist ::= expr",
 /* 106 */ "exprlist ::= exprlist_2",
 /* 107 */ "exprlist ::= exprlist_3",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *jx_py_TokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to jx_py_ and jx_py_Free.
*/
void *jx_py_Alloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
#line 67 "jx_py_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 806 "jx_py_parse.c"
      break;
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
#line 59 "jx_py_parse.lem"
{ jx_ob_free((yypminor->yy67)); }
#line 815 "jx_py_parse.c"
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor( yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from jx_py_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void jx_py_Free(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  /* if( pParser->yyidx<0 ) return YY_NO_ACTION;  */
  i = yy_shift_ofst[stateno];
  if( i==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
    int iFallback;            /* Fallback token */
    if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
           && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
           yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
      }
#endif
      return yy_find_shift_action(pParser, iFallback);
    }
#endif
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  i = yy_reduce_ofst[stateno];
  if( i==YY_REDUCE_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     jx_py_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     jx_py_ARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 52, 1 },
  { 52, 1 },
  { 52, 2 },
  { 54, 5 },
  { 55, 3 },
  { 57, 1 },
  { 58, 1 },
  { 58, 3 },
  { 47, 2 },
  { 47, 2 },
  { 47, 3 },
  { 47, 3 },
  { 47, 4 },
  { 48, 1 },
  { 48, 1 },
  { 48, 1 },
  { 48, 1 },
  { 61, 2 },
  { 62, 1 },
  { 60, 3 },
  { 60, 3 },
  { 60, 3 },
  { 49, 1 },
  { 49, 2 },
  { 49, 3 },
  { 53, 1 },
  { 53, 1 },
  { 53, 1 },
  { 53, 1 },
  { 65, 1 },
  { 65, 4 },
  { 68, 5 },
  { 68, 4 },
  { 66, 4 },
  { 67, 6 },
  { 56, 1 },
  { 56, 4 },
  { 69, 2 },
  { 69, 2 },
  { 69, 1 },
  { 69, 1 },
  { 64, 1 },
  { 64, 1 },
  { 70, 1 },
  { 72, 1 },
  { 73, 2 },
  { 73, 1 },
  { 74, 1 },
  { 74, 3 },
  { 74, 3 },
  { 74, 3 },
  { 74, 3 },
  { 74, 3 },
  { 74, 3 },
  { 75, 1 },
  { 76, 1 },
  { 77, 1 },
  { 78, 1 },
  { 79, 1 },
  { 79, 3 },
  { 79, 3 },
  { 80, 1 },
  { 80, 3 },
  { 80, 3 },
  { 80, 3 },
  { 80, 3 },
  { 81, 1 },
  { 81, 2 },
  { 81, 2 },
  { 81, 2 },
  { 82, 2 },
  { 82, 1 },
  { 51, 2 },
  { 51, 3 },
  { 51, 2 },
  { 51, 3 },
  { 51, 1 },
  { 51, 1 },
  { 51, 1 },
  { 84, 1 },
  { 84, 3 },
  { 71, 4 },
  { 83, 2 },
  { 83, 3 },
  { 83, 3 },
  { 87, 1 },
  { 87, 3 },
  { 88, 1 },
  { 85, 3 },
  { 85, 5 },
  { 86, 1 },
  { 86, 3 },
  { 89, 1 },
  { 50, 1 },
  { 50, 2 },
  { 51, 1 },
  { 90, 3 },
  { 90, 3 },
  { 91, 2 },
  { 59, 1 },
  { 59, 1 },
  { 59, 1 },
  { 92, 3 },
  { 92, 3 },
  { 93, 2 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  jx_py_ARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<sizeof(yyRuleName)/sizeof(yyRuleName[0]) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0:
#line 116 "jx_py_parse.lem"
{  
#ifdef JX_PY_PARSER_DEBUG
  printf("single_input ::= NEWLINE ");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 1143 "jx_py_parse.c"
        break;
      case 1:
#line 123 "jx_py_parse.lem"
{  
#ifdef JX_PY_PARSER_DEBUG
  printf("single_input ::= simple_stmt(yymsp[0].minor.yy67).\n");
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[0].minor.yy67; /* save the result */
}
#line 1154 "jx_py_parse.c"
        break;
      case 2:
#line 132 "jx_py_parse.lem"
{  
#ifdef JX_PY_PARSER_DEBUG
  printf("single_input ::= compound_stmt(yymsp[-1].minor.yy0) NEWLINE.\n");
#endif
  jx_ob_free(context->result); 
  context->result = yymsp[-1].minor.yy0; 
  yy_destructor(2,&yymsp[0].minor);
}
#line 1166 "jx_py_parse.c"
        break;
      case 3:
#line 143 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("def"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(3,&yymsp[-4].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1179 "jx_py_parse.c"
        break;
      case 4:
#line 154 "jx_py_parse.lem"
{
  yygotominor.yy0=yymsp[-1].minor.yy0;
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 1188 "jx_py_parse.c"
        break;
      case 5:
#line 164 "jx_py_parse.lem"
{
  yygotominor.yy0=yymsp[0].minor.yy0;
}
#line 1195 "jx_py_parse.c"
        break;
      case 6:
      case 40:
      case 79:
      case 90:
#line 171 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1206 "jx_py_parse.c"
        break;
      case 7:
#line 177 "jx_py_parse.lem"
{
  yymsp[-2].minor.yy0= yygotominor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(8,&yymsp[-1].minor);
}
#line 1215 "jx_py_parse.c"
        break;
      case 8:
#line 188 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[-1].minor.yy67;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy67) ::= small_stmt(yymsp[-1].minor.yy67) NEWLINE.\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 1226 "jx_py_parse.c"
        break;
      case 9:
#line 196 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[-1].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy67) ::= small_stmt(yymsp[-1].minor.yy0) NEWLINE.\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 1237 "jx_py_parse.c"
        break;
      case 10:
#line 204 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[-2].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy67) ::= small_stmt(yymsp[-2].minor.yy0) NEWLINE.\n");
#endif
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1249 "jx_py_parse.c"
        break;
      case 11:
#line 212 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[-2].minor.yy67;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy67) ::= small_stmt(yymsp[-2].minor.yy67) SEMICOLON NEWLINE.\n");
#endif
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1261 "jx_py_parse.c"
        break;
      case 12:
#line 220 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[-3].minor.yy67;
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy67);
#ifdef JX_PY_PARSER_DEBUG
  printf("single_stmt(yygotominor.yy67) ::= small_stmt(yymsp[-3].minor.yy67) SEMICOLON small_stmt(yymsp[-1].minor.yy67).\n");
#endif
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1274 "jx_py_parse.c"
        break;
      case 13:
#line 232 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy67) ::= expr_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1284 "jx_py_parse.c"
        break;
      case 14:
#line 240 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[0].minor.yy67;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy67) ::= print_stmt(yymsp[0].minor.yy67).\n");
#endif
}
#line 1294 "jx_py_parse.c"
        break;
      case 15:
#line 248 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy67) ::= del_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1304 "jx_py_parse.c"
        break;
      case 16:
#line 256 "jx_py_parse.lem"
{  
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy67) ::= pass_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1314 "jx_py_parse.c"
        break;
      case 17:
#line 266 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("del"));
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("del_stmt(yygotominor.yy0) ::= DEL exprlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 1327 "jx_py_parse.c"
        break;
      case 18:
#line 278 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
#ifdef JX_PY_PARSER_DEBUG
  printf("pass_stmt(yygotominor.yy0) ::= PASS.\n");
#endif
  yy_destructor(11,&yymsp[0].minor);
}
#line 1338 "jx_py_parse.c"
        break;
      case 19:
#line 295 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("incr"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("expr_stmt(yygotominor.yy0) ::= testlist(yymsp[-2].minor.yy0) PLUS_EQUALS testlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(12,&yymsp[-1].minor);
}
#line 1352 "jx_py_parse.c"
        break;
      case 20:
#line 306 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("decr"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy0) ::= PRINT.\n");
#endif
  yy_destructor(13,&yymsp[-1].minor);
}
#line 1366 "jx_py_parse.c"
        break;
      case 21:
#line 317 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("set"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("expr_stmt(yygotominor.yy0) ::= testlist(yymsp[-2].minor.yy0) EQUALS testlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(14,&yymsp[-1].minor);
}
#line 1381 "jx_py_parse.c"
        break;
      case 22:
#line 332 "jx_py_parse.lem"
{  
   yygotominor.yy67 = jx_list_new();
   jx_list_append(yygotominor.yy67,jx_ob_from_ident("print"));
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy67) ::= PRINT..\n");
#endif
  yy_destructor(15,&yymsp[0].minor);
}
#line 1393 "jx_py_parse.c"
        break;
      case 23:
#line 341 "jx_py_parse.lem"
{  
   yygotominor.yy67 = jx_list_new();
   jx_list_append(yygotominor.yy67,jx_ob_from_ident("print"));
   jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy67) ::= PRINT test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(15,&yymsp[-1].minor);
}
#line 1406 "jx_py_parse.c"
        break;
      case 24:
#line 351 "jx_py_parse.lem"
{  
   yygotominor.yy67=yymsp[-2].minor.yy67;
   jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy67) ::= print_stmt(yymsp[-2].minor.yy67) COMMA test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 1418 "jx_py_parse.c"
        break;
      case 25:
      case 26:
      case 27:
      case 28:
#line 363 "jx_py_parse.lem"
{  
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("compound_stmt(yygotominor.yy0) ::= while_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1431 "jx_py_parse.c"
        break;
      case 29:
      case 87:
      case 92:
#line 397 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
}
#line 1440 "jx_py_parse.c"
        break;
      case 30:
#line 402 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-3].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1450 "jx_py_parse.c"
        break;
      case 31:
#line 408 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-4].minor.yy0;
   jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
   jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1461 "jx_py_parse.c"
        break;
      case 32:
#line 415 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("if"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("if_head(yygotominor.yy0) ::= IF test(yymsp[-2].minor.yy0) COLON suite(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1476 "jx_py_parse.c"
        break;
      case 33:
#line 429 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("while"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("while_stmt(yygotominor.yy0) ::= test(yymsp[0].minor.yy0) COLON suite(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1491 "jx_py_parse.c"
        break;
      case 34:
#line 442 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("foreach"));
  jx_list_append(yygotominor.yy0,yymsp[-4].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("while_stmt(yygotominor.yy0) ::= FOR exprlist(yymsp[-4].minor.yy0) IN testlist(yymsp[-2].minor.yy0) COLON suite(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1508 "jx_py_parse.c"
        break;
      case 35:
#line 456 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy67;
#ifdef JX_PY_PARSER_DEBUG
  printf("suite(yygotominor.yy0) ::= simple_stmt(yymsp[0].minor.yy67).\n");
#endif
}
#line 1518 "jx_py_parse.c"
        break;
      case 36:
#line 464 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("suite(yygotominor.yy0) ::= simple_stmt(yymsp[-1].minor.yy0).\n");
#endif
  yy_destructor(2,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
#line 1531 "jx_py_parse.c"
        break;
      case 37:
#line 475 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy67);
}
#line 1539 "jx_py_parse.c"
        break;
      case 38:
#line 482 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1547 "jx_py_parse.c"
        break;
      case 39:
#line 489 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy67);
}
#line 1555 "jx_py_parse.c"
        break;
      case 41:
#line 503 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("test(yygotominor.yy0) ::= or_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1565 "jx_py_parse.c"
        break;
      case 42:
#line 511 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf(".\n");
#endif
}
#line 1575 "jx_py_parse.c"
        break;
      case 43:
#line 521 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("or_test(yygotominor.yy0) ::= and_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1585 "jx_py_parse.c"
        break;
      case 44:
#line 531 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("and_test(yygotominor.yy0) ::= not_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1595 "jx_py_parse.c"
        break;
      case 45:
#line 541 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("not_test(yygotominor.yy0) ::= NOT not_test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(24,&yymsp[-1].minor);
}
#line 1606 "jx_py_parse.c"
        break;
      case 46:
#line 549 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("not_test(yygotominor.yy0) ::= comparison(yymsp[0].minor.yy0).\n");
#endif
}
#line 1616 "jx_py_parse.c"
        break;
      case 47:
#line 559 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1626 "jx_py_parse.c"
        break;
      case 48:
#line 567 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("lt"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(25,&yymsp[-1].minor);
}
#line 1641 "jx_py_parse.c"
        break;
      case 49:
#line 579 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("le"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(26,&yymsp[-1].minor);
}
#line 1656 "jx_py_parse.c"
        break;
      case 50:
#line 591 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("eq"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(27,&yymsp[-1].minor);
}
#line 1671 "jx_py_parse.c"
        break;
      case 51:
#line 603 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("eq"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(28,&yymsp[-1].minor);
}
#line 1686 "jx_py_parse.c"
        break;
      case 52:
#line 615 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("gt"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(29,&yymsp[-1].minor);
}
#line 1701 "jx_py_parse.c"
        break;
      case 53:
#line 627 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("ge"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(30,&yymsp[-1].minor);
}
#line 1716 "jx_py_parse.c"
        break;
      case 54:
#line 641 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("expr(yygotominor.yy0) ::= xor_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1726 "jx_py_parse.c"
        break;
      case 55:
#line 651 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("xor_expr(yygotominor.yy0) ::= and_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1736 "jx_py_parse.c"
        break;
      case 56:
#line 661 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("and_expr(yygotominor.yy0) ::= shift_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1746 "jx_py_parse.c"
        break;
      case 57:
#line 671 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("shirt_exp(yygotominor.yy0) ::= arith_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1756 "jx_py_parse.c"
        break;
      case 58:
#line 681 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[0].minor.yy0)\n");
#endif
}
#line 1766 "jx_py_parse.c"
        break;
      case 59:
#line 689 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("add"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[-2].minor.yy0) PLUS term(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(31,&yymsp[-1].minor);
}
#line 1780 "jx_py_parse.c"
        break;
      case 60:
#line 700 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("sub"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[-2].minor.yy0) MINUS term(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(32,&yymsp[-1].minor);
}
#line 1794 "jx_py_parse.c"
        break;
      case 61:
#line 713 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[0].minor.yy0).\n");
#endif
}
#line 1804 "jx_py_parse.c"
        break;
      case 62:
#line 721 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("mul"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(33,&yymsp[-1].minor);
}
#line 1818 "jx_py_parse.c"
        break;
      case 63:
#line 732 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("div"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(34,&yymsp[-1].minor);
}
#line 1832 "jx_py_parse.c"
        break;
      case 64:
#line 743 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("div"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(35,&yymsp[-1].minor);
}
#line 1846 "jx_py_parse.c"
        break;
      case 65:
#line 754 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("mod"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(36,&yymsp[-1].minor);
}
#line 1860 "jx_py_parse.c"
        break;
      case 66:
#line 767 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= power(yymsp[0].minor.yy0).\n");
#endif
}
#line 1870 "jx_py_parse.c"
        break;
      case 67:
#line 775 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= PLUS factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(31,&yymsp[-1].minor);
}
#line 1881 "jx_py_parse.c"
        break;
      case 68:
#line 783 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= PLUS factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(32,&yymsp[-1].minor);
}
#line 1892 "jx_py_parse.c"
        break;
      case 69:
#line 791 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= TILDE factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(37,&yymsp[-1].minor);
}
#line 1903 "jx_py_parse.c"
        break;
      case 70:
#line 813 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
    /*    
          if(jx_ob_identical(tmp,jx_ob_from_ident("get")))
    jx_ob tmp = jx_list_get(yygotominor.yy0,0);
      jx_list_insert(yygotominor.yy0,1,yymsp[-1].minor.yy67);
    else
  jx_ob_free(tmp);
    */
  jx_list_insert(yygotominor.yy0,0,yymsp[-1].minor.yy67);
#ifdef JX_PY_PARSER_DEBUG
  printf("power(yygotominor.yy0) ::= atom(yymsp[-1].minor.yy67) trailer(yymsp[0].minor.yy0).\n");
#endif
}
#line 1921 "jx_py_parse.c"
        break;
      case 71:
#line 829 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy67;
#ifdef JX_PY_PARSER_DEBUG
  printf("power(yygotominor.yy0) ::= atom(yymsp[0].minor.yy67).\n");
#endif
}
#line 1931 "jx_py_parse.c"
        break;
      case 72:
#line 856 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = jx_list_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
#line 1943 "jx_py_parse.c"
        break;
      case 73:
#line 864 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
#line 1955 "jx_py_parse.c"
        break;
      case 74:
#line 872 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = jx_hash_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
#line 1967 "jx_py_parse.c"
        break;
      case 75:
#line 881 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
#line 1979 "jx_py_parse.c"
        break;
      case 76:
#line 890 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = ICON\n"); 
#endif
}
#line 1989 "jx_py_parse.c"
        break;
      case 77:
#line 898 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = FCON\n"); 
#endif
}
#line 1999 "jx_py_parse.c"
        break;
      case 78:
#line 906 "jx_py_parse.lem"
{ 
  yygotominor.yy67 = yymsp[0].minor.yy67;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = string\n"); 
#endif
}
#line 2009 "jx_py_parse.c"
        break;
      case 80:
      case 86:
      case 91:
#line 922 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2020 "jx_py_parse.c"
        break;
      case 81:
#line 930 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("lambda"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 2032 "jx_py_parse.c"
        break;
      case 82:
#line 940 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf("trailer(yygotominor.yy0) ::= OPEN_PAR CLOSE_PAR.\n"); 
#endif
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 2044 "jx_py_parse.c"
        break;
      case 83:
#line 948 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("trailer(yygotominor.yy0) ::= OPEN_PAR arglist(yymsp[-1].minor.yy0) CLOSE_PAR.\n"); 
#endif
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 2056 "jx_py_parse.c"
        break;
      case 84:
#line 956 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("trailer(yygotominor.yy0) ::= OPEN_RECT_BRACKET subscriptlist(yymsp[-1].minor.yy0) CLOSE_RECT_BRACKET.\n"); 
#endif
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
#line 2068 "jx_py_parse.c"
        break;
      case 85:
#line 966 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("get"));
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 2077 "jx_py_parse.c"
        break;
      case 88:
#line 988 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_hash_new();
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);  
  yy_destructor(5,&yymsp[-1].minor);
}
#line 2086 "jx_py_parse.c"
        break;
      case 89:
#line 994 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-4].minor.yy0;
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);  
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 2096 "jx_py_parse.c"
        break;
      case 93:
#line 1023 "jx_py_parse.lem"
{
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("string = SCON\n"); 
#endif
}
#line 2106 "jx_py_parse.c"
        break;
      case 94:
#line 1033 "jx_py_parse.lem"
{
  yygotominor.yy67 = jx_ob_add(yymsp[-1].minor.yy67,yymsp[0].minor.yy0);
  jx_ob_free(yymsp[-1].minor.yy67);
  jx_ob_free(yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = SCON\n"); 
#endif
}
#line 2118 "jx_py_parse.c"
        break;
      case 95:
#line 1043 "jx_py_parse.lem"
{
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = NAME\n"); 
#endif
}
#line 2128 "jx_py_parse.c"
        break;
      case 96:
#line 1053 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_2(yygotominor.yy0) ::= testlist_2(yymsp[-2].minor.yy0) COMMA test(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2141 "jx_py_parse.c"
        break;
      case 97:
#line 1063 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_2(yygotominor.yy0) ::= test(yymsp[-2].minor.yy0) COMMA test(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2154 "jx_py_parse.c"
        break;
      case 98:
#line 1073 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_3(yygotominor.yy0) ::= test(yymsp[-1].minor.yy0) COMMA.\n"); 
#endif
  yy_destructor(8,&yymsp[0].minor);
}
#line 2166 "jx_py_parse.c"
        break;
      case 99:
#line 1082 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= test(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2176 "jx_py_parse.c"
        break;
      case 100:
#line 1090 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= testlist_2(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2186 "jx_py_parse.c"
        break;
      case 101:
#line 1098 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= testlist_3(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2196 "jx_py_parse.c"
        break;
      case 102:
#line 1108 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_2(yygotominor.yy0) ::= exprlist_2(yymsp[-2].minor.yy0) COMMA expr(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2209 "jx_py_parse.c"
        break;
      case 103:
#line 1118 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_2(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMMA expr(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2222 "jx_py_parse.c"
        break;
      case 104:
#line 1128 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_3(yygotominor.yy0) ::= expr(yymsp[-1].minor.yy0) COMMA.\n"); 
#endif
  yy_destructor(8,&yymsp[0].minor);
}
#line 2234 "jx_py_parse.c"
        break;
      case 105:
#line 1137 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= expr(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2244 "jx_py_parse.c"
        break;
      case 106:
#line 1145 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= exprlist_2(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2254 "jx_py_parse.c"
        break;
      case 107:
#line 1153 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= exprlist_3(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2264 "jx_py_parse.c"
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yypParser,yygoto);
  if( yyact < YYNSTATE ){
    yy_shift(yypParser,yyact,yygoto,&yygotominor);
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  jx_py_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 84 "jx_py_parse.lem"

  context->status = JX_STATUS_SYNTAX_ERROR;
#ifdef JX_PY_PARSER_DEBUG
  printf("parse failed.\n");
#endif
#line 2300 "jx_py_parse.c"
  jx_py_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  jx_py_ARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 93 "jx_py_parse.lem"
  
#ifdef JX_PY_PARSER_DEBUG
  printf("jx_py_parse-error: syntax error.\n");
#endif
#line 2320 "jx_py_parse.c"
  jx_py_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  jx_py_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 75 "jx_py_parse.lem"

    context->status = 1;
#ifdef JX_PY_PARSER_DEBUG
  printf("accept called\n");
#endif
#line 2346 "jx_py_parse.c"
  jx_py_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "jx_py_Alloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void jx_py_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  jx_py_TOKENTYPE yyminor       /* The value for the token */
  jx_py_ARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    if( yymajor==0 ) return;
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  jx_py_ARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
      int yymx;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_shift_action(yypParser,YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
