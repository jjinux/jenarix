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

#include "jx_private.h"
#include "jx_mem_wrap.h"
#include "jx_selectors.h"

#include "jx_safe.h"

static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char * ident, jx_int selector)
{
  if(ok)
    ok = jx_ok(jx_hash_set(names, jx_ob_from_ident(ident),
                           jx_builtin_new_from_selector(selector)));
  return ok;
}

jx_status jx_code_expose_special_forms(jx_ob names)
{
  jx_bool ok = JX_TRUE;

  ok = jx_declare(ok, names, "nop", JX_SELECTOR_NOP);

  ok = jx_declare(ok, names, "if", JX_SELECTOR_IF);
  ok = jx_declare(ok, names, "while", JX_SELECTOR_WHILE);
  ok = jx_declare(ok, names, "do", JX_SELECTOR_DO);
  ok = jx_declare(ok, names, "for", JX_SELECTOR_FOR);
  ok = jx_declare(ok, names, "foreach", JX_SELECTOR_FOREACH);
  ok = jx_declare(ok, names, "test", JX_SELECTOR_TEST);

  ok = jx_declare(ok, names, "quote", JX_SELECTOR_QUOTE);
  ok = jx_declare(ok, names, "raw", JX_SELECTOR_RAW);

  ok = jx_declare(ok, names, "def", JX_SELECTOR_DEF);

  ok = jx_declare(ok, names, "defun", JX_SELECTOR_DEFUN);

  ok = jx_declare(ok, names, "defmac", JX_SELECTOR_DEFMAC);

  ok = jx_declare(ok, names, "code", JX_SELECTOR_CODE);
  ok = jx_declare(ok, names, "lambda", JX_SELECTOR_LAMBDA);

  ok = jx_declare(ok, names, "apply", JX_SELECTOR_APPLY);
  ok = jx_declare(ok, names, "map", JX_SELECTOR_MAP);
  ok = jx_declare(ok, names, "reduce", JX_SELECTOR_REDUCE);

  ok = jx_declare(ok, names, "resolve", JX_SELECTOR_RESOLVE);

  ok = jx_declare(ok, names, "eval", JX_SELECTOR_EVAL);
  ok = jx_declare(ok, names, "exec", JX_SELECTOR_EXEC);
  ok = jx_declare(ok, names, "dispatch", JX_SELECTOR_DISPATCH);
  ok = jx_declare(ok, names, "switch", JX_SELECTOR_SWITCH);

  ok = jx_declare(ok, names, "select", JX_SELECTOR_SELECT);
  return ok ? JX_SUCCESS : JX_FAILURE;
}

jx_status jx_code_expose_secure_builtins(jx_ob names)
{
  jx_bool ok = JX_TRUE;

  ok = ok && jx_ok(jx_safe_expose_all_builtins(names));
  ok = ok && jx_ok(jx_code_expose_special_forms(names));
  
  return ok ? JX_SUCCESS : JX_FAILURE;
}

#if 0

// unneccesary -- we've eliminated weak references from JXON runtime

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
      jx_ob first = jx_list_borrow(source,0);
      while(size--) {
        jx_ob entry1 = jx_list_remove(source,size);
        if((!level) && 
           jx_builtin_selector_check(entry1) && 
           (entry1.data.io.int_ == JX_SELECTOR_BORROW)) {
          jx_ob_replace(&entry1,jx_builtin_new_from_selector(JX_SELECTOR_GET));
        } else if(jx_list_check(entry1)) {
          jx_int size1 = jx_list_size(entry1);
          while(size1--) {
            jx_ob entry2 = jx_list_borrow(entry1,size1);              
            if((!level) && 
               jx_builtin_selector_check(entry2) && 
               (entry2.data.io.int_ == JX_SELECTOR_BORROW)) {
              jx_list_replace(entry1,size1,jx_builtin_new_from_selector(JX_SELECTOR_GET));
            } else if(jx_list_check(entry2)) {
              jx_int size2 = jx_list_size(entry2);
              while(size2--) {
                jx_ob entry3 = jx_list_borrow(entry2,size2);              
                if(jx_builtin_selector_check(entry3) && 
                   (entry3.data.io.int_ == JX_SELECTOR_BORROW)) {
                  if(allow_borrow) {
                    must_secure = JX_TRUE; 
                  } else {
                    jx_list_replace(entry2,size2,jx_builtin_new_from_selector(JX_SELECTOR_GET));
                  }
                }
              }
            }
          }
        }
        jx_list_replace(list, size,jx__code_secure_with_source(entry1,level+1));
      }
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
#endif

/* consume source & return builtin-bound executable */
static jx_ob jx__code_bind_with_source(jx_ob names, jx_ob source)
{
  switch (JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else {
      /* [ indent ...]  becomes [ builtin ...] */
      jx_int size = jx_list_size(source);
      jx_list *source_list = source.data.io.list;
      jx_ob ident = jx_list_borrow(source, 0);
      jx_int unresolved = 0;
      if(jx_ident_check(ident)) {
        jx_ob builtin = jx_hash_borrow(names, ident);
        if(jx_builtin_selector_check(builtin)) { 
          /* known builtin function (early / fixed binding) */
          unresolved = 1;
          switch(builtin.data.io.int_) {
          case JX_SELECTOR_NOP: /* [nop # ...] -> bind(#) */
            jx_ob_replace(&source, jx_list_remove(source,1));
            return jx__code_bind_with_source(names,source);
            break;
          case JX_SELECTOR_RAW: /* [raw # ...] -> [*raw* # ...] */
            jx_list_replace(source, 0, builtin);
            return source;
            break;
          case JX_SELECTOR_QUOTE: /* [quote # ...] -> [*quote* bind(#) ...] */
            jx_list_replace(source, 0, builtin);
            return jx__code_bind_with_source(names,source);
            break;
          case JX_SELECTOR_ENTITY:
          case JX_SELECTOR_GET:
          case JX_SELECTOR_SET:
          case JX_SELECTOR_HAS:
          case JX_SELECTOR_DEL:
          case JX_SELECTOR_FOREACH:
          case JX_SELECTOR_LAMBDA:
          case JX_SELECTOR_INCR:
          case JX_SELECTOR_CODE:
          case JX_SELECTOR_DECR:
            unresolved = 2;
            break;
          case JX_SELECTOR_DEF:
          case JX_SELECTOR_DEFUN:
          case JX_SELECTOR_DEFMAC:
            unresolved = 3;
            break;
          }
          jx_list_replace(source, 0, builtin);
        }
      } else if(jx_builtin_selector_check(ident) &&
                ident.data.io.int_ == JX_SELECTOR_NOP) {
        /* [*nop* # ...] -> bind(#) */
        jx_ob_replace(&source, jx_list_remove(source,1));
        return jx__code_bind_with_source(names,source);
      }
      //      jx_jxon_dump(stdout,"post-sub",source);
      /* now process the source list, introducing symbol resolution where needed */
      while((size>unresolved)&&(size--)) {
        jx_ob entry = jx__list_swap_with_null(source_list,size);
        if(jx_ident_check(entry)) { /* # -> [*res* #] */
          jx_ob new_entry = jx_list_new_with_size(2);
          jx_list_replace(new_entry, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
          jx_list_replace(new_entry, 1, entry); 
          entry = new_entry;
        } else if(entry.meta.bits & JX_META_BIT_GC) {
          entry = jx__code_bind_with_source(names,entry);
        }
        jx__list_replace(source_list, size, entry);
      }
      //      jx_jxon_dump(stdout,"post-bind",source);
      return source;
    }
    break;
  case JX_META_BIT_HASH:
    { /* TO DO refactor to avoid rehashing */
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_with_hash(source);       /* destroys source */
      jx_int size = jx_list_size(list);
      while(size) {
        jx_ob key = jx_list_remove(list, 0);
        jx_ob value = jx_list_remove(list, 0);
        size = size - 2;
        if(jx_ident_check(value)) {
          jx_ob new_entry = jx_list_new_with_size(2);
          jx_list_replace(new_entry, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
          jx_list_replace(new_entry, 1, value);
          jx_hash_set(result, key, new_entry);
        } else {
          jx_hash_set(result, key, jx__code_bind_with_source(names, value));
        }
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
  default:                     /* unrecognized or invalid input -> null */
    jx_ob_free(source);
    return jx_ob_from_null();
    break;
  }
}

jx_ob jx_code_bind_with_source(jx_ob names, jx_ob source)
{
  if(jx_ident_check(source)) {  /* if code consists solely of a top-level identifier */
    jx_ob builtin = jx_hash_borrow(names, source);
    jx_ob result;
    if(jx_builtin_check(builtin)) { /* known builtin function (early / fixed binding) */
      result = jx_list_new_with_size(1);
      jx_list_replace(result, 0, builtin);
    } else {                    /* unknown identifier (fall back upon late binding) */
      result = jx_list_new_with_size(2);
      jx_list_replace(result, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
      jx_list_replace(result, 1, source);
    }
    return result;
  } else
    return jx__code_bind_with_source(names, source);
}

static jx_ob jx__code_eval_allow_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_eval_allow_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_eval_allow_weak(tls,flags, node, expr) : expr;
}

static jx_ob jx__code_exec_allow_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_exec_allow_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_LIST) ?
    jx__code_exec_allow_weak(tls,flags, node, expr) : jx_ob_copy(expr);
}

static void jx__code_make_strong(jx_ob *ob,jx_int size)
{
  while(size--) {
    if(ob->meta.bits & JX_META_BIT_WEAK_REF) 
      *ob = jx_ob_copy(*ob); 
    ob++;
  }
}

JX_INLINE jx_ob jx__code_apply_callable(jx_tls *tls, jx_ob node, 
                                        jx_ob callable, jx_ob payload)
{ /* frees callable; payload is consumed */
  //    jx_jxon_dump(stdout,"apply callable",callable);
  //    jx_jxon_dump(stdout,"       payload",payload);
  switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
  case JX_META_BIT_BUILTIN_ENTITY:
    /* nothing should happen right now */
    jx_list_unshift(payload, callable);
    return payload;
    break;
  case JX_META_BIT_BUILTIN_NATIVE_FN:
    {
      jx_native_fn native_fn = callable.data.io.native_fn;
      if(native_fn) {
        jx_ob result = native_fn(node, jx_ob_not_weak_with_ob(payload));
        jx_ob_free(callable);
        return result;
      }
    }
    break;
  case JX_META_BIT_BUILTIN_FUNCTION:
    return jx__function_call(tls, node, callable, jx_ob_not_weak_with_ob(payload));
    break;
  case JX_META_BIT_BUILTIN_SELECTOR:
    {
      //      printf("have seletor %d\n",callable.data.io.int_);
      /* NOTE for performance reasons, the "safe" builtin
         selector functions pull from but do not free the
         result object, hence the use of jx_replace below */
      switch (callable.data.io.int_) {
      case JX_SELECTOR_ENTITY:
        jx_tls_ob_replace(tls, &payload, jx_safe_entity(node, payload));
        break;
      case JX_SELECTOR_SET:
        jx_tls_ob_replace(tls, &payload, jx_safe_set(node, payload));
        break;
      case JX_SELECTOR_GET:
        jx_tls_ob_replace(tls, &payload, jx_safe_get(node, payload));
        break;
      case JX_SELECTOR_TAKE:
        jx_tls_ob_replace(tls, &payload, jx_safe_take(node, payload));
        break;
      case JX_SELECTOR_DEL:
        jx_tls_ob_replace(tls, &payload, jx_safe_del(node, payload));
        break;
      case JX_SELECTOR_SIZE:
        jx_tls_ob_replace(tls, &payload, jx_safe_size(node, payload));
        break;
      case JX_SELECTOR_SAME:
        jx_tls_ob_replace(tls, &payload, jx_safe_same(node, payload));
        break;
      case JX_SELECTOR_IDENTICAL:
        jx_tls_ob_replace(tls, &payload, jx_safe_identical(node, payload));
        break;
      case JX_SELECTOR_EQ:
        jx_tls_ob_replace(tls, &payload, jx_safe_eq(node, payload));
        break;
      case JX_SELECTOR_NE:
        jx_tls_ob_replace(tls, &payload, jx_safe_ne(node, payload));
        break;
      case JX_SELECTOR_LT:
        jx_tls_ob_replace(tls, &payload, jx_safe_lt(node, payload));
        break;
      case JX_SELECTOR_GT:
        jx_tls_ob_replace(tls, &payload, jx_safe_gt(node, payload));
        break;
      case JX_SELECTOR_LE:
        jx_tls_ob_replace(tls, &payload, jx_safe_le(node, payload));
        break;
      case JX_SELECTOR_GE:
        jx_tls_ob_replace(tls, &payload, jx_safe_ge(node, payload));
        break;
      case JX_SELECTOR_ADD:
        jx_tls_ob_replace(tls, &payload, jx_safe_add(node, payload));
        break;
      case JX_SELECTOR_SUB:
        jx_tls_ob_replace(tls, &payload, jx_safe_sub(node, payload));
        break;
      case JX_SELECTOR_MUL:
        jx_tls_ob_replace(tls, &payload, jx_safe_mul(node, payload));
        break;
      case JX_SELECTOR_DIV:
        jx_tls_ob_replace(tls, &payload, jx_safe_div(node, payload));
        break;
      case JX_SELECTOR_MOD:
        jx_tls_ob_replace(tls, &payload, jx_safe_mod(node, payload));
        break;

      case JX_SELECTOR_AND:
        jx_tls_ob_replace(tls, &payload, jx_safe_and(node, payload));
        break;
      case JX_SELECTOR_OR:
        jx_tls_ob_replace(tls, &payload, jx_safe_or(node, payload));
        break;

      case JX_SELECTOR_NEG:
        jx_tls_ob_replace(tls, &payload, jx_safe_neg(node, payload));
        break;
      case JX_SELECTOR_NOT:
        jx_tls_ob_replace(tls, &payload, jx_safe_not(node, payload));
        break;

      case JX_SELECTOR_OUTPUT:
        jx_tls_ob_replace(tls, &payload, jx_safe_output(node, payload));
        break;
      case JX_SELECTOR_ERROR:
        jx_tls_ob_replace(tls, &payload, jx_safe_error(node, payload));
        break;

      case JX_SELECTOR_APPEND:
        jx_tls_ob_replace(tls, &payload, jx_safe_append(node, payload));
        break;
      case JX_SELECTOR_EXTEND:
        jx_tls_ob_replace(tls, &payload, jx_safe_extend(node, payload));
        break;
      case JX_SELECTOR_INSERT:
        jx_tls_ob_replace(tls, &payload, jx_safe_insert(node, payload));
        break;
      case JX_SELECTOR_RESIZE:
        jx_tls_ob_replace(tls, &payload, jx_safe_resize(node, payload));
        break;
      case JX_SELECTOR_POP:
        jx_tls_ob_replace(tls, &payload, jx_safe_pop(node, payload));
        break;
      case JX_SELECTOR_PUSH:
        jx_tls_ob_replace(tls, &payload, jx_safe_append(node, payload));
        break;
      case JX_SELECTOR_SHIFT:
        jx_tls_ob_replace(tls, &payload, jx_safe_shift(node, payload));
        break;
      case JX_SELECTOR_UNSHIFT:
        jx_tls_ob_replace(tls, &payload, jx_safe_unshift(node, payload));
        break;
      case JX_SELECTOR_SLICE:
        jx_tls_ob_replace(tls, &payload, jx_safe_slice(node, payload));
        break;
      case JX_SELECTOR_CUTOUT:
        jx_tls_ob_replace(tls, &payload, jx_safe_cutout(node, payload));
        break;
      case JX_SELECTOR_IMPL:
        jx_tls_ob_replace(tls, &payload, jx_safe_impl(node, payload));
        break;
      case JX_SELECTOR_DECR:
        jx_tls_ob_replace(tls, &payload, jx_safe_decr(node, payload));
        break;
      case JX_SELECTOR_INCR:
        jx_tls_ob_replace(tls, &payload, jx_safe_incr(node, payload));
        break;
      case JX_SELECTOR_RANGE:
        jx_tls_ob_replace(tls, &payload, jx_safe_range(node, payload));
        break;
      case JX_SELECTOR_FILL:
        jx_tls_ob_replace(tls, &payload, jx_safe_fill(node, payload));
        break;
      case JX_SELECTOR_SYMBOLS:
        jx_tls_ob_replace(tls, &payload, jx_safe_symbols(node, payload));
        break;
      case JX_SELECTOR_HAS:
        jx_tls_ob_replace(tls, &payload, jx_safe_has(node, payload));
        break;
      case JX_SELECTOR_STR:
        jx_tls_ob_replace(tls, &payload, jx_safe_str(node, payload));
        break;
      default: /* unrecognized selector? purge weak */
        jx_ob_free(callable);
        jx_ob_replace_with_null(&payload);
        break;
      }
    }
    return payload;
    break;
  }

  jx_ob_free(callable);
  jx_ob_free(payload);

  return jx_ob_from_null();
}


JX_INLINE jx_ob jx__code_reduce(jx_tls *tls, jx_int flags, jx_ob node, jx_ob callable, jx_ob inp)
{
  jx_ob out = JX_OB_NULL;
  if(jx_builtin_callable_check(callable) && jx_list_check(inp)) {
    register jx_int i,size = jx_list_size(inp);
    jx_ob arg_ob = jx_tls_list_new_with_size(tls, 2);
    if(jx_list_size(inp) && jx_list_size(arg_ob)) {
      jx_ob *left_ob = arg_ob.data.io.list->data.ob_vla;
      jx_ob *right_ob = left_ob + 1;
      jx_list *inp_list = inp.data.io.list;
      jx_ob callable_weak_ref = jx_ob_take_weak_ref(callable);
      if(!inp_list->packed_meta_bits) { /* normal object input */
        jx_ob *inp_ob = inp_list->data.ob_vla;
        *left_ob = *(inp_ob++);
        switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i=1;i<size;i++) {
                *right_ob = *(inp_ob++);
                jx_tls_ob_replace(tls, left_ob, 
                                  native_fn(node,jx_tls_ob_copy(tls,arg_ob)));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i=1;i<size;i++) {
            *right_ob = *(inp_ob++);
            jx_tls_ob_replace(tls, left_ob, 
                              jx__function_call(tls, node, 
                                                callable_weak_ref, 
                                                jx_tls_ob_copy(tls,arg_ob)));
          }
          break;
        default:
          for(i=1;i<size;i++) {
            *right_ob = *(inp_ob++);
            jx_tls_ob_replace(tls, left_ob, 
                              jx__code_apply_callable
                              (tls, node, callable_weak_ref, jx_tls_ob_copy(tls,arg_ob)));
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) { 
        /* packed integer input */
        jx_int *inp_int = inp_list->data.int_vla;
        *left_ob = jx_ob_from_int(0);        
        *right_ob = jx_ob_from_int(0);
        left_ob->data.io.int_ = *(inp_int++);
        switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i=1;i<size;i++) {
                right_ob->data.io.int_ = *(inp_int++);
                jx_tls_ob_replace(tls, left_ob, 
                                  native_fn(node, 
                                            jx_tls_ob_copy(tls,arg_ob)));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i=1;i<size;i++) {
            right_ob->data.io.int_ = *(inp_int++);
            jx_tls_ob_replace(tls, left_ob, 
                              jx__function_call
                              (tls, node, callable_weak_ref, 
                               jx_tls_ob_copy(tls,arg_ob)));
          }
          break;
        default:
          for(i=1;i<size;i++) {
            right_ob->data.io.int_ = *(inp_int++);
            jx_tls_ob_replace(tls, left_ob, 
                               jx__code_apply_callable
                              (tls, node, callable_weak_ref, 
                               jx_tls_ob_copy(tls,arg_ob)));
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) { 
        /* packed float input */
        jx_float *inp_float = inp_list->data.float_vla;
        *left_ob = jx_ob_from_float(0.0);        
        *right_ob = jx_ob_from_float(0.0);
        left_ob->data.io.float_ = *(inp_float++);
        switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i=1;i<size;i++) {
                right_ob->data.io.float_ = *(inp_float++);
                jx_tls_ob_replace(tls, left_ob, 
                                  native_fn(node, jx_tls_ob_copy(tls,arg_ob)));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i=1;i<size;i++) {
            right_ob->data.io.float_ = *(inp_float++);
            jx_tls_ob_replace(tls, left_ob, 
                              jx__function_call
                              (tls, node, callable_weak_ref,
                               jx_tls_ob_copy(tls,arg_ob)));
          }
          break;
        default:
          for(i=1;i<size;i++) {
            right_ob->data.io.float_ = *(inp_float++);
            jx_tls_ob_replace(tls, left_ob, 
                               jx__code_apply_callable
                              (tls, node, callable_weak_ref, 
                               jx_tls_ob_copy(tls,arg_ob)));
          }
          break;
        }
      }
      out = jx_list_remove(arg_ob,0);
      jx_vla_reset(&inp_list->data.ob_vla);
    }
    jx_tls_ob_free(tls, inp);
    jx_tls_ob_free(tls, arg_ob);
    jx_tls_ob_free(tls, callable);
  } else { 
    /* trying to apply a non-function -- that just won't work */
    jx_tls_ob_free(tls, inp);
    jx_tls_ob_free(tls, callable);
  }
  return out;
}

JX_INLINE jx_ob jx__code_map1(jx_tls *tls, jx_int flags, jx_ob node, jx_ob callable, jx_ob inp) 
{
  if(jx_builtin_callable_check(callable) && jx_list_check(inp)) {
    register jx_int i,size = jx_list_size(inp);
    jx_ob out = jx_tls_list_new_with_size(tls, size);
    if(jx_list_check(out) && jx_list_size(inp)) {
      jx_list *inp_list = inp.data.io.list;
      jx_list *out_list = out.data.io.list;
      jx_ob callable_weak_ref = jx_ob_take_weak_ref(callable);
      if(!inp_list->packed_meta_bits) { /* normal object input */
        jx_ob *inp_ob = inp_list->data.ob_vla;
        jx_ob *out_ob = out_list->data.ob_vla;
        switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i=0;i<size;i++) {
                *(out_ob++) = native_fn(node, *(inp_ob++));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i=0;i<size;i++) {
            *(out_ob++) = jx__function_call(tls, node, callable_weak_ref, *(inp_ob++));
          }
          break;
        default:
          for(i=0;i<size;i++) {
            *(out_ob++) = jx__code_apply_callable
              (tls, node, callable_weak_ref, *(inp_ob++));
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) { 
        /* packed integer input */
        jx_int *inp_int = inp_list->data.int_vla;
        jx_ob *out_ob = out_list->data.ob_vla;
        jx_ob inp_ob = JX_OB_INT;
        switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i=0;i<size;i++) {
                inp_ob.data.io.int_ = *(inp_int++);
                *(out_ob++) = native_fn(node, inp_ob);
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i=0;i<size;i++) {
            inp_ob.data.io.int_ = *(inp_int++);
            *(out_ob++) = jx__function_call(tls, node, callable_weak_ref, inp_ob);
          }
          break;
        default:
          for(i=0;i<size;i++) {
            inp_ob.data.io.int_ = *(inp_int++);
            *(out_ob++) = jx__code_apply_callable
              (tls, node, callable_weak_ref, inp_ob);
          }
          break;
        }
        jx_list_repack(out);
      } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) { 
        /* packed float input */
        jx_float *inp_float = inp_list->data.float_vla;
        jx_ob *out_ob = out_list->data.ob_vla;
        jx_ob inp_ob = JX_OB_FLOAT;
        switch(callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i=0;i<size;i++) {
                inp_ob.data.io.float_ = *(inp_float++);
                *(out_ob++) = native_fn(node, inp_ob);
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i=0;i<size;i++) {
            inp_ob.data.io.float_ = *(inp_float++);
            *(out_ob++) = jx__function_call(tls, node, callable_weak_ref, inp_ob);
          }
          break;
        default:
          for(i=0;i<size;i++) {
            inp_ob.data.io.float_ = *(inp_float++);
            *(out_ob++) = jx__code_apply_callable
              (tls, node, callable_weak_ref, inp_ob);
          }
          break;
        }
        jx_list_repack(out);
      } else { /* fallback: slower but safe */
        for(i=0;i<size;i++) {
          jx__list_replace
            (out_list, i,jx__code_apply_callable
             (tls, node, callable_weak_ref, jx__list_swap_with_null(inp_list, i)));
        }
      }
      jx_vla_reset(&inp_list->data.ob_vla);
    }
    jx_tls_ob_free(tls, inp);
    jx_tls_ob_free(tls, callable);
    return out;
  } else { /* trying to apply a non-function -- that just won't work */
    jx_tls_ob_free(tls, inp);
    jx_tls_ob_free(tls, callable);
    return jx_ob_from_null();
  }
}

JX_INLINE jx_ob jx__code_mapN(jx_tls *tls, jx_int flags, jx_ob node, 
                              jx_ob callable, jx_ob src_list)
{
  jx_size max_size = 0;
  jx_int n_src = jx_list_size(src_list);
  {
    jx_int i;
    for(i=0;i<n_src;i++) {
      jx_int size = jx_list_size(jx_list_borrow(src_list,i));
      if(size > max_size)
        max_size = size;
    }
  }
  {
    jx_ob out = jx_tls_list_new_with_size(tls, max_size);
    jx_ob arg_ob = jx_tls_list_new_with_size(tls, n_src);
    jx_ob callable_weak_ref = jx_ob_take_weak_ref(callable);
    if(jx_list_size(out)) {
      jx_int i,j;
      
      for(i=0;i<max_size;i++) {
        for(j=0;j<n_src;j++) {
          jx_list_replace(arg_ob, j, jx_ob_take_weak_ref(jx_list_borrow(jx_list_borrow(src_list,j),i)));
        }
        jx_list_replace(out,i,jx__code_apply_callable(tls, node, callable_weak_ref, 
                                                      jx_tls_ob_copy(tls,arg_ob)));
      }
      jx_list_repack(out);
    }
    jx_tls_ob_free(tls, arg_ob);
    jx_tls_ob_free(tls, src_list);
    jx_tls_ob_free(tls, callable);
    return out;
  }
}

static jx_ob jx__code_eval_allow_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr)
{

  //  jx_jxon_dump(stdout,"eval entered with",expr);
  //  printf("                and flags %d\n",flags);
  switch (expr.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_LIST:
    {
      jx_list *expr_list = expr.data.io.list;
      if(expr_list->packed_meta_bits || !jx__list_size(expr_list)) {
        /* empty and/packed lists cannot contain any expressions */
        return jx_tls_ob_copy(tls,expr);
      } else {
        jx_ob *expr_vla = expr_list->data.ob_vla;
        jx_bits bits = 0;
        /* first we check for a special form in slot 0 */
        if(jx_builtin_callable_check(expr_vla[0]) &&
           ((bits = expr_vla->meta.bits) & JX_META_BIT_BUILTIN_SELECTOR) &&
           (expr_vla->data.io.int_ < JX_SELECTOR_SPECIAL_FORMS_LIMIT) &&
           ((!(flags & JX_EVAL_DEFER_INVOCATION)) || 
            (expr_vla->data.io.int_ == JX_SELECTOR_RESOLVE))) {
          jx_int size = jx_vla_size(&expr_vla);
          //          printf("found special form %d\n",expr_vla->data.io.int_);
          switch (expr_vla->data.io.int_) {
          case JX_SELECTOR_QUOTE:
          case JX_SELECTOR_RAW:
            {
              jx_ob payload = (size > 1) ? expr_vla[1] : jx_ob_from_null();
              return jx_tls_ob_copy(tls,payload);
            }
            break;
          case JX_SELECTOR_EVAL:                             
            {
              jx_ob ob = (size>1) ? jx_code_eval_tls(tls, flags, node, expr_vla[1]) : 
                jx_ob_from_null();
              jx_ob result = jx_code_eval_tls(tls, flags, node, ob);
              jx_tls_ob_free(tls, ob);
              return result;
            }
            break;
          case JX_SELECTOR_EXEC:                             
            return  ((size>1) ? jx_code_exec_tls(tls, flags, node, expr_vla[1]) : 
                     jx_ob_from_null());
            break;
          case JX_SELECTOR_IF: /* [*if* expr code expr code ... code] */
            {
              int i = 1;
              while(i<size) {
                jx_ob cond = ((size>1) ? jx_code_eval_tls(tls, flags, node, expr_vla[i]) : 
                              jx_ob_from_null());
                jx_bool cond_bool = jx_ob_as_bool(cond);
                jx_ob_free(cond);
                i+=2;
                if(cond_bool) {
                  return ((size>2) ? jx_code_exec_tls(tls,flags, node, expr_vla[i-1]) :
                          jx_ob_from_null());
                } else if(size == i) {
                  break;
                } else if(size == i+1) {
                  return jx_code_exec_tls(tls,flags, node, expr_vla[i]);
                }
              }
              return jx_ob_from_null();
            }
            break;
          case JX_SELECTOR_TEST: /* [*test* expr expr expr] */
            {
              jx_ob cond = (size>1) ? jx_code_eval_tls(tls, flags, node, expr_vla[1]) : 
                jx_ob_from_null();
              jx_bool cond_bool = jx_ob_as_bool(cond);
              jx_ob_free(cond);
              if(cond_bool) {
                return (size>2) ? jx_code_eval_tls(tls,flags,node, expr_vla[2]) : 
                  jx_ob_from_null();
              } else {
                return (size>3) ? jx_code_eval_tls(tls,flags,node, expr_vla[3]) : 
                  jx_ob_from_null();
              }
            }
            break;
          case JX_SELECTOR_RESOLVE: /* [*resolve* ident] */
            {
              jx_ob symbol = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob result;
              //              jx_jxon_dump(stdout,"resolve symbol",symbol);
              //              jx_jxon_dump(stdout,"resolve node",node);
              if(jx_hash_peek(&result, node, symbol)) {
                /* looked-up objects are always returned as weak references...
                   it's EVAL's job to make sure they don't leak into end-user code */
                jx_ob weak = jx_ob_take_weak_ref(result);
                //                printf("returning possible weak ref\n");
                return weak;
              } else {
                return jx_tls_ob_copy(tls,symbol );
              }
            }
            break;
          case JX_SELECTOR_DEF:   /* [def name node code] */
          case JX_SELECTOR_DEFUN:  /* [defun name node expr] */
            {
              jx_ob name = (size > 1) ? jx_tls_ob_copy(tls,expr_vla[1]) : jx_ob_from_null();
              jx_ob args = (size > 2) ? jx_tls_ob_copy(tls,expr_vla[2]) : jx_ob_from_null();
              jx_ob body = (size > 3) ? jx_tls_ob_copy(tls,expr_vla[3]) : jx_ob_from_null();
              jx_ob key = jx_tls_ob_copy(tls,name);
              jx_ob function = jx_function_new_with_def
                (name, args, body, expr_vla->data.io.int_ == JX_SELECTOR_DEF);
              if(!jx_ok( jx_hash_set(node,key,function))) {
                jx_ob_free(key);
                jx_ob_free(function);
              }
              return jx_ob_from_null();
            }
            break;
          case JX_SELECTOR_CODE: /* [code args code] */
          case JX_SELECTOR_LAMBDA: /* [expr args expr] */
            {
              jx_ob name = JX_OB_NULL;
              jx_ob args = (size > 1) ? jx_tls_ob_copy(tls,expr_vla[1]) : jx_ob_from_null();
              jx_ob body = (size > 2) ? jx_tls_ob_copy(tls,expr_vla[2]) : jx_ob_from_null();
              return jx_function_new_with_def
                (name,args,body,expr_vla->data.io.int_ == JX_SELECTOR_CODE);
            }
            break;
          case JX_SELECTOR_DEFMAC:  /* [defmac name node expr] */
            {
              jx_ob name = (size > 1) ? jx_tls_ob_copy(tls,expr_vla[1]) : jx_ob_from_null();
              jx_ob args = (size > 2) ? jx_tls_ob_copy(tls,expr_vla[2]) : jx_ob_from_null();
              jx_ob body = (size > 3) ? jx_tls_ob_copy(tls,expr_vla[3]) : jx_ob_from_null();
              jx_ob key = jx_tls_ob_copy(tls,name);
              jx_ob function = jx_macro_new_with_def
                (name, args, body );
              //jx_ob_dump(stdout,"macro",function);
              if(!jx_ok( jx_hash_set(node,key,function))) {
                jx_ob_free(key);
                jx_ob_free(function);
              }
              return jx_ob_from_null();
            }
            break;
          case JX_SELECTOR_DISPATCH: /* [dispatch index [callable0 ...] payload ] */
            {
              jx_ob index = (size > 1) ?  jx_code_eval_tls(tls, flags, node, expr_vla[1]) :
                jx_ob_from_null();
              jx_ob table = (size > 2) ? 
                jx_code_eval_allow_weak(tls,flags | JX_EVAL_ALLOW_NESTED_WEAK_REFS,
                                        node, expr_vla[2]) :
                jx_ob_from_null();
              jx_ob payload = (size > 3) ? jx_code_eval_tls(tls, flags, node, expr_vla[3]) :
                jx_ob_from_null();
              //              jx_jxon_dump(stdout,"table",table);
              jx_ob callable = jx_list_swap_with_null(table,jx_ob_as_int(index));
              jx_ob result = JX_OB_NULL;
              if(jx_builtin_callable_check(callable)) {
              } else {
                jx_ob_free(callable);
                jx_tls_ob_free(tls,payload);
              }
              jx_tls_ob_free(tls,index);
              jx_tls_ob_free(tls,table);
              return result;
            }
            break;
          case JX_SELECTOR_SWITCH:  /* [switch index [code0 code1 code2]] */
            {
              jx_ob index = (size > 1) ?  jx_code_eval_tls(tls, flags, node, expr_vla[1]) :
                jx_ob_from_null();
              jx_ob table = (size > 2) ? expr_vla[2] : jx_ob_from_null();
              jx_int idx = jx_ob_as_int(index);
              jx_ob_free(index);
              return jx_code_exec_tls(tls,flags,node, jx_list_borrow(table,idx));
            }
            break;
          case JX_SELECTOR_SELECT:  /* [select index [expr0 expr1 expr2]] */
            {
              jx_ob index = (size > 1) ?  jx_code_eval_tls(tls, flags, node, expr_vla[1]) :
                jx_ob_from_null();
              jx_ob table = (size > 2) ? expr_vla[2] : jx_ob_from_null();
              jx_int idx = jx_ob_as_int(index);
              jx_ob_free(index);
              return jx_code_eval_tls(tls, flags, node, jx_list_borrow(table,idx));
            }
            break;
          case JX_SELECTOR_WHILE:
            {
              jx_ob cond = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob body = (size>2) ? expr_vla[2] : jx_ob_from_null();
              jx_ob result = JX_OB_NULL;
              while(1) {
                jx_ob test = jx_code_eval_allow_weak(tls, flags, node, cond); 
                jx_int tst = jx_ob_as_bool(test);
                jx_ob_free(test);
                if(tst) {
                  jx_ob_replace(&result, jx_code_exec_tls(tls,flags,node, body));
                } else
                  break;
              }
              return result;
            }
            break;
          case JX_SELECTOR_DO:
            {
              jx_ob body = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob cond = (size>2) ? expr_vla[2] : jx_ob_from_null();
              jx_ob result = JX_OB_NULL;
              jx_int tst = 0;
              do {
                jx_ob_replace(&result, jx_code_exec_tls(tls,flags,node, body));
                jx_ob test = jx_code_eval_tls(tls, flags, node, cond);
                tst = jx_ob_as_bool(test);
                jx_ob_free(test);
              } while(tst);
            }
            break;
          case JX_SELECTOR_FOR:
            {
              jx_ob init = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob cond = (size>2) ? expr_vla[2] : jx_ob_from_null();
              jx_ob step = (size>3) ? expr_vla[3] : jx_ob_from_null();
              jx_ob body = (size>4) ? expr_vla[4] : jx_ob_from_null();
              jx_ob result = JX_OB_NULL;
              jx_int tst = JX_TRUE;
              jx_ob_free(jx_code_eval_tls(tls, flags, node, init));
              while(tst) {
                jx_ob test = jx_code_eval_tls(tls, flags, node, cond);
                tst = jx_ob_as_bool(test);
                jx_ob_free(test);
                if(tst) {
                  jx_ob_replace(&result, jx_code_exec_tls(tls,flags,node, body));
                }
                jx_ob_free(jx_code_eval_tls(tls, flags, node, step));
              }
              return result;
            }
            break;
          case JX_SELECTOR_FOREACH:
            {
              jx_ob expr = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob list = (size>2) ? 
                jx_code_eval_allow_weak(tls,flags,node, expr_vla[2]) :
                jx_ob_from_null();
              jx_ob body = (size>3) ? expr_vla[3] : jx_ob_from_null();
              jx_ob result = JX_OB_NULL;
              jx_list *inp_list = list.data.io.list;
              jx_int i,size = jx_list_size(list);
              if(size) {
                if(!inp_list->packed_meta_bits) { /* normal object input */
                  jx_ob *ob = inp_list->data.ob_vla;
                  for(i=0;i<size;i++) {
                    jx_hash_set(node, expr, *(ob++));
                    jx_ob_replace(&result, jx_code_exec_tls(tls,flags,node,body));
                  } 
                } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) { 
                    /* packed integer input */
                  jx_ob ob = JX_OB_INT;
                  jx_int *inp_int = inp_list->data.int_vla;
                  for(i=0;i<size;i++) {
                    ob.data.io.int_ = *(inp_int++);
                    jx_hash_set(node, expr, ob);
                    jx_ob_replace(&result, jx_code_exec_tls(tls,flags,node,body));
                  }
                } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) { 
                  jx_ob ob = JX_OB_FLOAT;
                  jx_float *inp_float = inp_list->data.float_vla;
                  for(i=0;i<size;i++) {
                    ob.data.io.float_ = *(inp_float++);
                    jx_hash_set(node, expr, ob);
                    jx_ob_replace(&result, jx_code_exec_tls(tls,flags,node,body));
                  }
                }
                jx_vla_reset(&inp_list->data.ob_vla);
                jx_tls_ob_free(tls,list);
              }
              return result;
            }
            break;
          case JX_SELECTOR_APPLY: /* [apply callable payload] */
            {
              jx_ob callable = (size>1) ? jx_code_eval_allow_weak
                (tls, flags, node, expr_vla[1]) : jx_ob_from_null();
              jx_ob payload = (size>2) ? jx_code_eval_tls(tls, flags, node, expr_vla[2]) :
                jx_ob_from_null();
              if(jx_builtin_callable_check(callable)) {
                return jx__code_apply_callable(tls, node, callable, payload);
              } else {   
                jx_ob_free(callable);
                jx_tls_ob_free(tls,payload);
                return jx_ob_from_null();
              }
            }
            break;
          case JX_SELECTOR_MAP: /* [map callable list] */
            {
              jx_ob callable = (size>1) ? jx_code_eval_allow_weak
                (tls, flags, node, expr_vla[1]) : jx_ob_from_null();
              if(size > 2) {
                if(size == 3) {
                  jx_ob src_list = (size>2) ? jx_code_eval_allow_weak
                    (tls, flags, node, expr_vla[2]) : 
                    jx_ob_from_null();
                  return jx__code_map1(tls,flags,node,callable,src_list);
                } else { 
                  /* zipping while mapping -- slower */
                  jx_int i,n = size - 2;
                  jx_ob src_list = jx_tls_list_new_with_size(tls,n);
                  for(i=0;i<n;i++) {
                    jx_list_replace(src_list, i, jx_code_eval_allow_weak
                                    (tls, flags, node, expr_vla[i+2]));
                  }
                  return jx__code_mapN(tls,flags,node,callable,src_list);
                }
              }
            }
            return jx_ob_from_null();
            break;
          case JX_SELECTOR_REDUCE: /* [reduce callable list] */
            {
              jx_ob callable = (size>1) ? jx_code_eval_allow_weak
                (tls, flags, node, expr_vla[1]) : jx_ob_from_null();
              if(size > 2) {
                if(size == 3) {
                  jx_ob src_list = (size>2) ? jx_code_eval_allow_weak
                    (tls, flags, node, expr_vla[2]) : 
                    jx_ob_from_null();
                  return jx__code_reduce(tls,flags,node,callable,src_list);
                }
              }
            }
            return jx_ob_from_null();
            break;
          }
          return jx_ob_from_null();
        } else { /* not a special form, so continue...*/
          jx_int i,size = jx__list_size(expr_list);
          jx_ob result = jx_tls_list_new_with_size(tls,size);
          //jx_jxon_dump(stdout,"not special",expr);
          if(!result.meta.bits & JX_META_BIT_LIST) {
            /* memory exhausted? */
            return result;
          } else {
            /* first we evaluate all containers */
            //jx_jxon_dump(stdout,"pre-eval",expr);
            jx_list *result_list = result.data.io.list;
            jx_ob *expr_ob = expr_list->data.ob_vla;
            jx_ob *result_vla = result_list->data.ob_vla;
            jx_ob *result_ob = result_vla;
            jx_bool macro_flag = JX_FALSE;
            for(i=0;i<size;i++) {
              if((!macro_flag) && (expr_ob->meta.bits & (JX_META_BIT_LIST|JX_META_BIT_HASH))) {
                *(result_ob++) = jx_code_eval_allow_weak(tls, flags, node, *expr_ob);
              } else { /* not a container or we're processing a macro */
                *(result_ob++) = jx_tls_ob_copy(tls,*expr_ob);
              }
              expr_ob++;
              if( (!(i||macro_flag)) && ((result_ob[-1].meta.bits & 
                            (JX_META_BIT_BUILTIN|JX_META_BIT_BUILTIN_MACRO))==
                           (JX_META_BIT_BUILTIN|JX_META_BIT_BUILTIN_MACRO))) {
                macro_flag = JX_TRUE;
              }
            }
            if(macro_flag) {
              jx_ob macro = jx__list_remove(result_list,0);
              // printf("expand macro\n");
              return jx__macro_call(tls,node,macro,result);
            }

            /* WARNING / REMINDER: we may now have some weak
               references at the root level of the result list --
               these will need to be eliminated or made strong (deep
               copied) before we return the frame to the caller */

            //jx_jxon_dump(stdout,"# post-eval",result);
            //printf("flags %d\n",flags);

            if(flags & JX_EVAL_DEFER_INVOCATION) { /* expanding a macro... */
              return result;
            } else if(!jx_builtin_callable_check(result_vla[0])) {
              /* first entry in list is not callable...but it is an Jenaric Entity? */
              if(jx_list_check(result_vla[0])) {
                jx_int entity_size;
                jx_list *list = result_vla->data.io.list;
                if((!list->packed_meta_bits) && 
                   (entity_size = jx_vla_size(&list->data))) { 
                  jx_ob *entity_ob = list->data.ob_vla;
                  /* non-zero, unpacked => vla valid */
                  if(jx_builtin_entity_check(entity_ob[0])) {
                    /* first entry in result contains an entity (a class / instance, etc.) */
                    if(size>1) {
                      /* the entry following the entity -- what is it? */
                      switch(result_vla[1].meta.bits & JX_META_MASK_TYPE_BITS) {
                      case 0: /* null method == copy constructor? */
                        break;
                      case JX_META_BIT_IDENT: /* identifier? -> standard method resolution */
                        if(entity_size>1) {
                          jx_ob method_name = result_vla[1];
                          /* first we look in the member */
                          jx_ob method = jx_hash_get(entity_ob[2],method_name);
                          if(jx_null_check(method)) {
                            /* no luck? then we consult the node's
                               namespace table using the entity object
                               as the namespace key */
                            method = jx_hash_get( jx_hash_borrow(node,entity_ob[0]),
                                                  method_name );
                          }
                          if(jx_builtin_callable_check(method)) {  /* hooray! method bound! */
                            jx__list_replace(result_list, 0, jx__list_remove(result_list, 0));
                            size--;
                            switch(method.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
                            case JX_META_BIT_BUILTIN_NATIVE_FN:
                              {
                                jx_native_fn native_fn = method.data.io.native_fn;
                                if(native_fn) {
                                  result = native_fn(node, result); 
                                  jx_ob_free(method);
                                  return result;
                                } else {
                                  jx_tls_ob_free(tls,result);
                                  return jx_ob_from_null();
                                }
                              }
                              break;
                            case JX_META_BIT_BUILTIN_FUNCTION:
                              return jx__function_call(tls, node, method, result);
                              break;
                            case JX_META_BIT_BUILTIN_SELECTOR:
                              return jx__code_apply_callable(tls, node, method, result);
                              break;
                            default:
                              jx__code_make_strong(result_list->data.ob_vla + 1,size - 1);
                              return jx__code_apply_callable(tls, node, method, result);
                              break;
                            }
                          } 
                        }
                        break;
                      case JX_META_BIT_INT: /* entity access (0=ident,1=content,2=attr) */
                        {
                          jx_int index = jx_ob_as_int(result_vla[1]);
                          if((index<entity_size)&&(index>=0)) {
                            jx_ob member = entity_ob[index];
                            jx_ob_free(result);
                            return jx_ob_take_weak_ref(member); /* is thie right??? */
                          } else { /* invalid accessor */
                            jx_ob_free(result);
                            return jx_ob_from_null();
                          }
                        }
                        break;
                      default: /* undefined case -- return null */
                        jx_ob_free(result);
                        return jx_ob_from_null();
                        break;
                      }
                    }
                  }
                }
              }
              /* strengthen result list unless weak-nesting is permitted */
              if(!(flags & JX_EVAL_ALLOW_NESTED_WEAK_REFS)) {
                jx__code_make_strong(result_vla,size);
              }
              /* return the evaluated value */
              return result;
            } else { /* known callable */
              switch(result_vla->meta.bits & JX_META_MASK_BUILTIN_TYPE) {
              case JX_META_BIT_BUILTIN_NATIVE_FN:
                {
                  jx_native_fn native_fn = result_vla->data.io.native_fn;
                  //                  jx_jxon_dump(stdin,"result",result);
                  if(native_fn) {
                    jx_ob function = jx__list_remove(result_list, 0); /* strip function pointer */
                    result = native_fn(node, result); 
                    //printf("got here\n");
                    jx_ob_free(function);       
                    return result;
                  } else {
                    jx_tls_ob_free(tls,result);
                    return jx_ob_from_null();
                  }
                }
                break;
              case JX_META_BIT_BUILTIN_FUNCTION:
                {
                  jx_ob fn = jx__list_remove(result_list,0);
                  return jx__function_call(tls, node, fn, result);
                }
                break;
              case JX_META_BIT_BUILTIN_SELECTOR:
                /* builtin safe functions do not consume their inputs  */
                {
                  jx_ob callable = jx__list_remove(result_list,0);
                  return jx__code_apply_callable(tls, node, callable, result);
                }
                break;
              default:
                jx__code_make_strong(result_list->data.ob_vla + 1,size - 1);
                {
                  jx_ob callable = jx__list_remove(result_list,0);
                  return jx__code_apply_callable(tls, node, callable, result);
                }
                break;
              }
            }
          }
        }
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
        jx_hash_set(result, key, jx_code_eval_tls(tls, flags, node, value));
        jx_tls_ob_free(tls,value);
      }
      jx_tls_ob_free(tls,list);
      return result;
    }
    break;
  default:
    return jx_tls_ob_copy(tls,expr);
    break;
  }
}

static jx_ob jx__code_exec_allow_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob code)
{
  /* on entry, code is known to be a list.
     if code is [ fn, ...] then evaluate & return result
     if code is [ [fn, ...], [fn, ...], ... ] then
     evaluate all members of a list, returning only the last result */
  jx_ob result = JX_OB_NULL;
  jx_int pc, size = jx_list_size(code);
  if(size) {
    jx_list *code_list = code.data.io.list;
    jx_ob inst = jx__list_borrow(code_list, 0);
    /* code = [ *fn*, ...] */
    if(jx_builtin_callable_check(inst)) { /* code block with only a single instruction */
      jx_ob_replace(&result, 
                    jx_code_eval_allow_weak(tls,flags, node, code));
    } else {
      /* code = [[ ... ]  .... ] */
      for(pc = 0; pc < size; pc++) { /* standard code block */
        if(pc)
          inst = jx__list_borrow(code_list, pc);
        jx_ob_replace(&result, jx_code_eval_allow_weak(tls,flags, node, inst));
      }
    }
  }
  return result;
}

jx_ob jx__code_eval(jx_tls *tls, jx_int flags, jx_ob node, jx_ob code)
{
  //  jx_jxon_dump(stdout,"jx__code_eval entered",code);
  if(tls) {
    return jx_ob_not_weak_with_ob(jx__code_eval_allow_weak(tls,flags,node,code));
  } else {
    jx_ob result;
    jx_tls *tls = jx_tls_new();
    result = jx_ob_not_weak_with_ob(jx__code_eval_allow_weak(tls,flags,node,code));    
    jx_tls_free(tls);
    return result;
  }
}

jx_ob jx__code_exec(jx_tls *tls, jx_int flags, jx_ob node, jx_ob code)
{ 
  //  jx_jxon_dump(stdout,"jx__code_exec entered with code",code);
  //  jx_jxon_dump(stdout,"                       and node",node);
  jx_ob result;
  if(tls) {
    result = jx_ob_not_weak_with_ob(jx__code_exec_allow_weak(tls,flags,node,code));
  } else {
    jx_tls *tls = jx_tls_new();
    result = jx_ob_not_weak_with_ob(jx__code_exec_allow_weak(tls,flags,node,code));
    jx_tls_free(tls);
  }
  //  jx_jxon_dump(stdout,"jx__code_exec existing with result",result);
  return result;
}
                  
