#ifndef _H_jx_core_public
#define _H_jx_core_public

/* The minimum public interface for linking your own C code against a
 * C-based Jenarix core implementation.
 * 
 * (Of course, to be practically useful, a Jenarix implementation must
 * also provide the means of converting native or external objects
 * into the implementation-specific object representations, but those
 * capabilities aren't actually part of the Jenarix core.)
*/

/* public 'jx_ob' object type */

typedef struct _jx_ob jx_ob;

/* public struct definition
 * (required since Jenarix objects are passed on the call stack) */

#ifndef JX_OB_STRUCT_DEFINED
#define JX_OB_STRUCT_DEFINED

struct _jx_ob {
  int    meta; /* meta information always consists of 32-bits */
  size_t data; /* data must be large enough to hold a native pointer */
};

#endif


/* returns pointer to the effective thread-environment object */

jx_ob *jx(void);


/* synchronous dispatch: how objects are combined to perform actions
 * (always returns an owned reference which must be released) */

jx_ob jx_dispatch(jx_ob *, jx_ob, jx_ob, jx_ob);


/* global constant object */

extern const jx_ob jx_null; /* perhaps jx_Null or Jx_Null? */


/* status codes */

#ifndef JX_STATUS_DEFINED
#define JX_STATUS_DEFINED

typedef int jx_status;

#endif

/* reference management */

jx_ob jx_retain(jx_ob *, jx_ob);  

/* returns jx_null if reference not retained, and returns the owned
 * jx_ob otherwise */

jx_status jx_release(jx_ob *, jx_ob); 

/* returns a negative status value if reference not released, and
 * returns a non-negative value otherwise */


/* object resolution (prebinding, module import, lookup, etc.) */

jx_ob jx_resolve(jx_ob *, jx_ob, jx_ob);


/* 
 best_rep = jx_dispatch(env, context, XX_RESOLVE, general_concept);

 jx_dispatch(env, context, general_concept, argument);

 jx_dispatch(env, context, jx_resolve(env, context, general_concept), argument);

 jx_dispatch(env, context, jx_dispatch(env, context, XX_RESOLVE, general_concept), argument);
*/


#endif
