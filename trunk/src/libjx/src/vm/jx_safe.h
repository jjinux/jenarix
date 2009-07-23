
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
JX_INLINE jx_ob jx_safe_ ## SUFFIX(jx_ob payload) \
{ \
  return jx_ob_ ## SUFFIX ( jx_list_borrow(payload,0), jx_list_borrow(payload,1) ); \
}

#define JX_UNI_OP(SUFFIX) \
JX_INLINE jx_ob jx_safe_ ## SUFFIX(jx_ob payload) \
{ \
  return jx_ob_ ## SUFFIX ( jx_list_borrow(payload,0) );    \
}

jx_status jx_safe_expose_all_builtins(jx_ob names);

JX_INLINE jx_ob jx_safe_entity(jx_env * E, jx_ob payload)
{
  jx_ob name = jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 0));
  jx_ob entity = jx_builtin_new_entity_with_name(E, Jx_ob_copy(E, name));
  jx_ob scope = Jx_scope_pop(E);
  jx_ob receiver = Jx_scope_borrow(E);
  Jx_hash_set(E, receiver, name, Jx_ob_copy(E, entity));
  {
    jx_ob def = jx_ob_from_null();
    jx_int size = jx_list_size(payload) - 1;

    if(size) {
      def = jx_list_new_with_size(size);
      switch (size) {
      default:
      case 4:
        jx_list_replace(def, JX_ENTITY_CONSTRUCTOR,
                        jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 4)));
      case 3:
        {
          jx_ob attr_hash = jx_list_swap_with_null(payload, 3);
          if(!jx_list_check(attr_hash)) {
            jx_list_replace(def, JX_ENTITY_ATTR_HASH, jx_ob_not_weak_with_ob(attr_hash));
          } else {
            jx_list_replace(def, JX_ENTITY_ATTR_HASH, scope);
            scope = jx_ob_from_null();
            Jx_ob_free(E, attr_hash);
          }
        }
      case 2:
        jx_list_replace(def, JX_ENTITY_CONTENT_LIST,
                        jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 2)));
      case 1:
        jx_list_replace(def, JX_ENTITY_BASE_HANDLE,
                        jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 1)));
        break;
      }
    }
    Jx_hash_set(E, E->node, Jx_ob_copy(E, entity), def);  /* implementations always stored at node level */
  }
  Jx_ob_free(E, scope);
  //  jx_ob_dump(stdout,"entity",entity);
  //printf("%08x [%s]\n",jx_ob_hash_code(entity),jx_ob_as_ident(entity));
  return entity;
}

JX_INLINE jx_status jx_set_from_path_with_value(jx_env * E, jx_ob container,
                                                jx_ob target, jx_ob value)
{
  //  jx_jxon_dump(stdout,"sfpwv conta",container);
  //jx_jxon_dump(stdout,"sfpwv target",target);
  //  jx_jxon_dump(stdout,"sfpwv value",value);
  jx_status status = jx__create_path(E, &container, &target);
  if(JX_OK(status)) {
    switch (container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(jx_int_check(target)) {
        return Jx_list_replace
          (E, container, jx_ob_as_int(target), jx_ob_not_weak_with_ob(value));
      } else {
        status = jx__list_entity_set(E, container.data.io.list, target, value);
        if(JX_ERR(status)) {
          Jx_ob_free(E, value);
        }
        return status;
      }
      break;
    case JX_META_BIT_HASH:
      return Jx_hash_set
        (E, container,
         jx_ob_not_weak_with_ob(Jx_ob_copy(E, target)), jx_ob_not_weak_with_ob(value));
      break;
    }
    return JX_STATUS_INVALID_CONTAINER;
  }
  return status;
}

JX_INLINE jx_ob jx_safe_map_set(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob path_list = jx_list_borrow(payload, 0);
  jx_ob value_list = jx_list_borrow(payload, 1);
  jx_int i, size = jx_list_size(path_list);
  jx_status status = JX_SUCCESS;
  for(i = 0; i < size; i++) {
    status = jx_set_from_path_with_value(E, container,
                                         jx_list_borrow(path_list, i),
                                         jx_list_swap_with_null(value_list, i));
  }
  return jx_ob_from_status(status);
}

JX_INLINE jx_ob jx_safe_set(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_int size = jx_list_size(payload);
  if(size < 3) {
    return jx_ob_from_status
      (jx_set_from_path_with_value(E, container,
                                   jx_list_borrow(payload, 0),
                                   jx_list_swap_with_null(payload, 1)));
  } else {
    jx_ob target = jx_list_borrow(payload, 0);
    jx_status status = jx__resolve_container(E, &container, &target);
    if(JX_OK(status)) {
      return jx_ob_from_status
        (jx_set_from_path_with_value(E, container,
                                     jx_list_borrow(payload, 1),
                                     jx_list_swap_with_null(payload, 2)));
    }
    return jx_ob_from_status(status);
  }
}

JX_INLINE jx_ob jx_safe_get(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_int size = jx_list_size(payload);
  jx_status status = (size > 1) ?
    jx__resolve_container(E, &container, &target) :
    jx__resolve_path(E, &container, &target);
  if(JX_OK(status)) {
    if(size > 1)
      target = jx_list_borrow(payload, 1);
    switch (container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(jx_int_check(target)) {
        return jx_ob_take_weak_ref(jx_list_borrow(container, jx_ob_as_int(target)));
      } else {
        return jx__list_entity_get(container.data.io.list, target);
      }
      break;
    case JX_META_BIT_HASH:
      return jx_ob_take_weak_ref(jx_hash_borrow(container, target));
      break;
    }
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_has(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_int size = jx_list_size(payload);
  jx_status status = size > 1 ?
    jx__resolve_container(E, &container, &target) :
    jx__resolve_path(E, &container, &target);
  if(JX_OK(status)) {
    if(size > 1)
      target = jx_list_borrow(payload, 1);
    switch (container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(jx_int_check(target)) {
        jx_int index = jx_ob_as_int(target);
        jx_int size = jx_list_size(container);
        if(index < 0)
          index += size;
        return jx_ob_from_bool((index >= 0) && (index < size));
      } else {
        return jx_ob_from_bool(jx__list_entity_has(container.data.io.list, target));
      }
      break;
    case JX_META_BIT_HASH:
      return jx_ob_from_bool(jx_hash_has_key(container, target));
      break;
    }
  }
  return jx_ob_from_bool(JX_FALSE);
}

JX_INLINE jx_ob jx_safe_take(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_int size = jx_list_size(payload);
  jx_status status = size > 1 ?
    jx__resolve_container(E, &container, &target) :
    jx__resolve_path(E, &container, &target);
  if(JX_OK(status)) {
    if(size > 1)
      target = jx_list_borrow(payload, 1);
    switch (container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(jx_int_check(target)) {
        return jx_list_remove(container, jx_ob_as_int(target));
      } else {
        return jx__list_entity_take(E, container.data.io.list, target);
      }
      break;
    case JX_META_BIT_HASH:
      return Jx_hash_remove(E, container, target);
      break;
    }
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_del(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_int size = jx_list_size(payload);
  jx_status status = (size > 1) ?
    jx__resolve_container(E, &container, &target) :
    jx__resolve_path(E, &container, &target);
  if(JX_OK(status)) {
    if(size > 1)
      target = jx_list_borrow(payload, 1);
    switch (container.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(jx_int_check(target)) {
        return jx_ob_from_status(jx_list_delete(container, jx_ob_as_int(target)));
      } else {
        return jx_ob_from_status
          (jx__list_entity_delete(E, container.data.io.list, target));
      }
      break;
    case JX_META_BIT_HASH:
      return jx_ob_from_status(Jx_hash_delete(E, container, target));
      break;
    default:
      return jx_ob_from_status(JX_STATUS_INVALID_CONTAINER);
      break;
    }
  }
  return jx_ob_from_status(status);
}

#if 0
JX_INLINE jx_ob jx_safe_borrow(jx_env * E, jx_ob payload)
{
  return jx_ob_take_weak_ref(jx_hash_borrow(node, jx_list_borrow(payload, 0)));
}
#endif

JX_INLINE jx_ob jx_safe_incr(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_int size = jx_list_size(payload);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_status status = jx__resolve_path(E, &container, &target);
  if(JX_OK(status)) {
    jx_ob result = jx_hash_get(container, target);
    switch (size) {
    case 1:
      Jx_hash_set(E, container, Jx_ob_copy(E, target),
                  jx_ob_add(result, jx_ob_from_int(1)));
      break;
    default:
      Jx_hash_set(E, container, Jx_ob_copy(E, target), jx_ob_add
                  (result, jx_list_borrow(payload, 1)));
      break;
    }
    return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_decr(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_int size = jx_list_size(payload);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_status status = jx__resolve_path(E, &container, &target);
  if(JX_OK(status)) {
    jx_ob result = jx_hash_get(container, target);
    switch (size) {
    case 1:
      Jx_hash_set(E, container, Jx_ob_copy(E, target),
                  jx_ob_sub(result, jx_ob_from_int(1)));
      break;
    default:
      Jx_hash_set(E, container, Jx_ob_copy(E, target), jx_ob_sub
                  (result, jx_list_borrow(payload, 1)));
      break;
    }
    return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_synchronize(jx_env * E, jx_ob payload)
{
  jx_ob flag = jx_list_borrow(payload, 1);
  if(jx_null_check(flag))
    flag = jx_ob_from_bool(JX_TRUE);
  {
    return jx_ob_from_status
      (jx_ob_set_synchronized(jx_list_borrow(payload, 0),
                              jx_ob_as_bool(flag),
                              jx_ob_as_bool(jx_list_borrow(payload, 2))));
  }
}

JX_INLINE jx_ob jx_safe_synchronized(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_synchronized(jx_list_borrow(payload, 0)));
}

JX_INLINE jx_ob jx_safe_share(jx_env * E, jx_ob payload)
{
  jx_ob target = jx_list_borrow(payload, 0);
  jx_ob flag = jx_list_borrow(payload, 1);
  if(jx_null_check(target))
    target = Jx_scope_borrow(E);
  if(jx_null_check(flag))
    flag = jx_ob_from_bool(JX_TRUE);
  return jx_ob_from_status(jx_ob_set_shared(target, jx_ob_as_bool(flag)));
}

JX_INLINE jx_ob jx_safe_shared(jx_env * E, jx_ob payload)
{
  jx_ob target = jx_list_borrow(payload, 0);
  if(jx_null_check(target))
    target = Jx_scope_borrow(E);
  return jx_ob_from_bool(jx_ob_shared(target));
}

JX_INLINE jx_ob jx_safe_fill(jx_env * E, jx_ob payload)
{
  return jx_list_new_with_fill(jx_ob_as_int(jx_list_borrow(payload, 0)),
                               jx_list_get(payload, 1));
}

JX_INLINE jx_ob jx_safe_range(jx_env * E, jx_ob payload)
{
  jx_int size = jx_list_size(payload);
  switch (size) {
  case 1:
    return jx_list_new_with_range(0,
                                  jx_ob_as_int(jx_list_borrow(payload, 0)),
                                  1, JX_FLOAT_ZERO);
    break;
  case 2:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload, 0)),
                                  jx_ob_as_int(jx_list_borrow(payload, 1)),
                                  1, JX_FLOAT_ZERO);

    break;
  case 3:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload, 0)),
                                  jx_ob_as_int(jx_list_borrow(payload, 1)),
                                  jx_ob_as_int(jx_list_borrow(payload, 2)),
                                  JX_FLOAT_ZERO);
    break;
  default:
    return jx_list_new_with_range(jx_ob_as_int(jx_list_borrow(payload, 0)),
                                  jx_ob_as_int(jx_list_borrow(payload, 1)),
                                  jx_ob_as_int(jx_list_borrow(payload, 2)),
                                  jx_ob_as_float(jx_list_borrow(payload, 3)));
    break;
  }
}

JX_INLINE jx_ob jx_safe_same(jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_same(jx_list_borrow(payload, 0),
                                    jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_identical(jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_identical(jx_list_borrow(payload, 0),
                                         jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_eq(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_equal(jx_list_borrow(payload, 0),
                                     jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_ne(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(!jx_ob_equal(jx_list_borrow(payload, 0),
                                      jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_lt(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_lt(jx_list_borrow(payload, 0),
                                  jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_gt(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_gt(jx_list_borrow(payload, 0),
                                  jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_le(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_le(jx_list_borrow(payload, 0),
                                  jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_ge(jx_env * E, jx_ob payload)
{
  return jx_ob_from_bool(jx_ob_ge(jx_list_borrow(payload, 0),
                                  jx_list_borrow(payload, 1)));
}

JX_INLINE jx_ob jx_safe_output(jx_env * E, jx_ob payload)
{
  /* should check for 'output' in the node namespace and use that
     instead if defined */
  jx_ob jxon = jx_ob_to_jxon_in_node(E->node, jx_list_borrow(payload, 0));
  fprintf(stdout, "%s;\n", jx_ob_as_str(&jxon));
  Jx_ob_free(E, jxon);
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_dump(jx_env * E, jx_ob payload)
{
  jx_jxon_dump_in_node(stdout, "#  dump", E->node, jx_list_borrow(payload, 0));
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_assert(jx_env * E, jx_ob payload)
{
  jx_ob first = jx_list_borrow(payload, 0);
  jx_ob second = jx_list_borrow(payload, 1);
  if(!jx_ob_identical(first, second)) {
    fprintf(stdout, "Error: assertion failure.\n");
    jx_jxon_dump(stdout, " first", first);
    jx_jxon_dump(stdout, "second", second);
    return jx_ob_from_bool(0);
  } else {
    return jx_ob_from_bool(1);
  }
}

JX_INLINE jx_ob jx_safe_error(jx_env * E, jx_ob payload)
{
  /* should check for 'error' in the node namespace and use that
     instead if defined */
  jx_ob jxon = jx_ob_to_jxon_in_node(E->node, jx_list_borrow(payload, 0));
  fprintf(stderr, "%s;\n", jx_ob_as_str(&jxon));
  Jx_ob_free(E, jxon);
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_add(jx_env * E, jx_ob payload)
{
  return Jx_ob_add(E, jx_list_borrow(payload, 0), jx_list_borrow(payload, 1));
}

JX_INLINE jx_ob jx_safe_mul(jx_env * E, jx_ob payload)
{
  return Jx_ob_mul(E, jx_list_borrow(payload, 0), jx_list_borrow(payload, 1));
}

JX_BIN_OP(sub)

  JX_BIN_OP(div)
  JX_BIN_OP(idiv)
  JX_BIN_OP(mod)
  JX_BIN_OP(and)
  JX_BIN_OP(or)

  JX_UNI_OP(neg)
  JX_UNI_OP(not)

     JX_INLINE jx_ob jx_safe_pow(jx_ob payload)
{
  return jx_ob_pow(jx_list_borrow(payload, 0), jx_list_borrow(payload, 1));
}

JX_INLINE jx_ob jx_safe_size(jx_ob payload)
{
  return jx_ob_size(jx_list_borrow(payload, 0));
}

JX_INLINE jx_ob jx_safe_reverse(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_ob_from_status(jx_list_reverse(container));
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_sort(jx_env * E, jx_ob payload)
{                               /* TO DO: support a comparison function */
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_ob_from_status(jx_list_sort(container));
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_append(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_ob_from_status
      (jx_list_append(container,
                      jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 1))));
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_extend(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_ob_from_status
      (jx_list_combine(container,
                       jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 1))));
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_slice(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_list_new_from_slice(container,
                                  jx_ob_as_int(jx_list_borrow(payload, 1)),
                                  jx_ob_as_int(jx_list_borrow(payload, 2)));
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_cutout(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_list_new_with_cutout(container,
                                   jx_ob_as_int(jx_list_borrow(payload, 1)),
                                   jx_ob_as_int(jx_list_borrow(payload, 2)));
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_shift(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_list_remove(container, 0);
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_pop(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  if(JX_OK(jx__resolve_container(E, &container, &target))) {
    return jx_list_pop(container);
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_safe_unshift(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_status status = jx__resolve_container(E, &container, &target);
  if(JX_OK(status)) {
    return jx_ob_from_status
      (jx_list_insert(container, 0,
                      jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 1))));
  }
  return jx_ob_from_status(status);
}

JX_INLINE jx_ob jx_safe_insert(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_status status = jx__resolve_container(E, &container, &target);
  if(JX_OK(status)) {
    return jx_ob_from_status
      (jx_list_insert(container,
                      jx_ob_as_int(jx_list_borrow(payload, 1)),
                      jx_ob_not_weak_with_ob(jx_list_swap_with_null(payload, 2))));
  }
  return jx_ob_from_status(status);
}

JX_INLINE jx_ob jx_safe_resize(jx_env * E, jx_ob payload)
{
  jx_ob container = Jx_scope_borrow(E);
  jx_ob target = jx_list_borrow(payload, 0);
  jx_status status = jx__resolve_container(E, &container, &target);
  if(JX_OK(status)) {
    return jx_ob_from_status
      (jx_list_resize_with_fill(container,
                                jx_ob_as_int(jx_list_borrow(payload, 1)),
                                jx_ob_not_weak_with_ob
                                (jx_list_swap_with_null(payload, 2))));
  }
  return jx_ob_from_status(status);
}

JX_INLINE jx_ob jx_safe_impl(jx_env * E, jx_ob payload)
{
  jx_ob arg = jx_list_borrow(payload, 0);
  if(jx_function_check(arg)) {
    return jx_function_to_impl(arg);
  } else if(jx_macro_check(arg)) {
    return jx_macro_to_impl(arg);
  } else {
    /* TO DO: resolve peroperly */
    jx_ob fn = jx_hash_borrow(E->node, arg);
    return jx_function_to_impl(fn);
  }
}

JX_INLINE jx_ob jx_safe_symbols(jx_env * E, jx_ob payload)
{
  jx_ob result = Jx_ob_copy(E, Jx_scope_borrow(E));
  if(!jx_ob_as_bool(jx_list_borrow(payload, 0))) {
    /* hide builtin symbols unless specifically asked for them */
    Jx_hash_delete(E, result, jx_builtins());
  }
  if(jx_null_check(jx_list_borrow(payload, 0))) {
    /* hide entity definitions too */
    jx_ob keys = Jx_hash_keys(E, result);
    jx_int i, size = jx_list_size(keys);
    for(i = 0; i < size; i++) {
      jx_ob key = jx_list_borrow(keys, i);
      if(jx_builtin_entity_check(key)) {
        Jx_hash_delete(E, result, key);
      }
    }
    Jx_ob_free(E, keys);
  }
  return result;
}

JX_INLINE jx_ob jx_safe_str(jx_env * E, jx_ob payload)
{
  return Jx_ob_to_str(E, jx_list_borrow(payload, 0));
}

JX_INLINE jx_ob jx_safe_int(jx_ob payload)
{
  return jx_ob_to_int(jx_list_borrow(payload, 0));
}

JX_INLINE jx_ob jx_safe_float(jx_ob payload)
{
  return jx_ob_to_float(jx_list_borrow(payload, 0));
}

JX_INLINE jx_ob jx_safe_bool(jx_ob payload)
{
  return jx_ob_to_bool(jx_list_borrow(payload, 0));
}

#endif
