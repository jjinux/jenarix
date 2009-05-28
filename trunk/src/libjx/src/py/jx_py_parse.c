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
#define YYNOCODE 96
#define YYACTIONTYPE unsigned short int
#define jx_py_TOKENTYPE jx_ob
typedef union {
  jx_py_TOKENTYPE yy0;
  jx_ob yy73;
  int yy191;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_py_ARG_SDECL jx_py_parse_context *context;
#define jx_py_ARG_PDECL ,jx_py_parse_context *context
#define jx_py_ARG_FETCH jx_py_parse_context *context = yypParser->context
#define jx_py_ARG_STORE yypParser->context = context
#define YYNSTATE 185
#define YYNRULE 109
#define YYERRORSYMBOL 46
#define YYERRSYMDT yy191
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
 /*     0 */   163,   60,   72,   73,   52,   27,  167,  153,   48,   51,
 /*    10 */    47,   50,   57,  166,  115,  116,   30,   81,  150,  151,
 /*    20 */   152,   66,    2,  162,  160,  158,  156,  149,   56,  141,
 /*    30 */   139,  137,  134,   63,   58,  127,  172,   60,   72,   73,
 /*    40 */    52,  295,   68,  153,   80,  136,   84,   21,   57,  166,
 /*    50 */   115,  116,   18,   81,  150,  151,  152,   66,  184,  162,
 /*    60 */   160,  158,  156,  149,   56,  141,  139,  137,  134,   63,
 /*    70 */    58,  127,  155,   60,   72,   73,   52,   29,  159,  153,
 /*    80 */    80,  136,   54,   92,   57,  166,  115,  116,   70,   81,
 /*    90 */   150,  151,  152,   66,   23,  162,  160,  158,  156,  149,
 /*   100 */    56,  141,  139,  137,  134,   63,   58,  127,  107,  147,
 /*   110 */    60,   72,   73,   52,   67,   74,   80,  136,  130,   43,
 /*   120 */    44,   57,  166,  115,  116,  122,   81,  103,    8,   10,
 /*   130 */    13,   12,  162,  160,  158,  156,  149,   56,  141,  139,
 /*   140 */   137,  134,   63,   58,  127,   11,  147,   60,   72,   73,
 /*   150 */    52,  179,   19,   80,  136,  165,   14,  119,   57,  166,
 /*   160 */   115,  116,    7,   81,    9,   90,   74,  113,  111,  162,
 /*   170 */   160,  158,  156,  149,   56,  141,  139,  137,  134,   63,
 /*   180 */    58,  127,    6,  147,   60,   72,   73,   52,   15,    5,
 /*   190 */    80,  136,  169,    4,    1,   57,  166,  115,  116,  146,
 /*   200 */    81,   22,   26,  145,   55,  168,  162,  160,  158,  156,
 /*   210 */   149,   56,  141,  139,  137,  134,   63,   58,  127,   25,
 /*   220 */   147,   60,   72,   73,   52,   35,    3,   80,  136,  133,
 /*   230 */    37,   31,   57,  166,  115,  116,   75,   81,   93,  296,
 /*   240 */    97,  296,  296,  162,  160,  158,  156,  149,   56,  141,
 /*   250 */   139,  137,  134,   63,   58,  127,  296,  147,   60,   72,
 /*   260 */    73,   52,  296,  296,   80,  136,  157,  296,  296,   57,
 /*   270 */   166,  115,  116,  296,   81,  296,  296,  296,  296,  296,
 /*   280 */   162,  160,  158,  156,  149,   56,  141,  139,  137,  134,
 /*   290 */    63,   58,  127,  296,  147,   60,   72,   73,   52,  296,
 /*   300 */   296,   80,  136,  161,   73,   52,   57,  166,  115,  116,
 /*   310 */   296,   81,  296,  296,  296,  296,  296,  162,  160,  158,
 /*   320 */   156,  149,   56,  141,  139,  137,  134,   63,   58,  127,
 /*   330 */    71,   72,   73,   52,  104,   58,  127,  296,   80,  136,
 /*   340 */   296,   59,  166,  115,  116,  296,   81,  296,   73,   52,
 /*   350 */   296,  296,  162,  160,  158,  156,  149,   56,  141,  139,
 /*   360 */   137,  134,   63,   58,  127,  296,  296,  296,   73,   52,
 /*   370 */    73,   52,  296,   80,  136,   73,   52,  140,  131,   58,
 /*   380 */   127,  296,   81,   73,   52,  296,   73,   52,  162,  160,
 /*   390 */   158,  156,  149,   56,  141,  139,  137,  134,   63,   58,
 /*   400 */   127,  102,  127,   91,   69,  173,  100,  127,  296,   80,
 /*   410 */   136,   33,  126,  296,  117,  127,   24,  120,  127,   20,
 /*   420 */    28,   32,  296,  296,  144,   34,   73,   52,  296,   73,
 /*   430 */    52,   82,   46,   49,   10,   13,   12,  296,   45,   17,
 /*   440 */   296,   16,  296,  101,   99,   53,  178,   69,  173,  296,
 /*   450 */   296,  296,  296,  296,   33,  126,  296,   98,  127,   24,
 /*   460 */   123,  127,   20,   28,   32,  296,  296,  148,   34,  296,
 /*   470 */   296,   73,   52,  296,  296,   46,   49,   73,   52,  296,
 /*   480 */   138,   45,   17,  296,   16,   81,  101,   99,   53,  178,
 /*   490 */   296,  162,  160,  158,  156,  149,   56,  141,  139,  137,
 /*   500 */   134,   63,   58,  127,  296,   73,   52,  296,  128,  127,
 /*   510 */   296,  296,   80,  136,  129,  296,  296,  296,  296,   81,
 /*   520 */   296,  296,  296,  296,  296,  162,  160,  158,  156,  149,
 /*   530 */    56,  141,  139,  137,  134,   63,   58,  127,  296,   73,
 /*   540 */    52,  296,  296,  296,  296,  296,   80,  136,   89,  296,
 /*   550 */   296,  296,  296,   81,  296,  296,  296,   73,   52,  162,
 /*   560 */   160,  158,  156,  149,   56,  141,  139,  137,  134,   63,
 /*   570 */    58,  127,  296,  296,  296,  296,   73,   52,  296,  296,
 /*   580 */    80,  136,  114,  141,  139,  137,  134,   63,   58,  127,
 /*   590 */   182,  296,  296,  296,  296,  296,  162,  160,  158,  156,
 /*   600 */   149,   56,  141,  139,  137,  134,   63,   58,  127,  296,
 /*   610 */    69,  173,  296,   61,  296,  296,  183,   33,  126,  296,
 /*   620 */   296,  296,   24,  296,  296,   20,   28,   32,  296,  296,
 /*   630 */   296,   34,  296,  296,  296,  296,  296,  296,   46,   49,
 /*   640 */   296,   73,   52,  296,   45,   17,  296,   16,  296,  101,
 /*   650 */    99,   53,  178,  296,  296,   95,   73,   52,  296,  296,
 /*   660 */   296,  162,  160,  158,  156,  149,   56,  141,  139,  137,
 /*   670 */   134,   63,   58,  127,  296,  296,   73,   52,  296,   64,
 /*   680 */    96,  112,  141,  139,  137,  134,   63,   58,  127,  296,
 /*   690 */    77,  296,   73,   52,  296,  296,  162,  160,  158,  156,
 /*   700 */   149,   56,  141,  139,  137,  134,   63,   58,  127,  296,
 /*   710 */   296,  296,   65,   73,   52,  154,  149,   56,  141,  139,
 /*   720 */   137,  134,   63,   58,  127,  296,  173,  176,  296,  296,
 /*   730 */   296,  296,  296,  162,  160,  158,  156,  149,   56,  141,
 /*   740 */   139,  137,  134,   63,   58,  127,   34,  296,   62,   73,
 /*   750 */    52,  296,  296,   46,   49,  296,  296,  296,  296,   45,
 /*   760 */    17,  296,   16,  182,  101,   99,   53,  178,  296,  162,
 /*   770 */   160,  158,  156,  149,   56,  141,  139,  137,  134,   63,
 /*   780 */    58,  127,  296,  296,   73,   52,  296,  296,  296,  181,
 /*   790 */    36,   42,   41,   40,   39,   38,  296,  296,   95,   73,
 /*   800 */    52,  296,  296,  296,  162,  160,  158,  156,  149,   56,
 /*   810 */   141,  139,  137,  134,   63,   58,  127,  296,  296,  296,
 /*   820 */    73,   52,  296,   94,  110,  141,  139,  137,  134,   63,
 /*   830 */    58,  127,   73,   52,   76,  296,  296,  296,  296,  296,
 /*   840 */   162,  160,  158,  156,  149,   56,  141,  139,  137,  134,
 /*   850 */    63,   58,  127,  296,  296,   73,   52,  108,  141,  139,
 /*   860 */   137,  134,   63,   58,  127,  296,  296,  296,  296,  174,
 /*   870 */   296,   73,   52,  296,  296,  162,  160,  158,  156,  149,
 /*   880 */    56,  141,  139,  137,  134,   63,   58,  127,  296,  296,
 /*   890 */   296,   73,   52,  296,  296,  296,  106,  141,  139,  137,
 /*   900 */   134,   63,   58,  127,  296,  164,   73,   52,  296,  296,
 /*   910 */   296,  162,  160,  158,  156,  149,   56,  141,  139,  137,
 /*   920 */   134,   63,   58,  127,  296,  296,   73,   52,  296,  296,
 /*   930 */   296,  124,  141,  139,  137,  134,   63,   58,  127,  296,
 /*   940 */   175,  296,   73,   52,  296,  296,  162,  160,  158,  156,
 /*   950 */   149,   56,  141,  139,  137,  134,   63,   58,  127,  296,
 /*   960 */   296,  296,   73,   52,  296,  296,  296,  143,  141,  139,
 /*   970 */   137,  134,   63,   58,  127,  296,   87,   73,   52,  296,
 /*   980 */   296,  296,  162,  160,  158,  156,  149,   56,  141,  139,
 /*   990 */   137,  134,   63,   58,  127,  296,  296,   73,   52,  296,
 /*  1000 */   296,  296,  121,  141,  139,  137,  134,   63,   58,  127,
 /*  1010 */   173,  171,  296,  296,  296,  296,  296,  162,  160,  158,
 /*  1020 */   156,  149,   56,  141,  139,  137,  134,   63,   58,  127,
 /*  1030 */   296,  296,  296,   73,   52,  296,  296,   46,   49,  296,
 /*  1040 */   296,  296,  296,   45,   17,  296,   16,  135,  101,   99,
 /*  1050 */   296,  178,  296,  162,  160,  158,  156,  149,   56,  141,
 /*  1060 */   139,  137,  134,   63,   58,  127,  296,  296,   73,   52,
 /*  1070 */   296,  296,  296,  296,  296,  296,  296,  296,  296,  296,
 /*  1080 */   296,  296,  132,  296,  296,  296,  296,  296,  162,  160,
 /*  1090 */   158,  156,  149,   56,  141,  139,  137,  134,   63,   58,
 /*  1100 */   127,  296,  296,  296,   73,   52,  296,  296,  296,  296,
 /*  1110 */   296,  296,  296,  296,  296,  296,  296,  296,  142,  296,
 /*  1120 */   296,  296,  296,  296,  162,  160,  158,  156,  149,   56,
 /*  1130 */   141,  139,  137,  134,   63,   58,  127,  296,  296,   73,
 /*  1140 */    52,  296,  296,  296,  296,  296,  296,  296,  296,  296,
 /*  1150 */   296,  296,  296,  180,  296,  296,  296,  296,  296,  162,
 /*  1160 */   160,  158,  156,  149,   56,  141,  139,  137,  134,   63,
 /*  1170 */    58,  127,  296,  296,  296,   73,   52,  296,  296,  296,
 /*  1180 */   296,  296,  296,  296,  296,  296,  296,  296,  296,   85,
 /*  1190 */   296,  296,  296,  296,  296,  162,  160,  158,  156,  149,
 /*  1200 */    56,  141,  139,  137,  134,   63,   58,  127,  296,  296,
 /*  1210 */    73,   52,  296,  296,  296,  296,  296,  296,  296,  296,
 /*  1220 */   296,  296,  296,  296,   86,  296,  296,  296,  296,  296,
 /*  1230 */   162,  160,  158,  156,  149,   56,  141,  139,  137,  134,
 /*  1240 */    63,   58,  127,   83,  296,  173,  296,  296,  296,  296,
 /*  1250 */   296,   33,  126,  296,  296,  296,   24,  296,  296,  296,
 /*  1260 */   296,  296,  296,  296,  296,   34,  296,  296,  296,  296,
 /*  1270 */   296,  296,   46,   49,  296,  296,  296,  296,   45,   17,
 /*  1280 */   296,   16,  296,  101,   99,   53,  178,  170,  296,  173,
 /*  1290 */   296,  296,  296,  296,  296,   33,  126,  296,  296,  296,
 /*  1300 */    24,  296,  296,  296,  296,  296,  296,   73,   52,   34,
 /*  1310 */   296,  296,  296,  296,  296,  296,   46,   49,  296,  296,
 /*  1320 */    88,  296,   45,   17,  296,   16,  296,  101,   99,   53,
 /*  1330 */   178,  296,   79,  141,  139,  137,  134,   63,   58,  127,
 /*  1340 */    73,   52,  296,  296,  296,  296,  296,  296,  296,  296,
 /*  1350 */    78,  125,  296,  118,  296,  296,  296,  296,  173,  296,
 /*  1360 */   296,  296,  296,  296,  296,   79,  141,  139,  137,  134,
 /*  1370 */    63,   58,  127,  296,  296,  173,  296,  296,   34,  296,
 /*  1380 */   296,  296,  296,   78,  125,   46,   49,  296,  296,  296,
 /*  1390 */   296,   45,   17,  109,   16,   34,  101,   99,   53,  178,
 /*  1400 */   296,  173,   46,   49,  177,  296,  296,  296,   45,   17,
 /*  1410 */   296,   16,  105,  101,   99,   53,  178,  296,  173,  296,
 /*  1420 */   296,   34,  296,  296,  296,  296,  296,  296,   46,   49,
 /*  1430 */   296,  296,  296,  296,   45,   17,  296,   16,   34,  101,
 /*  1440 */    99,   53,  178,  296,  296,   46,   49,  296,  296,  296,
 /*  1450 */   296,   45,   17,  296,   16,  296,  101,   99,  296,  178,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    47,   48,   49,   50,   51,    5,   53,   54,   33,   34,
 /*    10 */    35,   36,   59,   60,   61,   62,    8,   64,   65,   66,
 /*    20 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*    30 */    77,   78,   79,   80,   81,   82,   47,   48,   49,   50,
 /*    40 */    51,   52,   53,   54,   91,   92,   16,   17,   59,   60,
 /*    50 */    61,   62,    8,   64,   65,   66,   67,   68,    4,   70,
 /*    60 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*    70 */    81,   82,   47,   48,   49,   50,   51,    8,   53,   54,
 /*    80 */    91,   92,    6,   39,   59,   60,   61,   62,   55,   64,
 /*    90 */    65,   66,   67,   68,    8,   70,   71,   72,   73,   74,
 /*   100 */    75,   76,   77,   78,   79,   80,   81,   82,   39,   47,
 /*   110 */    48,   49,   50,   51,   57,   58,   91,   92,   56,   31,
 /*   120 */    32,   59,   60,   61,   62,    7,   64,   41,    5,   12,
 /*   130 */    13,   14,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   140 */    78,   79,   80,   81,   82,   21,   47,   48,   49,   50,
 /*   150 */    51,    7,    8,   91,   92,   56,    6,    2,   59,   60,
 /*   160 */    61,   62,    5,   64,    9,   57,   58,   83,   84,   70,
 /*   170 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   180 */    81,   82,    5,   47,   48,   49,   50,   51,   38,    5,
 /*   190 */    91,   92,   56,    5,   22,   59,   60,   61,   62,    2,
 /*   200 */    64,    5,    8,    2,    4,    2,   70,   71,   72,   73,
 /*   210 */    74,   75,   76,   77,   78,   79,   80,   81,   82,    8,
 /*   220 */    47,   48,   49,   50,   51,    8,    5,   91,   92,   56,
 /*   230 */     8,    5,   59,   60,   61,   62,    8,   64,    4,   95,
 /*   240 */    45,   95,   95,   70,   71,   72,   73,   74,   75,   76,
 /*   250 */    77,   78,   79,   80,   81,   82,   95,   47,   48,   49,
 /*   260 */    50,   51,   95,   95,   91,   92,   56,   95,   95,   59,
 /*   270 */    60,   61,   62,   95,   64,   95,   95,   95,   95,   95,
 /*   280 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   290 */    80,   81,   82,   95,   47,   48,   49,   50,   51,   95,
 /*   300 */    95,   91,   92,   56,   50,   51,   59,   60,   61,   62,
 /*   310 */    95,   64,   95,   95,   95,   95,   95,   70,   71,   72,
 /*   320 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   330 */    48,   49,   50,   51,   80,   81,   82,   95,   91,   92,
 /*   340 */    95,   59,   60,   61,   62,   95,   64,   95,   50,   51,
 /*   350 */    95,   95,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   360 */    78,   79,   80,   81,   82,   95,   95,   95,   50,   51,
 /*   370 */    50,   51,   95,   91,   92,   50,   51,   59,   80,   81,
 /*   380 */    82,   95,   64,   50,   51,   95,   50,   51,   70,   71,
 /*   390 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*   400 */    82,   81,   82,    2,    3,    4,   81,   82,   95,   91,
 /*   410 */    92,   10,   11,   95,   81,   82,   15,   81,   82,   18,
 /*   420 */    19,   20,   95,   95,    2,   24,   50,   51,   95,   50,
 /*   430 */    51,    9,   31,   32,   12,   13,   14,   95,   37,   38,
 /*   440 */    95,   40,   95,   42,   43,   44,   45,    3,    4,   95,
 /*   450 */    95,   95,   95,   95,   10,   11,   95,   81,   82,   15,
 /*   460 */    81,   82,   18,   19,   20,   95,   95,   23,   24,   95,
 /*   470 */    95,   50,   51,   95,   95,   31,   32,   50,   51,   95,
 /*   480 */    59,   37,   38,   95,   40,   64,   42,   43,   44,   45,
 /*   490 */    95,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   500 */    79,   80,   81,   82,   95,   50,   51,   95,   81,   82,
 /*   510 */    95,   95,   91,   92,   59,   95,   95,   95,   95,   64,
 /*   520 */    95,   95,   95,   95,   95,   70,   71,   72,   73,   74,
 /*   530 */    75,   76,   77,   78,   79,   80,   81,   82,   95,   50,
 /*   540 */    51,   95,   95,   95,   95,   95,   91,   92,   59,   95,
 /*   550 */    95,   95,   95,   64,   95,   95,   95,   50,   51,   70,
 /*   560 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   570 */    81,   82,   95,   95,   95,   95,   50,   51,   95,   95,
 /*   580 */    91,   92,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   590 */    64,   95,   95,   95,   95,   95,   70,   71,   72,   73,
 /*   600 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   95,
 /*   610 */     3,    4,   95,   87,   95,   95,   90,   10,   11,   95,
 /*   620 */    95,   95,   15,   95,   95,   18,   19,   20,   95,   95,
 /*   630 */    95,   24,   95,   95,   95,   95,   95,   95,   31,   32,
 /*   640 */    95,   50,   51,   95,   37,   38,   95,   40,   95,   42,
 /*   650 */    43,   44,   45,   95,   95,   64,   50,   51,   95,   95,
 /*   660 */    95,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   670 */    79,   80,   81,   82,   95,   95,   50,   51,   95,   88,
 /*   680 */    89,   75,   76,   77,   78,   79,   80,   81,   82,   95,
 /*   690 */    64,   95,   50,   51,   95,   95,   70,   71,   72,   73,
 /*   700 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   95,
 /*   710 */    95,   95,   86,   50,   51,   73,   74,   75,   76,   77,
 /*   720 */    78,   79,   80,   81,   82,   95,    4,   64,   95,   95,
 /*   730 */    95,   95,   95,   70,   71,   72,   73,   74,   75,   76,
 /*   740 */    77,   78,   79,   80,   81,   82,   24,   95,   85,   50,
 /*   750 */    51,   95,   95,   31,   32,   95,   95,   95,   95,   37,
 /*   760 */    38,   95,   40,   64,   42,   43,   44,   45,   95,   70,
 /*   770 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   780 */    81,   82,   95,   95,   50,   51,   95,   95,   95,   90,
 /*   790 */    25,   26,   27,   28,   29,   30,   95,   95,   64,   50,
 /*   800 */    51,   95,   95,   95,   70,   71,   72,   73,   74,   75,
 /*   810 */    76,   77,   78,   79,   80,   81,   82,   95,   95,   95,
 /*   820 */    50,   51,   95,   89,   75,   76,   77,   78,   79,   80,
 /*   830 */    81,   82,   50,   51,   64,   95,   95,   95,   95,   95,
 /*   840 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   850 */    80,   81,   82,   95,   95,   50,   51,   75,   76,   77,
 /*   860 */    78,   79,   80,   81,   82,   95,   95,   95,   95,   64,
 /*   870 */    95,   50,   51,   95,   95,   70,   71,   72,   73,   74,
 /*   880 */    75,   76,   77,   78,   79,   80,   81,   82,   95,   95,
 /*   890 */    95,   50,   51,   95,   95,   95,   75,   76,   77,   78,
 /*   900 */    79,   80,   81,   82,   95,   64,   50,   51,   95,   95,
 /*   910 */    95,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   920 */    79,   80,   81,   82,   95,   95,   50,   51,   95,   95,
 /*   930 */    95,   75,   76,   77,   78,   79,   80,   81,   82,   95,
 /*   940 */    64,   95,   50,   51,   95,   95,   70,   71,   72,   73,
 /*   950 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   95,
 /*   960 */    95,   95,   50,   51,   95,   95,   95,   75,   76,   77,
 /*   970 */    78,   79,   80,   81,   82,   95,   64,   50,   51,   95,
 /*   980 */    95,   95,   70,   71,   72,   73,   74,   75,   76,   77,
 /*   990 */    78,   79,   80,   81,   82,   95,   95,   50,   51,   95,
 /*  1000 */    95,   95,   75,   76,   77,   78,   79,   80,   81,   82,
 /*  1010 */     4,   64,   95,   95,   95,   95,   95,   70,   71,   72,
 /*  1020 */    73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
 /*  1030 */    95,   95,   95,   50,   51,   95,   95,   31,   32,   95,
 /*  1040 */    95,   95,   95,   37,   38,   95,   40,   64,   42,   43,
 /*  1050 */    95,   45,   95,   70,   71,   72,   73,   74,   75,   76,
 /*  1060 */    77,   78,   79,   80,   81,   82,   95,   95,   50,   51,
 /*  1070 */    95,   95,   95,   95,   95,   95,   95,   95,   95,   95,
 /*  1080 */    95,   95,   64,   95,   95,   95,   95,   95,   70,   71,
 /*  1090 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   81,
 /*  1100 */    82,   95,   95,   95,   50,   51,   95,   95,   95,   95,
 /*  1110 */    95,   95,   95,   95,   95,   95,   95,   95,   64,   95,
 /*  1120 */    95,   95,   95,   95,   70,   71,   72,   73,   74,   75,
 /*  1130 */    76,   77,   78,   79,   80,   81,   82,   95,   95,   50,
 /*  1140 */    51,   95,   95,   95,   95,   95,   95,   95,   95,   95,
 /*  1150 */    95,   95,   95,   64,   95,   95,   95,   95,   95,   70,
 /*  1160 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*  1170 */    81,   82,   95,   95,   95,   50,   51,   95,   95,   95,
 /*  1180 */    95,   95,   95,   95,   95,   95,   95,   95,   95,   64,
 /*  1190 */    95,   95,   95,   95,   95,   70,   71,   72,   73,   74,
 /*  1200 */    75,   76,   77,   78,   79,   80,   81,   82,   95,   95,
 /*  1210 */    50,   51,   95,   95,   95,   95,   95,   95,   95,   95,
 /*  1220 */    95,   95,   95,   95,   64,   95,   95,   95,   95,   95,
 /*  1230 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*  1240 */    80,   81,   82,    2,   95,    4,   95,   95,   95,   95,
 /*  1250 */    95,   10,   11,   95,   95,   95,   15,   95,   95,   95,
 /*  1260 */    95,   95,   95,   95,   95,   24,   95,   95,   95,   95,
 /*  1270 */    95,   95,   31,   32,   95,   95,   95,   95,   37,   38,
 /*  1280 */    95,   40,   95,   42,   43,   44,   45,    2,   95,    4,
 /*  1290 */    95,   95,   95,   95,   95,   10,   11,   95,   95,   95,
 /*  1300 */    15,   95,   95,   95,   95,   95,   95,   50,   51,   24,
 /*  1310 */    95,   95,   95,   95,   95,   95,   31,   32,   95,   95,
 /*  1320 */    63,   95,   37,   38,   95,   40,   95,   42,   43,   44,
 /*  1330 */    45,   95,   75,   76,   77,   78,   79,   80,   81,   82,
 /*  1340 */    50,   51,   95,   95,   95,   95,   95,   95,   95,   95,
 /*  1350 */    93,   94,   95,   63,   95,   95,   95,   95,    4,   95,
 /*  1360 */    95,   95,   95,   95,   95,   75,   76,   77,   78,   79,
 /*  1370 */    80,   81,   82,   95,   95,    4,   95,   95,   24,   95,
 /*  1380 */    95,   95,   95,   93,   94,   31,   32,   95,   95,   95,
 /*  1390 */    95,   37,   38,   39,   40,   24,   42,   43,   44,   45,
 /*  1400 */    95,    4,   31,   32,    7,   95,   95,   95,   37,   38,
 /*  1410 */    95,   40,   41,   42,   43,   44,   45,   95,    4,   95,
 /*  1420 */    95,   24,   95,   95,   95,   95,   95,   95,   31,   32,
 /*  1430 */    95,   95,   95,   95,   37,   38,   95,   40,   24,   42,
 /*  1440 */    43,   44,   45,   95,   95,   31,   32,   95,   95,   95,
 /*  1450 */    95,   37,   38,   95,   40,   95,   42,   43,   95,   45,
};
#define YY_SHIFT_USE_DFLT (-26)
#define YY_SHIFT_MAX 90
static const short yy_shift_ofst[] = {
 /*     0 */   401,  607,  444, 1241, 1241, 1241, 1241, 1241, 1241, 1285,
 /*    10 */   722,  722,  722,  722, 1397,  722, 1371, 1354,  722,  722,
 /*    20 */   722,  722,  722,  722,  722,  722,  722,  722,  722,  722,
 /*    30 */   722,  722, 1006, 1006, 1414, 1006, 1006, 1006, 1006, 1006,
 /*    40 */  1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006,
 /*    50 */  1006, 1006,  150,   54,   54,   76,  765,  422,  -25,  117,
 /*    60 */   155,  144,   69,   88,   44,   86,   30,  196,  201,  200,
 /*    70 */   221,  203,    8,  195,  228,  234,    0,  226,  222,  217,
 /*    80 */   211,  194,  197,  172,  188,  184,  177,  157,  124,  123,
 /*    90 */   118,
};
#define YY_REDUCE_USE_DFLT (-48)
#define YY_REDUCE_MAX 55
static const short yy_reduce_ofst[] = {
 /*     0 */   -11,  -47,   25,   62,  210,  247,   99,  136,  173,  282,
 /*    10 */   455,  489,  318,  421,  526,  591,  626,  663,  734,  699,
 /*    20 */  1160, 1125, 1089,  770, 1054, 1018,  983,  947,  912,  876,
 /*    30 */   841,  805, 1257, 1290,  642,  856,  892,  927,  507,  606,
 /*    40 */   749,  782,  821,  298,  254,  333,  379,  325,  427,  336,
 /*    50 */   320,  376,   84,   57,  108,   33,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*    10 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*    20 */   294,  294,  294,  294,  207,  294,  284,  294,  294,  294,
 /*    30 */   294,  294,  294,  294,  294,  290,  294,  294,  294,  294,
 /*    40 */   294,  294,  294,  294,  294,  294,  294,  294,  294,  294,
 /*    50 */   294,  294,  257,  294,  294,  294,  232,  294,  246,  294,
 /*    60 */   294,  294,  294,  243,  294,  294,  214,  294,  294,  294,
 /*    70 */   294,  294,  199,  264,  190,  294,  294,  294,  292,  291,
 /*    80 */   286,  285,  294,  294,  294,  294,  294,  294,  294,  294,
 /*    90 */   294,  185,  270,  192,  272,  273,  271,  280,  248,  263,
 /*   100 */   249,  262,  250,  261,  245,  260,  234,  259,  235,  258,
 /*   110 */   236,  256,  237,  255,  238,  200,  201,  254,  202,  193,
 /*   120 */   253,  288,  189,  252,  289,  293,  203,  251,  247,  204,
 /*   130 */   188,  244,  282,  219,  242,  283,  287,  241,  205,  240,
 /*   140 */   206,  239,  208,  233,  194,  187,  195,  220,  221,  231,
 /*   150 */   210,  211,  212,  213,  230,  222,  229,  215,  228,  223,
 /*   160 */   227,  216,  226,  224,  209,  217,  198,  225,  197,  218,
 /*   170 */   196,  275,  186,  281,  274,  266,  265,  268,  279,  269,
 /*   180 */   267,  277,  278,  276,  191,
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
  "term",          "factor",        "power",         "call_trailer",
  "access_trailer",  "listmaker",     "dictmaker",     "arglist",     
  "subscriptlist",  "subscript",     "argument",      "testlist_2",  
  "testlist_3",    "exprlist_2",    "exprlist_3",  
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
 /*  70 */ "power ::= atom call_trailer",
 /*  71 */ "power ::= atom access_trailer",
 /*  72 */ "power ::= atom",
 /*  73 */ "atom ::= OPEN_RECT_BRACE CLOSE_RECT_BRACE",
 /*  74 */ "atom ::= OPEN_RECT_BRACE listmaker CLOSE_RECT_BRACE",
 /*  75 */ "atom ::= OPEN_CURLY_BRACE CLOSE_CURLY_BRACE",
 /*  76 */ "atom ::= OPEN_CURLY_BRACE dictmaker CLOSE_CURLY_BRACE",
 /*  77 */ "atom ::= ICON",
 /*  78 */ "atom ::= FCON",
 /*  79 */ "atom ::= string",
 /*  80 */ "listmaker ::= test",
 /*  81 */ "listmaker ::= listmaker COMMA test",
 /*  82 */ "lambdef ::= LAMBDA varargslist COLON test",
 /*  83 */ "call_trailer ::= OPEN_PAR CLOSE_PAR",
 /*  84 */ "call_trailer ::= OPEN_PAR arglist CLOSE_PAR",
 /*  85 */ "access_trailer ::= OPEN_RECT_BRACE subscriptlist CLOSE_RECT_BRACE",
 /*  86 */ "subscriptlist ::= subscript",
 /*  87 */ "subscriptlist ::= subscriptlist COMMA subscript",
 /*  88 */ "subscript ::= test",
 /*  89 */ "dictmaker ::= test COLON test",
 /*  90 */ "dictmaker ::= dictmaker COMMA test COLON test",
 /*  91 */ "arglist ::= argument",
 /*  92 */ "arglist ::= arglist COMMA argument",
 /*  93 */ "argument ::= test",
 /*  94 */ "string ::= SCON",
 /*  95 */ "string ::= string SCON",
 /*  96 */ "atom ::= NAME",
 /*  97 */ "testlist_2 ::= testlist_2 COMMA test",
 /*  98 */ "testlist_2 ::= test COMMA test",
 /*  99 */ "testlist_3 ::= test COMMA",
 /* 100 */ "testlist ::= test",
 /* 101 */ "testlist ::= testlist_2",
 /* 102 */ "testlist ::= testlist_3",
 /* 103 */ "exprlist_2 ::= exprlist_2 COMMA expr",
 /* 104 */ "exprlist_2 ::= expr COMMA expr",
 /* 105 */ "exprlist_3 ::= expr COMMA",
 /* 106 */ "exprlist ::= expr",
 /* 107 */ "exprlist ::= exprlist_2",
 /* 108 */ "exprlist ::= exprlist_3",
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
#line 805 "jx_py_parse.c"
      break;
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
#line 59 "jx_py_parse.lem"
{ jx_ob_free((yypminor->yy73)); }
#line 814 "jx_py_parse.c"
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
  { 82, 2 },
  { 82, 1 },
  { 51, 2 },
  { 51, 3 },
  { 51, 2 },
  { 51, 3 },
  { 51, 1 },
  { 51, 1 },
  { 51, 1 },
  { 85, 1 },
  { 85, 3 },
  { 71, 4 },
  { 83, 2 },
  { 83, 3 },
  { 84, 3 },
  { 88, 1 },
  { 88, 3 },
  { 89, 1 },
  { 86, 3 },
  { 86, 5 },
  { 87, 1 },
  { 87, 3 },
  { 90, 1 },
  { 50, 1 },
  { 50, 2 },
  { 51, 1 },
  { 91, 3 },
  { 91, 3 },
  { 92, 2 },
  { 59, 1 },
  { 59, 1 },
  { 59, 1 },
  { 93, 3 },
  { 93, 3 },
  { 94, 2 },
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
  printf("single_input ::= simple_stmt(yymsp[0].minor.yy73).\n");
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[0].minor.yy73; /* save the result */
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
      case 80:
      case 91:
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
  yygotominor.yy73 = yymsp[-1].minor.yy73;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy73) ::= small_stmt(yymsp[-1].minor.yy73) NEWLINE.\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 1226 "jx_py_parse.c"
        break;
      case 9:
#line 196 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[-1].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy73) ::= small_stmt(yymsp[-1].minor.yy0) NEWLINE.\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 1237 "jx_py_parse.c"
        break;
      case 10:
#line 204 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[-2].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy73) ::= small_stmt(yymsp[-2].minor.yy0) NEWLINE.\n");
#endif
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1249 "jx_py_parse.c"
        break;
      case 11:
#line 212 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[-2].minor.yy73;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy73) ::= small_stmt(yymsp[-2].minor.yy73) SEMICOLON NEWLINE.\n");
#endif
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1261 "jx_py_parse.c"
        break;
      case 12:
#line 220 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[-3].minor.yy73;
  jx_list_append(yygotominor.yy73,yymsp[-1].minor.yy73);
#ifdef JX_PY_PARSER_DEBUG
  printf("single_stmt(yygotominor.yy73) ::= small_stmt(yymsp[-3].minor.yy73) SEMICOLON small_stmt(yymsp[-1].minor.yy73).\n");
#endif
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1274 "jx_py_parse.c"
        break;
      case 13:
#line 232 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy73) ::= expr_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1284 "jx_py_parse.c"
        break;
      case 14:
#line 240 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[0].minor.yy73;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy73) ::= print_stmt(yymsp[0].minor.yy73).\n");
#endif
}
#line 1294 "jx_py_parse.c"
        break;
      case 15:
#line 248 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy73) ::= del_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1304 "jx_py_parse.c"
        break;
      case 16:
#line 256 "jx_py_parse.lem"
{  
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy73) ::= pass_stmt(yymsp[0].minor.yy0).\n");
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
  if(jx_list_size(yymsp[-2].minor.yy0) &&
     (jx_ob_identical(jx_list_borrow(yymsp[-2].minor.yy0,0),
                      jx_ob_from_ident("get")))) {
    yygotominor.yy0 = yymsp[-2].minor.yy0;
    /* lvalue transformation */
    jx_list_replace(yygotominor.yy0,0,jx_ob_from_ident("set"));
    jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  } else {
    yygotominor.yy0 = jx_list_new();
    jx_list_append(yygotominor.yy0,jx_ob_from_ident("set"));
    jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
    jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  }

#ifdef JX_PY_PARSER_DEBUG
  printf("expr_stmt(yygotominor.yy0) ::= testlist(yymsp[-2].minor.yy0) EQUALS testlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(14,&yymsp[-1].minor);
}
#line 1390 "jx_py_parse.c"
        break;
      case 22:
#line 341 "jx_py_parse.lem"
{  
   yygotominor.yy73 = jx_list_new();
   jx_list_append(yygotominor.yy73,jx_ob_from_ident("print"));
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy73) ::= PRINT..\n");
#endif
  yy_destructor(15,&yymsp[0].minor);
}
#line 1402 "jx_py_parse.c"
        break;
      case 23:
#line 350 "jx_py_parse.lem"
{  
   yygotominor.yy73 = jx_list_new();
   jx_list_append(yygotominor.yy73,jx_ob_from_ident("print"));
   jx_list_append(yygotominor.yy73,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy73) ::= PRINT test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(15,&yymsp[-1].minor);
}
#line 1415 "jx_py_parse.c"
        break;
      case 24:
#line 360 "jx_py_parse.lem"
{  
   yygotominor.yy73=yymsp[-2].minor.yy73;
   jx_list_append(yygotominor.yy73,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy73) ::= print_stmt(yymsp[-2].minor.yy73) COMMA test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 1427 "jx_py_parse.c"
        break;
      case 25:
      case 26:
      case 27:
      case 28:
#line 372 "jx_py_parse.lem"
{  
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("compound_stmt(yygotominor.yy0) ::= while_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1440 "jx_py_parse.c"
        break;
      case 29:
      case 88:
      case 93:
#line 406 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
}
#line 1449 "jx_py_parse.c"
        break;
      case 30:
#line 411 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-3].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1459 "jx_py_parse.c"
        break;
      case 31:
#line 417 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-4].minor.yy0;
   jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
   jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1470 "jx_py_parse.c"
        break;
      case 32:
#line 424 "jx_py_parse.lem"
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
#line 1485 "jx_py_parse.c"
        break;
      case 33:
#line 438 "jx_py_parse.lem"
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
#line 1500 "jx_py_parse.c"
        break;
      case 34:
#line 451 "jx_py_parse.lem"
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
#line 1517 "jx_py_parse.c"
        break;
      case 35:
#line 465 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy73;
#ifdef JX_PY_PARSER_DEBUG
  printf("suite(yygotominor.yy0) ::= simple_stmt(yymsp[0].minor.yy73).\n");
#endif
}
#line 1527 "jx_py_parse.c"
        break;
      case 36:
#line 473 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("suite(yygotominor.yy0) ::= simple_stmt(yymsp[-1].minor.yy0).\n");
#endif
  yy_destructor(2,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
#line 1540 "jx_py_parse.c"
        break;
      case 37:
#line 484 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy73);
}
#line 1548 "jx_py_parse.c"
        break;
      case 38:
#line 491 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1556 "jx_py_parse.c"
        break;
      case 39:
#line 498 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy73);
}
#line 1564 "jx_py_parse.c"
        break;
      case 41:
#line 512 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("test(yygotominor.yy0) ::= or_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1574 "jx_py_parse.c"
        break;
      case 42:
#line 520 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf(".\n");
#endif
}
#line 1584 "jx_py_parse.c"
        break;
      case 43:
#line 530 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("or_test(yygotominor.yy0) ::= and_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1594 "jx_py_parse.c"
        break;
      case 44:
#line 540 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("and_test(yygotominor.yy0) ::= not_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1604 "jx_py_parse.c"
        break;
      case 45:
#line 550 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("not_test(yygotominor.yy0) ::= NOT not_test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(24,&yymsp[-1].minor);
}
#line 1615 "jx_py_parse.c"
        break;
      case 46:
#line 558 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("not_test(yygotominor.yy0) ::= comparison(yymsp[0].minor.yy0).\n");
#endif
}
#line 1625 "jx_py_parse.c"
        break;
      case 47:
#line 568 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1635 "jx_py_parse.c"
        break;
      case 48:
#line 576 "jx_py_parse.lem"
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
#line 1650 "jx_py_parse.c"
        break;
      case 49:
#line 588 "jx_py_parse.lem"
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
#line 1665 "jx_py_parse.c"
        break;
      case 50:
#line 600 "jx_py_parse.lem"
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
#line 1680 "jx_py_parse.c"
        break;
      case 51:
#line 612 "jx_py_parse.lem"
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
#line 1695 "jx_py_parse.c"
        break;
      case 52:
#line 624 "jx_py_parse.lem"
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
#line 1710 "jx_py_parse.c"
        break;
      case 53:
#line 636 "jx_py_parse.lem"
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
#line 1725 "jx_py_parse.c"
        break;
      case 54:
#line 650 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("expr(yygotominor.yy0) ::= xor_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1735 "jx_py_parse.c"
        break;
      case 55:
#line 660 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("xor_expr(yygotominor.yy0) ::= and_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1745 "jx_py_parse.c"
        break;
      case 56:
#line 670 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("and_expr(yygotominor.yy0) ::= shift_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1755 "jx_py_parse.c"
        break;
      case 57:
#line 680 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("shirt_exp(yygotominor.yy0) ::= arith_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1765 "jx_py_parse.c"
        break;
      case 58:
#line 690 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[0].minor.yy0)\n");
#endif
}
#line 1775 "jx_py_parse.c"
        break;
      case 59:
#line 698 "jx_py_parse.lem"
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
#line 1789 "jx_py_parse.c"
        break;
      case 60:
#line 709 "jx_py_parse.lem"
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
#line 1803 "jx_py_parse.c"
        break;
      case 61:
#line 722 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[0].minor.yy0).\n");
#endif
}
#line 1813 "jx_py_parse.c"
        break;
      case 62:
#line 730 "jx_py_parse.lem"
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
#line 1827 "jx_py_parse.c"
        break;
      case 63:
#line 741 "jx_py_parse.lem"
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
#line 1841 "jx_py_parse.c"
        break;
      case 64:
#line 752 "jx_py_parse.lem"
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
#line 1855 "jx_py_parse.c"
        break;
      case 65:
#line 763 "jx_py_parse.lem"
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
#line 1869 "jx_py_parse.c"
        break;
      case 66:
#line 776 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= power(yymsp[0].minor.yy0).\n");
#endif
}
#line 1879 "jx_py_parse.c"
        break;
      case 67:
#line 784 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= PLUS factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(31,&yymsp[-1].minor);
}
#line 1890 "jx_py_parse.c"
        break;
      case 68:
#line 792 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= PLUS factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(32,&yymsp[-1].minor);
}
#line 1901 "jx_py_parse.c"
        break;
      case 69:
#line 800 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= TILDE factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(37,&yymsp[-1].minor);
}
#line 1912 "jx_py_parse.c"
        break;
      case 70:
#line 822 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
  jx_list_unshift(yygotominor.yy0,yymsp[-1].minor.yy73);
#ifdef JX_PY_PARSER_DEBUG
  printf("power(yygotominor.yy0) ::= atom(yymsp[-1].minor.yy73) trailer(yymsp[0].minor.yy0).\n");
#endif
}
#line 1923 "jx_py_parse.c"
        break;
      case 71:
#line 831 "jx_py_parse.lem"
{
  /* yymsp[-1].minor.yy73[yymsp[0].minor.yy0 ...] becomes (yymsp[-1].minor.yy73 yymsp[0].minor.yy0 ...) */
  yygotominor.yy0 = yymsp[0].minor.yy0;
  jx_list_insert(yygotominor.yy0,1,yymsp[-1].minor.yy73);
#ifdef JX_PY_PARSER_DEBUG
  printf("power(yygotominor.yy0) ::= atom(yymsp[-1].minor.yy73) trailer(yymsp[0].minor.yy0).\n");
#endif
}
#line 1935 "jx_py_parse.c"
        break;
      case 72:
#line 841 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy73;
#ifdef JX_PY_PARSER_DEBUG
  printf("power(yygotominor.yy0) ::= atom(yymsp[0].minor.yy73).\n");
#endif
}
#line 1945 "jx_py_parse.c"
        break;
      case 73:
#line 867 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = jx_list_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
#line 1957 "jx_py_parse.c"
        break;
      case 74:
#line 875 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
#line 1969 "jx_py_parse.c"
        break;
      case 75:
#line 883 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = jx_hash_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
#line 1981 "jx_py_parse.c"
        break;
      case 76:
#line 892 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf(".\n"); 
#endif
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
#line 1993 "jx_py_parse.c"
        break;
      case 77:
#line 901 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = ICON\n"); 
#endif
}
#line 2003 "jx_py_parse.c"
        break;
      case 78:
#line 909 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = FCON\n"); 
#endif
}
#line 2013 "jx_py_parse.c"
        break;
      case 79:
#line 917 "jx_py_parse.lem"
{ 
  yygotominor.yy73 = yymsp[0].minor.yy73;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = string\n"); 
#endif
}
#line 2023 "jx_py_parse.c"
        break;
      case 81:
      case 87:
      case 92:
#line 933 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2034 "jx_py_parse.c"
        break;
      case 82:
#line 941 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("lambda"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 2046 "jx_py_parse.c"
        break;
      case 83:
#line 951 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf("call_trailer(yygotominor.yy0) ::= OPEN_PAR CLOSE_PAR.\n"); 
#endif
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 2058 "jx_py_parse.c"
        break;
      case 84:
#line 959 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("call_trailer(yygotominor.yy0) ::= OPEN_PAR arglist(yymsp[-1].minor.yy0) CLOSE_PAR.\n"); 
#endif
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 2070 "jx_py_parse.c"
        break;
      case 85:
#line 967 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("access_trailer(yygotominor.yy0) ::= OPEN_RECT_BRACKET subscriptlist(yymsp[-1].minor.yy0) CLOSE_RECT_BRACKET.\n"); 
#endif
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
#line 2082 "jx_py_parse.c"
        break;
      case 86:
#line 977 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("get"));
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 2091 "jx_py_parse.c"
        break;
      case 89:
#line 999 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_hash_new();
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);  
  yy_destructor(5,&yymsp[-1].minor);
}
#line 2100 "jx_py_parse.c"
        break;
      case 90:
#line 1005 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-4].minor.yy0;
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);  
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 2110 "jx_py_parse.c"
        break;
      case 94:
#line 1034 "jx_py_parse.lem"
{
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("string = SCON\n"); 
#endif
}
#line 2120 "jx_py_parse.c"
        break;
      case 95:
#line 1044 "jx_py_parse.lem"
{
  yygotominor.yy73 = jx_ob_add(yymsp[-1].minor.yy73,yymsp[0].minor.yy0);
  jx_ob_free(yymsp[-1].minor.yy73);
  jx_ob_free(yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = SCON\n"); 
#endif
}
#line 2132 "jx_py_parse.c"
        break;
      case 96:
#line 1054 "jx_py_parse.lem"
{
  yygotominor.yy73 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = NAME\n"); 
#endif
}
#line 2142 "jx_py_parse.c"
        break;
      case 97:
#line 1064 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_2(yygotominor.yy0) ::= testlist_2(yymsp[-2].minor.yy0) COMMA test(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2155 "jx_py_parse.c"
        break;
      case 98:
#line 1074 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_2(yygotominor.yy0) ::= test(yymsp[-2].minor.yy0) COMMA test(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2168 "jx_py_parse.c"
        break;
      case 99:
#line 1084 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_3(yygotominor.yy0) ::= test(yymsp[-1].minor.yy0) COMMA.\n"); 
#endif
  yy_destructor(8,&yymsp[0].minor);
}
#line 2180 "jx_py_parse.c"
        break;
      case 100:
#line 1093 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= test(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2190 "jx_py_parse.c"
        break;
      case 101:
#line 1101 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= testlist_2(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2200 "jx_py_parse.c"
        break;
      case 102:
#line 1109 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= testlist_3(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2210 "jx_py_parse.c"
        break;
      case 103:
#line 1119 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_2(yygotominor.yy0) ::= exprlist_2(yymsp[-2].minor.yy0) COMMA expr(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2223 "jx_py_parse.c"
        break;
      case 104:
#line 1129 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_2(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMMA expr(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 2236 "jx_py_parse.c"
        break;
      case 105:
#line 1139 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_3(yygotominor.yy0) ::= expr(yymsp[-1].minor.yy0) COMMA.\n"); 
#endif
  yy_destructor(8,&yymsp[0].minor);
}
#line 2248 "jx_py_parse.c"
        break;
      case 106:
#line 1148 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= expr(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2258 "jx_py_parse.c"
        break;
      case 107:
#line 1156 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= exprlist_2(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2268 "jx_py_parse.c"
        break;
      case 108:
#line 1164 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= exprlist_3(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 2278 "jx_py_parse.c"
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
#line 2314 "jx_py_parse.c"
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
#line 2334 "jx_py_parse.c"
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
#line 2360 "jx_py_parse.c"
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
