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
#include "jx_selectors.h"

#define JX_BIN_OP(SUFFIX) \
JX_INLINE jx_ob jx_safe_ ## SUFFIX(jx_ob node, jx_ob payload) \
{ \
  return jx_ob_ ## SUFFIX ( jx_list_borrow(payload,0), jx_list_borrow(payload,1) ); \
}

#define JX_UNI_OP(SUFFIX) \
JX_INLINE jx_ob jx_safe_ ## SUFFIX(jx_ob node, jx_ob payload) \
{ \
  return jx_ob_ ## SUFFIX ( jx_list_borrow(payload,0) );    \
}

jx_status jx_safe_expose_all_builtins(jx_ob names);

JX_INLINE jx_status jx__resolve(jx_ob *container,jx_ob *ident)
{
  if(jx_list_check(*ident)) { /* compound indentifier */
    jx_int i,path_size = jx_list_size(*ident);
    jx_ob path = *ident;
    if(!path_size) /* empty list implies fully-computed set operation */
      return JX_NO;
    for(i=0;i<path_size;i++) {
      *ident = jx_list_borrow(path,i);
      if((1+i)<path_size) {
        switch(container->meta.bits & JX_META_MASK_TYPE_BITS) {
        case JX_META_BIT_LIST:
          *container = jx_list_borrow(*container,jx_ob_as_int(*ident));
          break;
        case JX_META_BIT_HASH:
          *container = jx_hash_borrow(*container,*ident);
          break;
        default:
          *container = jx_ob_from_null();
          break;
        }
      }
    }
  }
  return JX_YES;
}

JX_INLINE jx_ob jx_safe_set(jx_ob node, jx_ob payload)
{
  //  jx_jxon_dump(stdout,"safe set payload",payload);
  //  jx_jxon_dump(stdout,"safe set node",node);
  jx_int size = jx_list_size(payload);
  jx_ob container = node;
  jx_ob target =  jx_list_borrow(payload,0);
  jx_status resolved = jx__resolve(&container,&target);
  switch(size) {
  case 2: /* (set literal-path value) */
    return jx_ob_from_status
      ( jx_hash_set
        (container, 
         target,
         jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,1))));
    break;
  case 3: /* ( set literal-path computed-path value ) */
    if(JX_POS(resolved)) {
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        container = jx_list_borrow(container,jx_ob_as_int(target));
        break;
      case JX_META_BIT_HASH:
        container = jx_hash_borrow(container,target);
        break;
      default:
        container = jx_ob_from_null();
        break;
      }
    }
    target = jx_list_borrow(payload,1);
    jx__resolve(&container,&target);
    //    jx_jxon_dump(stdout,"container",container);
    //    jx_jxon_dump(stdout,"target",target);
    switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      return jx_ob_from_status
        ( jx_list_replace
          (container, 
           jx_ob_as_int(target),
           jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,2))));
      break;
    case JX_META_BIT_HASH:
      return jx_ob_from_status
        ( jx_hash_set
          (container, 
           jx_ob_copy(target),
           jx_ob_not_weak_with_ob( jx_list_swap_with_null(payload,2))));
      break;
    default:
      return jx_ob_from_null();
      break;
    }
    break;
  default:
    return jx_ob_from_null();
    break;
  }
  return jx_ob_from_null();
}

 /* "deep" sets proof of concept.  

    # Python: a.b.c.d = value becomes JXON:  (`set (`path a b c d) value)

    Where b is resolved within a, c within b, and then d is set to
    value in scope c.

    a,b,c,d could of course be expressions too, and for lists, they'll need to be numeric indices. 0-N, -1 -> -N

    Could this work for all our container access methods?

    Also consider permitting the user to alias portions of a path (in lieu of taking an actual reference):

    g = path(a.b.c)

    g.d = value  ->  (set (`path (`resolve g) d) value)

    g.d.e = value -> (set (`path (`resolve g) d 3) value)

    another option:

    g = take(a.b.c) (take g (`path a b c));

    g.d = value

    g.d.e = value

    a.b.c = g

    could do this automatically using "with"

    with g = a.b.c:
        g.d = value
        
        g.d.e = value

        # exiting scope automatically restores a.b.c = g and destroys g 

 */


JX_INLINE jx_ob jx_safe_get(jx_ob node, jx_ob payload)
{
  //  jx_jxon_dump(stdout,"safe get payload",payload);
  //  jx_jxon_dump(stdout,"safe get node",node);
  jx_int size = jx_list_size(payload);
  jx_ob container = node;
  jx_ob target =  jx_list_borrow(payload,0);
  jx_status resolved = jx__resolve(&container,&target);
  switch(size) {
  case 1:
    return jx_hash_get(container, target);
    break;
  case 2:
    if(JX_POS(resolved)) {
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        container = jx_list_borrow(container,jx_ob_as_int(target));
        break;
      case JX_META_BIT_HASH:
        container = jx_hash_borrow(container,target);
        break;
      default:
        container = jx_ob_from_null();
        break;
      }
    }
    target = jx_list_borrow(payload,1);
    jx__resolve(&container,&target);
    switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      return jx_list_get(container, jx_ob_as_int(target));
      break;
    case JX_META_BIT_HASH:
      return jx_hash_get(container, jx_ob_not_weak_with_ob(target));
      break;
    default:
      return jx_ob_from_null();
      break;
    }
    break;
  default:
    return jx_ob_from_null();
    break;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_has(jx_ob node, jx_ob payload)
{
   jx_int size = jx_list_size(payload);
  jx_ob container = node;
  jx_ob target =  jx_list_borrow(payload,0);
  jx_status resolved = jx__resolve(&container,&target);
  switch(size) {
  case 1:
    return jx_hash_get(container, target);
    break;
  case 2:
    if(JX_POS(resolved)) {
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        container = jx_list_borrow(container,jx_ob_as_int(target));
        break;
      case JX_META_BIT_HASH:
        container = jx_hash_borrow(container,target);
        break;
      default:
        container = jx_ob_from_null();
        break;
      }
    }
    target = jx_list_borrow(payload,1);
    jx__resolve(&container,&target);
    switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      return jx_ob_from_bool( jx_list_index(container, target) >= 0);
      break;
    case JX_META_BIT_HASH:
      return jx_ob_from_bool( jx_hash_has_key(container, jx_ob_not_weak_with_ob(target)));
      break;
    default:
      return jx_ob_from_null();
      break;
    }
    break;
  default:
    return jx_ob_from_null();
    break;
  }
  return jx_ob_from_null();
}

#if 0
JX_INLINE jx_ob jx_safe_borrow(jx_ob node, jx_ob payload)
{
  return jx_ob_take_weak_ref( jx_hash_borrow(node, jx_list_borrow(payload,0)));
}
#endif

JX_INLINE jx_ob jx_safe_incr(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  jx_ob sym = jx_list_borrow(payload,0);
  jx_ob result = jx_hash_get(node,sym);
  if(size == 2) {
    jx_hash_set(node,sym,jx_ob_add
                (jx_ob_copy(result),
                 jx_list_get(payload,1)));
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
  jx_ob sym = jx_list_borrow(payload,0);
  jx_ob result = jx_hash_get(node,sym);
  if(size == 2) {
    jx_hash_set(node,sym,jx_ob_sub
                (jx_ob_copy(result),
                 jx_list_get(payload,1)));
  } else {
    jx_hash_set(node,sym,jx_ob_sub
                (jx_ob_copy(result),
                 jx_ob_from_int(1)));
  }
  return result;
}

JX_INLINE jx_ob jx_safe_fill(jx_ob node, jx_ob payload)
{
  return jx_list_new_with_fill(jx_ob_as_int(jx_list_borrow(payload,0)),
                               jx_list_get(payload,1));
}

JX_INLINE jx_ob jx_safe_range(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 1:
    return jx_list_new_with_range(0,
                                  jx_ob_as_int(jx_list_borrow(payload,0)),
                                  1, JX_FLOAT_ZERO);
    break;
  case 2:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload,0)),
                                  jx_ob_as_int(jx_list_borrow(payload,1)),
                                  1, JX_FLOAT_ZERO);

    break;
  case 3:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload,0)),
                                  jx_ob_as_int(jx_list_borrow(payload,1)),
                                  jx_ob_as_int(jx_list_borrow(payload,2)),
                                  JX_FLOAT_ZERO);
    break;
  default:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload,0)),
                                  jx_ob_as_int(jx_list_borrow(payload,1)),
                                  jx_ob_as_int(jx_list_borrow(payload,2)),
                                  jx_ob_as_float(jx_list_borrow(payload,3)));
    break;
  }
}

JX_INLINE jx_ob jx_safe_take(jx_ob node, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch(size) {
  case 1:
    return jx_hash_remove(node, jx_list_borrow(payload,0));
    break;
  case 2:
    {
      jx_ob container = jx_list_borrow(payload,0);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_list_remove(container,
                           jx_ob_as_int(jx_list_borrow(payload,1)));
        break;
      case JX_META_BIT_HASH:
        return jx_hash_remove(container,
                           jx_list_borrow(payload,1));
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
  case 1:
    return jx_ob_from_status( jx_hash_delete(node, jx_list_borrow(payload,0)));
    break;
  case 2:
    {
      jx_ob container = jx_list_borrow(payload,0);
      if(jx_ident_check(container))
        container = jx_hash_borrow(node,container);
      switch(container.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_LIST:
        return jx_ob_from_status
          (jx_list_delete(container,
                          jx_ob_as_int(jx_list_borrow(payload,1))));
        break;
      case JX_META_BIT_HASH:
        return jx_ob_from_status
          (jx_hash_delete(container,
                          jx_list_borrow(payload,1)));
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

  return jx_ob_from_status(jx_hash_delete(node, jx_list_borrow(payload,0)));
}


JX_INLINE jx_ob jx_safe_same(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_same(jx_list_borrow(payload,0),
                                    jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_identical(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_identical(jx_list_borrow(payload,0),
                                         jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_eq(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_equal(jx_list_borrow(payload,0),
                                     jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_ne(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(!jx_ob_equal(jx_list_borrow(payload,0),
                                      jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_lt(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_lt(jx_list_borrow(payload,0),
                                  jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_gt(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_gt(jx_list_borrow(payload,0),
                                  jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_le(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_le(jx_list_borrow(payload,0),
                                  jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_ge(jx_ob node, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_ge(jx_list_borrow(payload,0),
                                  jx_list_borrow(payload,1)));
}


JX_INLINE jx_ob jx_safe_output(jx_ob node, jx_ob payload)
{
  /* should check for 'output' in the node namespace and use that
     instead if defined */
  jx_ob jxon = jx_ob_to_jxon(jx_list_borrow(payload,0));
  fprintf(stdout,"%s;\n",jx_ob_as_str(&jxon));
  jx_ob_free(jxon);
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_error(jx_ob node, jx_ob payload)
{
  /* should check for 'error' in the node namespace and use that
     instead if defined */
  jx_ob jxon = jx_ob_to_jxon(jx_list_borrow(payload,0));
  fprintf(stderr,"%s;\n",jx_ob_as_str(&jxon));
  jx_ob_free(jxon);
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_add(jx_ob node, jx_ob payload)
{
  return jx_ob_add( jx_list_borrow(payload,0), jx_list_borrow(payload,1) );
}

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
  return jx_ob_size(jx_list_borrow(payload,0));
}

JX_INLINE jx_ob jx_safe_append(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_append( container,
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,1))));
}

JX_INLINE jx_ob jx_safe_extend(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_combine( container,
                       jx_ob_not_weak_with_ob
                       ( jx_list_swap_with_null(payload,1))));
}

JX_INLINE jx_ob jx_safe_slice(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_new_from_slice(container,
                                jx_ob_as_int( jx_list_borrow(payload,1)),
                                jx_ob_as_int( jx_list_borrow(payload,2)));

}

JX_INLINE jx_ob jx_safe_cutout(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_new_with_cutout(container,
                                 jx_ob_as_int( jx_list_borrow(payload,1)),
                                 jx_ob_as_int( jx_list_borrow(payload,2)));
}

JX_INLINE jx_ob jx_safe_shift(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_remove(container,0);
}

JX_INLINE jx_ob jx_safe_pop(jx_ob node, jx_ob payload)
{
  jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container))
    container = jx_hash_borrow(node,container);
  return jx_list_pop(container);
}

JX_INLINE jx_ob jx_safe_unshift(jx_ob node, jx_ob payload)
{
 jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_insert( container, 0,
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,1))));
}

JX_INLINE jx_ob jx_safe_insert(jx_ob node, jx_ob payload)
{
 jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_insert( container,
                      jx_ob_as_int( jx_list_borrow(payload,1)),
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,2))));
}

JX_INLINE jx_ob jx_safe_resize(jx_ob node, jx_ob payload)
{
 jx_ob container = jx_list_borrow(payload,0);
  if(jx_ident_check(container)) 
    container = jx_hash_borrow(node,container);
  return jx_ob_from_status
    ( jx_list_resize( container,
                      jx_ob_as_int( jx_list_borrow(payload,1)),
                      jx_ob_not_weak_with_ob
                      ( jx_list_swap_with_null(payload,2))));
}

JX_INLINE jx_ob jx_safe_impl(jx_ob node, jx_ob payload)
{
  jx_ob arg = jx_list_borrow(payload,0);
  if(jx_function_check(arg)) {
    return jx_function_to_impl(arg);
  } else if(jx_macro_check(arg)) {
    return jx_macro_to_impl(arg);
  } else {
    jx_ob fn = jx_hash_borrow(node, arg);
    return jx_function_to_impl(fn);
  }
}

JX_INLINE jx_ob jx_safe_symbols(jx_ob node, jx_ob payload)
{
  return jx_ob_copy(node);
}

JX_INLINE jx_ob jx_safe_str(jx_ob node, jx_ob payload)
{
  jx_ob something = jx_list_borrow(payload,0);
  return jx_ob_to_str(something);
}

#endif
