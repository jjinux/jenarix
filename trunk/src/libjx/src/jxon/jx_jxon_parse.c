/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 87 "jx_jxon_parse.lem"


#include <stdio.h>

#include "jx_jxon_private.h"

#line 16 "jx_jxon_parse.c"
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
**    jx_jxon_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is jx_jxon_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    jx_jxon_ARG_SDECL     A static variable declaration for the %extra_argument
**    jx_jxon_ARG_PDECL     A parameter declaration for the %extra_argument
**    jx_jxon_ARG_STORE     Code to store %extra_argument into yypParser
**    jx_jxon_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 38
#define YYACTIONTYPE unsigned char
#define jx_jxon_TOKENTYPE jx_ob
typedef union {
  jx_jxon_TOKENTYPE yy0;
  jx_ob yy3;
  int yy75;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_jxon_ARG_SDECL jx_jxon_parse_context *context;
#define jx_jxon_ARG_PDECL ,jx_jxon_parse_context *context
#define jx_jxon_ARG_FETCH jx_jxon_parse_context *context = yypParser->context
#define jx_jxon_ARG_STORE yypParser->context = context
#define YYNSTATE 59
#define YYNRULE 43
#define YYERRORSYMBOL 25
#define YYERRSYMDT yy75
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
 /*     0 */    11,   40,   42,    7,    1,   17,    2,   57,   19,   20,
 /*    10 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*    20 */    31,   41,   43,   11,   10,  104,    9,    1,    8,    2,
 /*    30 */    15,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*    40 */    28,   29,   30,   31,   11,  104,  104,    7,    1,   33,
 /*    50 */     2,  104,   19,   20,   21,   22,   23,   24,   25,   26,
 /*    60 */    27,   28,   29,   30,   31,   45,   46,   11,  104,  104,
 /*    70 */   104,    1,  104,    2,  104,   19,   20,   21,   22,   23,
 /*    80 */    24,   25,   26,   27,   28,   29,   30,   31,   37,   38,
 /*    90 */    11,  104,  104,  104,    1,  104,    2,  104,   19,   20,
 /*   100 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   110 */    31,   11,  104,  104,  104,    1,  104,    2,   56,   19,
 /*   120 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*   130 */    30,   31,   11,  104,  104,  104,    1,   55,    2,  104,
 /*   140 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*   150 */    29,   30,   31,   11,  104,  104,  104,    1,  104,    2,
 /*   160 */   104,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*   170 */    28,   29,   30,   31,   53,  104,   13,  104,  104,  104,
 /*   180 */   104,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*   190 */    28,   29,   30,   31,   52,  104,  104,  104,  104,  104,
 /*   200 */   104,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*   210 */    28,   29,   30,   31,   19,   20,   21,   22,   23,   24,
 /*   220 */    25,   26,   27,   28,   29,   30,   31,  104,  104,    4,
 /*   230 */   104,   44,    5,   14,   50,  104,   51,  104,   32,  103,
 /*   240 */    48,   49,   50,    3,   51,   32,  104,   48,   49,   50,
 /*   250 */     6,   51,   18,  104,   48,   49,   50,   58,   51,   48,
 /*   260 */    49,   50,   47,   51,   48,   49,   50,   39,   51,   48,
 /*   270 */    49,   50,   36,   51,   48,   49,   50,   34,   51,   48,
 /*   280 */    49,   50,   54,   51,   48,   49,   50,   16,   51,  104,
 /*   290 */   104,  104,   35,   12,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     4,    2,    3,    7,    8,   29,   10,   11,   12,   13,
 /*    10 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    20 */    24,    2,    3,    4,    6,   37,    6,    8,    6,   10,
 /*    30 */    29,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*    40 */    21,   22,   23,   24,    4,   37,   37,    7,    8,    9,
 /*    50 */    10,   37,   12,   13,   14,   15,   16,   17,   18,   19,
 /*    60 */    20,   21,   22,   23,   24,    2,    3,    4,   37,   37,
 /*    70 */    37,    8,   37,   10,   37,   12,   13,   14,   15,   16,
 /*    80 */    17,   18,   19,   20,   21,   22,   23,   24,    2,    3,
 /*    90 */     4,   37,   37,   37,    8,   37,   10,   37,   12,   13,
 /*   100 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*   110 */    24,    4,   37,   37,   37,    8,   37,   10,   11,   12,
 /*   120 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*   130 */    23,   24,    4,   37,   37,   37,    8,    9,   10,   37,
 /*   140 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*   150 */    22,   23,   24,    4,   37,   37,   37,    8,   37,   10,
 /*   160 */    37,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*   170 */    21,   22,   23,   24,    5,   37,    7,   37,   37,   37,
 /*   180 */    37,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*   190 */    21,   22,   23,   24,    5,   37,   37,   37,   37,   37,
 /*   200 */    37,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*   210 */    21,   22,   23,   24,   12,   13,   14,   15,   16,   17,
 /*   220 */    18,   19,   20,   21,   22,   23,   24,   37,   37,   26,
 /*   230 */    37,   28,   29,   30,   31,   37,   33,   37,   27,   36,
 /*   240 */    29,   30,   31,   32,   33,   27,   37,   29,   30,   31,
 /*   250 */    32,   33,   27,   37,   29,   30,   31,   27,   33,   29,
 /*   260 */    30,   31,   27,   33,   29,   30,   31,   27,   33,   29,
 /*   270 */    30,   31,   27,   33,   29,   30,   31,   27,   33,   29,
 /*   280 */    30,   31,   27,   33,   29,   30,   31,   29,   33,   37,
 /*   290 */    37,   37,   34,   35,
};
#define YY_SHIFT_USE_DFLT (-5)
#define YY_SHIFT_MAX 17
static const short yy_shift_ofst[] = {
 /*     0 */    19,  128,  107,   40,   63,   86,   -4,  149,  149,  149,
 /*    10 */   149,  189,  169,  202,   -1,   18,   22,   20,
};
#define YY_REDUCE_USE_DFLT (-25)
#define YY_REDUCE_MAX 13
static const short yy_reduce_ofst[] = {
 /*     0 */   203,  211,  218,  230,  235,  240,  230,  225,  245,  250,
 /*    10 */   255,  258,    1,  -24,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   102,  102,  102,  102,   62,  102,  102,  102,  102,  102,
 /*    10 */   102,  102,  102,  102,   67,  102,  102,  102,   88,   89,
 /*    20 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*    30 */   100,  101,   86,   84,   79,   80,   81,   63,   64,   71,
 /*    40 */    65,   59,   66,   60,   61,   68,   69,   70,   72,   73,
 /*    50 */    74,   75,   76,   77,   78,   82,   83,   85,   87,
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
  jx_jxon_ARG_SDECL                /* A place to hold %extra_argument */
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
void jx_jxon_Trace(FILE *TraceFILE, char *zTracePrompt){
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
  "$",             "ERROR",         "EOI",           "SEMICOLON",   
  "OPEN_CURLY_BRACE",  "CLOSE_CURLY_BRACE",  "COLON",         "COMMA",       
  "OPEN_RECT_BRACE",  "CLOSE_RECT_BRACE",  "OPEN_PAR",      "CLOSE_PAR",   
  "BUILTIN",       "ICON",          "FCON",          "SCON",        
  "IDENT",         "TRUE",          "FALSE",         "NULL",        
  "JUMP_REL",      "BREAK",         "CONTINUE",      "RETURN",      
  "TAIL_CALL",     "error",         "implicit_list",  "expression",  
  "stop_expression",  "primitive",     "container",     "list",        
  "list_of_expressions",  "hash",          "association",   "list_of_associations",
  "statement",   
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "statement ::= EOI",
 /*   1 */ "statement ::= SEMICOLON",
 /*   2 */ "statement ::= stop_expression",
 /*   3 */ "statement ::= implicit_list",
 /*   4 */ "stop_expression ::= primitive EOI",
 /*   5 */ "stop_expression ::= primitive SEMICOLON",
 /*   6 */ "stop_expression ::= container EOI",
 /*   7 */ "stop_expression ::= container SEMICOLON",
 /*   8 */ "stop_expression ::= container",
 /*   9 */ "implicit_list ::= implicit_list EOI",
 /*  10 */ "implicit_list ::= implicit_list SEMICOLON",
 /*  11 */ "implicit_list ::= implicit_list expression",
 /*  12 */ "implicit_list ::= primitive expression",
 /*  13 */ "expression ::= primitive",
 /*  14 */ "expression ::= container",
 /*  15 */ "container ::= list",
 /*  16 */ "container ::= hash",
 /*  17 */ "hash ::= OPEN_CURLY_BRACE CLOSE_CURLY_BRACE",
 /*  18 */ "hash ::= OPEN_CURLY_BRACE list_of_associations CLOSE_CURLY_BRACE",
 /*  19 */ "list_of_associations ::= list_of_associations primitive COLON expression",
 /*  20 */ "list_of_associations ::= list_of_associations COMMA primitive COLON expression",
 /*  21 */ "list_of_associations ::= association",
 /*  22 */ "association ::= primitive COLON expression",
 /*  23 */ "list ::= OPEN_RECT_BRACE CLOSE_RECT_BRACE",
 /*  24 */ "list ::= OPEN_PAR CLOSE_PAR",
 /*  25 */ "list ::= OPEN_RECT_BRACE list_of_expressions CLOSE_RECT_BRACE",
 /*  26 */ "list ::= OPEN_PAR list_of_expressions CLOSE_PAR",
 /*  27 */ "list_of_expressions ::= expression",
 /*  28 */ "list_of_expressions ::= list_of_expressions expression",
 /*  29 */ "list_of_expressions ::= list_of_expressions COMMA expression",
 /*  30 */ "primitive ::= BUILTIN",
 /*  31 */ "primitive ::= ICON",
 /*  32 */ "primitive ::= FCON",
 /*  33 */ "primitive ::= SCON",
 /*  34 */ "primitive ::= IDENT",
 /*  35 */ "primitive ::= TRUE",
 /*  36 */ "primitive ::= FALSE",
 /*  37 */ "primitive ::= NULL",
 /*  38 */ "primitive ::= JUMP_REL",
 /*  39 */ "primitive ::= BREAK",
 /*  40 */ "primitive ::= CONTINUE",
 /*  41 */ "primitive ::= RETURN",
 /*  42 */ "primitive ::= TAIL_CALL",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *jx_jxon_TokenName(int tokenType){
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
** to jx_jxon_ and jx_jxon_Free.
*/
void *jx_jxon_Alloc(void *(*mallocProc)(size_t)){
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
#line 122 "jx_jxon_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 447 "jx_jxon_parse.c"
      break;
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
#line 109 "jx_jxon_parse.lem"
{ jx_ob_free((yypminor->yy3)); }
#line 461 "jx_jxon_parse.c"
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
**       obtained from jx_jxon_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void jx_jxon_Free(
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
     jx_jxon_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     jx_jxon_ARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 28, 2 },
  { 28, 2 },
  { 28, 2 },
  { 28, 2 },
  { 28, 1 },
  { 26, 2 },
  { 26, 2 },
  { 26, 2 },
  { 26, 2 },
  { 27, 1 },
  { 27, 1 },
  { 30, 1 },
  { 30, 1 },
  { 33, 2 },
  { 33, 3 },
  { 35, 4 },
  { 35, 5 },
  { 35, 1 },
  { 34, 3 },
  { 31, 2 },
  { 31, 2 },
  { 31, 3 },
  { 31, 3 },
  { 32, 1 },
  { 32, 2 },
  { 32, 3 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
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
  jx_jxon_ARG_FETCH;
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
#line 174 "jx_jxon_parse.lem"
{  
#ifdef JX_JXON_PARSER_DEBUG
  printf("statement = EOI\n"); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 724 "jx_jxon_parse.c"
        break;
      case 1:
#line 181 "jx_jxon_parse.lem"
{  
#ifdef JX_JXON_PARSER_DEBUG
  printf("statement = SEMICOLON\n"); 
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 734 "jx_jxon_parse.c"
        break;
      case 2:
#line 188 "jx_jxon_parse.lem"
{  
#ifdef JX_JXON_PARSER_DEBUG
  printf("statement = expression(yymsp[0].minor.yy3) EOI\n"); 
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[0].minor.yy3; /* save the result */
}
#line 745 "jx_jxon_parse.c"
        break;
      case 3:
#line 197 "jx_jxon_parse.lem"
{  
#ifdef JX_JXON_PARSER_DEBUG
  printf("statement ::= implicit_list(yymsp[0].minor.yy3).\n"); 
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[0].minor.yy3; /* save the result */
}
#line 756 "jx_jxon_parse.c"
        break;
      case 4:
      case 6:
#line 206 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[-1].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("stop_expression(yygotominor.yy3) ::= expression(yymsp[-1].minor.yy3) EOI\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 768 "jx_jxon_parse.c"
        break;
      case 5:
#line 214 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[-1].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("stop_expression(yygotominor.yy3) ::= expression(yymsp[-1].minor.yy3) EOI\n");
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 779 "jx_jxon_parse.c"
        break;
      case 7:
#line 230 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[-1].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("stop_expression(yygotominor.yy3) ::= expression(yymsp[-1].minor.yy3) SEMICOLON\n");
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 790 "jx_jxon_parse.c"
        break;
      case 8:
#line 238 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[0].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("stop_expression(yygotominor.yy3) ::= expression(yymsp[0].minor.yy3) SEMICOLON\n");
#endif
}
#line 800 "jx_jxon_parse.c"
        break;
      case 9:
#line 247 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[-1].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("implicit_list(yygotominor.yy3) ::= implicit_list(yymsp[-1].minor.yy3) EOI.\n");
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 811 "jx_jxon_parse.c"
        break;
      case 10:
#line 255 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[-1].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("implicit_list(yygotominor.yy3) ::= implicit_list(yymsp[-1].minor.yy3) SEMICOLON.\n");
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 822 "jx_jxon_parse.c"
        break;
      case 11:
#line 263 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = yymsp[-1].minor.yy3;
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy3);
#ifdef JX_JXON_PARSER_DEBUG
  printf("implicit_list(yygotominor.yy3) ::= implicit_list(yymsp[-1].minor.yy3) expression(yymsp[0].minor.yy3) EOI.\n");
#endif
}
#line 833 "jx_jxon_parse.c"
        break;
      case 12:
#line 272 "jx_jxon_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,yymsp[-1].minor.yy3);
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy3);
#ifdef JX_JXON_PARSER_DEBUG
  printf("implicit_list(yygotominor.yy3) ::= primitive(yymsp[-1].minor.yy3) expression(yymsp[0].minor.yy3).\n");
#endif
}
#line 845 "jx_jxon_parse.c"
        break;
      case 13:
#line 282 "jx_jxon_parse.lem"
{  
   yygotominor.yy3 = yymsp[0].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("expression(yygotominor.yy3) = primitive(yymsp[0].minor.yy3)\n");
#endif
}
#line 855 "jx_jxon_parse.c"
        break;
      case 14:
      case 15:
#line 290 "jx_jxon_parse.lem"
{  
   yygotominor.yy3 = yymsp[0].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("expression(yygotominor.yy3) = list(yymsp[0].minor.yy3)\n");
#endif
}
#line 866 "jx_jxon_parse.c"
        break;
      case 16:
#line 306 "jx_jxon_parse.lem"
{  
   yygotominor.yy3 = yymsp[0].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("expression(yygotominor.yy3) = hash(yymsp[0].minor.yy3)\n");
#endif
}
#line 876 "jx_jxon_parse.c"
        break;
      case 17:
#line 314 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = jx_hash_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf("hash(yygotominor.yy3) = {}\n");
#endif
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(5,&yymsp[0].minor);
}
#line 888 "jx_jxon_parse.c"
        break;
      case 18:
#line 322 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[-1].minor.yy3; 
#ifdef JX_JXON_PARSER_DEBUG
  printf("hash(yygotominor.yy3) = { yymsp[-1].minor.yy3 }\n");
#endif
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
#line 900 "jx_jxon_parse.c"
        break;
      case 19:
#line 330 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[-3].minor.yy3;
  jx_hash_set(yygotominor.yy3,yymsp[-2].minor.yy3,yymsp[0].minor.yy3);
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy3) = list_of_associations(yymsp[-3].minor.yy3), primitive(yymsp[-2].minor.yy3) : expression(yymsp[0].minor.yy3)\n");
#endif
  yy_destructor(6,&yymsp[-1].minor);
}
#line 912 "jx_jxon_parse.c"
        break;
      case 20:
#line 339 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[-4].minor.yy3;
  jx_hash_set(yygotominor.yy3,yymsp[-2].minor.yy3,yymsp[0].minor.yy3);
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy3) = list_of_associations(yymsp[-4].minor.yy3), primitive(yymsp[-2].minor.yy3) : expression(yymsp[0].minor.yy3)\n");
#endif
  yy_destructor(7,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
#line 925 "jx_jxon_parse.c"
        break;
      case 21:
#line 348 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[0].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy3) = association(yymsp[0].minor.yy3)\n");
#endif
}
#line 935 "jx_jxon_parse.c"
        break;
      case 22:
#line 356 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = jx_hash_new();
  jx_hash_set(yygotominor.yy3,yymsp[-2].minor.yy3,yymsp[0].minor.yy3); /* takes ownership of yymsp[-2].minor.yy3 & yymsp[0].minor.yy3 */
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy3) = primitive(yymsp[-2].minor.yy3) : expression(yymsp[0].minor.yy3)\n");
#endif
  yy_destructor(6,&yymsp[-1].minor);
}
#line 947 "jx_jxon_parse.c"
        break;
      case 23:
#line 365 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = jx_list_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf("list(yygotominor.yy3) = []\n");
#endif
  yy_destructor(8,&yymsp[-1].minor);
  yy_destructor(9,&yymsp[0].minor);
}
#line 959 "jx_jxon_parse.c"
        break;
      case 24:
#line 373 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = jx_list_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf("list(yygotominor.yy3) = []\n");
#endif
  yy_destructor(10,&yymsp[-1].minor);
  yy_destructor(11,&yymsp[0].minor);
}
#line 971 "jx_jxon_parse.c"
        break;
      case 25:
#line 381 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[-1].minor.yy3; 
#ifdef JX_JXON_PARSER_DEBUG
  printf("list(yygotominor.yy3) = [ yymsp[-1].minor.yy3 ]\n");
#endif
  yy_destructor(8,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
#line 983 "jx_jxon_parse.c"
        break;
      case 26:
#line 389 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[-1].minor.yy3; 
#ifdef JX_JXON_PARSER_DEBUG
  printf("list(yygotominor.yy3) = [ yymsp[-1].minor.yy3 ]\n");
#endif
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
#line 995 "jx_jxon_parse.c"
        break;
      case 27:
#line 397 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy3);
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_expressions(yygotominor.yy3) = expression(yymsp[0].minor.yy3)\n");
#endif
}
#line 1006 "jx_jxon_parse.c"
        break;
      case 28:
#line 406 "jx_jxon_parse.lem"
{ 
  jx_list_append(yymsp[-1].minor.yy3,yymsp[0].minor.yy3); /* takes ownership of yymsp[0].minor.yy3 */
  yygotominor.yy3 = yymsp[-1].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_expressions(yygotominor.yy3) = list_of_expressions(yymsp[-1].minor.yy3) , expression(yymsp[0].minor.yy3)\n");
#endif
}
#line 1017 "jx_jxon_parse.c"
        break;
      case 29:
#line 415 "jx_jxon_parse.lem"
{ 
  jx_list_append(yymsp[-2].minor.yy3,yymsp[0].minor.yy3); /* takes ownership of yymsp[0].minor.yy3 */
  yygotominor.yy3 = yymsp[-2].minor.yy3;
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_expressions(yygotominor.yy3) = list_of_expressions(yymsp[-2].minor.yy3) , expression(yymsp[0].minor.yy3)\n");
#endif
  yy_destructor(7,&yymsp[-1].minor);
}
#line 1029 "jx_jxon_parse.c"
        break;
      case 30:
#line 424 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = BUILTIN\n"); 
#endif
}
#line 1039 "jx_jxon_parse.c"
        break;
      case 31:
#line 432 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = ICON\n"); 
#endif
}
#line 1049 "jx_jxon_parse.c"
        break;
      case 32:
#line 440 "jx_jxon_parse.lem"
{ 
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = FCON\n"); 
#endif
}
#line 1059 "jx_jxon_parse.c"
        break;
      case 33:
#line 448 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = SCON\n"); 
#endif
}
#line 1069 "jx_jxon_parse.c"
        break;
      case 34:
#line 456 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = IDENT\n"); 
#endif
}
#line 1079 "jx_jxon_parse.c"
        break;
      case 35:
#line 464 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = TRUE\n"); 
#endif
}
#line 1089 "jx_jxon_parse.c"
        break;
      case 36:
#line 472 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = FALSE\n"); 
#endif
}
#line 1099 "jx_jxon_parse.c"
        break;
      case 37:
#line 480 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive = NULL\n"); 
#endif
}
#line 1109 "jx_jxon_parse.c"
        break;
      case 38:
#line 488 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive(yygotominor.yy3) ::= JUMP_REL(yymsp[0].minor.yy0).\n");
#endif
}
#line 1119 "jx_jxon_parse.c"
        break;
      case 39:
#line 496 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive(yygotominor.yy3) ::= LEAVE(yymsp[0].minor.yy0).\n");
#endif
}
#line 1129 "jx_jxon_parse.c"
        break;
      case 40:
#line 503 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive(yygotominor.yy3) ::= CONTINUE(yymsp[0].minor.yy0).\n");
#endif
}
#line 1139 "jx_jxon_parse.c"
        break;
      case 41:
#line 510 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive(yygotominor.yy3) ::= RETURN(yymsp[0].minor.yy0).\n");
#endif
}
#line 1149 "jx_jxon_parse.c"
        break;
      case 42:
#line 518 "jx_jxon_parse.lem"
{
  yygotominor.yy3 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("primitive(yygotominor.yy3) ::= TAIL_CALL(yymsp[0].minor.yy0).\n");
#endif
}
#line 1159 "jx_jxon_parse.c"
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
  jx_jxon_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 139 "jx_jxon_parse.lem"

  context->status = JX_STATUS_SYNTAX_ERROR;
#ifdef JX_JXON_PARSER_DEBUG
  printf("parse failed.\n");
#endif
#line 1195 "jx_jxon_parse.c"
  jx_jxon_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  jx_jxon_ARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 148 "jx_jxon_parse.lem"
  
#ifdef JX_JXON_PARSER_DEBUG
  printf("jx_jxon_parse-error: syntax error.\n");
#endif
#line 1215 "jx_jxon_parse.c"
  jx_jxon_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  jx_jxon_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 130 "jx_jxon_parse.lem"

    context->status = 1;
#ifdef JX_JXON_PARSER_DEBUG
  printf("accept called\n");
#endif
#line 1241 "jx_jxon_parse.c"
  jx_jxon_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "jx_jxon_Alloc" which describes the current state of the parser.
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
void jx_jxon_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  jx_jxon_TOKENTYPE yyminor       /* The value for the token */
  jx_jxon_ARG_PDECL               /* Optional %extra_argument parameter */
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
  jx_jxon_ARG_STORE;

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
