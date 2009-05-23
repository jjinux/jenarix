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

/* The first 512 selectors are reserved for "safe" functions which
   only involve internal actions performed within the interpreter and
   its container (no access to filesystem, network, memory,
   etc.). Furthermore, these functions must carefully prevent the
   propagation of weak references by making it impossible to store a weak
   reference into a container */

/* symbols */

#define JX_BUILTIN_FILL        15
#define JX_BUILTIN_RANGE       16
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
#define JX_BUILTIN_APPEND      56
#define JX_BUILTIN_POP         66
#define JX_BUILTIN_SHIFT       67
#define JX_BUILTIN_UNSHIFT     68
#define JX_BUILTIN_INSERT      58
#define JX_BUILTIN_RESIZE      61
#define JX_BUILTIN_EXTEND      62
#define JX_BUILTIN_SLICE       69
#define JX_BUILTIN_CUTOUT      70
#define JX_BUILTIN_IMPL        63

#define JX_BUILTIN_INCR        64
#define JX_BUILTIN_DECR        65
#define JX_BUILTIN_SYMBOLS     71
#define JX_BUILTIN_HAS         72
#define JX_BUILTIN_SAME        73

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
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 3:
    {
      return jx_ob_from_status
        ( jx_hash_set
          (node, 
           jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,1)),
           jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,2))));
    }
    break;
  case 4:
    {
      jx_ob container = jx_list_borrow(payload,1);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_ob_from_status
          ( jx_list_replace
            (container, 
             jx_ob_as_int( jx_list_borrow(payload,2)),
             jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,3))));
        break;
      case JX_META_BIT_HASH:
        return jx_ob_from_status
          ( jx_hash_set
            (container, 
             jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,2)), 
             jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,3))));
        break;
      default:
        return jx_ob_from_null();
        break;
      }
    }
    break;
  default:
    return jx_ob_from_null();
    break;
  }
}

JX_INLINE jx_ob jx_safe_get(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 2:
    return jx_hash_get(node, jx_list_borrow(payload,1));
    break;
  case 3:
    {
      jx_ob container = jx_list_borrow(payload,1);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_list_get(container,
                           jx_ob_as_int(jx_list_borrow(payload,2)));
        break;
      case JX_META_BIT_HASH:
        return jx_hash_get(container,
                           jx_list_borrow(payload,2));
        break;
      }
    }
    break;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_has(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 2:
    return jx_ob_from_bool( jx_hash_has_key(node, jx_list_borrow(payload,1)));
    break;
  case 3:
    {
      jx_ob container = jx_list_borrow(payload,1);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_ob_from_bool
          ( jx_list_index(container,
                         jx_list_borrow(payload,2)) >= 0);
        break;
      case JX_META_BIT_HASH:
        return jx_ob_from_bool
          ( jx_hash_has_key(container,
                            jx_list_borrow(payload,2)));
        break;
      }
    }
    break;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_borrow(jx_ob node, jx_ob payload)
{
  return jx_ob_take_weak_ref( jx_hash_borrow(node, jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_incr(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  jx_ob sym = jx_list_borrow(payload,1);
  jx_ob result = jx_hash_get(node,sym);
  if(size == 3) {
    jx_hash_set(node,sym,jx_ob_add
                (jx_ob_copy(result),
                 jx_list_get(payload,2)));
  } else {
    jx_hash_set(node,sym,jx_ob_add
                (jx_ob_copy(result),
                 jx_ob_from_int(1)));
  }
  return result;
}

JX_INLINE jx_ob jx_safe_decr(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  jx_ob sym = jx_list_borrow(payload,1);
  jx_ob result = jx_hash_get(node,sym);
  if(size == 3) {
    jx_hash_set(node,sym,jx_ob_sub
                (jx_ob_copy(result),
                 jx_list_get(payload,2)));
  } else {
    jx_hash_set(node,sym,jx_ob_sub
                (jx_ob_copy(result),
                 jx_ob_from_int(1)));
  }
  return result;
}

JX_INLINE jx_ob jx_safe_fill(jx_ob node, jx_ob payload)
{
  return jx_list_new_with_fill(jx_ob_as_int(jx_list_borrow(payload,1)),
                               jx_list_get(payload,2));
}

JX_INLINE jx_ob jx_safe_range(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 2:
    return jx_list_new_with_range(0,
                                  jx_ob_as_int(jx_list_borrow(payload,1)),
                                  1);
    break;
  case 3:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload,1)),
                                  jx_ob_as_int(jx_list_borrow(payload,2)),
                                  1);
    break;
  default:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload,1)),
                                  jx_ob_as_int(jx_list_borrow(payload,2)),
                                  jx_ob_as_int(jx_list_borrow(payload,3)));
    break;
  }
}

JX_INLINE jx_ob jx_safe_take(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 2:
    return jx_hash_remove(node, jx_list_borrow(payload,1));
    break;
  case 3:
    {
      jx_ob container = jx_list_borrow(payload,1);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_list_remove(container,
                           jx_ob_as_int(jx_list_borrow(payload,2)));
        break;
      case JX_META_BIT_HASH:
        return jx_hash_remove(container,
                           jx_list_borrow(payload,2));
        break;
      }
    }
    break;
  }
  return jx_ob_from_null();
}


JX_INLINE jx_ob jx_safe_del(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 2:
    return jx_ob_from_status( jx_hash_delete(node, jx_list_borrow(payload,1)));
    break;
  case 3:
    {
      jx_ob container = jx_list_borrow(payload,1);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_ob_from_status
          (jx_list_delete(container,
                          jx_ob_as_int(jx_list_borrow(payload,2))));
        break;
      case JX_META_BIT_HASH:
        return jx_ob_from_status
          (jx_hash_delete(container,
                          jx_list_borrow(payload,2)));
        break;
      default:
        return jx_ob_from_null();
        break;
      }
    }
    break;
  default:
    return jx_ob_from_null();
    break;
  }

  return jx_ob_from_status(jx_hash_delete(node, jx_list_borrow(payload,1)));
}


JX_INLINE jx_ob jx_safe_same(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_same(jx_list_borrow(payload,1),
                                    jx_list_borrow(payload,2)));
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

JX_INLINE jx_ob jx_safe_append(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_append( container,
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,2))));
}

JX_INLINE jx_ob jx_safe_extend(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_combine( container,
                       jx_ob_not_weak_with_ob
                       ( jx_list_swap_with_null(payload,2))));
}

JX_INLINE jx_ob jx_safe_slice(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_new_from_slice(container,
                                jx_ob_as_int( jx_list_borrow(payload,2)),
                                jx_ob_as_int( jx_list_borrow(payload,3)));

}

JX_INLINE jx_ob jx_safe_cutout(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_new_with_cutout(container,
                                 jx_ob_as_int( jx_list_borrow(payload,2)),
                                 jx_ob_as_int( jx_list_borrow(payload,3)));
}

JX_INLINE jx_ob jx_safe_shift(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_remove(container,0);
}

JX_INLINE jx_ob jx_safe_pop(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_pop(container);
}

JX_INLINE jx_ob jx_safe_unshift(jx_ob node, jx_ob payload)
{
 jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_insert( container, 0,
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,2))));
}

JX_INLINE jx_ob jx_safe_insert(jx_ob node, jx_ob payload)
{
 jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_insert( container,
                      jx_ob_as_int( jx_list_borrow(payload,2)),
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,3))));
}

JX_INLINE jx_ob jx_safe_resize(jx_ob node, jx_ob payload)
{
 jx_ob container = jx_list_borrow(payload,1);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_resize( container,
                      jx_ob_as_int( jx_list_borrow(payload,2)),
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,3))));
}

JX_INLINE jx_ob jx_safe_impl(jx_ob node, jx_ob payload)
{
  jx_ob arg = jx_list_borrow(payload,1);
  if(jx_function_check(arg)) {
    return jx_function_to_impl(arg);
  } else {
    jx_ob fn = jx_hash_borrow(node, arg);
    return jx_function_to_impl(fn);
  }
}

JX_INLINE jx_ob jx_safe_symbols(jx_ob node, jx_ob payload)
{
  return jx_ob_copy(node);
}



#endif
