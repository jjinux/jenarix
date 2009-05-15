/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 3 "jx_json_parse.lem"


#include <stdio.h>

#include "jx_json_private.h"

#line 16 "jx_json_parse.c"
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
**    jx_json_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is jx_json_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    jx_json_ARG_SDECL     A static variable declaration for the %extra_argument
**    jx_json_ARG_PDECL     A parameter declaration for the %extra_argument
**    jx_json_ARG_STORE     Code to store %extra_argument into yypParser
**    jx_json_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 26
#define YYACTIONTYPE unsigned char
#define jx_json_TOKENTYPE jx_ob
typedef union {
  jx_json_TOKENTYPE yy0;
  jx_ob yy13;
  int yy51;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_json_ARG_SDECL jx_json_parse_context *context;
#define jx_json_ARG_PDECL ,jx_json_parse_context *context
#define jx_json_ARG_FETCH jx_json_parse_context *context = yypParser->context
#define jx_json_ARG_STORE yypParser->context = context
#define YYNSTATE 33
#define YYNRULE 21
#define YYERRORSYMBOL 15
#define YYERRSYMDT yy51
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
 /*     0 */     1,   32,    4,    5,    8,   11,   20,   21,   22,   23,
 /*    10 */    24,   25,    1,    3,   10,    5,   31,    2,   20,   21,
 /*    20 */    22,   23,   24,   25,   16,   18,   29,   30,   56,   14,
 /*    30 */     7,   56,   16,   18,    9,   56,   55,   14,   13,   56,
 /*    40 */    26,   27,   28,   16,   18,   12,   56,   56,   14,   16,
 /*    50 */    18,   17,   56,   56,   14,   16,   18,   19,   56,   56,
 /*    60 */    14,   26,   27,   28,   56,   56,    6,   56,   15,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,    4,    8,    6,   23,   24,    9,   10,   11,   12,
 /*    10 */    13,   14,    3,    8,   24,    6,    4,    5,    9,   10,
 /*    20 */    11,   12,   13,   14,   16,   17,   18,    2,   25,   21,
 /*    30 */    22,   25,   16,   17,   18,   25,   20,   21,    7,   25,
 /*    40 */     9,   10,   11,   16,   17,   18,   25,   25,   21,   16,
 /*    50 */    17,   18,   25,   25,   21,   16,   17,   18,   25,   25,
 /*    60 */    21,    9,   10,   11,   25,   25,    5,   25,    7,
};
#define YY_SHIFT_USE_DFLT (-7)
#define YY_SHIFT_MAX 11
static const signed char yy_shift_ofst[] = {
 /*     0 */     9,   -3,    9,    9,    9,   31,   52,   12,   61,   25,
 /*    10 */     5,   -6,
};
#define YY_REDUCE_USE_DFLT (-20)
#define YY_REDUCE_MAX 6
static const signed char yy_reduce_ofst[] = {
 /*     0 */    16,    8,   27,   39,   33,  -19,  -10,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    54,   54,   54,   54,   54,   54,   54,   54,   54,   54,
 /*    10 */    54,   54,   40,   41,   36,   42,   35,   43,   34,   44,
 /*    20 */    48,   49,   50,   51,   52,   53,   45,   46,   47,   39,
 /*    30 */    33,   38,   37,
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
  jx_json_ARG_SDECL                /* A place to hold %extra_argument */
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
void jx_json_Trace(FILE *TraceFILE, char *zTracePrompt){
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
  "$",             "ERROR",         "EOI",           "OPEN_RECT_BRACE",
  "CLOSE_RECT_BRACE",  "COMMA",         "OPEN_CURLY_BRACE",  "CLOSE_CURLY_BRACE",
  "COLON",         "ICON",          "FCON",          "SCON",        
  "TRUE",          "FALSE",         "NULL",          "error",       
  "list",          "primitive",     "expression",    "expression_list",
  "statement",     "hash",          "list_of_expressions",  "list_of_associations",
  "key",         
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "statement ::= expression EOI",
 /*   1 */ "expression ::= primitive",
 /*   2 */ "expression ::= list",
 /*   3 */ "expression ::= hash",
 /*   4 */ "list ::= OPEN_RECT_BRACE CLOSE_RECT_BRACE",
 /*   5 */ "list ::= OPEN_RECT_BRACE list_of_expressions CLOSE_RECT_BRACE",
 /*   6 */ "list_of_expressions ::= expression",
 /*   7 */ "list_of_expressions ::= list_of_expressions COMMA expression",
 /*   8 */ "hash ::= OPEN_CURLY_BRACE CLOSE_CURLY_BRACE",
 /*   9 */ "hash ::= OPEN_CURLY_BRACE list_of_associations CLOSE_CURLY_BRACE",
 /*  10 */ "list_of_associations ::= key COLON expression",
 /*  11 */ "list_of_associations ::= list_of_associations COMMA key COLON expression",
 /*  12 */ "key ::= ICON",
 /*  13 */ "key ::= FCON",
 /*  14 */ "key ::= SCON",
 /*  15 */ "primitive ::= ICON",
 /*  16 */ "primitive ::= FCON",
 /*  17 */ "primitive ::= SCON",
 /*  18 */ "primitive ::= TRUE",
 /*  19 */ "primitive ::= FALSE",
 /*  20 */ "primitive ::= NULL",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *jx_json_TokenName(int tokenType){
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
** to jx_json_ and jx_json_Free.
*/
void *jx_json_Alloc(void *(*mallocProc)(size_t)){
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
#line 31 "jx_json_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 363 "jx_json_parse.c"
      break;
    case 16:
    case 17:
#line 25 "jx_json_parse.lem"
{ jx_ob_free((yypminor->yy13)); }
#line 369 "jx_json_parse.c"
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
**       obtained from jx_json_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void jx_json_Free(
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
     jx_json_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     jx_json_ARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 20, 2 },
  { 18, 1 },
  { 18, 1 },
  { 18, 1 },
  { 16, 2 },
  { 16, 3 },
  { 22, 1 },
  { 22, 3 },
  { 21, 2 },
  { 21, 3 },
  { 23, 3 },
  { 23, 5 },
  { 24, 1 },
  { 24, 1 },
  { 24, 1 },
  { 17, 1 },
  { 17, 1 },
  { 17, 1 },
  { 17, 1 },
  { 17, 1 },
  { 17, 1 },
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
  jx_json_ARG_FETCH;
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
#line 88 "jx_json_parse.lem"
{  
#ifdef JX_JSON_PARSER_DEBUG
  printf("statement = expression(yymsp[-1].minor.yy13) EOI\n"); 
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[-1].minor.yy13; /* save the result */
  yy_destructor(2,&yymsp[0].minor);
}
#line 612 "jx_json_parse.c"
        break;
      case 1:
#line 97 "jx_json_parse.lem"
{  
   yygotominor.yy13 = yymsp[0].minor.yy13;
#ifdef JX_JSON_PARSER_DEBUG
  printf("expression(yygotominor.yy13) = primitive(yymsp[0].minor.yy13)\n");
#endif
}
#line 622 "jx_json_parse.c"
        break;
      case 2:
#line 105 "jx_json_parse.lem"
{  
   yygotominor.yy13 = yymsp[0].minor.yy13;
#ifdef JX_JSON_PARSER_DEBUG
  printf("expression(yygotominor.yy13) = list(yymsp[0].minor.yy13)\n");
#endif
}
#line 632 "jx_json_parse.c"
        break;
      case 3:
#line 113 "jx_json_parse.lem"
{  
   yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("expression(yygotominor.yy13) = hash(yymsp[0].minor.yy0)\n");
#endif
}
#line 642 "jx_json_parse.c"
        break;
      case 4:
#line 130 "jx_json_parse.lem"
{ 
  yygotominor.yy13 = jx_list_new();
#ifdef JX_JSON_PARSER_DEBUG
  printf("list(yygotominor.yy13) = []\n");
#endif
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
#line 654 "jx_json_parse.c"
        break;
      case 5:
#line 138 "jx_json_parse.lem"
{ 
  yygotominor.yy13 = yymsp[-1].minor.yy0; 
#ifdef JX_JSON_PARSER_DEBUG
  printf("list(yygotominor.yy13) = [ yymsp[-1].minor.yy0 ]\n");
#endif
  yy_destructor(3,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
#line 666 "jx_json_parse.c"
        break;
      case 6:
#line 146 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy13);
#ifdef JX_JSON_PARSER_DEBUG
  printf("list_of_expressions(yygotominor.yy0) = expression(yymsp[0].minor.yy13)\n");
#endif
}
#line 677 "jx_json_parse.c"
        break;
      case 7:
#line 155 "jx_json_parse.lem"
{ 
  jx_list_append(yymsp[-2].minor.yy0,yymsp[0].minor.yy13); /* takes ownership of yymsp[0].minor.yy13 */
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("list_of_expressions(yygotominor.yy0) = list_of_expressions(yymsp[-2].minor.yy0) , expression(yymsp[0].minor.yy13)\n");
#endif
  yy_destructor(5,&yymsp[-1].minor);
}
#line 689 "jx_json_parse.c"
        break;
      case 8:
#line 164 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = jx_hash_new();
#ifdef JX_JSON_PARSER_DEBUG
  printf("hash(yygotominor.yy0) = {}\n");
#endif
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 701 "jx_json_parse.c"
        break;
      case 9:
#line 172 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-1].minor.yy0; 
#ifdef JX_JSON_PARSER_DEBUG
  printf("hash(yygotominor.yy0) = { yymsp[-1].minor.yy0 }\n");
#endif
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
#line 713 "jx_json_parse.c"
        break;
      case 10:
#line 180 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = jx_hash_new();
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy13); /* takes ownership of yymsp[-2].minor.yy0 & yymsp[0].minor.yy13 */
#ifdef JX_JSON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy0) = key(yymsp[-2].minor.yy0) : value(yymsp[0].minor.yy13)\n");
#endif
  yy_destructor(8,&yymsp[-1].minor);
}
#line 725 "jx_json_parse.c"
        break;
      case 11:
#line 189 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-4].minor.yy0;
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy13);
#ifdef JX_JSON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy0) = list_of_associations(yymsp[-4].minor.yy0) , expression(yymsp[-2].minor.yy0)\n");
#endif
  yy_destructor(5,&yymsp[-3].minor);
  yy_destructor(8,&yymsp[-1].minor);
}
#line 738 "jx_json_parse.c"
        break;
      case 12:
#line 198 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("key = ICON\n"); 
#endif
}
#line 748 "jx_json_parse.c"
        break;
      case 13:
#line 206 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("key = FCON\n"); 
#endif
}
#line 758 "jx_json_parse.c"
        break;
      case 14:
#line 214 "jx_json_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("key = SCON\n"); 
#endif
}
#line 768 "jx_json_parse.c"
        break;
      case 15:
#line 222 "jx_json_parse.lem"
{ 
  yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("primitive = ICON\n"); 
#endif
}
#line 778 "jx_json_parse.c"
        break;
      case 16:
#line 230 "jx_json_parse.lem"
{ 
  yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("primitive = FCON\n"); 
#endif
}
#line 788 "jx_json_parse.c"
        break;
      case 17:
#line 238 "jx_json_parse.lem"
{
  yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("primitive = SCON\n"); 
#endif
}
#line 798 "jx_json_parse.c"
        break;
      case 18:
#line 246 "jx_json_parse.lem"
{
  yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("primitive = TRUE\n"); 
#endif
}
#line 808 "jx_json_parse.c"
        break;
      case 19:
#line 254 "jx_json_parse.lem"
{
  yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("primitive = FALSE\n"); 
#endif
}
#line 818 "jx_json_parse.c"
        break;
      case 20:
#line 262 "jx_json_parse.lem"
{
  yygotominor.yy13 = yymsp[0].minor.yy0;
#ifdef JX_JSON_PARSER_DEBUG
  printf("primitive = NULL\n"); 
#endif
}
#line 828 "jx_json_parse.c"
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
  jx_json_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 48 "jx_json_parse.lem"

  context->status = -1;
#ifdef JX_JSON_PARSER_DEBUG
  printf("parse failed.\n");
#endif
#line 864 "jx_json_parse.c"
  jx_json_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  jx_json_ARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 57 "jx_json_parse.lem"
   
#ifdef JX_JSON_PARSER_DEBUG
  printf("jx_json_parse-error: syntax error.\n");
#endif
#line 884 "jx_json_parse.c"
  jx_json_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  jx_json_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 39 "jx_json_parse.lem"

    context->status = 1;
#ifdef JX_JSON_PARSER_DEBUG
  printf("accept called\n");
#endif
#line 910 "jx_json_parse.c"
  jx_json_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "jx_json_Alloc" which describes the current state of the parser.
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
void jx_json_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  jx_json_TOKENTYPE yyminor       /* The value for the token */
  jx_json_ARG_PDECL               /* Optional %extra_argument parameter */
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
  jx_json_ARG_STORE;

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
