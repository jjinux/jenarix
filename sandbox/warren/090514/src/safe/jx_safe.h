#ifndef _H_jx_safe
#define _H_jx_safe

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

#include "jx_private.h"

/* the first 512 selectors are reserved for "safe" functions which
   involve only internal actions within the interpreter and container
   space (no access to filesystem, network, memory, etc.) */

/* symbols */

#define JX_BUILTIN_SET         17
#define JX_BUILTIN_GET         18
#define JX_BUILTIN_BORROW      19
#define JX_BUILTIN_TAKE        20
#define JX_BUILTIN_DEL         21

#define JX_BUILTIN_IDENTICAL   22
#define JX_BUILTIN_EQ          23
#define JX_BUILTIN_LT          24
#define JX_BUILTIN_GT          25
#define JX_BUILTIN_LE          26
#define JX_BUILTIN_GE          27
#define JX_BUILTIN_NE          28
#define JX_BUILTIN_AND         29      
#define JX_BUILTIN_OR          30
#define JX_BUILTIN_BIT_AND     31
#define JX_BUILTIN_BIT_OR      32
#define JX_BUILTIN_BIT_XOR     33
#define JX_BUILTIN_BIT_SHL     34
#define JX_BUILTIN_BIT_SHR     35

#define JX_BUILTIN_ADD         36
#define JX_BUILTIN_SUB         37
#define JX_BUILTIN_MUL         38
#define JX_BUILTIN_DIV         39
#define JX_BUILTIN_MOD         40

#define JX_BUILTIN_BIT_NOT     41
#define JX_BUILTIN_NOT         42
#define JX_BUILTIN_NEG         43

#define JX_BUILTIN_OUTPUT      50
#define JX_BUILTIN_ERROR       51

#define JX_BUILTIN_SIZE        52
#define JX_BUILTIN_HASH_SET    53
#define JX_BUILTIN_HASH_GET    54
#define JX_BUILTIN_LIST_GET    55
#define JX_BUILTIN_APPEND      56
#define JX_BUILTIN_REPLACE     57
#define JX_BUILTIN_INSERT      58
#define JX_BUILTIN_LIST_REMOVE 59
#define JX_BUILTIN_LIST_DELETE 60
#define JX_BUILTIN_RESIZE      61

#define JX_BUILTIN_IMPL        62

#define JX_BIN_OP(SUFFIX) \
JX_INLINE jx_ob jx_safe_ ## SUFFIX(jx_ob node, jx_ob payload) \
{ \
  return jx_ob_ ## SUFFIX ( jx_list_borrow(payload,1), jx_list_borrow(payload,2) ); \
}

#define JX_UNI_OP(SUFFIX) \
JX_INLINE jx_ob jx_safe_ ## SUFFIX(jx_ob node, jx_ob payload) \
{ \
  return jx_ob_ ## SUFFIX ( jx_list_borrow(payload,1) );    \
}


jx_status jx_safe_expose_all_builtins(jx_ob namespace);

JX_INLINE jx_ob jx_safe_set(jx_ob node, jx_ob payload)
{
  jx_ob key = jx_ob_strong_with_ob(jx_list_remove(payload,1));
  jx_ob value = jx_ob_strong_with_ob(jx_list_remove(payload,1)); /* tricky */
  return jx_ob_from_status( jx_hash_set(node, key, value ) );
}

JX_INLINE jx_ob jx_safe_get(jx_ob node, jx_ob payload)
{
  return jx_hash_get(node, jx_list_borrow(payload,1));
}

JX_INLINE jx_ob jx_safe_borrow(jx_ob node, jx_ob payload)
{
  return jx_ob_take_weak_ref( jx_hash_borrow(node, jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_take(jx_ob node, jx_ob payload)
{
  return jx_hash_remove(node, jx_list_borrow(payload,1));
}

JX_INLINE jx_ob jx_safe_del(jx_ob node, jx_ob payload)
{
  return jx_ob_from_status(jx_hash_delete(node, jx_list_borrow(payload,1)));
}


JX_INLINE jx_ob jx_safe_identical(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_identical(jx_list_borrow(payload,1),
                                         jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_eq(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_equal(jx_list_borrow(payload,1),
                                     jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_ne(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(!jx_ob_equal(jx_list_borrow(payload,1),
                                      jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_lt(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_lt(jx_list_borrow(payload,1),
                                  jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_gt(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_gt(jx_list_borrow(payload,1),
                                  jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_le(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_le(jx_list_borrow(payload,1),
                                  jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_ge(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_ge(jx_list_borrow(payload,1),
                                  jx_list_borrow(payload,2)));
}


JX_INLINE jx_ob jx_safe_output(jx_ob node, jx_ob payload)
{
  /* should check for 'output' in the node namespace and use that
     instead if defined */
  jx_ob jxon = jx_ob_to_jxon(jx_list_borrow(payload,1));
  fprintf(stdout,"%s;\n",jx_ob_as_str(&jxon));
  jx_ob_free(jxon);
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_error(jx_ob node, jx_ob payload)
{
  /* should check for 'error' in the node namespace and use that
     instead if defined */
  jx_ob jxon = jx_ob_to_jxon(jx_list_borrow(payload,1));
  fprintf(stderr,"%s;\n",jx_ob_as_str(&jxon));
  jx_ob_free(jxon);
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_add(jx_ob node, jx_ob payload)
{
  return jx_ob_add( jx_list_borrow(payload,1), jx_list_borrow(payload,2) );
}

JX_BIN_OP(add)
JX_BIN_OP(sub)
JX_BIN_OP(mul)
JX_BIN_OP(div)
JX_BIN_OP(mod)
JX_BIN_OP(and)
JX_BIN_OP(or)

JX_UNI_OP(neg)
JX_UNI_OP(not)


JX_INLINE jx_ob jx_safe_size(jx_ob node, jx_ob payload)
{
  return jx_ob_size( jx_list_borrow(payload,1) );
}

JX_INLINE jx_ob jx_safe_hash_set(jx_ob node, jx_ob payload)
{
  jx_ob key = jx_ob_strong_with_ob(jx_list_borrow(payload,2));
  jx_ob value = jx_ob_strong_with_ob(jx_list_borrow(payload,3)); 
  return jx_ob_from_status(jx_hash_set(jx_list_borrow(payload,1),
                                       key, value));
}

JX_INLINE jx_ob jx_safe_hash_get(jx_ob node, jx_ob payload)
{
  return jx_hash_get(jx_list_borrow(payload,1),
                     jx_list_borrow(payload,2));
}


JX_INLINE jx_ob jx_safe_list_get(jx_ob node, jx_ob payload)
{
  return jx_list_get(jx_list_borrow(payload,1),
                     jx_ob_as_int(jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_append(jx_ob node, jx_ob payload)
{
  jx_ob value = jx_ob_strong_with_ob(jx_list_borrow(payload,2)); 
  return jx_ob_from_status( jx_list_append(jx_list_borrow(payload,1),
                                           value));
}

JX_INLINE jx_ob jx_safe_insert(jx_ob node, jx_ob payload)
{
  jx_ob value = jx_ob_strong_with_ob(jx_list_borrow(payload,3)); 
  return jx_ob_from_status
    ( jx_list_insert(jx_list_borrow(payload,1),
                     jx_ob_as_int(jx_list_borrow(payload,2)),
                     value));
}

JX_INLINE jx_ob jx_safe_replace(jx_ob node, jx_ob payload)
{
  jx_ob value = jx_ob_strong_with_ob(jx_list_borrow(payload,3)); 
  return jx_ob_from_status
    ( jx_list_replace(jx_list_borrow(payload,1),
                      jx_ob_as_int(jx_list_borrow(payload,2)),
                      value));
}

JX_INLINE jx_ob jx_safe_resize(jx_ob node, jx_ob payload)
{
  jx_ob fill = jx_ob_strong_with_ob(jx_list_borrow(payload,3)); 
  return jx_ob_from_status
    ( jx_list_resize(jx_list_borrow(payload,1),
                     jx_ob_as_int(jx_list_borrow(payload,2)),
                     fill));
}


JX_INLINE jx_ob jx_safe_list_remove(jx_ob node, jx_ob payload)
{
  return jx_list_remove(jx_list_borrow(payload,1),
                        jx_ob_as_int(jx_list_borrow(payload,2)));

}
JX_INLINE jx_ob jx_safe_list_delete(jx_ob node, jx_ob payload)
{
  return jx_ob_from_status
    ( jx_list_delete(jx_list_borrow(payload,1),
                     jx_ob_as_int(jx_list_borrow(payload,2))));
}

JX_INLINE jx_ob jx_safe_impl(jx_ob node, jx_ob payload)
{
  jx_ob fn = jx_hash_borrow(node, jx_list_borrow(payload,1));
  return jx_function_to_impl(fn);
}


#endif
