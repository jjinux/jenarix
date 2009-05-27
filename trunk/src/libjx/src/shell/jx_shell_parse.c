/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 87 "jx_shell_parse.lem"


#include <stdio.h>

#include "jx_shell_private.h"

#line 16 "jx_shell_parse.c"
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
**    jx_shell_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is jx_shell_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    jx_shell_ARG_SDECL     A static variable declaration for the %extra_argument
**    jx_shell_ARG_PDECL     A parameter declaration for the %extra_argument
**    jx_shell_ARG_STORE     Code to store %extra_argument into yypParser
**    jx_shell_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 37
#define YYACTIONTYPE unsigned char
#define jx_shell_TOKENTYPE jx_ob
typedef union {
  jx_shell_TOKENTYPE yy0;
  int yy73;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_shell_ARG_SDECL jx_shell_parse_context *context;
#define jx_shell_ARG_PDECL ,jx_shell_parse_context *context
#define jx_shell_ARG_FETCH jx_shell_parse_context *context = yypParser->context
#define jx_shell_ARG_STORE yypParser->context = context
#define YYNSTATE 59
#define YYNRULE 40
#define YYERRORSYMBOL 22
#define YYERRSYMDT yy73
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
 /*     0 */    24,    9,   13,   15,  100,   18,    3,   29,    4,    2,
 /*    10 */    55,   56,   57,   58,   25,   26,   27,   24,    9,   21,
 /*    20 */    15,   23,    5,    3,   12,    4,   52,   55,   56,   57,
 /*    30 */    58,   25,   26,   27,   24,   11,  101,   15,  101,  101,
 /*    40 */     3,  101,    4,   51,   55,   56,   57,   58,   25,   26,
 /*    50 */    27,   24,  101,  101,   15,  101,  101,    3,   50,    4,
 /*    60 */   101,   55,   56,   57,   58,   25,   26,   27,   24,  101,
 /*    70 */   101,   15,  101,  101,    3,  101,    4,  101,   55,   56,
 /*    80 */    57,   58,   25,   26,   27,   24,   17,  101,   10,   48,
 /*    90 */    43,   34,   44,   45,   46,   55,   56,   57,   58,   25,
 /*   100 */    26,   27,   24,   39,   41,   40,   47,   43,   49,   44,
 /*   110 */    45,   46,   55,   56,   57,   58,   25,   26,   27,   24,
 /*   120 */   101,   22,   14,   36,   19,   34,   44,   45,   46,   55,
 /*   130 */    56,   57,   58,   25,   26,   27,  101,  101,  101,    8,
 /*   140 */    33,   19,   34,   44,   45,   46,   43,   28,   44,   45,
 /*   150 */    46,  101,  101,    6,   43,   28,   44,   45,   46,  101,
 /*   160 */   101,    7,   43,   53,   44,   45,   46,  101,   43,   42,
 /*   170 */    44,   45,   46,   43,   54,   44,   45,   46,   43,   32,
 /*   180 */    44,   45,   46,   43,   30,   44,   45,   46,   35,   37,
 /*   190 */    38,    1,   20,  101,  101,  101,  101,   16,   31,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     5,    6,   10,    8,   23,   24,   11,   12,   13,    6,
 /*    10 */    15,   16,   17,   18,   19,   20,   21,    5,    6,   28,
 /*    20 */     8,   28,    7,   11,   10,   13,   14,   15,   16,   17,
 /*    30 */    18,   19,   20,   21,    5,   10,   36,    8,   36,   36,
 /*    40 */    11,   36,   13,   14,   15,   16,   17,   18,   19,   20,
 /*    50 */    21,    5,   36,   36,    8,   36,   36,   11,   12,   13,
 /*    60 */    36,   15,   16,   17,   18,   19,   20,   21,    5,   36,
 /*    70 */    36,    8,   36,   36,   11,   36,   13,   36,   15,   16,
 /*    80 */    17,   18,   19,   20,   21,    5,    6,   36,   26,    9,
 /*    90 */    28,   29,   30,   31,   32,   15,   16,   17,   18,   19,
 /*   100 */    20,   21,    5,    2,    3,    4,    9,   28,   29,   30,
 /*   110 */    31,   32,   15,   16,   17,   18,   19,   20,   21,    5,
 /*   120 */    36,   25,   26,   27,   28,   29,   30,   31,   32,   15,
 /*   130 */    16,   17,   18,   19,   20,   21,   36,   36,   36,   26,
 /*   140 */    27,   28,   29,   30,   31,   32,   28,   29,   30,   31,
 /*   150 */    32,   36,   36,   35,   28,   29,   30,   31,   32,   36,
 /*   160 */    36,   35,   28,   29,   30,   31,   32,   36,   28,   29,
 /*   170 */    30,   31,   32,   28,   29,   30,   31,   32,   28,   29,
 /*   180 */    30,   31,   32,   28,   29,   30,   31,   32,    2,    3,
 /*   190 */     4,    5,   28,   36,   36,   36,   36,   33,   34,
};
#define YY_SHIFT_USE_DFLT (-9)
#define YY_SHIFT_MAX 23
static const short yy_shift_ofst[] = {
 /*     0 */   186,   63,   63,   46,   29,   63,   12,   -5,   63,   63,
 /*    10 */    63,   63,   63,   63,   63,   97,   80,  114,  101,   15,
 /*    20 */    25,   14,    3,   -8,
};
#define YY_REDUCE_USE_DFLT (-20)
#define YY_REDUCE_MAX 17
static const short yy_reduce_ofst[] = {
 /*     0 */   -19,   96,  113,  126,  118,   62,  134,  134,  140,  145,
 /*    10 */   140,  150,   79,  155,  140,  164,   -9,   -7,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    99,   65,   99,   99,   99,   99,   99,   99,   69,   99,
 /*    10 */    71,   99,   99,   99,   67,   99,   99,   99,   99,   74,
 /*    20 */    99,   99,   66,   99,   95,   96,   97,   98,   88,   86,
 /*    30 */    81,   82,   83,   70,   72,   59,   68,   60,   61,   62,
 /*    40 */    63,   64,   73,   74,   75,   76,   77,   78,   79,   80,
 /*    50 */    84,   85,   87,   89,   90,   91,   92,   93,   94,
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
  jx_shell_ARG_SDECL                /* A place to hold %extra_argument */
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
void jx_shell_Trace(FILE *TraceFILE, char *zTracePrompt){
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
  "EOL",           "IDENT",         "COMMA",         "EQUALS",      
  "OPEN_CURLY_BRACE",  "CLOSE_CURLY_BRACE",  "COLON",         "OPEN_RECT_BRACE",
  "CLOSE_RECT_BRACE",  "OPEN_PAR",      "CLOSE_PAR",     "BUILTIN",     
  "ICON",          "FCON",          "SCON",          "TRUE",        
  "FALSE",         "NULL",          "error",         "statement",   
  "shell_expression",  "argument_list_of_lists",  "expression_list",  "keyed_expression_list",
  "primitive",     "expression",    "container",     "list",        
  "hash",          "list_of_associations",  "association",   "inner_expression_list",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "statement ::= EOI",
 /*   1 */ "statement ::= SEMICOLON",
 /*   2 */ "statement ::= EOL",
 /*   3 */ "statement ::= shell_expression EOI",
 /*   4 */ "statement ::= shell_expression EOL",
 /*   5 */ "statement ::= shell_expression SEMICOLON",
 /*   6 */ "shell_expression ::= IDENT",
 /*   7 */ "shell_expression ::= IDENT argument_list_of_lists",
 /*   8 */ "argument_list_of_lists ::= expression_list",
 /*   9 */ "argument_list_of_lists ::= keyed_expression_list",
 /*  10 */ "argument_list_of_lists ::= argument_list_of_lists COMMA expression_list",
 /*  11 */ "argument_list_of_lists ::= argument_list_of_lists COMMA keyed_expression_list",
 /*  12 */ "keyed_expression_list ::= primitive EQUALS expression_list",
 /*  13 */ "expression_list ::= expression",
 /*  14 */ "expression_list ::= expression_list expression",
 /*  15 */ "expression ::= primitive",
 /*  16 */ "expression ::= container",
 /*  17 */ "container ::= list",
 /*  18 */ "container ::= hash",
 /*  19 */ "hash ::= OPEN_CURLY_BRACE CLOSE_CURLY_BRACE",
 /*  20 */ "hash ::= OPEN_CURLY_BRACE list_of_associations CLOSE_CURLY_BRACE",
 /*  21 */ "list_of_associations ::= list_of_associations primitive COLON expression",
 /*  22 */ "list_of_associations ::= list_of_associations COMMA primitive COLON expression",
 /*  23 */ "list_of_associations ::= association",
 /*  24 */ "association ::= primitive COLON expression",
 /*  25 */ "list ::= OPEN_RECT_BRACE CLOSE_RECT_BRACE",
 /*  26 */ "list ::= OPEN_PAR CLOSE_PAR",
 /*  27 */ "list ::= OPEN_RECT_BRACE inner_expression_list CLOSE_RECT_BRACE",
 /*  28 */ "list ::= OPEN_PAR inner_expression_list CLOSE_PAR",
 /*  29 */ "inner_expression_list ::= expression",
 /*  30 */ "inner_expression_list ::= inner_expression_list expression",
 /*  31 */ "inner_expression_list ::= inner_expression_list COMMA expression",
 /*  32 */ "primitive ::= BUILTIN",
 /*  33 */ "primitive ::= ICON",
 /*  34 */ "primitive ::= FCON",
 /*  35 */ "primitive ::= SCON",
 /*  36 */ "primitive ::= IDENT",
 /*  37 */ "primitive ::= TRUE",
 /*  38 */ "primitive ::= FALSE",
 /*  39 */ "primitive ::= NULL",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *jx_shell_TokenName(int tokenType){
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
** to jx_shell_ and jx_shell_Free.
*/
void *jx_shell_Alloc(void *(*mallocProc)(size_t)){
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
#line 113 "jx_shell_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 420 "jx_shell_parse.c"
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
**       obtained from jx_shell_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void jx_shell_Free(
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
     jx_shell_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     jx_shell_ARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 23, 1 },
  { 23, 1 },
  { 23, 1 },
  { 23, 2 },
  { 23, 2 },
  { 23, 2 },
  { 24, 1 },
  { 24, 2 },
  { 25, 1 },
  { 25, 1 },
  { 25, 3 },
  { 25, 3 },
  { 27, 3 },
  { 26, 1 },
  { 26, 2 },
  { 29, 1 },
  { 29, 1 },
  { 30, 1 },
  { 30, 1 },
  { 32, 2 },
  { 32, 3 },
  { 33, 4 },
  { 33, 5 },
  { 33, 1 },
  { 34, 3 },
  { 31, 2 },
  { 31, 2 },
  { 31, 3 },
  { 31, 3 },
  { 35, 1 },
  { 35, 2 },
  { 35, 3 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
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
  jx_shell_ARG_FETCH;
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
#line 156 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("statement = EOI\n"); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 680 "jx_shell_parse.c"
        break;
      case 1:
#line 162 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("statement = SEMICOLON\n"); 
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 690 "jx_shell_parse.c"
        break;
      case 2:
#line 169 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("statement = EOL\n"); 
#endif
  yy_destructor(4,&yymsp[0].minor);
}
#line 700 "jx_shell_parse.c"
        break;
      case 3:
#line 176 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("statement ::= shell_expression(yymsp[-1].minor.yy0) EOI.\n"); 
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[-1].minor.yy0; /* save the result */
  yy_destructor(2,&yymsp[0].minor);
}
#line 712 "jx_shell_parse.c"
        break;
      case 4:
#line 185 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("statement = expression(yymsp[-1].minor.yy0) EOL.\n"); 
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[-1].minor.yy0; /* save the result */
  yy_destructor(4,&yymsp[0].minor);
}
#line 724 "jx_shell_parse.c"
        break;
      case 5:
#line 194 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("statement = expression(yymsp[-1].minor.yy0) SEMICOLON.\n"); 
#endif
  jx_ob_free(context->result); /* free previous result (if any) */ 
  context->result = yymsp[-1].minor.yy0; /* save the result */
  yy_destructor(3,&yymsp[0].minor);
}
#line 736 "jx_shell_parse.c"
        break;
      case 6:
#line 203 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("shell_expression(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0).\n"); 
#endif
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 747 "jx_shell_parse.c"
        break;
      case 7:
#line 212 "jx_shell_parse.lem"
{  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("shell_expression ::= keyword(yymsp[-1].minor.yy0) argument_list(yymsp[0].minor.yy0)\n"); 
#endif
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 759 "jx_shell_parse.c"
        break;
      case 8:
      case 9:
#line 222 "jx_shell_parse.lem"
{ 
#ifdef JX_SHELL_PARSER_DEBUG
  printf("pos_arg_list(yygotominor.yy0) = expression(yymsp[0].minor.yy0)\n");
#endif
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 771 "jx_shell_parse.c"
        break;
      case 10:
      case 11:
#line 240 "jx_shell_parse.lem"
{ 
#ifdef JX_SHELL_PARSER_DEBUG
  printf("pos_arg_list(yygotominor.yy0) = expression(yymsp[-2].minor.yy0)\n");
#endif
  yygotominor.yy0=yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(6,&yymsp[-1].minor);
}
#line 784 "jx_shell_parse.c"
        break;
      case 12:
#line 258 "jx_shell_parse.lem"
{  
  yygotominor.yy0 = jx_hash_new();
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);

#ifdef JX_SHELL_PARSER_DEBUG
  printf("expression(yygotominor.yy0) = primitive(yymsp[-2].minor.yy0)\n");
#endif
  yy_destructor(7,&yymsp[-1].minor);
}
#line 797 "jx_shell_parse.c"
        break;
      case 13:
#line 268 "jx_shell_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_SHELL_PARSER_DEBUG
  printf("expression(yygotominor.yy0) = primitive(yymsp[0].minor.yy0)\n");
#endif
}
#line 808 "jx_shell_parse.c"
        break;
      case 14:
#line 277 "jx_shell_parse.lem"
{  
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_SHELL_PARSER_DEBUG
  printf("expression_list(yygotominor.yy0) ::= expression_list(yymsp[-1].minor.yy0) expression(yymsp[0].minor.yy0)\n");
#endif
}
#line 819 "jx_shell_parse.c"
        break;
      case 15:
#line 287 "jx_shell_parse.lem"
{  
   yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("expression(yygotominor.yy0) = primitive(yymsp[0].minor.yy0)\n");
#endif
}
#line 829 "jx_shell_parse.c"
        break;
      case 16:
      case 17:
      case 18:
#line 295 "jx_shell_parse.lem"
{  
   yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("expression(yygotominor.yy0) = list(yymsp[0].minor.yy0)\n");
#endif
}
#line 841 "jx_shell_parse.c"
        break;
      case 19:
#line 319 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = jx_hash_new();
#ifdef JX_JXON_PARSER_DEBUG
  printf("hash(yygotominor.yy0) = {}\n");
#endif
  yy_destructor(8,&yymsp[-1].minor);
  yy_destructor(9,&yymsp[0].minor);
}
#line 853 "jx_shell_parse.c"
        break;
      case 20:
#line 327 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-1].minor.yy0; 
#ifdef JX_JXON_PARSER_DEBUG
  printf("hash(yygotominor.yy0) = { yymsp[-1].minor.yy0 }\n");
#endif
  yy_destructor(8,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
#line 865 "jx_shell_parse.c"
        break;
      case 21:
#line 335 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-3].minor.yy0;
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy0) = list_of_associations(yymsp[-3].minor.yy0), primitive(yymsp[-2].minor.yy0) : expression(yymsp[0].minor.yy0)\n");
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 877 "jx_shell_parse.c"
        break;
      case 22:
#line 344 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-4].minor.yy0;
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy0) = list_of_associations(yymsp[-4].minor.yy0), primitive(yymsp[-2].minor.yy0) : expression(yymsp[0].minor.yy0)\n");
#endif
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(10,&yymsp[-1].minor);
}
#line 890 "jx_shell_parse.c"
        break;
      case 23:
#line 353 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy0) = association(yymsp[0].minor.yy0)\n");
#endif
}
#line 900 "jx_shell_parse.c"
        break;
      case 24:
#line 361 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = jx_hash_new();
  jx_hash_set(yygotominor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0); /* takes ownership of yymsp[-2].minor.yy0 & yymsp[0].minor.yy0 */
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_associations(yygotominor.yy0) = primitive(yymsp[-2].minor.yy0) : expression(yymsp[0].minor.yy0)\n");
#endif
  yy_destructor(10,&yymsp[-1].minor);
}
#line 912 "jx_shell_parse.c"
        break;
      case 25:
#line 370 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = jx_list_new();
#ifdef JX_SHELL_PARSER_DEBUG
  printf("list(yygotominor.yy0) = []\n");
#endif
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
#line 924 "jx_shell_parse.c"
        break;
      case 26:
#line 378 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = jx_list_new();
#ifdef JX_SHELL_PARSER_DEBUG
  printf("list(yygotominor.yy0) = []\n");
#endif
  yy_destructor(13,&yymsp[-1].minor);
  yy_destructor(14,&yymsp[0].minor);
}
#line 936 "jx_shell_parse.c"
        break;
      case 27:
#line 386 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-1].minor.yy0; 
#ifdef JX_SHELL_PARSER_DEBUG
  printf("list(yygotominor.yy0) = [ yymsp[-1].minor.yy0 ]\n");
#endif
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
#line 948 "jx_shell_parse.c"
        break;
      case 28:
#line 394 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[-1].minor.yy0; 
#ifdef JX_SHELL_PARSER_DEBUG
  printf("list(yygotominor.yy0) = [ yymsp[-1].minor.yy0 ]\n");
#endif
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(14,&yymsp[0].minor);
}
#line 960 "jx_shell_parse.c"
        break;
      case 29:
#line 402 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_SHELL_PARSER_DEBUG
  printf("inner_expression_list(yygotominor.yy0) = expression(yymsp[0].minor.yy0)\n");
#endif
}
#line 971 "jx_shell_parse.c"
        break;
      case 30:
#line 411 "jx_shell_parse.lem"
{ 
  jx_list_append(yymsp[-1].minor.yy0,yymsp[0].minor.yy0); /* takes ownership of yymsp[0].minor.yy0 */
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("inner_expression_list(yygotominor.yy0) = inner_expression_list(yymsp[-1].minor.yy0) , expression(yymsp[0].minor.yy0)\n");
#endif
}
#line 982 "jx_shell_parse.c"
        break;
      case 31:
#line 420 "jx_shell_parse.lem"
{ 
  jx_list_append(yymsp[-2].minor.yy0,yymsp[0].minor.yy0); /* takes ownership of yymsp[0].minor.yy0 */
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("list_of_expressions(yygotominor.yy0) = list_of_expressions(yymsp[-2].minor.yy0) , expression(yymsp[0].minor.yy0)\n");
#endif
  yy_destructor(6,&yymsp[-1].minor);
}
#line 994 "jx_shell_parse.c"
        break;
      case 32:
#line 429 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = BUILTIN\n"); 
#endif
}
#line 1004 "jx_shell_parse.c"
        break;
      case 33:
#line 437 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = ICON\n"); 
#endif
}
#line 1014 "jx_shell_parse.c"
        break;
      case 34:
#line 445 "jx_shell_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = FCON\n"); 
#endif
}
#line 1024 "jx_shell_parse.c"
        break;
      case 35:
#line 453 "jx_shell_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = SCON\n"); 
#endif
}
#line 1034 "jx_shell_parse.c"
        break;
      case 36:
#line 461 "jx_shell_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = IDENT\n"); 
#endif
}
#line 1044 "jx_shell_parse.c"
        break;
      case 37:
#line 469 "jx_shell_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = TRUE\n"); 
#endif
}
#line 1054 "jx_shell_parse.c"
        break;
      case 38:
#line 477 "jx_shell_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = FALSE\n"); 
#endif
}
#line 1064 "jx_shell_parse.c"
        break;
      case 39:
#line 485 "jx_shell_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("primitive = NULL\n"); 
#endif
}
#line 1074 "jx_shell_parse.c"
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
  jx_shell_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 130 "jx_shell_parse.lem"

  context->status = JX_STATUS_SYNTAX_ERROR;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("parse failed.\n");
#endif
#line 1110 "jx_shell_parse.c"
  jx_shell_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  jx_shell_ARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 139 "jx_shell_parse.lem"
  
#ifdef JX_SHELL_PARSER_DEBUG
  printf("jx_shell_parse-error: syntax error.\n");
#endif
#line 1130 "jx_shell_parse.c"
  jx_shell_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  jx_shell_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 121 "jx_shell_parse.lem"

    context->status = 1;
#ifdef JX_SHELL_PARSER_DEBUG
  printf("accept called\n");
#endif
#line 1156 "jx_shell_parse.c"
  jx_shell_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "jx_shell_Alloc" which describes the current state of the parser.
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
void jx_shell_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  jx_shell_TOKENTYPE yyminor       /* The value for the token */
  jx_shell_ARG_PDECL               /* Optional %extra_argument parameter */
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
  jx_shell_ARG_STORE;

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
