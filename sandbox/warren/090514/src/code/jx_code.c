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

#define JX_BUILTIN_IF     1
#define JX_BUILTIN_WHILE  2
#define JX_BUILTIN_DO     3
#define JX_BUILTIN_FOR    4
#define JX_BUILTIN_QUOTE  5

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

  ok = jx_declare(ok, namespace, "if", JX_BUILTIN_IF);
  ok = jx_declare(ok, namespace, "while", JX_BUILTIN_WHILE);
  ok = jx_declare(ok, namespace, "do", JX_BUILTIN_DO);
  ok = jx_declare(ok, namespace, "for", JX_BUILTIN_FOR);
  ok = jx_declare(ok, namespace, "quote", JX_BUILTIN_QUOTE);

  return ok ? JX_SUCCESS : JX_FAILURE;
}

jx_status jx_code_expose_secure_builtins(jx_ob namespace)
{
  jx_bool ok = JX_TRUE;

  ok = ok && jx_ok(jx_safe_expose_all_builtins(namespace));
  ok = ok && jx_ok(jx_code_expose_special_forms(namespace));

  return ok ? JX_SUCCESS : JX_FAILURE;
}

/* consume source & return builtin-bound executable */

jx_ob jx_code_bind_with_source(jx_ob namespace, jx_ob source)
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
      if(jx_ident_check(ident)) {
        jx_ob builtin = jx_hash_borrow(namespace, ident);
        if(jx_builtin_check(builtin)) {
          /* wrap builtin invocation around this list */
          result = jx_list_new_with_size(2);
          jx_list_replace(result, 0, builtin);
          jx_list_replace(result, 1, list);
        }
      }
      /* now process the source list */
      while(size--) {
        jx_list_replace(list, size,
                        jx_code_bind_with_source(namespace,
                                                 jx_list_remove(source, size)));
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
        jx_hash_set(result, key, jx_code_bind_with_source(namespace, value));
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
        if(jx_builtin_fn_check(builtin)) {
          jx_bits bits = builtin.meta.bits;
          if(bits & JX_META_BIT_BUILTIN_SELECTOR) {
            if(builtin.data.io.int_ < JX_BUILTIN_SPECIAL_FORMS_LIMIT) {
              return jx_code_exec(node, expr);
            } else {
              jx_ob payload = jx_code_eval(node, jx__list_borrow(expr_list, 1));
              switch (builtin.data.io.int_) {
              case JX_BUILTIN_SET:
                result = jx_safe_set(node, payload);
                break;
              case JX_BUILTIN_GET:
                result = jx_safe_get(node, payload);
                break;
              case JX_BUILTIN_BORROW:
                result = jx_safe_borrow(node, payload);
                break;
              case JX_BUILTIN_TAKE:
                result = jx_safe_take(node, payload);
                break;
              case JX_BUILTIN_DEL:
                result = jx_safe_del(node, payload);
                break;

              case JX_BUILTIN_SIZE:
                result = jx_safe_size(node, payload);
                break;
              case JX_BUILTIN_HASH_SET:
                result = jx_safe_hash_set(node, payload);
                break;
              case JX_BUILTIN_HASH_GET:
                result = jx_safe_hash_get(node, payload);
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

              case JX_BUILTIN_APPEND:
                result = jx_safe_append(node, payload);
                break;

              case JX_BUILTIN_OUTPUT:
                result = jx_safe_output(node, payload);
                break;
              case JX_BUILTIN_ERROR:
                result = jx_safe_error(node, payload);
                break;
              }
              jx_ob_free(payload);
            }
          } else if(bits & JX_META_BIT_BUILTIN_NATIVE_FN) {
            jx_ob payload = jx_code_eval(node, jx__list_borrow(expr_list, 1));
            jx_native_fn fn = builtin.data.io.native_fn;
            if(fn) {
              return fn(node, payload); /* consumes payload */
            }
          } else {
            /* jenarix-based fn */
          }
        } else {
          jx_int size = jx__list_size(expr_list);
          jx_int i = 0;
          result = jx_list_new_with_size(size);
          if(result.meta.bits & JX_META_BIT_LIST) {
            jx_list *result_list = result.data.io.list;
            while(i < size) {
              jx_ob_replace(result_list->data.ob_vla + i,
                            jx_code_eval(node, jx__list_borrow(expr_list, i)));
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
        jx_hash_set(result, key, jx_code_eval(node, value));
        jx_ob_free(value);
      }
      jx_ob_free(list);
      return result;
    }
    break;
  default:                     /* when in doubt... */
    return jx_ob_copy(expr);
    break;
  }
}

jx_ob jx__code_exec(jx_ob node, jx_ob code)
{
  /* execute code: should be either 
     [builtin_fn, [...]] or [ [builtin_fn,[...]], [builtin_fn,[...]], ... ] */

  if(!jx_list_check(code))
    return jx_ob_copy(code);
  else {
    jx_ob result = JX_OB_NULL;
    jx_int pc, size = jx_list_size(code);
    jx_list *code_list = code.data.io.list;
    for(pc = 0; pc < size; pc++) {
      jx_ob inst = jx__list_borrow(code_list, pc);
      jx_list *inst_list = NULL;
      jx_ob builtin;
      if(jx_list_check(inst)) { /* list with list of instructions */
        inst_list = inst.data.io.list;
        builtin = jx__list_borrow(inst_list, 0);
      } else {                  /* list with a single instruction */
        builtin = jx__list_borrow(code_list, 0);
        if(jx_builtin_check(builtin)) {
          inst = code;
          inst_list = code_list;
          pc = size;
        }
      }
      if((builtin.meta.bits & (JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR)) ==
         (JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR)) {
        switch (builtin.data.io.int_) {
          /* ALL SPECIAL FORMS MUST APPEAR HERE */
        case JX_BUILTIN_WHILE:
          {
            jx_ob payload = jx__list_borrow(inst_list, 1);
            jx_list *payload_list = payload.data.io.list;
            jx_ob cond = jx__list_borrow(payload_list, 1);
            jx_ob body = jx__list_borrow(payload_list, 2);
            while(1) {
              jx_ob test = jx_code_eval(node, cond);
              if(jx_ob_as_bool(test)) {
                jx_ob_free(test);
                jx_ob_replace(&result, jx_code_exec(node, body));
              } else {
                jx_ob_free(test);
                break;
              }
            }
          }
          break;
        case JX_BUILTIN_DO:
          {
            jx_ob payload = jx__list_borrow(inst_list, 1);
            jx_list *payload_list = payload.data.io.list;
            jx_ob body = jx__list_borrow(payload_list, 1);
            jx_ob cond = jx__list_borrow(payload_list, 2);
            jx_bool keep_going = JX_TRUE;
            while(keep_going) {
              jx_ob_replace(&result, jx_code_exec(node, body));
              jx_ob test = jx_code_eval(node, cond);
              keep_going = jx_ob_as_bool(test);
              jx_ob_free(test);
            }
          }
          break;
        case JX_BUILTIN_IF:
          {
            jx_ob payload = jx__list_borrow(inst_list, 1);
            jx_list *payload_list = payload.data.io.list;
            jx_ob cond = jx_code_eval(node, jx__list_borrow(payload_list, 1));
            if(jx_ob_as_bool(cond)) {
              jx_ob_replace(&result,
                            jx_code_exec(node, jx__list_borrow(payload_list, 2)));
            } else {
              jx_ob_replace(&result,
                            jx_code_exec(node, jx__list_borrow(payload_list, 3)));
            }
            jx_ob_free(cond);
          }
          break;
        case JX_BUILTIN_FOR:   /* [for x [get x] [add x 1] [get x]] */
          {
            jx_ob payload = jx__list_borrow(inst_list, 1);
            jx_list *payload_list = payload.data.io.list;
            jx_ob cond = jx__list_borrow(payload_list, 2);
            jx_ob step = jx__list_borrow(payload_list, 3);
            jx_ob body = jx__list_borrow(payload_list, 4);
            jx_ob_free(jx_code_eval(node, jx__list_borrow(payload_list, 1)));

            while(1) {
              jx_ob test = jx_code_eval(node, cond);
              if(jx_ob_as_bool(test)) {
                jx_ob_free(test);
                jx_ob_replace(&result, jx_code_exec(node, body));
              } else {
                jx_ob_free(test);
                break;
              }
              jx_ob_free(jx_code_eval(node, step));
            }
          }
          break;
        case JX_BUILTIN_QUOTE:
          jx_ob_replace(&result, jx_ob_copy(jx__list_borrow(inst_list, 1)));
          break;
        default:
          /* OTHERWISE, THE INSTRUCTION FALLS THROUGH TO CONVENTIONAL EVALUATION */
          jx_ob_replace(&result, jx_code_eval(node, inst));
          break;
        }
      } else {
        jx_ob_replace(&result, jx_code_eval(node, inst));
      }
    }
    return result;
  }
}
