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
 /*     0 */   104,   67,   30,  160,    1,    9,   64,   90,   26,   49,
 /*    10 */    27,   67,   28,   29,   69,   23,   25,  100,   86,    8,
 /*    20 */    64,   70,   26,   14,   27,   96,   28,   29,   67,   23,
 /*    30 */    25,  100,   53,   44,   67,   70,   26,   57,   27,   67,
 /*    40 */    28,   42,   20,   23,   25,  100,   61,    2,   19,   17,
 /*    50 */    90,   26,   21,   27,   74,   28,   42,   35,   67,   54,
 /*    60 */    50,   46,   95,   19,   66,   36,   31,   21,   19,   32,
 /*    70 */     4,   43,   21,   67,   54,   50,   46,   65,   67,   54,
 /*    80 */    50,   46,   84,   85,   37,   38,   16,   80,   76,   77,
 /*    90 */    78,   97,   19,   97,   89,   31,   21,   31,   32,   58,
 /*   100 */    32,   72,   67,   10,   97,   11,   73,   63,   31,  103,
 /*   110 */    15,   32,   39,   64,   40,   64,   12,   31,   82,   31,
 /*   120 */    32,   33,   32,    3,   76,   77,   78,   50,   31,   31,
 /*   130 */    83,   87,   88,   71,   46,   62,   18,   24,   17,   93,
 /*   140 */    99,   33,   68,   34,   75,   41,   81,   22,   79,   13,
 /*   150 */    15,   91,   92,   45,   48,   47,   94,    5,   52,   98,
 /*   160 */    51,   56,  101,    6,   55,   59,  102,  161,    7,  161,
 /*   170 */   161,  161,  161,   60,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    25,   14,   27,   28,   29,   30,   31,   32,   33,   12,
 /*    10 */    35,   14,   37,   38,   25,   40,   41,   42,    2,   30,
 /*    20 */    31,   32,   33,    7,   35,   11,   37,   38,   14,   40,
 /*    30 */    41,   42,   12,   30,   14,   32,   33,   12,   35,   14,
 /*    40 */    37,   38,    6,   40,   41,   42,    2,   29,    4,   13,
 /*    50 */    32,   33,    8,   35,    2,   37,   38,   36,   14,   15,
 /*    60 */    16,   17,   31,    4,   14,   26,   35,    8,    4,   38,
 /*    70 */    18,   12,    8,   14,   15,   16,   17,   34,   14,   15,
 /*    80 */    16,   17,   14,   44,   45,   46,   35,   19,   20,   21,
 /*    90 */    22,   31,    4,   31,   39,   35,    8,   35,   38,   11,
 /*   100 */    38,   34,   14,   43,   31,   43,   34,   25,   35,   25,
 /*   110 */     7,   38,   26,   31,   35,   31,   43,   35,   14,   35,
 /*   120 */    38,   18,   38,   10,   20,   21,   22,   16,   35,   35,
 /*   130 */    44,   38,   38,    2,   17,    3,    5,   41,   13,   42,
 /*   140 */    42,   18,    2,   19,    2,    9,   19,    2,   22,    5,
 /*   150 */     7,    2,    2,   12,    2,    9,    2,   10,    2,    2,
 /*   160 */     9,    2,    2,   10,    9,    2,    2,   47,   10,   47,
 /*   170 */    47,   47,   47,   12,
};
#define YY_SHIFT_USE_DFLT (-14)
#define YY_SHIFT_MAX 60
static const short yy_shift_ofst[] = {
 /*     0 */    44,   64,   59,   88,   68,   14,   14,   14,  -13,  -13,
 /*    10 */    -3,   20,   25,  104,  -13,  -13,   52,   50,   50,   50,
 /*    20 */   -13,  -13,  113,  111,  117,  117,  131,   36,   16,  103,
 /*    30 */   132,  125,  123,  124,  140,  142,  126,  127,  144,  126,
 /*    40 */   136,  145,  143,  149,  141,  150,  146,  152,  147,  154,
 /*    50 */   151,  156,  153,  157,  155,  159,  158,  160,  163,  161,
 /*    60 */   164,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_MAX 25
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -25,  -11,    3,   18,   39,   60,   62,   73,   82,   84,
 /*    10 */    31,   31,   31,   86,   93,   94,   21,   43,   67,   72,
 /*    20 */    51,   79,   55,   96,   97,   98,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   159,  107,  159,  159,  159,  159,  159,  159,  108,  109,
 /*    10 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  159,
 /*    20 */   159,  159,  159,  133,  134,  136,  159,  129,  159,  159,
 /*    30 */   159,  129,  159,  159,  159,  159,  155,  159,  152,  156,
 /*    40 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  159,
 /*    50 */   159,  159,  159,  159,  159,  159,  159,  159,  159,  159,
 /*    60 */   159,  105,  106,  111,  114,  130,  132,  131,  145,  110,
 /*    70 */   116,  117,  119,  118,  120,  121,  146,  147,  148,  149,
 /*    80 */   150,  151,  154,  158,  153,  157,  122,  124,  123,  125,
 /*    90 */   115,  127,  128,  135,  141,  144,  142,  143,  140,  137,
 /*   100 */   138,  139,  126,  112,  113,
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
#line 445 "jx_net_parse.c"
      break;
    case 24:
    case 25:
#line 59 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy0)); }
#line 451 "jx_net_parse.c"
      break;
    case 26:
#line 61 "jx_net_parse.lem"
{ jx_ob_free((yypminor->yy67)); }
#line 456 "jx_net_parse.c"
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
#ifdef JX_PY_PARSER_DEBUG
  printf("module ::= NEWLINE.%s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 730 "jx_net_parse.c"
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
#line 743 "jx_net_parse.c"
        break;
      case 2:
#line 128 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 

#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 756 "jx_net_parse.c"
        break;
      case 3:
#line 139 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[-1].minor.yy0) body(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 769 "jx_net_parse.c"
        break;
      case 4:
#line 150 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node")); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= body(yymsp[0].minor.yy0)..%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 782 "jx_net_parse.c"
        break;
      case 5:
#line 161 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,jx_ob_from_null());
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 796 "jx_net_parse.c"
        break;
      case 6:
#line 173 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,yymsp[-2].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= wiring(yymsp[-2].minor.yy0) body(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 810 "jx_net_parse.c"
        break;
      case 7:
#line 185 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,jx_ob_from_null()); 
  jx_list_append(yygotominor.yy67,yymsp[-1].minor.yy0); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= body(yymsp[-1].minor.yy0) starter(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 824 "jx_net_parse.c"
        break;
      case 8:
#line 197 "jx_net_parse.lem"
{  
  yygotominor.yy67 = jx_list_new();
  jx_list_append(yygotominor.yy67,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy67,jx_ob_from_null());
  jx_list_append(yygotominor.yy67,jx_ob_from_null()); 
  jx_list_append(yygotominor.yy67,yymsp[0].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("network(yygotominor.yy67) ::= starter(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 838 "jx_net_parse.c"
        break;
      case 9:
#line 209 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("starter(yygotominor.yy0) ::= action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 848 "jx_net_parse.c"
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
#line 859 "jx_net_parse.c"
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
#line 870 "jx_net_parse.c"
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
#line 881 "jx_net_parse.c"
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
#line 894 "jx_net_parse.c"
        break;
      case 14:
#line 253 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("port_decl(yygotominor.yy0) ::= port_decl(yymsp[-2].minor.yy0) COMMA port_name(yymsp[0].minor.yy0).\n");
#endif
  yy_destructor(5,&yymsp[-1].minor);
}
#line 906 "jx_net_parse.c"
        break;
      case 15:
#line 262 "jx_net_parse.lem"
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
#line 921 "jx_net_parse.c"
        break;
      case 16:
#line 273 "jx_net_parse.lem"
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
#line 942 "jx_net_parse.c"
        break;
      case 17:
#line 290 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= pipe_chain(yymsp[-1].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(2,&yymsp[0].minor);
}
#line 953 "jx_net_parse.c"
        break;
      case 18:
#line 298 "jx_net_parse.lem"
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
#line 967 "jx_net_parse.c"
        break;
      case 19:
#line 309 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("pipe_chain(yygotominor.yy0) ::= pipe_chain(yymsp[-2].minor.yy0) PIPE node_or_port(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(7,&yymsp[-1].minor);
}
#line 979 "jx_net_parse.c"
        break;
      case 20:
#line 320 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node_def"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("entry(yygotominor.yy0) ::= NODE node_name(yymsp[-3].minor.yy0) COLON NEWLINE node(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(8,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(2,&yymsp[-1].minor);
}
#line 995 "jx_net_parse.c"
        break;
      case 21:
#line 331 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node"));
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(C) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(2,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1011 "jx_net_parse.c"
        break;
      case 22:
#line 340 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1026 "jx_net_parse.c"
        break;
      case 23:
#line 350 "jx_net_parse.lem"
{  
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_ident("node"));
  jx_list_append(yygotominor.yy0,yymsp[-3].minor.yy0); 
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0); 
#ifdef JX_NET_PARSER_DEBUG
  printf("node(yygotominor.yy0) ::= INDENT wiring(yymsp[-3].minor.yy0) body(yymsp[-2].minor.yy0) DEDENT. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(12,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
#line 1042 "jx_net_parse.c"
        break;
      case 24:
#line 361 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("node_or_port(yygotominor.yy0) ::= node_name(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1052 "jx_net_parse.c"
        break;
      case 25:
#line 369 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("node_or_port(yygotominor.yy0) ::= node_name(yymsp[-2].minor.yy0) DOT port_name(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(13,&yymsp[-1].minor);
}
#line 1065 "jx_net_parse.c"
        break;
      case 26:
#line 379 "jx_net_parse.lem"
{
    yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("node_name(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0).%s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1075 "jx_net_parse.c"
        break;
      case 27:
#line 387 "jx_net_parse.lem"
{
    yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_NET_PARSER_DEBUG
  printf("port_name(yygotominor.yy0) ::= IDENT(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1085 "jx_net_parse.c"
        break;
      case 28:
#line 395 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1096 "jx_net_parse.c"
        break;
      case 29:
#line 404 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[-1].minor.yy0) run(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1108 "jx_net_parse.c"
        break;
      case 30:
#line 414 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[-2].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= start(yymsp[-2].minor.yy0) run(yymsp[-1].minor.yy0) stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1121 "jx_net_parse.c"
        break;
      case 31:
#line 425 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= run(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1133 "jx_net_parse.c"
        break;
      case 32:
#line 435 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[-1].minor.yy0);
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= run(yymsp[-1].minor.yy0) stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1146 "jx_net_parse.c"
        break;
      case 33:
#line 446 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,jx_ob_from_null());
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("body(yygotominor.yy0) ::= stop(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1159 "jx_net_parse.c"
        break;
      case 34:
#line 457 "jx_net_parse.lem"
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
#line 1175 "jx_net_parse.c"
        break;
      case 35:
#line 465 "jx_net_parse.lem"
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
#line 1191 "jx_net_parse.c"
        break;
      case 36:
#line 473 "jx_net_parse.lem"
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
#line 1207 "jx_net_parse.c"
        break;
      case 37:
#line 481 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= PASS. %s:%d\n",__FILE__,__LINE__); 
#endif
  yy_destructor(11,&yymsp[0].minor);
}
#line 1218 "jx_net_parse.c"
        break;
      case 38:
#line 489 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1229 "jx_net_parse.c"
        break;
      case 39:
#line 498 "jx_net_parse.lem"
{
  yygotominor.yy0 = yymsp[-1].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
#ifdef JX_NET_PARSER_DEBUG
  printf("code(yygotominor.yy0) ::= code(yymsp[-1].minor.yy0) action(yymsp[0].minor.yy0). %s:%d\n",__FILE__,__LINE__); 
#endif
}
#line 1240 "jx_net_parse.c"
        break;
      case 40:
#line 507 "jx_net_parse.lem"
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
#line 1254 "jx_net_parse.c"
        break;
      case 41:
#line 516 "jx_net_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("number = ICON\n"); 
#endif
}
#line 1264 "jx_net_parse.c"
        break;
      case 42:
#line 524 "jx_net_parse.lem"
{ 
  yygotominor.yy0 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("number = FCON\n"); 
#endif
}
#line 1274 "jx_net_parse.c"
        break;
      case 43:
#line 532 "jx_net_parse.lem"
{
  yygotominor.yy67 = yymsp[0].minor.yy0;
#ifdef JX_JXON_PARSER_DEBUG
  printf("string = SCON\n"); 
#endif
}
#line 1284 "jx_net_parse.c"
        break;
      case 44:
#line 542 "jx_net_parse.lem"
{
  yygotominor.yy67 = jx_ob_add(yymsp[-1].minor.yy67,yymsp[0].minor.yy0);
  jx_ob_free(yymsp[-1].minor.yy67);
  jx_ob_free(yymsp[0].minor.yy0);
#ifdef JX_JXON_PARSER_DEBUG
  printf("atom = SCON\n"); 
#endif
}
#line 1296 "jx_net_parse.c"
        break;
      case 45:
#line 554 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_ob_from_null();
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
#line 1305 "jx_net_parse.c"
        break;
      case 46:
#line 559 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[-1].minor.yy0;
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
#line 1314 "jx_net_parse.c"
        break;
      case 47:
#line 568 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[0].minor.yy0;
}
#line 1321 "jx_net_parse.c"
        break;
      case 48:
      case 52:
#line 575 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
}
#line 1330 "jx_net_parse.c"
        break;
      case 49:
      case 53:
#line 581 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy0);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1340 "jx_net_parse.c"
        break;
      case 50:
#line 587 "jx_net_parse.lem"
{
  yygotominor.yy0 = jx_list_new();
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy67);
}
#line 1348 "jx_net_parse.c"
        break;
      case 51:
#line 593 "jx_net_parse.lem"
{
  yygotominor.yy0=yymsp[-2].minor.yy0;
  jx_list_append(yygotominor.yy0,yymsp[0].minor.yy67);
  yy_destructor(5,&yymsp[-1].minor);
}
#line 1357 "jx_net_parse.c"
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
#line 1393 "jx_net_parse.c"
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
#line 1413 "jx_net_parse.c"
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
#line 1439 "jx_net_parse.c"
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
