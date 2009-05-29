/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 37 "jx_net_parse.lem"


#include <stdio.h>

#include "jx_net_private.h"

#line 16 "jx_net_parse.c"
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
**    jx_net_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is jx_net_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    jx_net_ARG_SDECL     A static variable declaration for the %extra_argument
**    jx_net_ARG_PDECL     A parameter declaration for the %extra_argument
**    jx_net_ARG_STORE     Code to store %extra_argument into yypParser
**    jx_net_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 38
#define YYACTIONTYPE unsigned char
#define jx_net_TOKENTYPE jx_ob
typedef union {
  jx_net_TOKENTYPE yy0;
  jx_ob yy3;
  int yy75;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_net_ARG_SDECL jx_net_parse_context *context;
#define jx_net_ARG_PDECL ,jx_net_parse_context *context
#define jx_net_ARG_FETCH jx_net_parse_context *context = yypParser->context
#define jx_net_ARG_STORE yypParser->context = context
#define YYNSTATE 84
#define YYNRULE 38
#define YYERRORSYMBOL 19
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
 /*     0 */    55,   26,  123,    1,    7,   60,   70,   30,   24,   22,
 /*    10 */    23,   83,   16,   15,   76,    8,   60,   79,   54,   24,
 /*    20 */    22,   23,   17,   16,   15,   76,   37,   29,   79,   14,
 /*    30 */    24,   22,   35,   78,   16,   15,   76,   53,    2,   20,
 /*    40 */    71,   70,   18,   24,   22,   35,   21,   61,   63,   46,
 /*    50 */    42,   39,   60,   20,   13,   27,   18,   28,   20,   34,
 /*    60 */    36,   18,   63,   46,   42,   39,   63,   63,   46,   42,
 /*    70 */    39,   59,   59,   74,   27,   27,   28,   28,   38,   82,
 /*    80 */    77,   10,   11,   59,   60,   40,   27,   27,   28,   28,
 /*    90 */     4,   20,   58,    9,   18,   63,   57,   50,   13,   27,
 /*   100 */    63,   28,   69,   25,   47,   49,   12,   63,   45,   29,
 /*   110 */    63,   56,   27,   41,   67,   63,   44,   27,   72,   64,
 /*   120 */    43,   48,    5,   66,   65,   51,   73,   62,   52,    3,
 /*   130 */    75,   39,  124,   42,   32,   33,   31,   19,    6,   80,
 /*   140 */    81,  124,   68,   14,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    21,   22,   23,   24,   25,   26,   27,   18,   29,   30,
 /*    10 */    31,   21,   33,   34,   35,   25,   26,   27,    2,   29,
 /*    20 */    30,   31,    5,   33,   34,   35,   25,   17,   27,   12,
 /*    30 */    29,   30,   31,    2,   33,   34,   35,    2,   24,    4,
 /*    40 */     2,   27,    7,   29,   30,   31,    2,   21,   13,   14,
 /*    50 */    15,   16,   26,    4,    6,   29,    7,   31,    4,    8,
 /*    60 */    11,    7,   13,   14,   15,   16,   13,   13,   14,   15,
 /*    70 */    16,   26,   26,    2,   29,   29,   31,   31,   11,   21,
 /*    80 */     2,   36,   36,   26,   26,    8,   29,   29,   31,   31,
 /*    90 */     9,    4,   10,   36,    7,   13,   26,   10,    6,   29,
 /*   100 */    13,   31,    2,    2,    8,   11,    6,   13,   11,   17,
 /*   110 */    13,    2,   29,   11,   31,   13,    2,   29,    2,   31,
 /*   120 */     8,    2,    9,    2,   13,    2,   28,    2,   11,    9,
 /*   130 */    35,   16,   37,   15,   29,   29,   28,   34,    9,   35,
 /*   140 */     3,   37,   32,   12,
};
#define YY_SHIFT_USE_DFLT (-12)
#define YY_SHIFT_MAX 52
static const short yy_shift_ofst[] = {
 /*     0 */    35,   54,   49,   87,   82,   82,   82,   53,   53,   94,
 /*    10 */    97,  102,   53,   53,  111,  115,  118,   53,   53,  115,
 /*    20 */   111,  120,  100,   92,   17,  129,  137,  131,   10,  -11,
 /*    30 */    16,   31,   38,   51,   44,   48,   71,   67,   78,   77,
 /*    40 */   101,  109,  112,  114,   81,  116,   96,  119,  113,  121,
 /*    50 */   123,  117,  125,
};
#define YY_REDUCE_USE_DFLT (-22)
#define YY_REDUCE_MAX 21
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -21,  -10,    1,   14,   45,   57,   46,   58,   26,   70,
 /*    10 */    70,   70,   83,   88,   98,   95,  103,  105,  106,  104,
 /*    20 */   108,  110,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   122,   86,  122,  122,  122,  122,  122,   88,   87,  122,
 /*    10 */   122,  122,  122,  122,  122,  112,  109,  122,  122,  110,
 /*    20 */   122,  122,  122,  122,  105,  122,  122,  105,  122,  122,
 /*    30 */   122,  122,  122,  122,  122,  122,  122,  122,  122,  122,
 /*    40 */   122,  122,  122,  122,  122,  122,  122,  122,  122,  122,
 /*    50 */   122,  122,  122,   84,  121,   92,  117,  120,  118,  119,
 /*    60 */    93,   90,  102,  107,   99,  108,  115,  100,  101,   98,
 /*    70 */    94,   97,  116,  106,  103,  113,  114,  104,   96,   95,
 /*    80 */   111,   85,   91,   89,
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
  jx_net_ARG_SDECL                /* A place to hold %extra_argument */
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
void jx_net_Trace(FILE *TraceFILE, char *zTracePrompt){
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
  "$",             "ERROR",         "NEWLINE",       "EOI",         
  "PORT",          "EQUALS",        "PIPE",          "NODE",        
  "COLON",         "INDENT",        "PASS",          "DEDENT",      
  "DOT",           "IDENT",         "START",         "RUN",         
  "STOP",          "OPEN_PAR",      "CLOSE_PAR",     "error",       
  "description",   "starter",       "network",       "module",      
  "wiring",        "body",          "action",        "entry",       
  "port_name",     "node_name",     "pipe_chain",    "node_or_port",
  "node",          "start",         "run",           "stop",        
  "code",        
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "module ::= NEWLINE",
 /*   1 */ "module ::= network EOI",
 /*   2 */ "network ::= wiring",
 /*   3 */ "network ::= wiring body",
 /*   4 */ "network ::= body",
 /*   5 */ "network ::= wiring starter",
 /*   6 */ "network ::= wiring body starter",
 /*   7 */ "network ::= body starter",
 /*   8 */ "network ::= starter",
 /*   9 */ "starter ::= action",
 /*  10 */ "wiring ::= entry",
 /*  11 */ "wiring ::= wiring entry",
 /*  12 */ "entry ::= PORT port_name NEWLINE",
 /*  13 */ "entry ::= node_name EQUALS node_name NEWLINE",
 /*  14 */ "entry ::= pipe_chain NEWLINE",
 /*  15 */ "pipe_chain ::= node_or_port PIPE node_or_port",
 /*  16 */ "pipe_chain ::= pipe_chain PIPE node_or_port",
 /*  17 */ "entry ::= NODE node_name COLON NEWLINE node",
 /*  18 */ "node ::= INDENT PASS NEWLINE DEDENT NEWLINE",
 /*  19 */ "node ::= INDENT wiring DEDENT NEWLINE",
 /*  20 */ "node ::= INDENT wiring body DEDENT NEWLINE",
 /*  21 */ "node_or_port ::= node_name",
 /*  22 */ "node_or_port ::= node_name DOT port_name",
 /*  23 */ "node_name ::= IDENT",
 /*  24 */ "port_name ::= IDENT",
 /*  25 */ "body ::= start",
 /*  26 */ "body ::= start run",
 /*  27 */ "body ::= start run stop",
 /*  28 */ "body ::= run",
 /*  29 */ "body ::= run stop",
 /*  30 */ "body ::= stop",
 /*  31 */ "start ::= START COLON NEWLINE INDENT code DEDENT NEWLINE",
 /*  32 */ "run ::= RUN COLON NEWLINE INDENT code DEDENT NEWLINE",
 /*  33 */ "stop ::= STOP COLON NEWLINE INDENT code DEDENT NEWLINE",
 /*  34 */ "code ::= PASS",
 /*  35 */ "code ::= action",
 /*  36 */ "code ::= code action",
 /*  37 */ "action ::= node_or_port OPEN_PAR CLOSE_PAR NEWLINE",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *jx_net_TokenName(int tokenType){
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
** to jx_net_ and jx_net_Free.
*/
void *jx_net_Alloc(void *(*mallocProc)(size_t)){
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
#line 64 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 414 "jx_net_parse.c"
      break;
    case 20:
    case 21:
#line 59 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy0)); }
#line 420 "jx_net_parse.c"
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
**       obtained from jx_net_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void jx_net_Free(
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
     jx_net_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     jx_net_ARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 23, 2 },
  { 22, 1 },
  { 22, 2 },
  { 22, 1 },
  { 22, 2 },
  { 22, 3 },
  { 22, 2 },
  { 22, 1 },
  { 21, 1 },
  { 24, 1 },
  { 24, 2 },
  { 27, 3 },
  { 27, 4 },
  { 27, 2 },
  { 30, 3 },
  { 30, 3 },
  { 27, 5 },
  { 32, 5 },
  { 32, 4 },
  { 32, 5 },
  { 31, 1 },
  { 31, 3 },
  { 29, 1 },
  { 28, 1 },
  { 25, 1 },
  { 25, 2 },
  { 25, 3 },
  { 25, 1 },
  { 25, 2 },
  { 25, 1 },
  { 33, 7 },
  { 34, 7 },
  { 35, 7 },
  { 36, 1 },
  { 36, 1 },
  { 36, 2 },
  { 26, 4 },
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
  jx_net_ARG_FETCH;
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
#line 109 "jx_net_parse.lem"
{  
#ifdef JX_PY_PARSER_DEBUG
  printf("module ::= NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 678 "jx_net_parse.c"
        break;
      case 1:
#line 116 "jx_net_parse.lem"
{
  /* do nothing (other than call accept!) */
  jx_ob_replace(&context->result, yymsp[-1].minor.yy3);

#ifdef JX_NET_PARSER_DEBUG
  printf("module ::= network(yymsp[-1].minor.yy3).%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 691 "jx_net_parse.c"
        break;
      case 2:
#line 126 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 

#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= wiring(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 704 "jx_net_parse.c"
        break;
      case 3:
#line 137 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= wiring(yymsp[-1].minor.yy0) body(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 717 "jx_net_parse.c"
        break;
      case 4:
#line 148 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node")); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= body(yymsp[0].minor.yy0)..%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 730 "jx_net_parse.c"
        break;
      case 5:
#line 159 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy3,jx_ob_from_null());
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= wiring(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 744 "jx_net_parse.c"
        break;
      case 6:
#line 171 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,yymsp[-2].minor.yy0); 
  jx_list_append(yygotominor.yy3,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= wiring(yymsp[-2].minor.yy0) body(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 758 "jx_net_parse.c"
        break;
      case 7:
#line 183 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,jx_ob_from_null()); 
  jx_list_append(yygotominor.yy3,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= body(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 772 "jx_net_parse.c"
        break;
      case 8:
#line 195 "jx_net_parse.lem"
{  
  yygotominor.yy3 = jx_list_new();
  jx_list_append(yygotominor.yy3,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy3,jx_ob_from_null());
  jx_list_append(yygotominor.yy3,jx_ob_from_null()); 
  jx_list_append(yygotominor.yy3,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy3) ::= starter(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 786 "jx_net_parse.c"
        break;
      case 9:
#line 207 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("starter(yygotominor.yy0) ::= action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 796 "jx_net_parse.c"
        break;
      case 10:
#line 215 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("wiring(yygotominor.yy0) ::= entry(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 807 "jx_net_parse.c"
        break;
      case 11:
#line 224 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("wiring(yygotominor.yy0) ::= wiring(yymsp[-1].minor.yy0) entry(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 818 "jx_net_parse.c"
        break;
      case 12:
#line 233 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("port"));
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= PORT port_name(yymsp[-1].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 832 "jx_net_parse.c"
        break;
      case 13:
#line 243 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("copy"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= node_name(yymsp[-3].minor.yy0) EQUALS node_name(yymsp[-1].minor.yy0) NEWLINE. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(5,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 847 "jx_net_parse.c"
        break;
      case 14:
#line 254 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= pipe_chain(yymsp[-1].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 858 "jx_net_parse.c"
        break;
      case 15:
#line 262 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("pipe"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("pipe_chain(yygotominor.yy0) ::= node_or_port(yymsp[-2].minor.yy0) PIPE node_or_port(yymsp[0].minor.yy0) NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(6,&yymsp[-1].minor);
}
#line 872 "jx_net_parse.c"
        break;
      case 16:
#line 273 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("pipe_chain(yygotominor.yy0) ::= pipe_chain(yymsp[-2].minor.yy0) PIPE node_or_port(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(6,&yymsp[-1].minor);
}
#line 884 "jx_net_parse.c"
        break;
      case 17:
#line 284 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node_def"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= NODE node_name(yymsp[-3].minor.yy0) COLON NEWLINE node(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(7,&yymsp[-4].minor);
  yy_destructor(8,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[-1].minor);
}
#line 900 "jx_net_parse.c"
        break;
      case 18:
#line 295 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node"));
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(C) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(9,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(2,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 916 "jx_net_parse.c"
        break;
      case 19:
#line 304 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(9,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 931 "jx_net_parse.c"
        break;
      case 20:
#line 314 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0); 
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(yymsp[-3].minor.yy0) body(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(9,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 947 "jx_net_parse.c"
        break;
      case 21:
#line 325 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("node_or_port(yygotominor.yy0) ::= node_name(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 957 "jx_net_parse.c"
        break;
      case 22:
#line 333 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("node_or_port(yygotominor.yy0) ::= node_name(yymsp[-2].minor.yy0) DOT port_name(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(12,&yymsp[-1].minor);
}
#line 970 "jx_net_parse.c"
        break;
      case 23:
#line 343 "jx_net_parse.lem"
{
    yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("node_name(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 980 "jx_net_parse.c"
        break;
      case 24:
#line 351 "jx_net_parse.lem"
{
    yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("port_name(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 990 "jx_net_parse.c"
        break;
      case 25:
#line 359 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1001 "jx_net_parse.c"
        break;
      case 26:
#line 368 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[-1].minor.yy0) run(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1013 "jx_net_parse.c"
        break;
      case 27:
#line 378 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[-2].minor.yy0) run(yymsp[-1].minor.yy0) stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1026 "jx_net_parse.c"
        break;
      case 28:
#line 389 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= run(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1038 "jx_net_parse.c"
        break;
      case 29:
#line 399 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= run(yymsp[-1].minor.yy0) stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1051 "jx_net_parse.c"
        break;
      case 30:
#line 410 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1064 "jx_net_parse.c"
        break;
      case 31:
#line 421 "jx_net_parse.lem"
{  
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("start(yygotominor.yy0) ::= START COLON INDENT code(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(8,&yymsp[-5].minor);
  yy_destructor(2,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1080 "jx_net_parse.c"
        break;
      case 32:
#line 429 "jx_net_parse.lem"
{  
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("run(yygotominor.yy0) ::= RUN COLON INDENT code(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(15,&yymsp[-6].minor);
  yy_destructor(8,&yymsp[-5].minor);
  yy_destructor(2,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1096 "jx_net_parse.c"
        break;
      case 33:
#line 437 "jx_net_parse.lem"
{  
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("stop(yygotominor.yy0) ::= STOP COLON INDENT code(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(8,&yymsp[-5].minor);
  yy_destructor(2,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1112 "jx_net_parse.c"
        break;
      case 34:
#line 445 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= PASS. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[0].minor);
}
#line 1123 "jx_net_parse.c"
        break;
      case 35:
#line 453 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1134 "jx_net_parse.c"
        break;
      case 36:
#line 462 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= code(yymsp[-1].minor.yy0) action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1145 "jx_net_parse.c"
        break;
      case 37:
#line 471 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("action(yygotominor.yy0) ::= node_or_port(yymsp[-3].minor.yy0) OPEN_PAR CLOSE_PAR NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1159 "jx_net_parse.c"
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
  jx_net_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 81 "jx_net_parse.lem"

  context->status = JX_STATUS_SYNTAX_ERROR;
#ifdef JX_NET_PARSER_DEBUG
  printf("parse failed.\n");
#endif
#line 1195 "jx_net_parse.c"
  jx_net_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  jx_net_ARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 90 "jx_net_parse.lem"
  
#ifdef JX_NET_PARSER_DEBUG
  printf("jx_net_parse-error: syntax error.\n");
#endif
#line 1215 "jx_net_parse.c"
  jx_net_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  jx_net_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 72 "jx_net_parse.lem"

    context->status = JX_YES;
#ifdef JX_NET_PARSER_DEBUG
  printf("accept called\n");
#endif
#line 1241 "jx_net_parse.c"
  jx_net_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "jx_net_Alloc" which describes the current state of the parser.
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
void jx_net_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  jx_net_TOKENTYPE yyminor       /* The value for the token */
  jx_net_ARG_PDECL               /* Optional %extra_argument parameter */
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
  jx_net_ARG_STORE;

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
