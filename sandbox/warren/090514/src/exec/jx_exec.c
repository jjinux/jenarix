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

#define JX_BUILTIN_SET    0
#define JX_BUILTIN_GET    1
#define JX_BUILTIN_ADD    2
#define JX_BUILTIN_WHILE  3
#define JX_BUILTIN_SUB    4
#define JX_BUILTIN_BORROW 5
#define JX_BUILTIN_APPEND 6

jx_status jx_expose_builtins(jx_ob namespace)
{
  jx_hash_set(namespace, jx_ob_from_ident("set"), 
              jx_builtin_new_from_selector(JX_BUILTIN_SET));
  jx_hash_set(namespace, jx_ob_from_ident("get"), 
              jx_builtin_new_from_selector(JX_BUILTIN_GET));
  jx_hash_set(namespace, jx_ob_from_ident("add"), 
              jx_builtin_new_from_selector(JX_BUILTIN_ADD));
  jx_hash_set(namespace, jx_ob_from_ident("while"), 
              jx_builtin_new_from_selector(JX_BUILTIN_WHILE));
  jx_hash_set(namespace, jx_ob_from_ident("sub"), 
              jx_builtin_new_from_selector(JX_BUILTIN_SUB));
  jx_hash_set(namespace, jx_ob_from_ident("borrow"), 
              jx_builtin_new_from_selector(JX_BUILTIN_BORROW));
  jx_hash_set(namespace, jx_ob_from_ident("append"), 
              jx_builtin_new_from_selector(JX_BUILTIN_APPEND));
  return JX_SUCCESS;
}

/* consume source & return builtin-bound executable */

jx_ob jx_bind(jx_ob namespace, jx_ob source)
{
  switch(JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else {
      /* [ indentifer, ...] becomes [ builtin, [identifier, ...]] */
      jx_int size = jx_list_size(source);
      jx_ob result = jx_list_new_with_size(size);
      jx_ob list = result;
      jx_ob ident = jx_list_borrow(source,0);
      if(jx_ident_check(ident)) {
        jx_ob builtin = jx_hash_borrow(namespace,ident);
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
                        jx_bind( namespace, jx_list_remove(source, size)));
      }
      jx_ob_free(source);
      return result;
    }
    break;
  case JX_META_BIT_HASH:
    {
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_with_hash(source); /* destroys source */
      jx_int size = jx_list_size(list);
      while(size--) {
        jx_ob value = jx_list_remove(list, size--);
        jx_ob key = jx_list_remove(list, size);
        jx_hash_set(result, key, jx_bind(namespace, value));
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
  default: /* unrecognized */
    jx_ob_free(source);
    return jx_ob_from_null();
    break;
  }
}

static jx_ob jx_eval(jx_ob node, jx_ob expr)
{
  jx_bits bits = expr.meta.bits;
  if(!(JX_META_BIT_GC & bits))
    return expr;
  else {
    jx_ob result = JX_OB_NULL;
    switch(bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(expr.data.io.list->packed_meta_bits) { /* packed data cannot contain code */
        return jx_ob_copy(expr);
      } else {
        jx_ob builtin = jx_list_borrow(expr, 0);
        if(jx_builtin_check(builtin)) {
          jx_ob payload = jx_eval(node, jx_list_borrow(expr,1));
          if(builtin.meta.bits & JX_META_BIT_GC) {
            /* native builtin function */
            jx_builtin fn = builtin.data.io.builtin;
            if(fn) {
              return fn(node, payload); /* consumes payload */
            }
          } else {
            switch(builtin.data.io.int_) {
            case JX_BUILTIN_SET:
              {
                jx_ob value = jx_list_remove(payload,2);
                jx_ob key = jx_list_remove(payload,1);
                result = jx_ob_from_status( jx_hash_set(node, key, value ) );
              }
              break;
            case JX_BUILTIN_GET:
              result = jx_hash_get(node, jx_list_borrow(payload,1));
              break;
            case JX_BUILTIN_BORROW:
              result = jx_ob_take_weak_ref( jx_hash_borrow(node, jx_list_borrow(payload,1)));
              break;
            case JX_BUILTIN_ADD:
              result = jx_add( jx_list_borrow(payload,1), jx_list_borrow(payload,2) );
              break;
            case JX_BUILTIN_SUB:
              result = jx_sub( jx_list_borrow(payload,1), jx_list_borrow(payload,2) );
              break;
            case JX_BUILTIN_APPEND:
              {
                jx_ob item = jx_list_remove(payload,2);
                jx_ob list = jx_list_borrow(payload,1);
                result = jx_ob_from_status( jx_list_append(list, item) );
              }
              break;
            }
            jx_ob_free(payload);
          }
        } else {
          jx_int size = jx_list_size(expr);
          jx_int i = 0;
          result = jx_list_new_with_size(size);
          /* now process the source list */
          while(i<size) {
            jx_list_replace(result, i, jx_eval(node, jx_list_borrow(expr, i)));
            i++;
          }
        }
      }
      break;
    case JX_META_BIT_HASH:
      result = jx_hash_new();
      {
        jx_ob list = jx_list_new_from_hash(expr);
        jx_int size = jx_list_size(list);
        while(size--) {
          jx_ob value = jx_list_remove(list, size--);
          jx_ob key = jx_list_remove(list, size);
          jx_hash_set(result, key, jx_eval(node,value));
          jx_ob_free(value);
        }
        jx_ob_free(list);
      }
      break;
    }
    return result;
  }
}

jx_ob jx_exec(jx_ob node, jx_ob code)
{
  /* executable code: [ [builtin,[...]], [builtin,[...]], ... ] */
  jx_ob result = JX_OB_NULL;
  jx_int pc, size = jx_list_size(code);
  for(pc=0;pc<size;pc++) {
    jx_ob inst = jx_list_borrow(code,pc);
    jx_ob builtin = jx_list_borrow(inst, 0);
    if(jx_builtin_check(builtin) && !(builtin.meta.bits & JX_META_BIT_GC)) {
      switch(builtin.data.io.int_) {
      case JX_BUILTIN_WHILE:
        {
          jx_ob payload = jx_list_borrow(inst,1);
          while(1) {
            jx_ob cond = jx_eval(node, jx_list_borrow(payload,1));
            if( jx_ob_as_int(cond) ) {
              jx_ob_free(cond);
              jx_ob_free(result);
              result = jx_exec(node, jx_list_borrow(payload,2));
            } else {
              jx_ob_free(cond);
              break;
            }
          }
        }
        break;
      default:
        jx_ob_free(result);
        result = jx_eval(node, inst);
        break;
      }
    } else {
      jx_ob_free(result);
      result = jx_eval(node, inst);
    }
  }
  return result;
}


