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

#define JX_BUILTIN_NOP       22

/* symbols */

#define JX_BUILTIN_SET       23
#define JX_BUILTIN_GET       24
#define JX_BUILTIN_BORROW    25
#define JX_BUILTIN_TAKE      26
#define JX_BUILTIN_DEL       27

#define JX_BUILTIN_IDENTICAL 28
#define JX_BUILTIN_EQ        29
#define JX_BUILTIN_LT        30
#define JX_BUILTIN_GT        31
#define JX_BUILTIN_LE        32
#define JX_BUILTIN_GE        33
#define JX_BUILTIN_NE        34
#define JX_BUILTIN_AND       35      
#define JX_BUILTIN_OR        36
#define JX_BUILTIN_BIT_AND   37
#define JX_BUILTIN_BIT_OR    38
#define JX_BUILTIN_BIT_XOR   39
#define JX_BUILTIN_BIT_SHL   40
#define JX_BUILTIN_BIT_SHR   41

#define JX_BUILTIN_ADD       42
#define JX_BUILTIN_SUB       43
#define JX_BUILTIN_MUL       44
#define JX_BUILTIN_DIV       45
#define JX_BUILTIN_MOD       46

#define JX_BUILTIN_BIT_NOT   47
#define JX_BUILTIN_NOT       48
#define JX_BUILTIN_NEG       49

#define JX_BUILTIN_APPEND    50
#define JX_BUILTIN_SIZE      51
#define JX_BUILTIN_HASH_SET  52
#define JX_BUILTIN_HASH_GET  53

#define JX_BUILTIN_OUTPUT    54
#define JX_BUILTIN_ERROR     55

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
  jx_ob value = jx_list_remove(payload,2);
  jx_ob key = jx_list_remove(payload,1);
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

JX_INLINE jx_ob jx_safe_append(jx_ob node, jx_ob payload)
{
  jx_ob item = jx_list_remove(payload,2);
  jx_ob list = jx_list_borrow(payload,1);
  return jx_ob_from_status( jx_list_append(list, item) );
}

JX_INLINE jx_ob jx_safe_size(jx_ob node, jx_ob payload)
{
  return jx_ob_size( jx_list_borrow(payload,1) );
}

JX_INLINE jx_ob jx_safe_hash_set(jx_ob node, jx_ob payload)
{
  return jx_ob_from_status(jx_hash_set(jx_list_borrow(payload,1),
                                       jx_list_borrow(payload,2),
                                       jx_list_borrow(payload,3)));
}

JX_INLINE jx_ob jx_safe_hash_get(jx_ob node, jx_ob payload)
{
  return jx_hash_get(jx_list_borrow(payload,1),
                     jx_list_borrow(payload,2));
}


#endif
