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

/* GNU indent options (designed to match EMACS C defaults):

   indent -l90 -nut -npcs -nbad -sob -br -ce -brs -npsl -nsaf -nsai -nsaw

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_private.h"
#include "jx_mem_wrap.h"
#include "jx_safe.h"

#define JX_BUILTIN_NOP     0
#define JX_BUILTIN_IF      1
#define JX_BUILTIN_WHILE   2
#define JX_BUILTIN_DO      3
#define JX_BUILTIN_FOR     4
#define JX_BUILTIN_QUOTE   5
#define JX_BUILTIN_DEF     6
#define JX_BUILTIN_LAMBDA  7
#define JX_BUILTIN_APPLY   8
#define JX_BUILTIN_INVOKE  9
#define JX_BUILTIN_SECURE  10
#define JX_BUILTIN_RESOLVE 11
#define JX_BUILTIN_MAP     12

#define JX_BUILTIN_SPECIAL_FORMS_LIMIT 16

static jx_bool jx_declare(jx_bool ok, jx_ob namespace, jx_char * ident, jx_int selector)
{
  if(ok)
    ok = jx_ok(jx_hash_set(namespace, jx_ob_from_ident(ident),
                           jx_builtin_new_from_selector(selector)));
  return ok;
}

jx_status jx_code_expose_special_forms(jx_ob namespace)
{
  jx_bool ok = JX_TRUE;

  ok = jx_declare(ok, namespace, "nop", JX_BUILTIN_NOP);
  ok = jx_declare(ok, namespace, "if", JX_BUILTIN_IF);
  ok = jx_declare(ok, namespace, "while", JX_BUILTIN_WHILE);
  ok = jx_declare(ok, namespace, "do", JX_BUILTIN_DO);
  ok = jx_declare(ok, namespace, "for", JX_BUILTIN_FOR);
  ok = jx_declare(ok, namespace, "quote", JX_BUILTIN_QUOTE);
  ok = jx_declare(ok, namespace, "def", JX_BUILTIN_DEF);
  ok = jx_declare(ok, namespace, "lambda", JX_BUILTIN_LAMBDA);
  ok = jx_declare(ok, namespace, "apply", JX_BUILTIN_APPLY);
  ok = jx_declare(ok, namespace, "map", JX_BUILTIN_MAP);
  ok = jx_declare(ok, namespace, "invoke", JX_BUILTIN_INVOKE);
  ok = jx_declare(ok, namespace, "secure", JX_BUILTIN_SECURE);
  ok = jx_declare(ok, namespace, "resolve", JX_BUILTIN_RESOLVE);

  return ok ? JX_SUCCESS : JX_FAILURE;
}

jx_status jx_code_expose_secure_builtins(jx_ob namespace)
{
  jx_bool ok = JX_TRUE;

  ok = ok && jx_ok(jx_safe_expose_all_builtins(namespace));
  ok = ok && jx_ok(jx_code_expose_special_forms(namespace));

  return ok ? JX_SUCCESS : JX_FAILURE;
}

/* this crucial transformation wraps secure branches containing borrow
   operations (weak references), and replaces insecure borrow
   operations with get operations */

static jx_ob jx__code_secure_with_source(jx_ob source,jx_int level)
{
  switch (JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else {
      jx_int size = jx_list_size(source);
      jx_ob result = jx_list_new_with_size(size);
      jx_ob list = result;
      jx_bool must_secure = JX_FALSE;
      jx_bool allow_borrow = JX_FALSE;
      jx_ob first = jx_list_borrow(source,0);
      if(jx_builtin_selector_check(first) && (first.data.io.int_ != JX_BUILTIN_INVOKE)) {
        allow_borrow = JX_TRUE;
      }
      while(size--) {
        jx_ob entry1 = jx_list_remove(source,size);
        if((!level) && 
           jx_builtin_selector_check(entry1) && 
           (entry1.data.io.int_ == JX_BUILTIN_BORROW)) {
          jx_ob_replace(&entry1,jx_builtin_new_from_selector(JX_BUILTIN_GET));
        } else if(jx_list_check(entry1)) {
          jx_int size1 = jx_list_size(entry1);
          while(size1--) {
            jx_ob entry2 = jx_list_borrow(entry1,size1);              
            if((!level) && 
               jx_builtin_selector_check(entry2) && 
               (entry2.data.io.int_ == JX_BUILTIN_BORROW)) {
              jx_list_replace(entry1,size1,jx_builtin_new_from_selector(JX_BUILTIN_GET));
            } else if(jx_list_check(entry2)) {
              jx_int size2 = jx_list_size(entry2);
              while(size2--) {
                jx_ob entry3 = jx_list_borrow(entry2,size2);              
                if(jx_builtin_selector_check(entry3) && 
                   (entry3.data.io.int_ == JX_BUILTIN_BORROW)) {
                  if(allow_borrow) {
                    must_secure = JX_TRUE; 
                  } else {
                    jx_list_replace(entry2,size2,jx_builtin_new_from_selector(JX_BUILTIN_GET));
                  }
                }
              }
            }
          }
        }
        jx_list_replace(list, size,jx__code_secure_with_source(entry1,level+1));
      }
#if 0
      /* not necessary anymore */
      if(must_secure) {
        result = jx_list_new_with_size(2);
        jx_list_replace(result, 0, jx_builtin_new_from_selector(JX_BUILTIN_SECURE));
        jx_list_replace(result, 1, list);
      }
#endif
      jx_ob_free(source);
      return result;
    }
    break;
  case JX_META_BIT_BUILTIN:
  case JX_META_BIT_IDENT:
  case JX_META_BIT_BOOL:
  case JX_META_BIT_FLOAT:
  case JX_META_BIT_INT:
  case JX_META_BIT_STR:
    return source;
    break;
  case JX_META_BIT_HASH:
    {
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_with_hash(source);       /* destroys source */
      jx_int size = jx_list_size(list);
      while(size) {
        jx_ob key = jx_list_remove(list, 0);
        jx_ob value = jx_list_remove(list, 0);
        size = size - 2;
        jx_hash_set(result, key, jx__code_secure_with_source(value,level+1));
      }
      jx_ob_free(list);
      return result;
    }
    break;
   default:                     
    jx_ob_free(source);
    return jx_ob_from_null();
    break;
  }
}

/* consume source & return builtin-bound executable */

static jx_ob jx__code_bind_with_source(jx_ob namespace, jx_ob source)
{
  switch (JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else {
      /* [ indentifer, ...] becomes [ builtin, [identifier, ...]] */
      jx_int size = jx_list_size(source);
      jx_ob result = jx_list_new_with_size(size);
      jx_ob list = result;
      jx_ob ident = jx_list_borrow(source, 0);
      jx_int unresolved = 0;
      if(jx_ident_check(ident)) {
        jx_ob builtin = jx_hash_borrow(namespace, ident);
        if(jx_builtin_check(builtin)) { /* known builtin function (early / fixed binding) */
          if(builtin.data.io.int_ == JX_BUILTIN_NOP) {
            jx_ob_free(result);               
            jx_ob_replace(&source, jx_list_remove(source,1));
            return jx__code_bind_with_source(namespace,source);
          } else {
            switch(builtin.data.io.int_) {
            case JX_BUILTIN_GET:
            case JX_BUILTIN_SET:
            case JX_BUILTIN_BORROW:
            case JX_BUILTIN_TAKE:
            case JX_BUILTIN_DEL:
            case JX_BUILTIN_DEF:
            case JX_BUILTIN_INCR:
            case JX_BUILTIN_DECR:
            case JX_BUILTIN_APPEND:
            case JX_BUILTIN_EXTEND:
            case JX_BUILTIN_INSERT:
            case JX_BUILTIN_RESIZE:
            case JX_BUILTIN_POP:
            case JX_BUILTIN_SHIFT:
            case JX_BUILTIN_UNSHIFT:
            case JX_BUILTIN_SLICE:
            case JX_BUILTIN_CUTOUT:
              unresolved = 1;
              break;
            }
            result = jx_list_new_with_size(2);
            jx_list_replace(result, 0, builtin);
            jx_list_replace(result, 1, list); /* NOTE: list aliases result */
          }
        } else {                /* unknown identifier (fall back upon late binding) */
          result = jx_list_new_with_size(2);
          jx_list_replace(result, 0, jx_builtin_new_from_selector(JX_BUILTIN_INVOKE));
          jx_list_replace(result, 1, list); /* NOTE: list aliases result */
        }
      } else if(jx_builtin_check(ident) && ident.data.io.int_ == JX_BUILTIN_NOP) {
        jx_ob_free(result);
        jx_ob_replace(&source, jx_list_remove(source,1));
        return jx__code_bind_with_source(namespace,source);
      }
      /* now process the source list, introducing symbol resolution where needed */
      while(size--) {
        jx_ob entry = jx__code_bind_with_source(namespace,jx_list_remove(source, size));
        if(jx_ident_check(entry)) {
          if(size>unresolved) {
            jx_ob new_entry = jx_list_new_with_size(2);
            jx_list_replace(new_entry, 0, jx_builtin_new_from_selector(JX_BUILTIN_RESOLVE));
            jx_list_replace(new_entry, 1, entry); 
            entry = new_entry;
          }
        }
        jx_list_replace(list, size, entry);
      }
      jx_ob_free(source);
      return result;
    }
    break;
  case JX_META_BIT_HASH:
    {
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_with_hash(source);       /* destroys source */
      jx_int size = jx_list_size(list);
      while(size) {
        jx_ob key = jx_list_remove(list, 0);
        jx_ob value = jx_list_remove(list, 0);
        size = size - 2;
        jx_hash_set(result, key, jx__code_bind_with_source(namespace, value));
      }
      jx_ob_free(list);
      return result;
    }
    break;
  case JX_META_BIT_IDENT:
  case JX_META_BIT_BOOL:
  case JX_META_BIT_FLOAT:
  case JX_META_BIT_INT:
  case JX_META_BIT_STR:
    return source;
    break;
  default:                     /* unrecognized */
    jx_ob_free(source);
    return jx_ob_from_null();
    break;
  }
}

jx_ob jx_code_bind_with_source(jx_ob namespace, jx_ob source)
{
  if(jx_ident_check(source)) {  /* if code consists solely of a top-level identifier */
    jx_ob builtin = jx_hash_borrow(namespace, source);
    jx_ob result = jx_list_new_with_size(2);
    if(jx_builtin_check(builtin)) {     /* known builtin function (early / fixed binding) */
      jx_list_replace(result, 0, builtin);
      jx_list_replace(result, 1, source);
    } else {                    /* unknown identifier (fall back upon late binding) */
      jx_list_replace(result, 0, jx_builtin_new_from_selector(JX_BUILTIN_RESOLVE));
      jx_list_replace(result, 1, source);
    }
    return jx__code_secure_with_source(result,0);
  } else
    return jx__code_secure_with_source
      ( jx__code_bind_with_source(namespace, source),0);
}

JX_INLINE jx_ob jx_code_eval_allow_weak(jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_eval(node,expr) : expr;
}

JX_INLINE jx_ob jx_code_exec_allow_weak(jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_exec(node,expr) : expr;
}

jx_ob jx__code_eval(jx_ob node, jx_ob expr)
{
  switch (expr.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_LIST:
    {
      jx_list *expr_list = expr.data.io.list;
      if(expr_list->packed_meta_bits) { /* packed data cannot contain code */
        return jx_ob_copy(expr);
      } else {
        jx_ob result = JX_OB_NULL;
        jx_ob builtin = jx__list_borrow(expr_list, 0);
        if(jx_builtin_any_fn_check(builtin)) {
          jx_bits bits = builtin.meta.bits;
          if(bits & JX_META_BIT_BUILTIN_SELECTOR) {
            if(builtin.data.io.int_ < JX_BUILTIN_SPECIAL_FORMS_LIMIT) {
              jx_ob payload = jx__list_borrow(expr_list, 1);
              switch (builtin.data.io.int_) {
              case JX_BUILTIN_IF:
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob cond = jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 1));
                  if(jx_ob_as_bool(cond)) {
                    jx_ob_replace(&result,
                                  jx_code_exec_allow_weak(node, jx__list_borrow(payload_list, 2)));
                  } else {
                    jx_ob_replace(&result,
                                  jx_code_exec_allow_weak(node, jx__list_borrow(payload_list, 3)));
                  }
                  jx_ob_free(cond);
                } else
                  jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_WHILE:
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob cond = jx__list_borrow(payload_list, 1);
                  jx_ob body = jx__list_borrow(payload_list, 2);
                  while(1) {
                    jx_ob test = jx_code_eval_allow_weak(node, cond);
                    if(jx_ob_as_bool(test)) {
                      jx_ob_free(test);
                      jx_ob_replace(&result, jx_code_exec_allow_weak(node, body));
                    } else {
                      jx_ob_free(test);
                      break;
                    }
                  }
                } else
                  jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_DO:
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob body = jx__list_borrow(payload_list, 1);
                  jx_ob cond = jx__list_borrow(payload_list, 2);
                  jx_bool keep_going = JX_TRUE;
                  while(keep_going) {
                    jx_ob_replace(&result, jx_code_exec_allow_weak(node, body));
                    jx_ob test = jx_code_eval_allow_weak(node, cond);
                    keep_going = jx_ob_as_bool(test);
                    jx_ob_free(test);
                  }
                } else
                    jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_FOR:   /* [for x [get x] [add x 1] [get x]] */
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob cond = jx__list_borrow(payload_list, 2);
                  jx_ob step = jx__list_borrow(payload_list, 3);
                  jx_ob body = jx__list_borrow(payload_list, 4);
                  jx_ob_free(jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 1)));
                  while(1) {
                    jx_ob test = jx_code_eval_allow_weak(node, cond);
                    if(jx_ob_as_bool(test)) {
                      jx_ob_free(test);
                      jx_ob_replace(&result, jx_code_exec_allow_weak(node, body));
                    } else {
                      jx_ob_free(test);
                      break;
                    }
                    jx_ob_free(jx_code_eval_allow_weak(node, step));
                  }
                } else
                    jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_DEF:   /* [def name node code] */
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob ident = jx_ob_only_strong_with_ob
                    (jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 1)));
                  jx_ob inv_node = jx_ob_only_strong_with_ob
                    (jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 2)));
                  jx_ob code = jx_ob_only_strong_with_ob
                    (jx_ob_copy(jx__list_borrow(payload_list, 3)));
                  jx_ob function = jx_function_new_with_def(ident, inv_node, code);
                  jx_ob_replace(&result,
                                jx_ob_from_status(jx_hash_set(node, ident, function)));
                } else
                    jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_LAMBDA:        /* [lambda node code] */
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob inv_node = jx_ob_only_strong_with_ob
                    (jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 1)));
                  jx_ob code = jx_ob_only_strong_with_ob
                    (jx_ob_copy(jx__list_borrow(payload_list, 2)));
                  jx_ob function = jx_function_new_with_def(jx_ob_from_null(),
                                                            inv_node, code);
                  jx_ob_replace(&result, function);
                } else
                    jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_APPLY: /* [apply fn payload] */
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob fn = jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 1));
                  if(jx_builtin_any_fn_check(fn)) {
                    jx_ob e_payload = jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 2));
                    jx_ob frame = jx_list_new_with_size(2);
                    jx_list_replace(frame, 0, fn);
                    jx_list_replace(frame, 1, e_payload);
                    jx_ob_replace(&result, jx_code_eval_allow_weak(node, frame));
                    jx_ob_free(frame);
                  } else {    /* trying to apply a non-function */
                    jx_ob_replace_with_null(&result);
                    jx_ob_free(fn);
                  }
                } else
                    jx_ob_replace_with_null(&result);
                break;
              case JX_BUILTIN_MAP: /* [map fn list] */
                if(jx_list_check(payload)) {
                  jx_list *payload_list = payload.data.io.list;
                  jx_ob builtin = jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 1));
                  if(jx_function_check(builtin)) {
                    jx_function *fn = builtin.data.io.function;
                    jx_ob inp_list = jx_code_eval_allow_weak(node, jx__list_borrow(payload_list, 2));
                    jx_int i,size = jx_list_size(inp_list);
                    jx_ob out_list = jx_list_new_with_size(size);
                    for(i=0;i<size;i++) {
                      jx_list_replace(out_list, i, jx_function_call(fn, node, jx_list_get(inp_list, i)));
                    }
                    jx_ob_free(inp_list);
                    jx_ob_replace(&result, out_list);
                  } else { /* trying to apply a non-function */
                    jx_ob_replace_with_null(&result);
                  }
                  jx_ob_free(builtin);
                } else
                    jx_ob_replace_with_null(&result);
                break;
               case JX_BUILTIN_INVOKE:
                {
                  jx_ob e_payload = jx_code_eval_allow_weak(node, jx__list_borrow(expr_list, 1));
                  if(jx_list_check(e_payload)) {
                    jx_list *e_payload_list = e_payload.data.io.list;
                    jx_ob ident = jx__list_borrow(e_payload_list, 0);
                    jx_ob fn;
                    if(jx_hash_peek(&fn, node, ident)) {
                      if(jx_builtin_any_fn_check(fn)) {
                        jx_ob frame = jx_list_new_with_size(2);
                        jx_list_replace(frame, 0, jx_ob_take_weak_ref(fn));
                        jx_list_replace(frame, 1, e_payload);
                        jx_ob_replace(&result, jx_code_eval_allow_weak(node, frame));
                        jx_ob_free(frame);
                      } else {
                        jx_list_replace(e_payload, 0, jx_ob_copy(fn));
                        jx_ob_replace(&result, e_payload);
                      }
                    } else {
                      jx_ob_replace(&result, e_payload);
                    }
                  } else if(jx_ident_check(e_payload)) {
                    jx_ob value;
                    if(jx_hash_peek(&value, node, e_payload)) {
                      jx_ob_replace(&result, jx_ob_copy(value));
                      jx_ob_free(e_payload);
                    } else {
                      jx_ob_replace(&result, e_payload);
                    }
                  } else {
                    jx_ob_replace(&result, e_payload);
                  }
                }
                break;
              case JX_BUILTIN_RESOLVE:
                {
                  jx_ob e_payload = jx_code_eval_allow_weak(node, jx__list_borrow(expr_list, 1));
                  if(jx_ident_check(e_payload)) {
                    jx_ob value;
                    if(jx_hash_peek(&value, node, e_payload)) {
                      jx_ob_replace(&result, jx_ob_copy(value));
                      jx_ob_free(e_payload);
                    } else {
                      jx_ob_replace(&result, e_payload);
                    }
                  } else {
                    jx_ob_replace(&result, e_payload);
                  }
                }
                break;
              case JX_BUILTIN_SECURE:
                /* prevents weak references from escaping the local frame */
                jx_ob_replace(&result, jx_ob_only_strong_with_ob(jx_code_eval_allow_weak(node, payload)));
                break;
              case JX_BUILTIN_QUOTE:
                jx_ob_replace(&result, jx_ob_copy(payload));
                break;
              default:
                jx_ob_replace(&result, jx_code_eval_allow_weak(node, payload));
                break;
              }
            } else {
              jx_ob payload = jx_code_eval_allow_weak(node, jx__list_borrow(expr_list, 1));
              switch (builtin.data.io.int_) {
              case JX_BUILTIN_SET:
                result = jx_safe_set(node, payload);
                break;
              case JX_BUILTIN_GET:
                result = jx_safe_get(node, payload);
                break;
              case JX_BUILTIN_TAKE:
                result = jx_safe_take(node, payload);
                break;
              case JX_BUILTIN_BORROW:
                result = jx_safe_borrow(node, payload);
                break;
              case JX_BUILTIN_DEL:
                result = jx_safe_del(node, payload);
                break;
              case JX_BUILTIN_SIZE:
                result = jx_safe_size(node, payload);
                break;
              case JX_BUILTIN_IDENTICAL:
                result = jx_safe_identical(node, payload);
                break;
              case JX_BUILTIN_EQ:
                result = jx_safe_eq(node, payload);
                break;
              case JX_BUILTIN_NE:
                result = jx_safe_ne(node, payload);
                break;
              case JX_BUILTIN_LT:
                result = jx_safe_lt(node, payload);
                break;
              case JX_BUILTIN_GT:
                result = jx_safe_gt(node, payload);
                break;
              case JX_BUILTIN_LE:
                result = jx_safe_le(node, payload);
                break;
              case JX_BUILTIN_GE:
                result = jx_safe_ge(node, payload);
                break;

              case JX_BUILTIN_ADD:
                result = jx_safe_add(node, payload);
                break;
              case JX_BUILTIN_SUB:
                result = jx_safe_sub(node, payload);
                break;
              case JX_BUILTIN_MUL:
                result = jx_safe_mul(node, payload);
                break;
              case JX_BUILTIN_DIV:
                result = jx_safe_div(node, payload);
                break;
              case JX_BUILTIN_MOD:
                result = jx_safe_mod(node, payload);
                break;

              case JX_BUILTIN_AND:
                result = jx_safe_and(node, payload);
                break;
              case JX_BUILTIN_OR:
                result = jx_safe_or(node, payload);
                break;

              case JX_BUILTIN_NEG:
                result = jx_safe_neg(node, payload);
                break;
              case JX_BUILTIN_NOT:
                result = jx_safe_not(node, payload);
                break;

              case JX_BUILTIN_OUTPUT:
                result = jx_safe_output(node, payload);
                break;
              case JX_BUILTIN_ERROR:
                result = jx_safe_error(node, payload);
                break;

              case JX_BUILTIN_APPEND:
                result = jx_safe_append(node, payload);
                break;
              case JX_BUILTIN_EXTEND:
                result = jx_safe_extend(node, payload);
                break;
              case JX_BUILTIN_INSERT:
                result = jx_safe_insert(node, payload);
                break;
              case JX_BUILTIN_RESIZE:
                result = jx_safe_resize(node, payload);
                break;
              case JX_BUILTIN_POP:
                result = jx_safe_pop(node, payload);
                break;
              case JX_BUILTIN_SHIFT:
                result = jx_safe_shift(node, payload);
                break;
              case JX_BUILTIN_UNSHIFT:
                result = jx_safe_unshift(node, payload);
                break;
              case JX_BUILTIN_SLICE:
                result = jx_safe_slice(node, payload);
                break;
              case JX_BUILTIN_CUTOUT:
                result = jx_safe_cutout(node, payload);
                break;
              case JX_BUILTIN_IMPL:
                result = jx_safe_impl(node, payload);
                break;
              case JX_BUILTIN_DECR:
                result = jx_safe_decr(node, payload);
                break;
              case JX_BUILTIN_INCR:
                result = jx_safe_incr(node, payload);
                break;
              }
              jx_ob_free(payload);
            }
          } else if(bits & JX_META_BIT_BUILTIN_NATIVE_FN) {
            jx_ob payload = jx_code_eval_allow_weak(node, jx__list_borrow(expr_list, 1));
            jx_native_fn fn = builtin.data.io.native_fn;
            if(fn) {
              jx_ob_replace(&result, fn(node, payload));        /* consumes payload */
            }
          } else if(bits & JX_META_BIT_BUILTIN_FUNCTION) {
            jx_ob payload = jx_code_eval_allow_weak(node, jx__list_borrow(expr_list, 1));
            jx_function *fn = builtin.data.io.function;
            jx_ob_replace(&result, jx_function_call(fn, node, payload));        /* consumes payload */
          } else {
            jx_ob_replace(&result, jx_ob_from_null());
          }
        } else {
          jx_int size = jx__list_size(expr_list);
          jx_int i = 0;
          result = jx_list_new_with_size(size);
          if(result.meta.bits & JX_META_BIT_LIST) {
            jx_list *result_list = result.data.io.list;
            while(i < size) {
              jx_ob_replace(result_list->data.ob_vla + i,
                            jx_code_eval_allow_weak(node, jx__list_borrow(expr_list, i)));
              i++;
            }
          }
        }
        return result;
      }
    }
    break;
  case JX_META_BIT_HASH:
    {
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_from_hash(expr);
      jx_int size = jx_list_size(list);
      while(size) {
        jx_ob key = jx_list_remove(list, 0);
        jx_ob value = jx_list_remove(list, 0);
        size = size - 2;
        jx_hash_set(result, key, jx_code_eval_allow_weak(node, value));
        jx_ob_free(value);
      }
      jx_ob_free(list);
      return result;
    }
    break;
  default:
    return jx_ob_copy(expr);
    break;
  }
}

jx_ob jx__code_exec(jx_ob node, jx_ob code)
{
  /* on entry, code is known to be a list.
     if code is [builtin_fn, [...]] then evaluate & return result
     if code is [ [builtin_fn,[...]], [builtin_fn,[...]], ... ] then
     evaluate all members of a list, returning only the last result */

  jx_ob result = JX_OB_NULL;
  jx_int pc, size = jx_list_size(code);
  jx_list *code_list = code.data.io.list;
  jx_ob inst = jx__list_borrow(code_list, 0);
  if(jx_builtin_any_fn_check(inst)) {   /* list with a single builtin instruction */
    jx_ob_replace(&result, jx_code_eval_allow_weak(node, code));
  } else {
    for(pc = 0; pc < size; pc++) {
      if(pc)
        inst = jx__list_borrow(code_list, pc);
      jx_ob_replace(&result, jx_code_eval_allow_weak(node, inst));
    }
  }
  return result;
}
