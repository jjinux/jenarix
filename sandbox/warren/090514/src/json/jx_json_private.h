#ifndef _H_jx_json_private
#define _H_jx_json_private

#include "jx_private.h"

#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#define noJX_JSON_PARSER_DEBUG

/* include token identifiers */

#include "jx_json_parse.h"

/* parser context */

typedef struct {
  jx_int status; /* 1 = accepted, -1 = error, 0 = still going */
  jx_ob result;
} jx_json_parse_context;

/* prototypes for calling the Lemon-generated parser */

void *jx_json_Alloc(void *(*mallocProc)(size_t));
void jx_json_(void *yyp, int yymajor, jx_ob ob, jx_json_parse_context *context);
void jx_json_Free(void *p, void (*freeProc)(void*));

#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif
