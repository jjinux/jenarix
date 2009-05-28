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
#define YYNOCODE 69
#define YYACTIONTYPE unsigned char
#define jx_py_TOKENTYPE jx_ob
typedef union {
  jx_py_TOKENTYPE yy0;
  jx_ob yy127;
  int yy137;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_py_ARG_SDECL jx_py_parse_context *context;
#define jx_py_ARG_PDECL ,jx_py_parse_context *context
#define jx_py_ARG_FETCH jx_py_parse_context *context = yypParser->context
#define jx_py_ARG_STORE yypParser->context = context
#define YYNSTATE 114
#define YYNRULE 74
#define YYERRORSYMBOL 35
#define YYERRSYMDT yy137
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
 /*     0 */    61,   38,   41,   43,   87,   68,   63,   69,  102,  103,
 /*    10 */    12,   37,   51,   54,    3,    2,   71,   72,   73,   75,
 /*    20 */    35,   77,   78,   79,   80,   39,   36,   83,   45,  112,
 /*    30 */    64,   38,   41,   43,   87,  189,   48,   69,  102,  103,
 /*    40 */    65,   37,   51,   54,   27,   26,   71,   72,   73,   75,
 /*    50 */    35,   77,   78,   79,   80,   39,   36,   83,   45,  112,
 /*    60 */    59,   38,   41,   43,   87,    8,   60,   69,  102,  103,
 /*    70 */    20,   37,   51,   54,   66,    5,   71,   72,   73,   75,
 /*    80 */    35,   77,   78,   79,   80,   39,   36,   83,   45,  112,
 /*    90 */    56,   38,   41,   43,   87,   13,    1,   69,  102,  103,
 /*   100 */    90,   37,   51,    4,   55,   10,   71,   72,   73,   75,
 /*   110 */    35,   77,   78,   79,   80,   39,   36,   83,   45,  112,
 /*   120 */    56,   38,   41,   43,   87,   21,  190,   69,  102,  103,
 /*   130 */   190,   37,   51,  190,   57,  190,   71,   72,   73,   75,
 /*   140 */    35,   77,   78,   79,   80,   39,   36,   83,   45,  112,
 /*   150 */    50,   41,   43,   87,  190,  190,   69,  102,  103,  190,
 /*   160 */    37,   51,   43,   87,  190,   71,   72,   73,   75,   35,
 /*   170 */    77,   78,   79,   80,   39,   36,   83,   45,  112,   43,
 /*   180 */    87,    6,    7,    9,   81,   36,   83,   52,   51,   43,
 /*   190 */    87,  190,   71,   72,   73,   75,   35,   77,   78,   79,
 /*   200 */    80,   39,   36,   83,   45,  112,   43,   87,  190,  190,
 /*   210 */   190,   96,   36,   83,  113,   51,   43,   87,  190,   71,
 /*   220 */    72,   73,   75,   35,   77,   78,   79,   80,   39,   36,
 /*   230 */    83,   45,  112,   43,   87,  190,  190,  190,  190,   82,
 /*   240 */    83,  109,   51,   43,   87,  190,   71,   72,   73,   75,
 /*   250 */    35,   77,   78,   79,   80,   39,   36,   83,   45,  112,
 /*   260 */    43,   87,  190,  190,  190,  190,   95,   83,   49,   51,
 /*   270 */    43,   87,  190,   71,   72,   73,   75,   35,   77,   78,
 /*   280 */    79,   80,   39,   36,   83,   45,  112,   43,   87,  190,
 /*   290 */   190,  190,  190,   94,   83,  190,   42,  190,  190,  190,
 /*   300 */    71,   72,   73,   75,   35,   77,   78,   79,   80,   39,
 /*   310 */    36,   83,  190,   43,   87,   24,   22,   23,   25,   18,
 /*   320 */    19,  190,  111,  190,  190,  190,   71,   72,   73,   75,
 /*   330 */    35,   77,   78,   79,   80,   39,   36,   83,   30,   32,
 /*   340 */    43,   87,  190,  190,   31,   88,   89,   91,   92,  110,
 /*   350 */    43,   87,  190,   71,   72,   73,   75,   35,   77,   78,
 /*   360 */    79,   80,   39,   36,   83,  190,   43,   87,   28,   33,
 /*   370 */    34,   29,  190,   93,   83,   53,   43,   87,  190,   71,
 /*   380 */    72,   73,   75,   35,   77,   78,   79,   80,   39,   36,
 /*   390 */    83,  190,  190,   43,   87,  190,  190,  190,  190,   85,
 /*   400 */    83,  190,   70,  190,  190,  190,   71,   72,   73,   75,
 /*   410 */    35,   77,   78,   79,   80,   39,   36,   83,   62,  190,
 /*   420 */    16,  108,  190,  190,  190,   11,  190,   14,  190,   15,
 /*   430 */    43,   87,  190,   17,  190,  190,  190,  190,  190,  190,
 /*   440 */    30,   32,   43,   87,  190,  190,   31,   88,   89,   91,
 /*   450 */    92,   16,  108,   86,   83,  190,   11,  190,   14,  190,
 /*   460 */    15,  190,  190,   58,   17,   84,   83,  190,  190,  190,
 /*   470 */   190,   30,   32,  190,  190,  190,  190,   31,   88,   89,
 /*   480 */    91,   92,   16,  108,  190,  190,  190,   11,  190,   14,
 /*   490 */   190,   15,  190,  190,  190,   17,  190,  190,   43,   87,
 /*   500 */   190,  190,   30,   32,  190,  104,  190,  190,   31,   88,
 /*   510 */    89,   91,   92,  190,  190,   40,   77,   78,   79,   80,
 /*   520 */    39,   36,   83,   43,   87,   46,  107,  190,  190,  190,
 /*   530 */    47,  190,  190,  190,  190,  190,  190,  190,   43,   87,
 /*   540 */    40,   77,   78,   79,   80,   39,   36,   83,  190,  190,
 /*   550 */    46,  107,  190,   74,   75,   35,   77,   78,   79,   80,
 /*   560 */    39,   36,   83,   67,  190,   16,  108,  190,  190,  190,
 /*   570 */    11,  190,  190,  190,  190,  190,  190,  190,   17,  190,
 /*   580 */   190,   44,  190,   16,  108,   30,   32,  190,   11,  190,
 /*   590 */   190,   31,   88,   89,   91,   92,   17,   43,   87,  190,
 /*   600 */   190,  190,  190,   30,   32,   43,   87,  190,  190,   31,
 /*   610 */    88,   89,   91,   92,   76,   77,   78,   79,   80,   39,
 /*   620 */    36,   83,   97,   77,   78,   79,   80,   39,   36,   83,
 /*   630 */   190,   43,   87,  190,  190,  190,  190,  190,  190,  190,
 /*   640 */   190,  190,  190,   43,   87,  190,  190,  190,   99,   77,
 /*   650 */    78,   79,   80,   39,   36,   83,   43,   87,  190,  190,
 /*   660 */   106,   77,   78,   79,   80,   39,   36,   83,   43,   87,
 /*   670 */   190,  190,  190,  105,   77,   78,   79,   80,   39,   36,
 /*   680 */    83,  190,  190,   43,   87,  101,   77,   78,   79,   80,
 /*   690 */    39,   36,   83,   43,   87,  190,  190,  190,  190,  190,
 /*   700 */   100,   77,   78,   79,   80,   39,   36,   83,  190,  190,
 /*   710 */    98,   77,   78,   79,   80,   39,   36,   83,   17,  190,
 /*   720 */   190,  190,  190,  190,  190,   30,   32,  190,  190,  190,
 /*   730 */   190,   31,   88,   89,   91,   92,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    36,   37,   38,   39,   40,    2,   42,   43,   44,   45,
 /*    10 */    10,   47,   48,   49,   12,   51,   52,   53,   54,   55,
 /*    20 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*    30 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*    40 */     2,   47,   48,   49,   24,   25,   52,   53,   54,   55,
 /*    50 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*    60 */    36,   37,   38,   39,   40,   14,   42,   43,   44,   45,
 /*    70 */    10,   47,   48,   49,    2,    3,   52,   53,   54,   55,
 /*    80 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*    90 */    36,   37,   38,   39,   40,   10,   15,   43,   44,   45,
 /*   100 */    33,   47,   48,   12,   50,   10,   52,   53,   54,   55,
 /*   110 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   120 */    36,   37,   38,   39,   40,   10,   68,   43,   44,   45,
 /*   130 */    68,   47,   48,   68,   50,   68,   52,   53,   54,   55,
 /*   140 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   150 */    37,   38,   39,   40,   68,   68,   43,   44,   45,   68,
 /*   160 */    47,   48,   39,   40,   68,   52,   53,   54,   55,   56,
 /*   170 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   39,
 /*   180 */    40,    6,    7,    8,   61,   62,   63,   47,   48,   39,
 /*   190 */    40,   68,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   200 */    60,   61,   62,   63,   64,   65,   39,   40,   68,   68,
 /*   210 */    68,   61,   62,   63,   47,   48,   39,   40,   68,   52,
 /*   220 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   230 */    63,   64,   65,   39,   40,   68,   68,   68,   68,   62,
 /*   240 */    63,   47,   48,   39,   40,   68,   52,   53,   54,   55,
 /*   250 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   260 */    39,   40,   68,   68,   68,   68,   62,   63,   47,   48,
 /*   270 */    39,   40,   68,   52,   53,   54,   55,   56,   57,   58,
 /*   280 */    59,   60,   61,   62,   63,   64,   65,   39,   40,   68,
 /*   290 */    68,   68,   68,   62,   63,   68,   48,   68,   68,   68,
 /*   300 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*   310 */    62,   63,   68,   39,   40,   18,   19,   20,   21,   22,
 /*   320 */    23,   68,   48,   68,   68,   68,   52,   53,   54,   55,
 /*   330 */    56,   57,   58,   59,   60,   61,   62,   63,   24,   25,
 /*   340 */    39,   40,   68,   68,   30,   31,   32,   33,   34,   48,
 /*   350 */    39,   40,   68,   52,   53,   54,   55,   56,   57,   58,
 /*   360 */    59,   60,   61,   62,   63,   68,   39,   40,   26,   27,
 /*   370 */    28,   29,   68,   62,   63,   48,   39,   40,   68,   52,
 /*   380 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   390 */    63,   68,   68,   39,   40,   68,   68,   68,   68,   62,
 /*   400 */    63,   68,   48,   68,   68,   68,   52,   53,   54,   55,
 /*   410 */    56,   57,   58,   59,   60,   61,   62,   63,    2,   68,
 /*   420 */     4,    5,   68,   68,   68,    9,   68,   11,   68,   13,
 /*   430 */    39,   40,   68,   17,   68,   68,   68,   68,   68,   68,
 /*   440 */    24,   25,   39,   40,   68,   68,   30,   31,   32,   33,
 /*   450 */    34,    4,    5,   62,   63,   68,    9,   68,   11,   68,
 /*   460 */    13,   68,   68,   16,   17,   62,   63,   68,   68,   68,
 /*   470 */    68,   24,   25,   68,   68,   68,   68,   30,   31,   32,
 /*   480 */    33,   34,    4,    5,   68,   68,   68,    9,   68,   11,
 /*   490 */    68,   13,   68,   68,   68,   17,   68,   68,   39,   40,
 /*   500 */    68,   68,   24,   25,   68,   46,   68,   68,   30,   31,
 /*   510 */    32,   33,   34,   68,   68,   56,   57,   58,   59,   60,
 /*   520 */    61,   62,   63,   39,   40,   66,   67,   68,   68,   68,
 /*   530 */    46,   68,   68,   68,   68,   68,   68,   68,   39,   40,
 /*   540 */    56,   57,   58,   59,   60,   61,   62,   63,   68,   68,
 /*   550 */    66,   67,   68,   54,   55,   56,   57,   58,   59,   60,
 /*   560 */    61,   62,   63,    2,   68,    4,    5,   68,   68,   68,
 /*   570 */     9,   68,   68,   68,   68,   68,   68,   68,   17,   68,
 /*   580 */    68,    2,   68,    4,    5,   24,   25,   68,    9,   68,
 /*   590 */    68,   30,   31,   32,   33,   34,   17,   39,   40,   68,
 /*   600 */    68,   68,   68,   24,   25,   39,   40,   68,   68,   30,
 /*   610 */    31,   32,   33,   34,   56,   57,   58,   59,   60,   61,
 /*   620 */    62,   63,   56,   57,   58,   59,   60,   61,   62,   63,
 /*   630 */    68,   39,   40,   68,   68,   68,   68,   68,   68,   68,
 /*   640 */    68,   68,   68,   39,   40,   68,   68,   68,   56,   57,
 /*   650 */    58,   59,   60,   61,   62,   63,   39,   40,   68,   68,
 /*   660 */    56,   57,   58,   59,   60,   61,   62,   63,   39,   40,
 /*   670 */    68,   68,   68,   56,   57,   58,   59,   60,   61,   62,
 /*   680 */    63,   68,   68,   39,   40,   56,   57,   58,   59,   60,
 /*   690 */    61,   62,   63,   39,   40,   68,   68,   68,   68,   68,
 /*   700 */    56,   57,   58,   59,   60,   61,   62,   63,   68,   68,
 /*   710 */    56,   57,   58,   59,   60,   61,   62,   63,   17,   68,
 /*   720 */    68,   68,   68,   68,   68,   24,   25,   68,   68,   68,
 /*   730 */    68,   30,   31,   32,   33,   34,
};
#define YY_SHIFT_USE_DFLT (-1)
#define YY_SHIFT_MAX 51
static const short yy_shift_ofst[] = {
 /*     0 */   416,  478,  447,  579,  579,  561,  701,  701,  701,  701,
 /*    10 */   701,  701,  701,  701,  701,  314,  314,  701,  314,  314,
 /*    20 */   314,  314,  314,  314,  314,  314,  314,  314,  314,  314,
 /*    30 */   314,  314,  314,  314,  314,  297,  342,  175,   72,   20,
 /*    40 */   115,   95,   91,   67,   81,   85,   60,   51,   38,    2,
 /*    50 */     3,    0,
};
#define YY_REDUCE_USE_DFLT (-37)
#define YY_REDUCE_MAX 34
static const short yy_reduce_ofst[] = {
 /*     0 */    -6,  -36,   24,   84,   54,  113,  194,  167,  221,  140,
 /*    10 */   354,  327,  274,  301,  248,  484,  459,  499,  644,  629,
 /*    20 */   617,  604,  566,  654,  558,  592,  150,  123,  177,  204,
 /*    30 */   403,  391,  337,  311,  231,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*    10 */   188,  129,  178,  188,  188,  188,  188,  188,  188,  188,
 /*    20 */   188,  184,  188,  188,  188,  188,  188,  188,  188,  188,
 /*    30 */   188,  188,  188,  188,  188,  146,  160,  188,  188,  157,
 /*    40 */   185,  121,  188,  172,  188,  180,  186,  188,  188,  188,
 /*    50 */   188,  179,  128,  130,  132,  133,  135,  134,  136,  137,
 /*    60 */   138,  139,  114,  140,  115,  116,  117,  118,  119,  120,
 /*    70 */   131,  141,  142,  143,  144,  145,  147,  153,  154,  155,
 /*    80 */   156,  158,  161,  165,  166,  167,  168,  169,  170,  171,
 /*    90 */   174,  173,  175,  162,  163,  164,  159,  148,  149,  150,
 /*   100 */   151,  152,  122,  123,  124,  182,  183,  187,  125,  126,
 /*   110 */   176,  177,  181,  127,
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
  "$",             "ERROR",         "NEWLINE",       "SEMICOLON",   
  "DEL",           "PASS",          "PLUS_EQUALS",   "MINUS_EQUALS",
  "EQUALS",        "PRINT",         "COMMA",         "WHILE",       
  "COLON",         "FOR",           "IN",            "INDENT",      
  "DEDENT",        "NOT",           "LESS_THAN",     "LESS_THAN_OR_EQ",
  "EQ_EQ",         "NOT_EQ",        "GREATER_THAN",  "GREATER_THAN_OR_EQ",
  "PLUS",          "MINUS",         "ASTERISK",      "SLASH",       
  "SLASHSLASH",    "PERCENT",       "TILDE",         "ICON",        
  "FCON",          "SCON",          "NAME",          "error",       
  "simple_stmt",   "small_stmt",    "print_stmt",    "string",      
  "atom",          "single_input",  "compound_stmt",  "expr_stmt",   
  "del_stmt",      "pass_stmt",     "exprlist",      "testlist",    
  "test",          "while_stmt",    "suite",         "stmts",       
  "or_test",       "and_test",      "not_test",      "comparison",  
  "expr",          "xor_expr",      "and_expr",      "shift_expr",  
  "arith_expr",    "term",          "factor",        "power",       
  "testlist_2",    "testlist_3",    "exprlist_2",    "exprlist_3",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "single_input ::= NEWLINE",
 /*   1 */ "single_input ::= simple_stmt",
 /*   2 */ "single_input ::= compound_stmt NEWLINE",
 /*   3 */ "simple_stmt ::= small_stmt NEWLINE",
 /*   4 */ "simple_stmt ::= small_stmt SEMICOLON NEWLINE",
 /*   5 */ "simple_stmt ::= small_stmt SEMICOLON small_stmt NEWLINE",
 /*   6 */ "small_stmt ::= expr_stmt",
 /*   7 */ "small_stmt ::= print_stmt",
 /*   8 */ "small_stmt ::= del_stmt",
 /*   9 */ "small_stmt ::= pass_stmt",
 /*  10 */ "del_stmt ::= DEL exprlist",
 /*  11 */ "pass_stmt ::= PASS",
 /*  12 */ "expr_stmt ::= testlist PLUS_EQUALS testlist",
 /*  13 */ "expr_stmt ::= testlist MINUS_EQUALS testlist",
 /*  14 */ "expr_stmt ::= testlist EQUALS testlist",
 /*  15 */ "print_stmt ::= PRINT",
 /*  16 */ "print_stmt ::= PRINT test",
 /*  17 */ "print_stmt ::= print_stmt COMMA test",
 /*  18 */ "compound_stmt ::= while_stmt",
 /*  19 */ "while_stmt ::= WHILE test COLON suite",
 /*  20 */ "while_stmt ::= FOR exprlist IN testlist COLON suite",
 /*  21 */ "suite ::= simple_stmt",
 /*  22 */ "suite ::= NEWLINE INDENT stmts DEDENT",
 /*  23 */ "stmts ::= stmts simple_stmt",
 /*  24 */ "stmts ::= stmts compound_stmt",
 /*  25 */ "stmts ::= simple_stmt",
 /*  26 */ "stmts ::= compound_stmt",
 /*  27 */ "test ::= or_test",
 /*  28 */ "or_test ::= and_test",
 /*  29 */ "and_test ::= not_test",
 /*  30 */ "not_test ::= NOT not_test",
 /*  31 */ "not_test ::= comparison",
 /*  32 */ "comparison ::= expr",
 /*  33 */ "comparison ::= expr LESS_THAN expr",
 /*  34 */ "comparison ::= expr LESS_THAN_OR_EQ expr",
 /*  35 */ "comparison ::= expr EQ_EQ expr",
 /*  36 */ "comparison ::= expr NOT_EQ expr",
 /*  37 */ "comparison ::= expr GREATER_THAN expr",
 /*  38 */ "comparison ::= expr GREATER_THAN_OR_EQ expr",
 /*  39 */ "expr ::= xor_expr",
 /*  40 */ "xor_expr ::= and_expr",
 /*  41 */ "and_expr ::= shift_expr",
 /*  42 */ "shift_expr ::= arith_expr",
 /*  43 */ "arith_expr ::= term",
 /*  44 */ "arith_expr ::= term PLUS term",
 /*  45 */ "arith_expr ::= term MINUS term",
 /*  46 */ "term ::= factor",
 /*  47 */ "term ::= factor ASTERISK factor",
 /*  48 */ "term ::= factor SLASH factor",
 /*  49 */ "term ::= factor SLASHSLASH factor",
 /*  50 */ "term ::= factor PERCENT factor",
 /*  51 */ "factor ::= power",
 /*  52 */ "factor ::= PLUS factor",
 /*  53 */ "factor ::= MINUS factor",
 /*  54 */ "factor ::= TILDE factor",
 /*  55 */ "power ::= atom",
 /*  56 */ "atom ::= ICON",
 /*  57 */ "atom ::= FCON",
 /*  58 */ "atom ::= string",
 /*  59 */ "string ::= SCON",
 /*  60 */ "string ::= string SCON",
 /*  61 */ "atom ::= NAME",
 /*  62 */ "testlist_2 ::= testlist_2 COMMA test",
 /*  63 */ "testlist_2 ::= test COMMA test",
 /*  64 */ "testlist_3 ::= test COMMA",
 /*  65 */ "testlist ::= test",
 /*  66 */ "testlist ::= testlist_2",
 /*  67 */ "testlist ::= testlist_3",
 /*  68 */ "exprlist_2 ::= exprlist_2 COMMA expr",
 /*  69 */ "exprlist_2 ::= expr COMMA expr",
 /*  70 */ "exprlist_3 ::= expr COMMA",
 /*  71 */ "exprlist ::= expr",
 /*  72 */ "exprlist ::= exprlist_2",
 /*  73 */ "exprlist ::= exprlist_3",
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
#line 67 "jx_py_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 595 "jx_py_parse.c"
      break;
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
#line 59 "jx_py_parse.lem"
{ jx_ob_free((yypminor->yy127)); }
#line 604 "jx_py_parse.c"
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
  { 41, 1 },
  { 41, 1 },
  { 41, 2 },
  { 36, 2 },
  { 36, 3 },
  { 36, 4 },
  { 37, 1 },
  { 37, 1 },
  { 37, 1 },
  { 37, 1 },
  { 44, 2 },
  { 45, 1 },
  { 43, 3 },
  { 43, 3 },
  { 43, 3 },
  { 38, 1 },
  { 38, 2 },
  { 38, 3 },
  { 42, 1 },
  { 49, 4 },
  { 49, 6 },
  { 50, 1 },
  { 50, 4 },
  { 51, 2 },
  { 51, 2 },
  { 51, 1 },
  { 51, 1 },
  { 48, 1 },
  { 52, 1 },
  { 53, 1 },
  { 54, 2 },
  { 54, 1 },
  { 55, 1 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 55, 3 },
  { 56, 1 },
  { 57, 1 },
  { 58, 1 },
  { 59, 1 },
  { 60, 1 },
  { 60, 3 },
  { 60, 3 },
  { 61, 1 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 62, 1 },
  { 62, 2 },
  { 62, 2 },
  { 62, 2 },
  { 63, 1 },
  { 40, 1 },
  { 40, 1 },
  { 40, 1 },
  { 39, 1 },
  { 39, 2 },
  { 40, 1 },
  { 64, 3 },
  { 64, 3 },
  { 65, 2 },
  { 47, 1 },
  { 47, 1 },
  { 47, 1 },
  { 66, 3 },
  { 66, 3 },
  { 67, 2 },
  { 46, 1 },
  { 46, 1 },
  { 46, 1 },
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
#line 898 "jx_py_parse.c"
        break;
      case 1:
#line 123 "jx_py_parse.lem"
{  
#ifdef JX_PY_PARSER_DEBUG
  printf("single_input ::= simple_stmt(yymsp[0].minor.yy127).\n");
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[0].minor.yy127; /* save the result */
}
#line 909 "jx_py_parse.c"
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
#line 921 "jx_py_parse.c"
        break;
      case 3:
#line 143 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[-1].minor.yy127;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy127) ::= small_stmt(yymsp[-1].minor.yy127) NEWLINE.\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 932 "jx_py_parse.c"
        break;
      case 4:
#line 151 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[-2].minor.yy127;
#ifdef JX_PY_PARSER_DEBUG
  printf("simple_stmt(yygotominor.yy127) ::= small_stmt(yymsp[-2].minor.yy127) SEMICOLON NEWLINE.\n");
#endif
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 944 "jx_py_parse.c"
        break;
      case 5:
#line 159 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[-3].minor.yy127;
  jx_list_append(yygotominor.yy127,yymsp[-1].minor.yy127);
#ifdef JX_PY_PARSER_DEBUG
  printf("single_stmt(yygotominor.yy127) ::= small_stmt(yymsp[-3].minor.yy127) SEMICOLON small_stmt(yymsp[-1].minor.yy127).\n");
#endif
  yy_destructor(3,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 957 "jx_py_parse.c"
        break;
      case 6:
#line 171 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy127) ::= expr_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 967 "jx_py_parse.c"
        break;
      case 7:
#line 179 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[0].minor.yy127;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy127) ::= print_stmt(yymsp[0].minor.yy127).\n");
#endif
}
#line 977 "jx_py_parse.c"
        break;
      case 8:
#line 187 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy127) ::= del_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 987 "jx_py_parse.c"
        break;
      case 9:
#line 195 "jx_py_parse.lem"
{  
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("small_stmt(yygotominor.yy127) ::= pass_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 997 "jx_py_parse.c"
        break;
      case 10:
#line 205 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("del"));
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("del_stmt(yygotominor.yy0) ::= DEL exprlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(4,&yymsp[-1].minor);
}
#line 1010 "jx_py_parse.c"
        break;
      case 11:
#line 217 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
#ifdef JX_PY_PARSER_DEBUG
  printf("pass_stmt(yygotominor.yy0) ::= PASS.\n");
#endif
  yy_destructor(5,&yymsp[0].minor);
}
#line 1021 "jx_py_parse.c"
        break;
      case 12:
#line 234 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("incr"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("expr_stmt(yygotominor.yy0) ::= testlist(yymsp[-2].minor.yy0) PLUS_EQUALS testlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(6,&yymsp[-1].minor);
}
#line 1035 "jx_py_parse.c"
        break;
      case 13:
#line 245 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("decr"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy0) ::= PRINT.\n");
#endif
  yy_destructor(7,&yymsp[-1].minor);
}
#line 1049 "jx_py_parse.c"
        break;
      case 14:
#line 256 "jx_py_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("set"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("expr_stmt(yygotominor.yy0) ::= testlist(yymsp[-2].minor.yy0) EQUALS testlist(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 1064 "jx_py_parse.c"
        break;
      case 15:
#line 271 "jx_py_parse.lem"
{  
   yygotominor.yy127 = jx_list_new();
   jx_list_append(yygotominor.yy127,jx_ob_from_ident("print"));
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy127) ::= PRINT..\n");
#endif
  yy_destructor(9,&yymsp[0].minor);
}
#line 1076 "jx_py_parse.c"
        break;
      case 16:
#line 280 "jx_py_parse.lem"
{  
   yygotominor.yy127 = jx_list_new();
   jx_list_append(yygotominor.yy127,jx_ob_from_ident("print"));
   jx_list_append(yygotominor.yy127,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy127) ::= PRINT test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(9,&yymsp[-1].minor);
}
#line 1089 "jx_py_parse.c"
        break;
      case 17:
#line 290 "jx_py_parse.lem"
{  
   yygotominor.yy127=yymsp[-2].minor.yy127;
   jx_list_append(yygotominor.yy127,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("print_stmt(yygotominor.yy127) ::= print_stmt(yymsp[-2].minor.yy127) COMMA test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 1101 "jx_py_parse.c"
        break;
      case 18:
#line 301 "jx_py_parse.lem"
{  
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("compound_stmt(yygotominor.yy0) ::= while_stmt(yymsp[0].minor.yy0).\n");
#endif
}
#line 1111 "jx_py_parse.c"
        break;
      case 19:
#line 311 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("while"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("while_stmt(yygotominor.yy0) ::= test(yymsp[0].minor.yy0) COLON suite(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
}
#line 1126 "jx_py_parse.c"
        break;
      case 20:
#line 324 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("foreach"));
  jx_list_append(yygotominor.yy0,yymsp[-4].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("while_stmt(yygotominor.yy0) ::= FOR exprlist(yymsp[-4].minor.yy0) IN testlist(yymsp[-2].minor.yy0) COLON suite(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
}
#line 1143 "jx_py_parse.c"
        break;
      case 21:
#line 338 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy127;
#ifdef JX_PY_PARSER_DEBUG
  printf("suite(yygotominor.yy0) ::= simple_stmt(yymsp[0].minor.yy127).\n");
#endif
}
#line 1153 "jx_py_parse.c"
        break;
      case 22:
#line 346 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("suite(yygotominor.yy0) ::= simple_stmt(yymsp[-1].minor.yy0).\n");
#endif
  yy_destructor(2,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
#line 1166 "jx_py_parse.c"
        break;
      case 23:
#line 357 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy127);
}
#line 1174 "jx_py_parse.c"
        break;
      case 24:
#line 364 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1182 "jx_py_parse.c"
        break;
      case 25:
#line 371 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy127);
}
#line 1190 "jx_py_parse.c"
        break;
      case 26:
#line 377 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1198 "jx_py_parse.c"
        break;
      case 27:
#line 385 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("test(yygotominor.yy0) ::= or_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1208 "jx_py_parse.c"
        break;
      case 28:
#line 395 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("or_test(yygotominor.yy0) ::= and_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1218 "jx_py_parse.c"
        break;
      case 29:
#line 405 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("and_test(yygotominor.yy0) ::= not_test(yymsp[0].minor.yy0).\n");
#endif
}
#line 1228 "jx_py_parse.c"
        break;
      case 30:
#line 415 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("not_test(yygotominor.yy0) ::= NOT not_test(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(17,&yymsp[-1].minor);
}
#line 1239 "jx_py_parse.c"
        break;
      case 31:
#line 423 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("not_test(yygotominor.yy0) ::= comparison(yymsp[0].minor.yy0).\n");
#endif
}
#line 1249 "jx_py_parse.c"
        break;
      case 32:
#line 433 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1259 "jx_py_parse.c"
        break;
      case 33:
#line 441 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("lt"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(18,&yymsp[-1].minor);
}
#line 1274 "jx_py_parse.c"
        break;
      case 34:
#line 453 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("le"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(19,&yymsp[-1].minor);
}
#line 1289 "jx_py_parse.c"
        break;
      case 35:
#line 465 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("eq"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(20,&yymsp[-1].minor);
}
#line 1304 "jx_py_parse.c"
        break;
      case 36:
#line 477 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("eq"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(21,&yymsp[-1].minor);
}
#line 1319 "jx_py_parse.c"
        break;
      case 37:
#line 489 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("gt"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(22,&yymsp[-1].minor);
}
#line 1334 "jx_py_parse.c"
        break;
      case 38:
#line 501 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("ge"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_PY_PARSER_DEBUG
  printf("comparion(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMP expr(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(23,&yymsp[-1].minor);
}
#line 1349 "jx_py_parse.c"
        break;
      case 39:
#line 515 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("expr(yygotominor.yy0) ::= xor_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1359 "jx_py_parse.c"
        break;
      case 40:
#line 525 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("xor_expr(yygotominor.yy0) ::= and_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1369 "jx_py_parse.c"
        break;
      case 41:
#line 535 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("and_expr(yygotominor.yy0) ::= shift_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1379 "jx_py_parse.c"
        break;
      case 42:
#line 545 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("shirt_exp(yygotominor.yy0) ::= arith_expr(yymsp[0].minor.yy0).\n");
#endif
}
#line 1389 "jx_py_parse.c"
        break;
      case 43:
#line 555 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[0].minor.yy0)\n");
#endif
}
#line 1399 "jx_py_parse.c"
        break;
      case 44:
#line 563 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("add"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[-2].minor.yy0) PLUS term(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(24,&yymsp[-1].minor);
}
#line 1413 "jx_py_parse.c"
        break;
      case 45:
#line 574 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("sub"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("arith_expr(yygotominor.yy0) ::= term(yymsp[-2].minor.yy0) MINUS term(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(25,&yymsp[-1].minor);
}
#line 1427 "jx_py_parse.c"
        break;
      case 46:
#line 587 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[0].minor.yy0).\n");
#endif
}
#line 1437 "jx_py_parse.c"
        break;
      case 47:
#line 595 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("mul"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(26,&yymsp[-1].minor);
}
#line 1451 "jx_py_parse.c"
        break;
      case 48:
#line 606 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("div"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(27,&yymsp[-1].minor);
}
#line 1465 "jx_py_parse.c"
        break;
      case 49:
#line 617 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("div"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(28,&yymsp[-1].minor);
}
#line 1479 "jx_py_parse.c"
        break;
      case 50:
#line 628 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("mod"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_PY_PARSER_DEBUG
  printf("term(yygotominor.yy0) ::= factor(yymsp[-2].minor.yy0).\n");
#endif
  yy_destructor(29,&yymsp[-1].minor);
}
#line 1493 "jx_py_parse.c"
        break;
      case 51:
#line 641 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= power(yymsp[0].minor.yy0).\n");
#endif
}
#line 1503 "jx_py_parse.c"
        break;
      case 52:
#line 649 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= PLUS factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(24,&yymsp[-1].minor);
}
#line 1514 "jx_py_parse.c"
        break;
      case 53:
#line 657 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= PLUS factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(25,&yymsp[-1].minor);
}
#line 1525 "jx_py_parse.c"
        break;
      case 54:
#line 665 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_PY_PARSER_DEBUG
  printf("factor(yygotominor.yy0) ::= TILDE factor(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(30,&yymsp[-1].minor);
}
#line 1536 "jx_py_parse.c"
        break;
      case 55:
#line 675 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy127;
#ifdef JX_PY_PARSER_DEBUG
  printf("power(yygotominor.yy0) ::= atom(yymsp[0].minor.yy127).\n");
#endif
}
#line 1546 "jx_py_parse.c"
        break;
      case 56:
#line 691 "jx_py_parse.lem"
{ 
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = ICON\n"); 
#endif
}
#line 1556 "jx_py_parse.c"
        break;
      case 57:
#line 699 "jx_py_parse.lem"
{ 
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = FCON\n"); 
#endif
}
#line 1566 "jx_py_parse.c"
        break;
      case 58:
#line 707 "jx_py_parse.lem"
{ 
  yygotominor.yy127 = yymsp[0].minor.yy127;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = string\n"); 
#endif
}
#line 1576 "jx_py_parse.c"
        break;
      case 59:
#line 715 "jx_py_parse.lem"
{
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("string = SCON\n"); 
#endif
}
#line 1586 "jx_py_parse.c"
        break;
      case 60:
#line 725 "jx_py_parse.lem"
{
  yygotominor.yy127 = jx_ob_add(yymsp[-1].minor.yy127,yymsp[-1].minor.yy127);
  jx_ob_free(yymsp[-1].minor.yy127);
  jx_ob_free(yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = SCON\n"); 
#endif
}
#line 1598 "jx_py_parse.c"
        break;
      case 61:
#line 735 "jx_py_parse.lem"
{
  yygotominor.yy127 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = NAME\n"); 
#endif
}
#line 1608 "jx_py_parse.c"
        break;
      case 62:
#line 745 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_2(yygotominor.yy0) ::= testlist_2(yymsp[-2].minor.yy0) COMMA test(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 1621 "jx_py_parse.c"
        break;
      case 63:
#line 755 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_2(yygotominor.yy0) ::= test(yymsp[-2].minor.yy0) COMMA test(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 1634 "jx_py_parse.c"
        break;
      case 64:
#line 765 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist_3(yygotominor.yy0) ::= test(yymsp[-1].minor.yy0) COMMA.\n"); 
#endif
  yy_destructor(10,&yymsp[0].minor);
}
#line 1646 "jx_py_parse.c"
        break;
      case 65:
#line 774 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= test(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 1656 "jx_py_parse.c"
        break;
      case 66:
#line 782 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= testlist_2(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 1666 "jx_py_parse.c"
        break;
      case 67:
#line 790 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("testlist(yygotominor.yy0) ::= testlist_3(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 1676 "jx_py_parse.c"
        break;
      case 68:
#line 800 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);

#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_2(yygotominor.yy0) ::= exprlist_2(yymsp[-2].minor.yy0) COMMA expr(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 1689 "jx_py_parse.c"
        break;
      case 69:
#line 810 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_2(yygotominor.yy0) ::= expr(yymsp[-2].minor.yy0) COMMA expr(yymsp[0].minor.yy0).\n"); 
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 1702 "jx_py_parse.c"
        break;
      case 70:
#line 820 "jx_py_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist_3(yygotominor.yy0) ::= expr(yymsp[-1].minor.yy0) COMMA.\n"); 
#endif
  yy_destructor(10,&yymsp[0].minor);
}
#line 1714 "jx_py_parse.c"
        break;
      case 71:
#line 829 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= expr(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 1724 "jx_py_parse.c"
        break;
      case 72:
#line 837 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= exprlist_2(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 1734 "jx_py_parse.c"
        break;
      case 73:
#line 845 "jx_py_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("exprlist(yygotominor.yy0) ::= exprlist_3(yymsp[0].minor.yy0).\n"); 
#endif
}
#line 1744 "jx_py_parse.c"
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
#line 1780 "jx_py_parse.c"
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
#line 1800 "jx_py_parse.c"
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
#line 1826 "jx_py_parse.c"
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
