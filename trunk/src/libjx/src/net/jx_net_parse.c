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
#define YYNOCODE 48
#define YYACTIONTYPE unsigned char
#define jx_net_TOKENTYPE jx_ob
typedef union {
  jx_net_TOKENTYPE yy0;
  jx_ob yy67;
  int yy95;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define jx_net_ARG_SDECL jx_net_parse_context *context;
#define jx_net_ARG_PDECL ,jx_net_parse_context *context
#define jx_net_ARG_FETCH jx_net_parse_context *context = yypParser->context
#define jx_net_ARG_STORE yypParser->context = context
#define YYNSTATE 105
#define YYNRULE 54
#define YYERRORSYMBOL 23
#define YYERRSYMDT yy95
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
 /*     0 */    76,   47,   31,  160,    1,    8,   82,   61,   26,   53,
 /*    10 */    29,   99,   27,   28,   69,   19,   17,   71,   48,    9,
 /*    20 */    82,   68,   26,   49,   29,   99,   27,   28,   77,   19,
 /*    30 */    17,   71,   57,   44,   99,   68,   26,    5,   29,   79,
 /*    40 */    27,   42,   99,   19,   17,   71,   75,    2,   25,   51,
 /*    50 */    61,   26,   22,   29,  104,   27,   42,   24,   99,   54,
 /*    60 */    50,   46,   78,   25,    6,   36,   32,   22,   25,   33,
 /*    70 */    99,   43,   22,   99,   54,   50,   46,    7,   99,   54,
 /*    80 */    50,   46,   89,   90,   37,   38,   73,   81,   95,   93,
 /*    90 */    91,   80,   82,   80,   55,   32,   32,   32,   33,   33,
 /*   100 */    33,   39,   25,   11,   80,   12,   22,   87,   32,   58,
 /*   110 */    98,   33,   99,   95,   93,   91,   10,   84,   32,   88,
 /*   120 */    14,   97,   32,   82,   92,   94,    4,   32,   60,   15,
 /*   130 */    33,   30,   21,   52,   63,   56,   62,   59,   35,   23,
 /*   140 */    46,   86,   66,   67,   50,   18,    3,   16,  102,   40,
 /*   150 */    72,  100,   65,  103,  101,   23,   74,   34,   30,   96,
 /*   160 */    13,   41,   20,   83,   64,   85,   14,   70,   45,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    25,    9,   27,   28,   29,   30,   31,   32,   33,   12,
 /*    10 */    35,   14,   37,   38,   25,   40,   41,   42,    2,   30,
 /*    20 */    31,   32,   33,   12,   35,   14,   37,   38,    2,   40,
 /*    30 */    41,   42,   12,   30,   14,   32,   33,   10,   35,   11,
 /*    40 */    37,   38,   14,   40,   41,   42,    2,   29,    4,    9,
 /*    50 */    32,   33,    8,   35,    2,   37,   38,    5,   14,   15,
 /*    60 */    16,   17,   31,    4,   10,   26,   35,    8,    4,   38,
 /*    70 */    14,   12,    8,   14,   15,   16,   17,   10,   14,   15,
 /*    80 */    16,   17,   14,   44,   45,   46,   25,   19,   20,   21,
 /*    90 */    22,   31,   31,   31,    9,   35,   35,   35,   38,   38,
 /*   100 */    38,   26,    4,   43,   31,   43,    8,   14,   35,   11,
 /*   110 */     2,   38,   14,   20,   21,   22,   43,   25,   35,   44,
 /*   120 */     7,   38,   35,   31,    2,   38,   18,   35,   12,    7,
 /*   130 */    38,   18,    6,    2,    2,    2,    2,    2,   36,   13,
 /*   140 */    17,    2,   42,    2,   16,   41,   10,   35,   39,   35,
 /*   150 */     3,   14,   34,   34,   34,   13,    2,   19,   18,    2,
 /*   160 */     5,    9,    2,   19,    2,   22,    7,   42,   12,
};
#define YY_SHIFT_USE_DFLT (-9)
#define YY_SHIFT_MAX 60
static const short yy_shift_ofst[] = {
 /*     0 */    44,   64,   59,   98,   68,   28,   28,   28,   56,   56,
 /*    10 */    20,   -3,   11,   93,   56,   56,  108,  123,  123,  128,
 /*    20 */   136,   56,   56,  137,  137,  137,   52,  122,  113,  126,
 /*    30 */   138,  147,  142,  140,  154,  157,  143,  144,  155,  143,
 /*    40 */   152,  160,  159,  162,  156,  141,   -8,   16,   27,   26,
 /*    50 */    40,  131,   54,  132,   85,  133,   67,  134,  135,  116,
 /*    60 */   139,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_MAX 25
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -25,  -11,    3,   18,   39,   62,   60,   73,   61,   92,
 /*    10 */    31,   31,   31,   75,   83,   87,  102,  100,  125,  104,
 /*    20 */   109,  112,  114,  118,  119,  120,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   159,  107,  159,  159,  159,  159,  159,  159,  109,  108,
 /*    10 */   159,  159,  159,  159,  159,  159,  159,  136,  134,  133,
 /*    20 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  129,
 /*    30 */   159,  159,  129,  159,  159,  159,  155,  159,  152,  156,
 /*    40 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  159,
 /*    50 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  159,
 /*    60 */   159,  115,  139,  140,  127,  130,  137,  128,  116,  110,
 /*    70 */   135,  138,  106,  112,  145,  105,  113,  141,  144,  142,
 /*    80 */   143,  150,  114,  151,  111,  149,  126,  154,  158,  153,
 /*    90 */   157,  148,  122,  147,  124,  146,  121,  123,  120,  131,
 /*   100 */   132,  118,  125,  119,  117,
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
  "PORT",          "COMMA",         "EQUALS",        "PIPE",        
  "NODE",          "COLON",         "INDENT",        "PASS",        
  "DEDENT",        "DOT",           "IDENT",         "START",       
  "RUN",           "STOP",          "OPEN_PAR",      "CLOSE_PAR",   
  "ICON",          "FCON",          "SCON",          "error",       
  "description",   "starter",       "string",        "network",     
  "module",        "wiring",        "body",          "action",      
  "entry",         "port_decl",     "port_name",     "node_name",   
  "parameters",    "pipe_chain",    "node_or_port",  "node",        
  "start",         "run",           "stop",          "code",        
  "number",        "varargslist",   "fpdefs",      
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
 /*  12 */ "entry ::= port_decl NEWLINE",
 /*  13 */ "port_decl ::= PORT port_name",
 /*  14 */ "port_decl ::= port_decl COMMA port_name",
 /*  15 */ "entry ::= node_name EQUALS node_name NEWLINE",
 /*  16 */ "entry ::= node_name EQUALS node_name parameters NEWLINE",
 /*  17 */ "entry ::= pipe_chain NEWLINE",
 /*  18 */ "pipe_chain ::= node_or_port PIPE node_or_port",
 /*  19 */ "pipe_chain ::= pipe_chain PIPE node_or_port",
 /*  20 */ "entry ::= NODE node_name COLON NEWLINE node",
 /*  21 */ "node ::= INDENT PASS NEWLINE DEDENT NEWLINE",
 /*  22 */ "node ::= INDENT wiring DEDENT NEWLINE",
 /*  23 */ "node ::= INDENT wiring body DEDENT NEWLINE",
 /*  24 */ "node_or_port ::= node_name",
 /*  25 */ "node_or_port ::= node_name DOT port_name",
 /*  26 */ "node_name ::= IDENT",
 /*  27 */ "port_name ::= IDENT",
 /*  28 */ "body ::= start",
 /*  29 */ "body ::= start run",
 /*  30 */ "body ::= start run stop",
 /*  31 */ "body ::= run",
 /*  32 */ "body ::= run stop",
 /*  33 */ "body ::= stop",
 /*  34 */ "start ::= START COLON NEWLINE INDENT code DEDENT NEWLINE",
 /*  35 */ "run ::= RUN COLON NEWLINE INDENT code DEDENT NEWLINE",
 /*  36 */ "stop ::= STOP COLON NEWLINE INDENT code DEDENT NEWLINE",
 /*  37 */ "code ::= PASS",
 /*  38 */ "code ::= action",
 /*  39 */ "code ::= code action",
 /*  40 */ "action ::= node_or_port OPEN_PAR CLOSE_PAR NEWLINE",
 /*  41 */ "number ::= ICON",
 /*  42 */ "number ::= FCON",
 /*  43 */ "string ::= SCON",
 /*  44 */ "string ::= string SCON",
 /*  45 */ "parameters ::= OPEN_PAR CLOSE_PAR",
 /*  46 */ "parameters ::= OPEN_PAR varargslist CLOSE_PAR",
 /*  47 */ "varargslist ::= fpdefs",
 /*  48 */ "fpdefs ::= IDENT",
 /*  49 */ "fpdefs ::= fpdefs COMMA IDENT",
 /*  50 */ "fpdefs ::= string",
 /*  51 */ "fpdefs ::= fpdefs COMMA string",
 /*  52 */ "fpdefs ::= number",
 /*  53 */ "fpdefs ::= fpdefs COMMA number",
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
    case 19:
    case 20:
    case 21:
    case 22:
#line 65 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy0));}
#line 443 "jx_net_parse.c"
      break;
    case 24:
    case 25:
#line 59 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy0)); }
#line 449 "jx_net_parse.c"
      break;
    case 26:
#line 61 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy67)); }
#line 454 "jx_net_parse.c"
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
  { 28, 1 },
  { 28, 2 },
  { 27, 1 },
  { 27, 2 },
  { 27, 1 },
  { 27, 2 },
  { 27, 3 },
  { 27, 2 },
  { 27, 1 },
  { 25, 1 },
  { 29, 1 },
  { 29, 2 },
  { 32, 2 },
  { 33, 2 },
  { 33, 3 },
  { 32, 4 },
  { 32, 5 },
  { 32, 2 },
  { 37, 3 },
  { 37, 3 },
  { 32, 5 },
  { 39, 5 },
  { 39, 4 },
  { 39, 5 },
  { 38, 1 },
  { 38, 3 },
  { 35, 1 },
  { 34, 1 },
  { 30, 1 },
  { 30, 2 },
  { 30, 3 },
  { 30, 1 },
  { 30, 2 },
  { 30, 1 },
  { 40, 7 },
  { 41, 7 },
  { 42, 7 },
  { 43, 1 },
  { 43, 1 },
  { 43, 2 },
  { 31, 4 },
  { 44, 1 },
  { 44, 1 },
  { 26, 1 },
  { 26, 2 },
  { 36, 2 },
  { 36, 3 },
  { 45, 1 },
  { 46, 1 },
  { 46, 3 },
  { 46, 1 },
  { 46, 3 },
  { 46, 1 },
  { 46, 3 },
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
#line 111 "jx_net_parse.lem"
{  
#ifdef JX_NET_PARSER_DEBUG
  printf("module ::= NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 728 "jx_net_parse.c"
        break;
      case 1:
#line 118 "jx_net_parse.lem"
{
  /* do nothing (other than call accept!) */
  jx_ob_replace(&context->result, yymsp[-1].minor.yy67);

#ifdef JX_NET_PARSER_DEBUG
  printf("module ::= network(yymsp[-1].minor.yy67).%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(3,&yymsp[0].minor);
}
#line 741 "jx_net_parse.c"
        break;
      case 2:
#line 128 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 

#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 754 "jx_net_parse.c"
        break;
      case 3:
#line 139 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[-1].minor.yy0) body(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 767 "jx_net_parse.c"
        break;
      case 4:
#line 150 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node")); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= body(yymsp[0].minor.yy0)..%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 780 "jx_net_parse.c"
        break;
      case 5:
#line 161 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,jx_ob_from_null());
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 794 "jx_net_parse.c"
        break;
      case 6:
#line 173 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,yymsp[-2].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[-2].minor.yy0) body(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 808 "jx_net_parse.c"
        break;
      case 7:
#line 185 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,jx_ob_from_null()); 
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= body(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 822 "jx_net_parse.c"
        break;
      case 8:
#line 197 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy67,jx_ob_from_null());
  jx_list_append(yygotominor.yy67,jx_ob_from_null()); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= starter(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 836 "jx_net_parse.c"
        break;
      case 9:
#line 209 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("starter(yygotominor.yy0) ::= action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 846 "jx_net_parse.c"
        break;
      case 10:
#line 217 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("wiring(yygotominor.yy0) ::= entry(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 857 "jx_net_parse.c"
        break;
      case 11:
#line 226 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("wiring(yygotominor.yy0) ::= wiring(yymsp[-1].minor.yy0) entry(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 868 "jx_net_parse.c"
        break;
      case 12:
#line 235 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= port_decl(yymsp[-1].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 879 "jx_net_parse.c"
        break;
      case 13:
#line 243 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("port"));
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("port_decl(yygotominor.yy0) ::= PORT port_name(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(4,&yymsp[-1].minor);
}
#line 892 "jx_net_parse.c"
        break;
      case 14:
#line 253 "jx_net_parse.lem"
{
  /* Split multiple named ports into each their own declaration. */
  if( jx_list_check(jx_list_borrow(yymsp[-2].minor.yy0, 1)) ) {
    jx_ob tmp = jx_list_new();
    yygotominor.yy0 = yymsp[-2].minor.yy0;
    jx_list_append(yygotominor.yy0,tmp);
    jx_list_append(tmp,jx_ob_from_ident("port"));
    jx_list_append(tmp,yymsp[0].minor.yy0);
  } else {
    jx_ob tmp = jx_list_new();
    yygotominor.yy0 = jx_list_new();
    jx_list_append(yygotominor.yy0,jx_ob_from_ident("null_op"));
    jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
    jx_list_append(yygotominor.yy0,tmp);
    jx_list_append(tmp,jx_ob_from_ident("port"));
    jx_list_append(tmp,yymsp[0].minor.yy0);
  }
#ifdef JX_NET_PARSER_DEBUG
  printf("port_decl(yygotominor.yy0) ::= port_decl(yymsp[-2].minor.yy0) COMMA port_name(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(5,&yymsp[-1].minor);
}
#line 918 "jx_net_parse.c"
        break;
      case 15:
#line 276 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("copy"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= node_name(yymsp[-3].minor.yy0) EQUALS node_name(yymsp[-1].minor.yy0) NEWLINE. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 933 "jx_net_parse.c"
        break;
      case 16:
#line 287 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("copy"));
  jx_list_append(yygotominor.yy0,yymsp[-4].minor.yy0);
  if( jx_null_check(yymsp[-1].minor.yy0) )
  {
    jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  } else {
    jx_list_insert(yymsp[-1].minor.yy0,0,yymsp[-2].minor.yy0);
    jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  }
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= node_name(yymsp[-4].minor.yy0) EQUALS node_name(yymsp[-2].minor.yy0) parameters(yymsp[-1].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 954 "jx_net_parse.c"
        break;
      case 17:
#line 304 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= pipe_chain(yymsp[-1].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 965 "jx_net_parse.c"
        break;
      case 18:
#line 312 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("pipe"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("pipe_chain(yygotominor.yy0) ::= node_or_port(yymsp[-2].minor.yy0) PIPE node_or_port(yymsp[0].minor.yy0) NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(7,&yymsp[-1].minor);
}
#line 979 "jx_net_parse.c"
        break;
      case 19:
#line 323 "jx_net_parse.lem"
{
  /* Split multiple named ports into each their own declaration. */
  if( jx_list_check(jx_list_borrow(yymsp[-2].minor.yy0, 1)) ) {
    jx_ob tmp = jx_list_new();
    jx_ob popped_pipe_def = jx_list_pop(yymsp[-2].minor.yy0);
    jx_ob start_of_new_pipe = jx_list_get(popped_pipe_def, 2);
    jx_list_push(yymsp[-2].minor.yy0, popped_pipe_def);
    yygotominor.yy0 = yymsp[-2].minor.yy0;
    jx_list_append(yygotominor.yy0,tmp);
    jx_list_append(tmp,jx_ob_from_ident("pipe"));
    jx_list_append(tmp,start_of_new_pipe);
    jx_list_append(tmp,yymsp[0].minor.yy0);
  } else {
    jx_ob tmp = jx_list_new();
    jx_ob start_of_new_pipe = jx_list_get(yymsp[-2].minor.yy0, 2);
    yygotominor.yy0 = jx_list_new();
    jx_list_append(yygotominor.yy0,jx_ob_from_ident("null_op"));
    jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
    jx_list_append(yygotominor.yy0,tmp);
    jx_list_append(tmp,jx_ob_from_ident("pipe"));
    jx_list_append(tmp,start_of_new_pipe);
    jx_list_append(tmp,yymsp[0].minor.yy0);
  }
#ifdef JX_NET_PARSER_DEBUG
  printf("pipe_chain(yygotominor.yy0) ::= pipe_chain(yymsp[-2].minor.yy0) PIPE node_or_port(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(7,&yymsp[-1].minor);
}
#line 1011 "jx_net_parse.c"
        break;
      case 20:
#line 354 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("entity"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0); /* name */
  jx_list_append(yygotominor.yy0,jx_ob_from_null()); /* base class */
  /* TODO: implement base node-class */
  jx_list_append(yygotominor.yy0,jx_ob_from_null()); /* content */
  jx_list_append(yygotominor.yy0,jx_ob_from_null()); /* attr_hash */
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("__init__")); /* name of constructor */
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0); /* code */
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= NODE node_name(yymsp[-3].minor.yy0) COLON NEWLINE node(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(8,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[-1].minor);
}
#line 1032 "jx_net_parse.c"
        break;
      case 21:
#line 370 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("null_op"));
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT PASS NEWLINE DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(2,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1048 "jx_net_parse.c"
        break;
      case 22:
#line 379 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1063 "jx_net_parse.c"
        break;
      case 23:
#line 389 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("null_op"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0); 
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(yymsp[-3].minor.yy0) body(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1079 "jx_net_parse.c"
        break;
      case 24:
#line 400 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("node_or_port(yygotominor.yy0) ::= node_name(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1089 "jx_net_parse.c"
        break;
      case 25:
#line 408 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("node_or_port(yygotominor.yy0) ::= node_name(yymsp[-2].minor.yy0) DOT port_name(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(13,&yymsp[-1].minor);
}
#line 1102 "jx_net_parse.c"
        break;
      case 26:
#line 418 "jx_net_parse.lem"
{
    yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("node_name(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1112 "jx_net_parse.c"
        break;
      case 27:
#line 426 "jx_net_parse.lem"
{
    yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("port_name(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1122 "jx_net_parse.c"
        break;
      case 28:
#line 434 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1133 "jx_net_parse.c"
        break;
      case 29:
#line 443 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[-1].minor.yy0) run(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1145 "jx_net_parse.c"
        break;
      case 30:
#line 453 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[-2].minor.yy0) run(yymsp[-1].minor.yy0) stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1158 "jx_net_parse.c"
        break;
      case 31:
#line 464 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= run(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1170 "jx_net_parse.c"
        break;
      case 32:
#line 474 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= run(yymsp[-1].minor.yy0) stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1183 "jx_net_parse.c"
        break;
      case 33:
#line 485 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1196 "jx_net_parse.c"
        break;
      case 34:
#line 496 "jx_net_parse.lem"
{  
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("start(yygotominor.yy0) ::= START COLON INDENT code(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(15,&yymsp[-6].minor);
  yy_destructor(9,&yymsp[-5].minor);
  yy_destructor(2,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1212 "jx_net_parse.c"
        break;
      case 35:
#line 504 "jx_net_parse.lem"
{  
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("run(yygotominor.yy0) ::= RUN COLON INDENT code(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(9,&yymsp[-5].minor);
  yy_destructor(2,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1228 "jx_net_parse.c"
        break;
      case 36:
#line 512 "jx_net_parse.lem"
{  
  yygotominor.yy0 = yymsp[-2].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("stop(yygotominor.yy0) ::= STOP COLON INDENT code(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(17,&yymsp[-6].minor);
  yy_destructor(9,&yymsp[-5].minor);
  yy_destructor(2,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1244 "jx_net_parse.c"
        break;
      case 37:
#line 520 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= PASS. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(11,&yymsp[0].minor);
}
#line 1255 "jx_net_parse.c"
        break;
      case 38:
#line 528 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1266 "jx_net_parse.c"
        break;
      case 39:
#line 537 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= code(yymsp[-1].minor.yy0) action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1277 "jx_net_parse.c"
        break;
      case 40:
#line 546 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("action(yygotominor.yy0) ::= node_or_port(yymsp[-3].minor.yy0) OPEN_PAR CLOSE_PAR NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1291 "jx_net_parse.c"
        break;
      case 41:
#line 555 "jx_net_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("number = ICON\n"); 
#endif
}
#line 1301 "jx_net_parse.c"
        break;
      case 42:
#line 563 "jx_net_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("number = FCON\n"); 
#endif
}
#line 1311 "jx_net_parse.c"
        break;
      case 43:
#line 571 "jx_net_parse.lem"
{
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("string = SCON\n"); 
#endif
}
#line 1321 "jx_net_parse.c"
        break;
      case 44:
#line 581 "jx_net_parse.lem"
{
  yygotominor.yy67 = jx_ob_add(yymsp[-1].minor.yy67,yymsp[0].minor.yy0);
  jx_ob_free(yymsp[-1].minor.yy67);
  jx_ob_free(yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = SCON\n"); 
#endif
}
#line 1333 "jx_net_parse.c"
        break;
      case 45:
#line 593 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_ob_from_null();
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
#line 1342 "jx_net_parse.c"
        break;
      case 46:
#line 598 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[-1].minor.yy0;
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
#line 1351 "jx_net_parse.c"
        break;
      case 47:
#line 607 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[0].minor.yy0;
}
#line 1358 "jx_net_parse.c"
        break;
      case 48:
      case 52:
#line 614 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1367 "jx_net_parse.c"
        break;
      case 49:
      case 53:
#line 620 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1377 "jx_net_parse.c"
        break;
      case 50:
#line 626 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy67);
}
#line 1385 "jx_net_parse.c"
        break;
      case 51:
#line 632 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy67);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1394 "jx_net_parse.c"
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
#line 82 "jx_net_parse.lem"

  context->status = JX_STATUS_SYNTAX_ERROR;
#ifdef JX_NET_PARSER_DEBUG
  printf("parse failed.\n");
#endif
#line 1430 "jx_net_parse.c"
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
#line 91 "jx_net_parse.lem"
  
#ifdef JX_NET_PARSER_DEBUG
  printf("jx_net_parse-error: syntax error.\n");
#endif
#line 1450 "jx_net_parse.c"
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
#line 73 "jx_net_parse.lem"

    context->status = JX_YES;
#ifdef JX_NET_PARSER_DEBUG
  printf("accept called\n");
#endif
#line 1476 "jx_net_parse.c"
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
