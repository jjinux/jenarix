#ifndef _H_jx_public
#define _H_jx_public

/* 
Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

* Neither the name of the DeLano Scientific LLC nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* owned C types (to be use exclusively throughout) */

typedef int jx_bool;
typedef char jx_char;
typedef int jx_status;

typedef unsigned short jx_uint16;       /* for small bit masks, etc. */

typedef int jx_int32;
typedef unsigned int jx_uint32; /* for hash codes, etc. */
typedef long long jx_int64;

#define notJX_64_BIT

#ifdef JX_64_BIT
/* uses 64-bit integers and double-precision floating point */
typedef jx_int64 jx_int;
typedef double jx_float;
#define JX_TINY_STR_MIN_SIZE 10
#else
/* uses 32-bit integers and single-precision floating point */
typedef int jx_int;
typedef float jx_float;
#define JX_TINY_STR_MIN_SIZE 6
#endif

/* our ubiquitous jenarix object type */

typedef struct jx__ob jx_ob;

/* status codes and constants */

#define JX_FAILURE  -1
#define JX_SUCCESS   0
#define JX_YES       1
#define JX_NO        0

#define JX_FALSE     0
#define JX_TRUE      1

#define JX_NULL      NULL

/* inline functions and structs and required by the compiler to be
 public, but should be treated by API-users as private */

#include "jx_inline.h"

/* meaningful prepositions, for systematic memory management:

   ..._ob_with_...(...) implies destructive conversion of input object (destroyed)

   ..._ob_from_...(...) implies constructive copy / conversion of input object (preserved)

   ..._ob_to_...(...) implies an constructive cast resulting in a new copy of the input object (preserved)

   ..._ob_as_...(...) implies a failsafe (volatile) cast of in-place borrowed object data (preserved)
   
*/

/* boxing (all of these return owned objects) */

jx_ob jx_ob_from_null(void);
jx_ob jx_ob_from_bool(jx_bool bool_);
jx_ob jx_ob_from_int(jx_int int_);
jx_ob jx_ob_from_float(jx_float float_);
jx_ob jx_ob_from_str(jx_char * str);    /* copies string into new storage */
jx_ob jx_ob_from_str_with_len(jx_char * str, jx_int len); /* copies string into new storage */
jx_ob jx_ob_from_ident(jx_char * ident);  /* copies identifier into new storage */

/* unboxing / volatile casts for inline use */

jx_bool jx_ob_as_bool(jx_ob ob);
jx_int jx_ob_as_int(jx_ob ob);
jx_float jx_ob_as_float(jx_ob ob);

jx_char *jx_ob_as_str(jx_ob * ob);      /* returns borrowed (volatile)
                                           jx_char pointer -- note possing
                                           object in as a pointer because we
                                           need to return a pointer to
                                           characters within the object
                                           itself when using tiny_str's */

jx_char *jx_ob_as_ident(jx_ob * ob);      /* returns borrowed (volatile)
                                           jx_char pointer -- note possing
                                           object in as a pointer because we
                                           need to return a pointer to
                                           characters within the object
                                           itself when using tiny_str's */

/* constructive casts which preserve the source object and return a new object */

jx_ob jx_ob_to_bool(jx_ob ob);
jx_ob jx_ob_to_int(jx_ob ob);
jx_ob jx_ob_to_float(jx_ob ob);
jx_ob jx_ob_to_str(jx_ob ob);
jx_ob jx_ob_to_ident(jx_ob ob);

/* determining object type */

jx_bool jx_null_check(jx_ob ob);
jx_bool jx_bool_check(jx_ob ob);
jx_bool jx_int_check(jx_ob ob);
jx_bool jx_float_check(jx_ob ob);
jx_bool jx_str_check(jx_ob ob);
jx_bool jx_list_check(jx_ob ob);
jx_bool jx_hash_check(jx_ob ob);
jx_bool jx_ident_check(jx_ob ob);
jx_bool jx_builtin_check(jx_ob ob);

/* comparing objects */

jx_bool jx_ob_identical(jx_ob left, jx_ob right);

jx_bool jx_ob_equal(jx_ob left, jx_ob right);   /* to be done */

/* common operations */

jx_ob jx_add(jx_ob left, jx_ob right);
jx_ob jx_sub(jx_ob left, jx_ob right);


/* read only status (no changes allowed, cannot be freed, but can be
   stored/conveyed through mutable containers, provided that those
   containers are gone by the time read only status is cleared) */

jx_ob jx_ob_set_read_only(jx_ob ob, jx_bool read_only);
jx_bool jx_ob_read_only(jx_ob ob);

/* copying */

jx_ob jx_ob_copy(jx_ob ob);     /* always deep / recursive; copies of
                                   read-only containers are mutable */

/* convenient method for freeing and re-initializing */

jx_ob jx_null_with_ob(jx_ob ob);

/* creating a weak reference to an object */

jx_ob jx_ob_take_weak_ref(jx_ob ob);  

/* strings */

jx_int jx_str_size(jx_ob ob);
jx_ob jx_str_concat(jx_ob left, jx_ob right);
jx_ob jx_str_concat_with_both(jx_ob left, jx_ob right);

jx_ob jx_str_join_with_list(jx_ob list);

/* lists */

jx_ob jx_list_new(void);
jx_ob jx_list_new_from_int_array(jx_int * array, jx_int size);  /* copies array */
jx_ob jx_list_new_from_float_array(jx_float * array, jx_float size);    /* copies array */
jx_ob jx_list_new_with_size(jx_int size); /* creates a null-filled list */
jx_ob jx_list_new_with_fill(jx_int size, jx_ob fill); /* takes ownership of fill */

jx_ob jx_list_new_from_hash(jx_ob hash); /* returns owned list of copied keys & values (interleaved) */
jx_ob jx_list_new_with_hash(jx_ob hash); /* frees hash and converts content to interleaved list */

jx_int jx_list_size(jx_ob list);
jx_status jx_list_resize(jx_ob list, jx_int size, jx_ob fill);  /* takes ownership of fill object */

jx_status jx_list_append(jx_ob list, jx_ob ob); /* takes ownership of ob */
jx_status jx_list_insert(jx_ob list, jx_int index, jx_ob ob);   /* takes ownership of ob */
jx_status jx_list_replace(jx_ob list, jx_int index, jx_ob ob);  /* takes ownership of ob */

jx_status jx_list_combine(jx_ob list1, jx_ob list2);    /* takes ownership of and consume list2 */

jx_ob jx_list_borrow(jx_ob list, jx_int index); /* borrows list entry */
jx_ob jx_list_get(jx_ob list, jx_int index); /* copies list entry (returns ownership) */
jx_ob jx_list_remove(jx_ob list, jx_int index); /* returns ownership of removed entry */
jx_status jx_list_delete(jx_ob list, jx_int index); /* frees entry at index */

/* homogenous lists with variable length arrays (vla's)  */

jx_int *jx_int_vla_new(jx_int size);
jx_status jx_int_vla_resize(jx_int ** ref, jx_int size);
jx_status jx_int_vla_free(jx_int ** ref);       /* will set (*ref) to NULL */

jx_ob jx_list_new_with_int_vla(jx_int ** ref);  /* takes ownership of vla */
jx_int *jx_list_as_int_vla(jx_ob ob);   /* borrows vla if homogenous, else NULL */
jx_status jx_list_set_int_vla(jx_ob list, jx_int ** ref);       /* update list vla */

jx_float *jx_float_vla_new(jx_float size);
jx_status jx_float_vla_resize(jx_float ** ref, jx_int size);
jx_status jx_float_vla_free(jx_float ** ref);   /* will set (*ref) to NULL */

jx_ob jx_list_new_with_float_vla(jx_float ** ref);      /* takes ownership of new vla */
jx_float *jx_list_as_float_vla(jx_ob ob);       /* borrows vla if homogenous, else NULL */
jx_status jx_list_set_float_vla(jx_ob ob, jx_float ** ref);     /* update list vla */

jx_ob jx_ob_with_str_vla(jx_char ** ref);

/* hashes and hashing */

jx_uint32 jx_ob_hash_code(jx_ob ob);

jx_ob jx_hash_new(void);

#define JX_HASH_FLAG_BIDIRECTIONAL 0x1

jx_ob jx_hash_new_with_flags(jx_int flags);
jx_ob jx_hash_new_from_list(jx_ob list);        /* returns owned hash made from interleaved keys & values */
jx_ob jx_hash_new_with_list(jx_ob list);        /* coverts list to hash, if allowed, else returns list */

jx_int jx_hash_size(jx_ob hash);

jx_status jx_hash_set(jx_ob hash, jx_ob key, jx_ob value);      /* assumes ownership of both key & value */

jx_bool jx_hash_has_key(jx_ob hash, jx_ob key); /* borrows key */

jx_ob jx_hash_keys(jx_ob hash); /* returns owned list of copied keys */
jx_ob jx_hash_values(jx_ob hash);       /* returns owned list of copied values */

jx_ob jx_hash_borrow(jx_ob hash, jx_ob key);    /* borrows key and returns borrowed value */
jx_ob jx_hash_get(jx_ob hash, jx_ob key);       /* borrows and returns copied (owned) value */
jx_ob jx_hash_remove(jx_ob hash, jx_ob key);    /* borrows key and returns owned value */
jx_status jx_hash_delete(jx_ob hash, jx_ob key);        /* borrows key and deletes matched key & value */

jx_ob jx_hash_borrow_key(jx_ob hash, jx_ob value);      /* borrows value and returns borrowed key */
jx_ob jx_hash_get_key(jx_ob hash, jx_ob value); /* borrows value and returns owned copy of key */

/* jxon output */

#define JX_JXON_FLAG_LOSSY_JSON  0x40000000
#define JX_JXON_FLAG_ENCODE_JSON 0x40000000
#define JX_JXON_FLAG_PRETTY      0x20000000
#define JX_JXON_FLAG_INDENT      0x10000000
#define JX_JXON_FLAG_INDENT_MASK 0x0000007F

jx_ob jx_ob_to_jxon(jx_ob ob);
jx_ob jx_ob_to_jxon_with_flags(jx_ob ob, jx_int flags);

/* json input */

jx_ob jx_ob_from_jxon_str(jx_char * str);

/* builtins */

jx_ob jx_builtin_new_from_vla(void **ref);
jx_ob jx_builtin_new_from_selector(jx_int selector);
jx_ob jx_builtin_new_from_native_fn(jx_native_fn fn);
jx_ob jx_builtin_new_from_opaque_ob(jx_opaque_ob *opaque);

/* parser */

jx_ob jx_jxon_scanner_new_with_file(FILE *file);
jx_status jx_jxon_scanner_next_ob(jx_ob *result, jx_ob scanner_ob);
jx_ob jx_jxon_scanner_get_error_message(jx_ob scanner_ob);

/* code execution engine */

jx_status jx_code_expose_secure_builtins(jx_ob namespace);
jx_status jx_code_expose_special_forms(jx_ob namespace);

jx_ob jx_code_bind_with_source(jx_ob namespace, jx_ob source);

jx_ob jx_code_eval(jx_ob node, jx_ob code);
jx_ob jx_code_exec(jx_ob node, jx_ob code);

/* destroying owned objects */

jx_status jx_ob_free(jx_ob ob);

#endif
