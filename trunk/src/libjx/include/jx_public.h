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

/* learn how we're being (or have been) compiled */

#include "jx_config.h"

/* establish our relationship with the underlying operating system */

#include "jx_os.h"

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

/* our ubiquitous jenarix object type */

typedef struct jx__ob jx_ob;

/* constants */

#define JX_TRUE                    1
#define JX_FALSE                   0

/* status codes: 
   errors status always < 0
   normal status = 0
   non-error informative status >=0
*/


#define JX_STATUS_YES                    JX_TRUE
#define JX_STATUS_NO                    JX_FALSE
#define JX_STATUS_SUCCESS               JX_FALSE

/* NOTE: if you change any of these numbers, then be prepared to
   update the unit tests as well! */

#define JX_STATUS_FREED_WEAK                   2
#define JX_STATUS_FREED_SHARED                 3

#define JX_STATUS_FAILURE                     -1
#define JX_STATUS_NULL_PTR                    -2
#define JX_STATUS_OUT_OF_MEMORY               -3
#define JX_STATUS_OS_THREAD_CREATION_FAILURE  -4
#define JX_STATUS_OS_THREADING_ERROR          -5
#define JX_STATUS_OS_MUTEX_ERROR              -6
#define JX_STATUS_OS_COND_ERROR               -7
#define JX_STATUS_OS_RLOCK_ERROR              -8
#define JX_STATUS_OS_ERROR                    -9
#define JX_STATUS_OS_TLS_ERROR               -10
#define JX_STATUS_OS_SPINLOCK_ERROR          -11
#define JX_STATUS_BAD_PTR_OR_CORRUPT_HEAP    -12
#define JX_STATUS_OVERFLOW                   -13
#define JX_STATUS_PERMISSION_DENIED          -14
#define JX_STATUS_INVALID_CONTAINER          -15
#define JX_STATUS_OB_NOT_HASHABLE            -16
#define JX_STATUS_NOT_FOUND                  -17
#define JX_STATUS_EXHAUSTED                  -18
#define JX_STATUS_SYNTAX_ERROR               -19
#define JX_STATUS_INVALID_IDENTIFIER         -20

/* shortcuts */

#define JX_SUCCESS JX_STATUS_SUCCESS
#define JX_FAILURE JX_STATUS_FAILURE
#define JX_YES     JX_STATUS_YES
#define JX_NO      JX_STATUS_NO

#define JX_OB_TYPE_NULL               2
#define JX_OB_TYPE_BOOL               3
#define JX_OB_TYPE_INT                4
#define JX_OB_TYPE_FLOAT              5
#define JX_OB_TYPE_STR                6
#define JX_OB_TYPE_LIST               7
#define JX_OB_TYPE_HASH               8
#define JX_OB_TYPE_IDENT              9
#define JX_OB_TYPE_OPCODE            10

/* enable C++ mangling */
#ifdef __cplusplus
#if 0
{
#endif
}
#endif


/* inline functions and structs and required by the compiler to be
 public, but should be treated by API-users as private */

#include "jx_inline.h"

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

/* meaningful prepositions, for systematic memory management:

   ..._ob_with_...(...) implies a destructive conversion of input object (destroyed)

   ..._ob_from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._ob_to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._ob_as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)
   
*/

void jx_jxon_dump(FILE *f, char *prefix, jx_ob ob);

void jx_jxon_dump_in_node(FILE *f, char *prefix, jx_ob node, jx_ob ob);

/* boxing (all of these return owned objects) */

// jx_ob jx_ob_from_null(void);
jx_ob jx_ob_from_bool(jx_bool bool_);
jx_ob jx_ob_from_int(jx_int int_);
jx_ob jx_ob_from_float(jx_float float_);
jx_ob jx_ob_from_str(jx_char * str);    /* copies string into new storage */
jx_ob jx_ob_from_str_with_len(jx_char * str, jx_int len); /* copies string into new storage */
jx_ob jx_ob_from_ident(jx_char * ident);  /* copies identifier into new storage */
jx_ob jx_ob_from_ident_with_len(jx_char * str, jx_int len); /* copies string into new storage */

/* unboxing / volatile / not-gaureanteed casts for inline use */

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

/* jxon output */

#define JX_JXON_FLAG_JSON_ENCODE    0x40000000
#define JX_JXON_FLAG_JSON_LOSSY     0x20000000
#define JX_JXON_FLAG_PRETTY         0x10000000
#define JX_JXON_FLAG_INDENT         0x08000000
#define JX_JXON_FLAG_SHOW_WEAK      0x04000000
#define JX_JXON_FLAG_COMMENT        0x02000000
#define JX_JXON_FLAG_NOT_NEWLINE    0x01000000
#define JX_JXON_FLAG_NO_ROUNDED     0x00800000
#define JX_JXON_FLAG_APPROX_FLOAT   0x00400000
#define JX_JXON_FLAG_SORT_HASHES    0x00200000
#define JX_JXON_FLAG_TAB_MASK       0x000F0000
#define JX_JXON_FLAG_WIDTH_MASK     0x0000FF00
#define JX_JXON_FLAG_INDENT_MASK    0x000000FF

jx_ob jx_ob_to_jxon(jx_ob ob);
jx_ob jx_ob_to_jxon_in_node(jx_ob node, jx_ob ob);
jx_ob jx_ob_to_jxon_with_flags(jx_ob node, jx_ob ob, jx_int flags, jx_int indent, 
                               jx_int width, jx_int space_left);

/* constructive casts which preserve the source object and return a new object */

jx_ob jx_ob_to_bool(jx_ob ob);
jx_ob jx_ob_to_int(jx_ob ob);
jx_ob jx_ob_to_float(jx_ob ob);
jx_ob jx_ob_to_str(jx_ob ob);
jx_ob jx_ob_to_ident(jx_ob ob);

jx_status jx_ob_into_str(jx_char *buf, jx_int buf_size, jx_ob ob);
jx_status jx_ob_into_strcat(jx_char *buf, jx_int buf_size, jx_ob ob);

/* determining object type */

jx_bool jx_gc_check(jx_ob ob);
jx_bool jx_null_check(jx_ob ob);
jx_bool jx_bool_check(jx_ob ob);
jx_bool jx_int_check(jx_ob ob);
jx_bool jx_float_check(jx_ob ob);
jx_bool jx_str_check(jx_ob ob);
jx_bool jx_list_check(jx_ob ob);
jx_bool jx_hash_check(jx_ob ob);
jx_bool jx_ident_check(jx_ob ob);

jx_ob jx_ident_new_from_dotted(jx_ob list);

/* builtin object types */

jx_bool jx_builtin_check(jx_ob ob);
jx_bool jx_builtin_entity_check(jx_ob ob);
jx_bool jx_builtin_selector_check(jx_ob ob);
jx_bool jx_builtin_opaque_ob_check(jx_ob ob);
jx_bool jx_builtin_native_fn_check(jx_ob ob);
jx_bool jx_builtin_callable_check(jx_ob ob);

/* comparing objects */

jx_bool jx_ob_same(jx_ob left, jx_ob right); /* refer to the same memory ([1] != [1]) */

jx_bool jx_ob_identical(jx_ob left, jx_ob right); /* bitwise identical (1 != 1.0) */

jx_bool jx_ob_equal(jx_ob left, jx_ob right); /* conceptually equal (1 == 1.0)*/

/* querying objects */

jx_ob jx_ob_size(jx_ob ob);

jx_status jx_ob_type(jx_ob ob);

/* common operations */

jx_ob jx_ob_add(jx_ob left, jx_ob right);
jx_ob jx_ob_sub(jx_ob left, jx_ob right);
jx_ob jx_ob_mul(jx_ob left, jx_ob right);
jx_ob jx_ob_div(jx_ob left, jx_ob right);
jx_ob jx_ob_idiv(jx_ob left, jx_ob right);
jx_ob jx_ob_mod(jx_ob left, jx_ob right);

jx_ob jx_ob_neg(jx_ob ob);
jx_ob jx_ob_not(jx_ob ob);

jx_ob jx_ob_pow(jx_ob left, jx_ob right);

jx_bool jx_ob_lt(jx_ob left, jx_ob right);
jx_bool jx_ob_gt(jx_ob left, jx_ob right);
jx_bool jx_ob_le(jx_ob left, jx_ob right);
jx_bool jx_ob_ge(jx_ob left, jx_ob right);

/* read only status (no changes allowed, cannot be freed, but can be
   stored/conveyed through mutable containers, provided that those
   containers are gone by the time read only status is cleared) */

jx_status jx_ob_set_shared(jx_ob ob, jx_bool shared);
jx_bool jx_ob_shared(jx_ob ob);

/* synchronized */

jx_status jx_ob_set_synchronized(jx_ob ob, jx_bool synchronized, jx_bool recursive);
jx_bool jx_ob_synchronized(jx_ob ob);

/* copying */

jx_ob jx_ob_copy(jx_ob ob); /* always deep / recursive; copies of
                               shared (read only) containers are not shared */

/* coping with weak references */

jx_ob jx_ob_not_weak_with_ob(jx_ob ob); /* filters out weak reference (root only) */

jx_ob jx_ob_only_strong_with_ob(jx_ob ob); /* replaces weak references with null */
jx_ob jx_ob_make_strong_with_ob(jx_ob ob); /* replaces weak references with deep copies */

/* replacing, swapping */

jx_status jx_ob_replace(jx_ob *ob_ptr,jx_ob ob);
jx_status jx_ob_replace_with_null(jx_ob *ob_ptr);

jx_ob jx_ob_swap(jx_ob *ob_ptr,jx_ob ob);
jx_ob jx_ob_swap_with_null(jx_ob *ob_ptr);

/* convenience method for freeing while re-initializing */

jx_ob jx_null_with_ob(jx_ob ob);

/* creating a weak reference to an object */

jx_ob jx_ob_take_weak_ref(jx_ob ob);  

/* strings */

jx_int jx_str_size(jx_ob ob);
jx_int jx_str_compare(jx_ob left, jx_ob right);
jx_ob jx_str_concat(jx_ob left, jx_ob right);
jx_ob jx_str_concat_with_both(jx_ob left, jx_ob right);

jx_ob jx_str_join_from_list(jx_ob list);
jx_ob jx_str_join_from_list_sep(jx_ob list,jx_ob sep); /* neither consumed */
jx_ob jx_str_join_with_list(jx_ob list);
jx_ob jx_str_join_with_list_sep(jx_ob list,jx_ob sep); /* both consumed */

/* lists */

jx_ob jx_list_new(void);
jx_ob jx_list_new_from_int_array(jx_int * array, jx_int size);  /* copies array */
jx_ob jx_list_new_from_float_array(jx_float * array, jx_float size);    /* copies array */
jx_ob jx_list_new_with_size(jx_int size); /* creates a null-filled list */
jx_ob jx_list_new_with_fill(jx_int size, jx_ob fill); /* consumes fill */
jx_ob jx_list_new_with_repeat(jx_int size, jx_ob repeat); /* consumes repeat */
jx_ob jx_list_new_with_range(jx_int start, jx_int stop, jx_int step, jx_float scale);

jx_ob jx_list_new_from_hash(jx_ob hash); /* returns owned list of copied keys & values (interleaved) */
jx_ob jx_list_new_with_hash(jx_ob hash); /* frees hash and converts content to interleaved list */

jx_int jx_list_size(jx_ob list);
jx_status jx_list_resize(jx_ob list, jx_int size);
jx_status jx_list_resize_with_fill(jx_ob list, jx_int size, jx_ob fill);
   /* takes ownership of fill object */

jx_status jx_list_append(jx_ob list, jx_ob ob); /* takes ownership of ob */
jx_status jx_list_insert(jx_ob list, jx_int index, jx_ob ob);   /* takes ownership of ob */
jx_status jx_list_replace(jx_ob list, jx_int index, jx_ob ob);  /* takes ownership of ob */

jx_status jx_list_unshift(jx_ob list, jx_ob ob); /* takes ownership of ob */
jx_status jx_list_push(jx_ob list, jx_ob ob);  /* takes ownership of ob */

jx_status jx_list_combine(jx_ob list1, jx_ob list2);    /* takes ownership of and consume list2 */

jx_ob jx_list_shift(jx_ob list); /* returns ownership of removed entry */
jx_ob jx_list_borrow(jx_ob list, jx_int index); /* borrows list entry */
jx_ob jx_list_get(jx_ob list, jx_int index); /* copies list entry (returns ownership) */
jx_ob jx_list_pop(jx_ob list); /* returns ownership of removed entry */

jx_ob jx_list_remove(jx_ob list, jx_int index); /* returns ownership of removed entry */
jx_ob jx_list_new_from_slice(jx_ob list, jx_int start, jx_int stop); /* returns ownership of sliced entry */
jx_ob jx_list_new_with_cutout(jx_ob list, jx_int start, jx_int stop); /* returns ownership of cutout entry */
jx_ob jx_list_swap_with_null(jx_ob list, jx_int index); /* return ownership of swapped-out object */
jx_ob jx_list_swap(jx_ob list, jx_int index, jx_ob ob); /* return ownership of swapped-out object */

jx_int jx_list_index(jx_ob list, jx_ob ob); 

jx_status jx_list_delete(jx_ob list, jx_int index); /* frees entry at index */
jx_status jx_list_reset(jx_ob list);
jx_status jx_list_reverse(jx_ob list);
jx_status jx_list_sort(jx_ob list);

/* homogenous lists with variable length arrays (vla's)  */

jx_int *jx_int_vla_new(jx_int size);
jx_status jx_int_vla_resize(jx_int ** ref, jx_int size);
jx_status jx_int_vla_free(jx_int ** ref);       /* will set (*ref) to NULL */

jx_ob jx_list_new_with_int_vla(jx_int ** ref);  /* takes ownership of vla */
jx_int *jx_list_as_int_vla(jx_ob ob);   /* borrows vla if homogenous, else NULL */
jx_status jx_list_set_int_vla(jx_ob list, jx_int ** ref);       /* update list vla */
jx_float *jx_list_take_int_vla(jx_ob ob);
jx_status jx_list_replace_int_vla(jx_ob ob, jx_float ** ref);   

jx_float *jx_float_vla_new(jx_float size);
jx_status jx_float_vla_resize(jx_float ** ref, jx_int size);
jx_status jx_float_vla_free(jx_float ** ref);   /* will set (*ref) to NULL */

jx_ob jx_list_new_with_float_vla(jx_float ** ref);      /* takes ownership of new vla */
jx_float *jx_list_as_float_vla(jx_ob ob);       /* borrows vla if homogenous, else NULL */
jx_status jx_list_set_float_vla(jx_ob ob, jx_float ** ref);     /* update list vla */
jx_float *jx_list_take_float_vla(jx_ob ob);
jx_status jx_list_replace_float_vla(jx_ob ob, jx_float ** ref);   

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

jx_bool jx_hash_peek(jx_ob *result, jx_ob hash, jx_ob key); /* borrows value if present */

jx_ob jx_hash_borrow(jx_ob hash, jx_ob key);    /* borrows key and returns borrowed value */
jx_ob jx_hash_get(jx_ob hash, jx_ob key);       /* borrows and returns copied (owned) value */
jx_ob jx_hash_remove(jx_ob hash, jx_ob key);    /* borrows key and returns owned value (destroys key inside hash) */
jx_status jx_hash_delete(jx_ob hash, jx_ob key);        /* borrows key and deletes matched key & value */

jx_ob jx_hash_borrow_key(jx_ob hash, jx_ob value);      /* borrows value and returns borrowed key */
jx_ob jx_hash_get_key(jx_ob hash, jx_ob value); /* borrows value and returns owned copy of key */

/* json input */

jx_ob jx_ob_from_jxon_str(jx_char * str);

/* builtins */

jx_ob jx_builtin_new_with_vla(void **ref);
jx_ob jx_builtin_new_from_selector(jx_int selector);
jx_ob jx_builtin_new_from_native_fn(jx_native_fn fn);
jx_ob jx_builtin_new_with_opaque_ob(jx_opaque_ob *opaque);

/* scanners & parsers */

#define JX_SCANNER_FLAG_ECHO_COMMENTS 0x1000

jx_ob jx_jxon_scanner_new_with_file(FILE *file,jx_int flags);
jx_ob jx_jxon_scanner_get_error_message(jx_ob scanner);
jx_status jx_jxon_scanner_next_ob(jx_ob *result, jx_ob scanner);
jx_status jx_jxon_scanner_purge_input(jx_ob scanner);

jx_ob jx_shell_scanner_new_with_file(FILE *file);
jx_ob jx_shell_scanner_get_error_message(jx_ob scanner);
jx_status jx_shell_scanner_next_ob(jx_ob *result, jx_ob scanner);
jx_status jx_shell_scanner_purge_input(jx_ob scanner);

jx_ob jx_net_scanner_new_with_file(FILE *file);
jx_ob jx_net_scanner_get_error_message(jx_ob scanner);
jx_status jx_net_scanner_next_ob(jx_ob *result, jx_ob scanner);
jx_status jx_net_scanner_purge_input(jx_ob scanner);

jx_ob jx_py_scanner_new_with_file(FILE *file,jx_int flags);
jx_ob jx_py_scanner_get_error_message(jx_ob scanner);
jx_ob jx_py_translate_with_tree(jx_ob tree);
jx_status jx_py_expose_python_builtins(jx_ob names);
jx_status jx_py_scanner_next_ob(jx_ob *result, jx_ob scanner);
jx_status jx_py_scanner_purge_input(jx_ob scanner);

/* macros */

jx_ob jx_macro_new_with_def(jx_ob name, jx_ob args, jx_ob body);
jx_ob jx_macro_to_impl(jx_ob ob);

/* JXON-based functions */

#define JX_FUNCTION_MODE_EVAL            0
#define JX_FUNCTION_MODE_EXEC            1
#define JX_FUNCTION_MODE_PARALLEL_EVAL   2
#define JX_FUNCTION_MODE_ASYNC_EXEC      3

jx_ob jx_function_new_with_def(jx_ob name, jx_ob args, jx_ob body, jx_int mode);
jx_ob jx_function_to_impl(jx_ob ob);
jx_ob jx_function_call(jx_ob node, jx_ob function, jx_ob payload);

/* code objects */

jx_status jx_code_expose_secure_builtins(jx_ob names);
jx_status jx_code_expose_special_forms(jx_ob names);

jx_ob jx_code_bind_with_source(jx_ob names, jx_ob source);
jx_ob jx_code_unbound_from_code(jx_ob code);

/* executing code in a node */

jx_ob jx_code_eval(jx_ob node, jx_ob code);
jx_ob jx_code_exec(jx_ob node, jx_ob code);

/* operating-system level process initialization */

jx_status jx_os_process_init(int argc, char* argv[]);
jx_status jx_os_process_complete(void);

/* destroying owned objects */

jx_status jx_ob_free(jx_ob ob); 

/* enable C++ mangling */
#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif
