
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

  ok = jx_declare(ok, names, "builtins", JX_SELECTOR_BUILTINS);

  ok = jx_declare(ok, names, "skip", JX_SELECTOR_SKIP);

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

  ok = jx_declare(ok, names, "debug", JX_SELECTOR_DEBUG);
  ok = jx_declare(ok, names, "eval", JX_SELECTOR_EVAL);
  ok = jx_declare(ok, names, "exec", JX_SELECTOR_EXEC);
  ok = jx_declare(ok, names, "dispatch", JX_SELECTOR_DISPATCH);
  ok = jx_declare(ok, names, "switch", JX_SELECTOR_SWITCH);

  ok = jx_declare(ok, names, "select", JX_SELECTOR_SELECT);
  ok = jx_declare(ok, names, "return", JX_SELECTOR_RETURN);
  ok = jx_declare(ok, names, "break", JX_SELECTOR_BREAK);
  ok = jx_declare(ok, names, "continue", JX_SELECTOR_CONTINUE);
  ok = jx_declare(ok, names, "tail", JX_SELECTOR_TAIL);

  ok = jx_declare(ok, names, "paramap", JX_SELECTOR_PARAMAP);
  ok = jx_declare(ok, names, "paraset", JX_SELECTOR_PARASET);
  ok = jx_declare(ok, names, "pareval", JX_SELECTOR_PAREVAL);

  ok = jx_declare(ok, names, "pass", JX_SELECTOR_PASS);

  ok = jx_declare(ok, names, "bind", JX_SELECTOR_BIND);

  return ok ? JX_SUCCESS : JX_FAILURE;
}

jx_status jx_code_expose_secure_builtins(jx_ob names)
{
  jx_bool ok = JX_TRUE;

  ok = ok && jx_ok(jx_safe_expose_all_builtins(names));
  ok = ok && jx_ok(jx_code_expose_special_forms(names));

  return ok ? JX_SUCCESS : JX_FAILURE;
}


/* consume source & return builtin-bound executable */

#define JX_RESOLUTION_NORMAL  0
#define JX_RESOLUTION_LVALUE  1
#define JX_RESOLUTION_RVALUE  2
#define JX_RESOLUTION_FUNDEF  3
#define JX_RESOLUTION_LAMDEF  4
#define JX_RESOLUTION_ENTITY  5
#define JX_RESOLUTION_MAP_LVALUE  6

static jx_ob jx__code_bind_with_source(jx_env * E, jx_ob prebind, jx_ob source,
                                       jx_int unresol_depth)
{
  if(unresol_depth < 0)
    unresol_depth = 0;
  switch (JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else if(unresol_depth) {
      jx_int slot = jx_list_size(source);
      while(slot--) {
        jx_ob entry = jx_list_swap_with_null(source, slot);
        entry = jx__code_bind_with_source(E, prebind, entry, unresol_depth - 1);
        Jx_list_replace(E, source, slot, entry);
      }
      return source;
    } else {
      /* [ indent ...]  becomes [ builtin ...] */
      jx_int size = jx_list_size(source);
      jx_list *source_list = source.data.io.list;
      jx_ob ident = jx_list_borrow(source, 0);
      jx_int unresolved = 0;
      jx_int resolution = JX_RESOLUTION_NORMAL;
      if(jx_ident_check(ident)) {
        jx_ob builtin = jx_hash_borrow(prebind, ident);
        if(jx_builtin_callable_check(builtin)) {
          if(jx_builtin_selector_check(builtin)) {
            /* known builtin function (early / fixed binding) */
            unresolved = 1;
            switch (builtin.data.io.int_) {
            case JX_SELECTOR_SKIP:     /* [skip # ...] -> bind(#) */
              Jx_ob_replace(E, &source, jx_list_take(source, 1));
              return jx__code_bind_with_source(E, prebind, source, unresol_depth - 1);
              break;
            case JX_SELECTOR_RAW:      /* [raw # ...] -> [*raw* # ...] */
              Jx_list_replace(E, source, 0, builtin);
              return source;
              break;
            case JX_SELECTOR_NOP:  /* [nop ...] -> [...] */
              {
                jx_ob tst = jx_list_borrow(source, 1);
                if(!(tst.meta.bits & (JX_META_BIT_LIST | JX_META_BIT_IDENT |
                                      JX_META_BIT_BUILTIN | JX_META_BIT_OPCODE))) {
                  jx_list_delete(source, 0);
                  return jx__code_bind_with_source(E, prebind, source, unresol_depth);
                }
              }
              break;
            case JX_SELECTOR_PASS:
              break;
            case JX_SELECTOR_QUOTE:    /* [quote # ...] -> [*quote* bind(#) ...] */
              Jx_list_replace(E, source, 0, builtin);
              return jx__code_bind_with_source(E, prebind, source, unresol_depth - 1);
              break;
            case JX_SELECTOR_ENTITY:
              resolution = JX_RESOLUTION_ENTITY;
              unresolved = 2;
              break;
            case JX_SELECTOR_MAP_SET:
            case JX_SELECTOR_PARASET:
              resolution = JX_RESOLUTION_MAP_LVALUE;
              unresolved = 2;
              break;
            case JX_SELECTOR_SET:
            case JX_SELECTOR_INCR:
            case JX_SELECTOR_DECR:
            case JX_SELECTOR_FOREACH:
              resolution = JX_RESOLUTION_LVALUE;
              unresolved = 2;
              break;
            case JX_SELECTOR_HAS:
            case JX_SELECTOR_GET:
            case JX_SELECTOR_TAKE:
            case JX_SELECTOR_DEL:
              resolution = JX_RESOLUTION_RVALUE;
              unresolved = 2;
              break;
            case JX_SELECTOR_CODE:
            case JX_SELECTOR_LAMBDA:
              unresolved = 2;
              resolution = JX_RESOLUTION_LAMDEF;
              break;
            case JX_SELECTOR_DEF:
            case JX_SELECTOR_DEFUN:
            case JX_SELECTOR_DEFMAC:
              resolution = JX_RESOLUTION_FUNDEF;
              unresolved = 3;
              break;
            case JX_SELECTOR_RESOLVE:
              resolution = JX_RESOLUTION_RVALUE;        /* ? */
              unresolved = 2;
              break;
            }
          }
          Jx_list_replace(E, source, 0, builtin);
        }
      } else if(jx_builtin_selector_check(ident) &&
                ident.data.io.int_ == JX_SELECTOR_SKIP) {
        /* [*skip* # ...] -> bind(#) */
        Jx_ob_replace(E, &source, jx_list_take(source, 1));
        return jx__code_bind_with_source(E, prebind, source, unresol_depth - 1);
      }
      //      jx_jxon_dump(stdout,"post-sub",source);
      /* now process the source list, introducing symbol resolution where needed */
      {
        jx_int slot = size;
        while((slot > unresolved) && (slot--)) {
          jx_ob entry = jx__list_swap_with_null(source_list, slot);
          jx_bool skip_bind = JX_FALSE;

          switch (resolution) {
          case JX_RESOLUTION_ENTITY:   /* entity name base content attr constr */
            switch (slot) {
            case 5:            /* constructor name */
              skip_bind = JX_TRUE;
              break;
            }
            break;
          }

          if(jx_ident_check(entry) && (!skip_bind)) {   /* # -> [*res* #] */
            jx_ob builtin = jx_hash_borrow(prebind, entry);
            if(jx_builtin_selector_check(builtin)) {
              switch (builtin.data.io.int_) {
              case JX_SELECTOR_BREAK:
                Jx_ob_replace(E, &entry, jx_ob_from_opcode(JX_OPCODE_BREAK, 0));
                break;
              case JX_SELECTOR_CONTINUE:
                Jx_ob_replace(E, &entry, jx_ob_from_opcode(JX_OPCODE_CONTINUE, 0));
                break;
              case JX_SELECTOR_RETURN:
                Jx_ob_replace(E, &entry, jx_ob_from_opcode(JX_OPCODE_RETURN, 0));
                break;
              case JX_SELECTOR_TAIL:
                Jx_ob_replace(E, &entry, jx_ob_from_opcode(JX_OPCODE_TAIL_CALL, 0));
                break;
              default:
                {
                  jx_ob new_entry = Jx_list_new_with_size(E, 2);
                  Jx_list_replace(E, new_entry, 0,
                                  jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
                  Jx_list_replace(E, new_entry, 1,
                                  jx_ident_split_with_dotted(E, entry));
                  entry = new_entry;
                }
                break;
              }
            } else {
              jx_ob new_entry = Jx_list_new_with_size(E, 2);
              Jx_list_replace(E, new_entry, 0,
                              jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
              Jx_list_replace(E, new_entry, 1, jx_ident_split_with_dotted(E, entry));
              entry = new_entry;
            }

          } else if(entry.meta.bits & JX_META_BIT_GC) {
            entry = jx__code_bind_with_source(E, prebind, entry, unresol_depth - 1);
          }
          Jx__list_replace(E, source_list, slot, entry);
        }
        /* special handling for "unresolved" slots */
        while((slot--) > 1) {
          jx_ob entry = jx__list_swap_with_null(source_list, slot);
          switch (resolution) {
          case JX_RESOLUTION_MAP_LVALUE:       /* (set (path_list path_list) (value_list)) */
            switch (slot) {
            case 1:
              {
                jx_int i, entry_size = jx_list_size(entry);
                for(i = 0; i < entry_size; i++) {
                  jx_ob entry_entry = jx_list_swap_with_null(entry, i);
                  if(jx_ident_check(entry_entry)) {
                    entry_entry = jx_ident_split_with_dotted(E, entry_entry);
                  } else {
                    entry_entry = jx__code_bind_with_source(E, prebind, entry_entry, 1);
                  }
                  Jx_list_replace(E, entry, i, entry_entry);
                }
              }
              break;
            default:
              entry = jx__code_bind_with_source(E, prebind, entry, 0);
              break;
            }
            break;
          case JX_RESOLUTION_LVALUE:   /* (set (lit lit (expr))) value) */
          case JX_RESOLUTION_RVALUE:   /* (get (lit (expr) lit (expr) (expr))) */
            switch (slot) {
            case 1:
              if(jx_ident_check(entry)) {
                entry = jx_ident_split_with_dotted(E, entry);
              } else {
                entry = jx__code_bind_with_source(E, prebind, entry, 1);
              }
              break;
            default:
              entry = jx__code_bind_with_source(E, prebind, entry, 0);
              break;
            }
            break;
          case JX_RESOLUTION_LAMDEF:
            switch (slot) {
            case 1:
              entry = jx__code_bind_with_source(E, prebind, entry, 2);        /* arg list */
              break;
            default:
              entry = jx__code_bind_with_source(E, prebind, entry, 1);
              break;
            }
            break;
          case JX_RESOLUTION_FUNDEF:
            switch (slot) {
            case 2:
              entry = jx__code_bind_with_source(E, prebind, entry, 2);        /* arg list */
              break;
            case 1:
              entry = jx__code_bind_with_source(E, prebind, entry, 1);        /* fn name */
              break;
            default:
              entry = jx__code_bind_with_source(E, prebind, entry, 1);
              break;
            }
            break;
          case JX_RESOLUTION_ENTITY:
          case JX_RESOLUTION_NORMAL:
          default:
            entry = jx__code_bind_with_source(E, prebind, entry, 0);
            break;
          }
          Jx__list_replace(JX_NULL, source_list, slot, entry);
        }
      }
      //      jx_jxon_dump(stdout,"post-bind",source);
      return source;
    }
    break;
  case JX_META_BIT_HASH:
    {                           /* TO DO refactor to avoid rehashing */
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_with_hash(source);       /* destroys source */
      jx_int size = jx_list_size(list);
      while(size) {
        jx_ob key = jx_list_take(list, 0);
        jx_ob value = jx_list_take(list, 0);
        size = size - 2;
        if(jx_ident_check(value) && (!unresol_depth)) {
          jx_ob new_entry = Jx_list_new_with_size(E, 2);
          Jx_list_replace(E, new_entry, 0,
                          jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
          Jx_list_replace(E, new_entry, 1, jx_ident_split_with_dotted(E, value));
          Jx_hash_set(E, result, key, new_entry);
        } else {
          Jx_hash_set(E, result, key,
                      jx__code_bind_with_source(E, prebind, value, unresol_depth - 1));
        }
      }
      Jx_ob_free(E, list);
      return result;
    }
    break;
  case JX_META_BIT_OPCODE:
  case JX_META_BIT_IDENT:
  case JX_META_BIT_BOOL:
  case JX_META_BIT_FLOAT:
  case JX_META_BIT_INT:
  case JX_META_BIT_STR:
    return source;
    break;
  default:                     /* unrecognized or invalid input -> null */
    Jx_ob_free(E, source);
    return jx_ob_from_null();
    break;
  }
}

jx_ob jx_node_code_bind_with_source(jx_ob node, jx_ob prebind, jx_ob source)
{
  jx_env env;
  jx_ob result;
  jx_env *E = jx_env_new_in_node(&env, node);
  if(jx_ident_check(source)) {  /* if code consists solely of a top-level identifier */
    jx_ob builtin = jx_hash_borrow(prebind, source);
    if(jx_builtin_check(builtin)) {     /* known builtin function (early / fixed binding) */
      result = Jx_list_new_with_size(E, 1);
      Jx_list_replace(E, result, 0, builtin);
      Jx_ob_free(E, source);
    } else {                    /* unknown identifier (fall back upon late binding) */
      result = Jx_list_new_with_size(E, 2);
      Jx_list_replace(E, result, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
      Jx_list_replace(E, result, 1, jx_ident_split_with_dotted(E, source));
    }
  } else {
    result = jx__code_bind_with_source(E, prebind, source, 0);
  }
  jx_env_free(E);
  return result;
}

jx_ob jx_code_bind_with_source(jx_ob prebind, jx_ob source)
{
  return jx_node_code_bind_with_source(jx_ob_from_null(), prebind, source);
}

static jx_status jx__code_unbound_from_code(jx_env * E, jx_ob unbound, jx_ob code)
{
  switch (JX_META_MASK_TYPE_BITS & code.meta.bits) {
  case JX_META_BIT_LIST:
    if(!code.data.io.list->packed_meta_bits) {  /* packed lists cannot contain code */
      jx_int size = jx_list_size(code);
      if(size) {
        jx_ob first = jx_list_borrow(code, 0);
        if(jx_builtin_selector_check(first)) {
          switch (first.data.io.int_) {
            /* all functions which (only) accept symbols need to be included here */
          case JX_SELECTOR_HAS:
          case JX_SELECTOR_GET:
          case JX_SELECTOR_TAKE:
          case JX_SELECTOR_DEL:
          case JX_SELECTOR_INCR:
          case JX_SELECTOR_DECR:
          case JX_SELECTOR_RESOLVE:
            {
              jx_ob sym = Jx_list_get(E, code, 1);
              if(jx_list_check(sym)) {
                Jx_ob_replace(E, &sym, Jx_list_get(E, sym, 0));
              }
              jx_list_unshift(unbound, sym);
            }
            break;
          }
        }
        while(size--) {
          jx__code_unbound_from_code(E, unbound, jx_list_borrow(code, size));
        }
      }
    }
    break;
  case JX_META_BIT_HASH:
    {
      jx_ob list = jx_list_new_from_hash(code); /* copies code */
      jx_int size = jx_list_size(list);
      while(size) {
        size = size - 2;
        jx__code_unbound_from_code(E, unbound, jx_list_borrow(list, size + 1));
      }
      Jx_ob_free(E, list);    /* ok */
    }
    break;
  }
  //jx_jxon_dump(stdout,"unbound",unbound);
  return JX_SUCCESS;
}

jx_ob jx_code_unbound_from_code(jx_ob code)
{
  jx_ob unbound = jx_list_new();
  jx__code_unbound_from_code(JX_NULL, unbound, code);
  return unbound;
}

JX_INLINE void jx__code_result_copy_weak_transients(jx_env * E, jx_ob * ob, jx_int size)
{
  while(size--) {
    if(ob->meta.bits & JX_META_BIT_WEAK_REF) {
      /* this is the point at which transient weak references either
         become genuine (potentially counted) weak references or are
         copied into strongly-referenced memory */
      *ob = JX_OWN(E, Jx_ob_copy(E, *ob));
    }
    ob++;
  }
}

JX_INLINE void jx_code_result_copy_weak_transients(jx_env * E, jx_ob ob)
{
  jx_int size = jx_list_size(ob);
  if(size) {
    jx_list *list = ob.data.io.list;
    if(!list->packed_meta_bits) {       /* normal object input */
      jx__code_result_copy_weak_transients(E, list->data.ob_vla, size);
    }
  }
}

JX_INLINE void jx__code_replace_payload(jx_env * E, jx_ob * ob, jx_ob result)
{
  jx_int size = jx_list_size(*ob);
  if(size) {
    jx_list *list = ob->data.io.list;
    if(!list->packed_meta_bits) {       /* normal object input */
      jx_ob *ob_vla = list->data.ob_vla;
      while(size--) {
        Jx_ob_replace_with_null(E, ob_vla++);
      }
    }
  }
  Jx_ob_replace(E, ob, result);
}

JX_INLINE jx_ob jx__code_apply_callable(jx_env * E, jx_ob callable, jx_ob payload)
{
  /* frees callable; payload is consumed */
  //    jx_jxon_dump(stdout,"apply callable",callable);
  //    jx_jxon_dump(stdout,"       payload",payload);
  switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
  case JX_META_BIT_BUILTIN_ENTITY:
    /* nothing should happen right now */
    jx_list_unshift(payload, callable); /* E */
    return payload;
    break;
  case JX_META_BIT_BUILTIN_NATIVE_FN:
    {
      jx_native_fn native_fn = callable.data.io.native_fn;
      if(native_fn) {
        jx_ob result = native_fn(E, Jx_ob_not_weak_with_ob(E, payload));
        Jx_ob_free(E, callable);
        return result;
      }
    }
    break;
  case JX_META_BIT_BUILTIN_FUNCTION:
    return jx__function_call(E, callable, Jx_ob_not_weak_with_ob(E, payload));
    break;
  case JX_META_BIT_BUILTIN_SELECTOR:
    {
      /* NOTE for performance reasons, the "safe" builtin
         selector functions pull from but do not free the
         payload object, hence the use of jx_replace below */
      switch (callable.data.io.int_) {
      case JX_SELECTOR_RETURN:
        E->leave = -1;
        if(jx_list_size(payload)) {
          E->have_result = JX_TRUE;
          jx__code_replace_payload(E, &E->result, Jx_ob_not_weak_with_ob
                                   (E, jx_list_take(payload, 0)));
        } else {
          jx__code_replace_payload(E, &payload, jx_ob_from_opcode(JX_OPCODE_RETURN, 0));
        }
        break;
      case JX_SELECTOR_TAIL:
        E->leave = -1;        /* break out of all blocks until we find a function */
        E->tail_call = JX_TRUE;
        if(jx_list_size(payload)) {
          E->have_result = JX_TRUE;
          jx__code_replace_payload(E, &E->result,
                                   Jx_ob_not_weak_with_ob(E, payload));
        } else {
          jx__code_replace_payload(E, &payload, jx_ob_from_opcode(JX_OPCODE_RETURN, 0));
        }
        payload = jx_ob_from_null();
        break;
      case JX_SELECTOR_NOP:
        /* simply returns payload as a list */
        break;
      case JX_SELECTOR_ENTITY:
        jx__code_replace_payload(E, &payload, jx_safe_entity(E, payload));
        break;
      case JX_SELECTOR_MAP_SET:
        jx__code_replace_payload(E, &payload, jx_safe_map_set(E, payload));
        break;
      case JX_SELECTOR_SET:
        jx__code_replace_payload(E, &payload, jx_safe_set(E, payload));
        break;
      case JX_SELECTOR_GET:
        jx__code_replace_payload(E, &payload, jx_safe_get(E, payload));
        break;
      case JX_SELECTOR_TAKE:
        jx__code_replace_payload(E, &payload, jx_safe_take(E, payload));
        break;
      case JX_SELECTOR_DEL:
        jx__code_replace_payload(E, &payload, jx_safe_del(E, payload));
        break;
      case JX_SELECTOR_SIZE:
        jx__code_replace_payload(E, &payload, jx_safe_size(payload));
        break;
      case JX_SELECTOR_SAME:
        jx__code_replace_payload(E, &payload, jx_safe_same(payload));
        break;
      case JX_SELECTOR_IDENTICAL:
        jx__code_replace_payload(E, &payload, jx_safe_identical(payload));
        break;
      case JX_SELECTOR_EQ:
        jx__code_replace_payload(E, &payload, jx_safe_eq(E, payload));
        break;
      case JX_SELECTOR_NE:
        jx__code_replace_payload(E, &payload, jx_safe_ne(E, payload));
        break;
      case JX_SELECTOR_LT:
        jx__code_replace_payload(E, &payload, jx_safe_lt(E, payload));
        break;
      case JX_SELECTOR_GT:
        jx__code_replace_payload(E, &payload, jx_safe_gt(E, payload));
        break;
      case JX_SELECTOR_LE:
        jx__code_replace_payload(E, &payload, jx_safe_le(E, payload));
        break;
      case JX_SELECTOR_GE:
        jx__code_replace_payload(E, &payload, jx_safe_ge(E, payload));
        break;
      case JX_SELECTOR_ADD:
        jx__code_replace_payload(E, &payload, jx_safe_add(E, payload));
        break;
      case JX_SELECTOR_SUB:
        jx__code_replace_payload(E, &payload, jx_safe_sub(payload));
        break;
      case JX_SELECTOR_MUL:
        jx__code_replace_payload(E, &payload, jx_safe_mul(E, payload));
        break;
      case JX_SELECTOR_DIV:
        jx__code_replace_payload(E, &payload, jx_safe_div(payload));
        break;
      case JX_SELECTOR_IDIV:
        jx__code_replace_payload(E, &payload, jx_safe_idiv(payload));
        break;
      case JX_SELECTOR_MOD:
        jx__code_replace_payload(E, &payload, jx_safe_mod(payload));
        break;
      case JX_SELECTOR_AND:
        jx__code_replace_payload(E, &payload, jx_safe_and(payload));
        break;
      case JX_SELECTOR_OR:
        jx__code_replace_payload(E, &payload, jx_safe_or(payload));
        break;
      case JX_SELECTOR_POW:
        jx__code_replace_payload(E, &payload, jx_safe_pow(payload));
        break;

      case JX_SELECTOR_NEG:
        jx__code_replace_payload(E, &payload, jx_safe_neg(payload));
        break;
      case JX_SELECTOR_NOT:
        jx__code_replace_payload(E, &payload, jx_safe_not(payload));
        break;

      case JX_SELECTOR_OUTPUT:
        jx__code_replace_payload(E, &payload, jx_safe_output(E, payload));
        break;
      case JX_SELECTOR_ERROR:
        jx__code_replace_payload(E, &payload, jx_safe_error(E, payload));
        break;

      case JX_SELECTOR_APPEND:
        jx__code_replace_payload(E, &payload, jx_safe_append(E, payload));
        break;
      case JX_SELECTOR_EXTEND:
        jx__code_replace_payload(E, &payload, jx_safe_extend(E, payload));
        break;
      case JX_SELECTOR_INSERT:
        jx__code_replace_payload(E, &payload, jx_safe_insert(E, payload));
        break;
      case JX_SELECTOR_RESIZE:
        jx__code_replace_payload(E, &payload, jx_safe_resize(E, payload));
        break;
      case JX_SELECTOR_POP:
        jx__code_replace_payload(E, &payload, jx_safe_pop(E, payload));
        break;
      case JX_SELECTOR_PUSH:
        jx__code_replace_payload(E, &payload, jx_safe_append(E, payload));
        break;
      case JX_SELECTOR_SHIFT:
        jx__code_replace_payload(E, &payload, jx_safe_shift(E, payload));
        break;
      case JX_SELECTOR_UNSHIFT:
        jx__code_replace_payload(E, &payload, jx_safe_unshift(E, payload));
        break;
      case JX_SELECTOR_SLICE:
        jx__code_replace_payload(E, &payload, jx_safe_slice(E, payload));
        break;
      case JX_SELECTOR_CUTOUT:
        jx__code_replace_payload(E, &payload, jx_safe_cutout(E, payload));
        break;
      case JX_SELECTOR_REVERSE:
        jx__code_replace_payload(E, &payload, jx_safe_reverse(E, payload));
        break;
      case JX_SELECTOR_SORT:
        jx__code_replace_payload(E, &payload, jx_safe_sort(E, payload));
        break;
      case JX_SELECTOR_IMPL:
        jx__code_replace_payload(E, &payload, jx_safe_impl(E, payload));
        break;
      case JX_SELECTOR_DECR:
        jx__code_replace_payload(E, &payload, jx_safe_decr(E, payload));
        break;
      case JX_SELECTOR_INCR:
        jx__code_replace_payload(E, &payload, jx_safe_incr(E, payload));
        break;
      case JX_SELECTOR_RANGE:
        jx__code_replace_payload(E, &payload, jx_safe_range(E, payload));
        break;
      case JX_SELECTOR_FILL:
        jx__code_replace_payload(E, &payload, jx_safe_fill(E, payload));
        break;
      case JX_SELECTOR_SYMBOLS:
        jx__code_replace_payload(E, &payload, jx_safe_symbols(E, payload));
        break;
      case JX_SELECTOR_HAS:
        jx__code_replace_payload(E, &payload, jx_safe_has(E, payload));
        break;
      case JX_SELECTOR_STR:
        jx__code_replace_payload(E, &payload, jx_safe_str(E, payload));
        break;
      case JX_SELECTOR_INT:
        jx__code_replace_payload(E, &payload, jx_safe_int(payload));
        break;
      case JX_SELECTOR_FLOAT:
        jx__code_replace_payload(E, &payload, jx_safe_float(payload));
        break;
      case JX_SELECTOR_BOOL:
        jx__code_replace_payload(E, &payload, jx_safe_bool(payload));
        break;
      case JX_SELECTOR_SYNCHRONIZE:
        jx__code_replace_payload(E, &payload, jx_safe_synchronize(E, payload));
        break;
      case JX_SELECTOR_SYNCHRONIZED:
        jx__code_replace_payload(E, &payload, jx_safe_synchronized(E, payload));
        break;
      case JX_SELECTOR_SHARE:
        jx__code_replace_payload(E, &payload, jx_safe_share(E, payload));
        break;
      case JX_SELECTOR_SHARED:
        jx__code_replace_payload(E, &payload, jx_safe_shared(E, payload));
        break;
      case JX_SELECTOR_ASSERT:
        jx__code_replace_payload(E, &payload, jx_safe_assert(E, payload));
        break;
      case JX_SELECTOR_DUMP:
        jx__code_replace_payload(E, &payload, jx_safe_dump(E, payload));
        break;
      case JX_SELECTOR_NEW:
        payload = Jx_ob_with_new(E, payload);
        break;
      case JX_SELECTOR_NULL_OP:
        jx__code_replace_payload(E, &payload, jx_ob_from_null());
        break;
      default:                 /* unrecognized selector? purge weak */
        Jx_ob_free(E, callable);
        jx__code_replace_payload(E, &payload, jx_ob_from_null());
        break;
      }
    }
    return payload;
    break;
  }
  Jx_ob_free(E, callable);
  Jx_ob_free(E, payload);

  return jx_ob_from_null();
}

JX_INLINE jx_ob jx__code_reduce(jx_env * E, jx_int flags, jx_ob callable, jx_ob inp)
{
  jx_ob out = jx_ob_from_null();
  if(jx_builtin_callable_check(callable) && jx_list_check(inp)) {
    register jx_int i, size = jx_list_size(inp);
    jx_ob arg_ob = Jx_list_new_with_size(E, 2);
    if(jx_list_size(inp) && jx_list_size(arg_ob)) {
      jx_ob *left_ob = arg_ob.data.io.list->data.ob_vla;
      jx_ob *right_ob = left_ob + 1;
      jx_list *inp_list = inp.data.io.list;
      jx_ob callable_weak_ref = jx_ob_take_weak_ref(callable);
      if(!inp_list->packed_meta_bits) { /* normal object input */
        jx_ob *inp_ob = inp_list->data.ob_vla;
        *left_ob = *(inp_ob++);
        switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i = 1; i < size; i++) {
                Jx_ob_replace(E, right_ob, *(inp_ob++)); /* *right_ob = *(inp_ob++); */
                Jx_ob_replace(E, left_ob,
                              native_fn(E, Jx_ob_copy(E, arg_ob)));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i = 1; i < size; i++) {
            Jx_ob_replace(E, right_ob, *(inp_ob++)); /* *right_ob = *(inp_ob++); */
            Jx_ob_replace(E, left_ob,
                          jx__function_call(E,
                                            callable_weak_ref, Jx_ob_copy(E, arg_ob)));
          }
          break;
        default:
          for(i = 1; i < size; i++) {
            Jx_ob_replace(E, right_ob, *(inp_ob++)); /* *right_ob = *(inp_ob++); */
            Jx_ob_replace(E, left_ob,
                          jx__code_apply_callable
                          (E, callable_weak_ref, Jx_ob_copy(E, arg_ob)));
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) {
        /* packed integer input */
        jx_int *inp_int = inp_list->data.int_vla;
        *left_ob = jx_ob_from_int(0);
        *right_ob = jx_ob_from_int(0);
        left_ob->data.io.int_ = *(inp_int++);
        switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i = 1; i < size; i++) {
                right_ob->data.io.int_ = *(inp_int++);
                Jx_ob_replace(E, left_ob,
                              native_fn(E, Jx_ob_copy(E, arg_ob)));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i = 1; i < size; i++) {
            right_ob->data.io.int_ = *(inp_int++);
            Jx_ob_replace(E, left_ob,
                          jx__function_call
                          (E, callable_weak_ref, Jx_ob_copy(E, arg_ob)));
          }
          break;
        default:
          for(i = 1; i < size; i++) {
            right_ob->data.io.int_ = *(inp_int++);
            Jx_ob_replace(E, left_ob,
                          jx__code_apply_callable
                          (E, callable_weak_ref, Jx_ob_copy(E, arg_ob)));
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) {
        /* packed float input */
        jx_float *inp_float = inp_list->data.float_vla;
        *left_ob = jx_ob_from_float(0.0);
        *right_ob = jx_ob_from_float(0.0);
        left_ob->data.io.float_ = *(inp_float++);
        switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i = 1; i < size; i++) {
                right_ob->data.io.float_ = *(inp_float++);
                Jx_ob_replace(E, left_ob,
                              native_fn(E, Jx_ob_copy(E, arg_ob)));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i = 1; i < size; i++) {
            right_ob->data.io.float_ = *(inp_float++);
            Jx_ob_replace(E, left_ob,
                          jx__function_call
                          (E, callable_weak_ref, Jx_ob_copy(E, arg_ob)));
          }
          break;
        default:
          for(i = 1; i < size; i++) {
            right_ob->data.io.float_ = *(inp_float++);
            Jx_ob_replace(E, left_ob,
                          jx__code_apply_callable
                          (E, callable_weak_ref, Jx_ob_copy(E, arg_ob)));
          }
          break;
        }
      }
      out = jx_list_take(arg_ob, 0);
      jx_vla_reset(&inp_list->data.ob_vla);
    }
    Jx_ob_free(E, inp);
    Jx_ob_free(E, arg_ob);
    Jx_ob_free(E, callable);
  } else {
    /* trying to apply a non-function -- that just won't work */
    Jx_ob_free(E, inp);
    Jx_ob_free(E, callable);
  }
  return out;
}

typedef struct {
  jx_int id;
  jx_ob callable;
  jx_ob node;
  jx_ob payload;
} jx_thread_info;

static void *thread_call(void *id_ptr)
{
  jx_thread_info *info = (jx_thread_info *) id_ptr;
  jx_env env;
  jx_env *E = jx_env_new_in_node(&env, info->node);

  info->payload = jx__code_apply_callable(E, info->callable, info->payload);

  jx_env_free(E);
  //  printf("thread %d complete\n",info->id);
  return NULL;
}

static void *thread_eval(void *id_ptr)
{
  jx_thread_info *info = (jx_thread_info *) id_ptr;
  jx_env env;
  jx_env *E = jx_env_new_in_node(&env, info->node);

  info->payload = Jx_code_eval(E, JX_EVAL_RESUME_INVOCATION, info->callable);
  jx_ob_free(info->callable);

  jx_env_free(E);
  //  printf("thread %d complete\n",info->id);
  return NULL;
}

JX_INLINE jx_ob jx__code_pareval(jx_env * E, jx_int flags, jx_ob src_list)
{
  jx_ob result = jx_ob_from_null();
  if(jx_ob_identical(jx_list_borrow(src_list, 0),
                     jx_builtin_new_from_selector(JX_SELECTOR_NOP)))
    Jx_list_del(E, src_list, 0);
  jx_int size = jx_list_size(src_list);
  if(size) {
    result = Jx_list_new_with_size(E, size);
    jx_os_thread *thread_array = NULL;
    jx_thread_info *thread_info =
      (jx_thread_info *) jx_vla_new(sizeof(jx_thread_info), size);
    if(jx_ok(jx_os_thread_array_new(&thread_array, size))) {
      jx_int i;
      for(i = 0; i < size; i++) {
        jx_os_thread *thread = jx_os_thread_array_entry(thread_array, i);
        thread_info[i].id = i;
        thread_info[i].node = E->node;
        thread_info[i].callable = jx_list_swap_with_null(src_list, i);
        thread_info[i].payload = jx_ob_from_null();
        jx_os_thread_start(thread, thread_eval, thread_info + i);
      }
      {
        jx_int i;
        for(i = 0; i < size; i++) {
          jx_os_thread *thread = jx_os_thread_array_entry(thread_array, i);
          jx_os_thread_join(thread);
          Jx_list_replace(E, result, i, thread_info[i].payload);
        }
      }
      jx_os_thread_array_free(&thread_array);
      jx_vla_free(&thread_info);
    }
  }
  Jx_ob_free(E, src_list);
  return result;
}

JX_INLINE jx_ob jx__code_paramap1(jx_env * E, jx_int flags, jx_ob callable, jx_ob inp)
{
  jx_ob result = jx_ob_from_null();
  jx_int size = jx_list_size(inp);
  if(jx_builtin_callable_check(callable) && size) {
    result = Jx_list_new_with_size(E, size);
    jx_os_thread *thread_array = NULL;
    jx_thread_info *thread_info =
      (jx_thread_info *) jx_vla_new(sizeof(jx_thread_info), size);
    if(jx_ok(jx_os_thread_array_new(&thread_array, size))) {
      jx_int i;
      for(i = 0; i < size; i++) {
        jx_os_thread *thread = jx_os_thread_array_entry(thread_array, i);
        thread_info[i].id = i;
        thread_info[i].node = E->node;
        thread_info[i].callable = callable;
        thread_info[i].payload = jx_list_swap_with_null(inp, i);
        jx_os_thread_start(thread, thread_call, thread_info + i);
      }
      {
        jx_int i;
        for(i = 0; i < size; i++) {
          jx_os_thread *thread = jx_os_thread_array_entry(thread_array, i);
          jx_os_thread_join(thread);
          Jx_list_replace(E, result, i, thread_info[i].payload);
        }
      }
      jx_os_thread_array_free(&thread_array);
      jx_vla_free(&thread_info);
    }
  }
  Jx_ob_free(E, inp);
  Jx_ob_free(E, callable);
  return result;
}

JX_INLINE jx_ob jx__code_map1(jx_env * E, jx_int flags, jx_ob callable, jx_ob inp)
{
  if(jx_builtin_callable_check(callable) && jx_list_check(inp)) {
    register jx_int i, size = jx_list_size(inp);
    jx_ob out = Jx_list_new_with_size(E, size);
    if(jx_list_check(out) && jx_list_size(inp)) {
      jx_list *inp_list = inp.data.io.list;
      jx_list *out_list = out.data.io.list;
      jx_ob callable_weak_ref = jx_ob_take_weak_ref(callable);
      if(!inp_list->packed_meta_bits) { /* normal object input */
        jx_ob *inp_ob = inp_list->data.ob_vla;
        jx_ob *out_ob = out_list->data.ob_vla;
        switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i = 0; i < size; i++) {
                *(out_ob++) = native_fn(E, *(inp_ob++));
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i = 0; i < size; i++) {
            *(out_ob++) = jx__function_call(E, callable_weak_ref, *(inp_ob++));
          }
          break;
        default:
          for(i = 0; i < size; i++) {
            *(out_ob++) = jx__code_apply_callable(E, callable_weak_ref, *(inp_ob++));
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) {
        /* packed integer input */
        jx_int *inp_int = inp_list->data.int_vla;
        jx_ob *out_ob = out_list->data.ob_vla;
        jx_ob inp_ob = jx_ob_from_int_zero();
        switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i = 0; i < size; i++) {
                inp_ob.data.io.int_ = *(inp_int++);
                *(out_ob++) = native_fn(E, inp_ob);
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i = 0; i < size; i++) {
            inp_ob.data.io.int_ = *(inp_int++);
            *(out_ob++) = jx__function_call(E, callable_weak_ref, inp_ob);
          }
          break;
        default:
          for(i = 0; i < size; i++) {
            inp_ob.data.io.int_ = *(inp_int++);
            *(out_ob++) = jx__code_apply_callable(E, callable_weak_ref, inp_ob);
          }
          break;
        }
      } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) {
        /* packed float input */
        jx_float *inp_float = inp_list->data.float_vla;
        jx_ob *out_ob = out_list->data.ob_vla;
        jx_ob inp_ob = jx_ob_from_float_zero();
        switch (callable.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
        case JX_META_BIT_BUILTIN_NATIVE_FN:
          {
            jx_native_fn native_fn = callable.data.io.native_fn;
            if(native_fn) {
              for(i = 0; i < size; i++) {
                inp_ob.data.io.float_ = *(inp_float++);
                *(out_ob++) = native_fn(E, inp_ob);
              }
            }
          }
          break;
        case JX_META_BIT_BUILTIN_FUNCTION:
          for(i = 0; i < size; i++) {
            inp_ob.data.io.float_ = *(inp_float++);
            *(out_ob++) = jx__function_call(E, callable_weak_ref, inp_ob);
          }
          break;
        default:
          for(i = 0; i < size; i++) {
            inp_ob.data.io.float_ = *(inp_float++);
            *(out_ob++) = jx__code_apply_callable(E, callable_weak_ref, inp_ob);
          }
          break;
        }
      } else {                  /* fallback: slower but safe */
        for(i = 0; i < size; i++) {
          Jx__list_replace
            (E, out_list, i, jx__code_apply_callable
             (E, callable_weak_ref, jx__list_swap_with_null(inp_list, i)));
        }
      }
      Jx_list_repack(E, out);
      jx_vla_reset(&inp_list->data.ob_vla);
    }
    Jx_ob_free(E, inp);
    Jx_ob_free(E, callable);
    return out;
  } else {                      /* trying to apply a non-function -- that just won't work */
    Jx_ob_free(E, inp);
    Jx_ob_free(E, callable);
    return jx_ob_from_null();
  }
}

JX_INLINE jx_ob jx__code_mapN(jx_env * E, jx_int flags, jx_ob callable, jx_ob src_list)
{
  jx_size max_size = 0;
  jx_int n_src = jx_list_size(src_list);
  {
    jx_int i;
    for(i = 0; i < n_src; i++) {
      jx_int size = jx_list_size(jx_list_borrow(src_list, i));
      if(size > max_size)
        max_size = size;
    }
  }
  {
    jx_ob template = jx_list_borrow(jx_list_borrow(src_list, 0), 0);
    jx_ob arg_ob = Jx_list_new_with_size(E, n_src);
    jx_ob callable_weak_ref = jx_ob_take_weak_ref(callable);
    jx_ob out;
    if(!jx_gc_check(template)) {        /* try to create list with proper receiver type */
      out = Jx_list_new_with_fill(E, max_size, template);
    } else {
      out = Jx_list_new_with_size(E, max_size);
    }
    if(jx_list_size(out)) {
      jx_int i, j;
      for(i = 0; i < max_size; i++) {
        for(j = 0; j < n_src; j++) {
          Jx_list_replace(E, arg_ob, j, jx_ob_take_weak_ref
                          (jx_list_borrow(jx_list_borrow(src_list, j), i)));
        }
        Jx_list_replace(E, out, i, jx__code_apply_callable
                        (E, callable_weak_ref, Jx_ob_copy(E, arg_ob)));
      }
      Jx_list_repack(E, out);
    }
    Jx_ob_free(E, arg_ob);
    Jx_ob_free(E, src_list);
    Jx_ob_free(E, callable);
    return out;
  }
}

JX_INLINE jx_ob jx__code_curry_method(jx_env * E, jx_ob instance, jx_ob method)
{
  if(jx_null_check(method)) {
    return jx_ob_from_null();
  } else {
    jx_ob names = Jx_hash_new(E);
    jx_ob code = Jx_list_new(E);

    Jx_hash_set(E, names, jx_ob_from_ident("self"), Jx_ob_copy(E, instance));
    Jx_hash_set(E, names, jx_ob_from_ident("func"), Jx_ob_copy(E, method));

    {
      jx_ob unshift = Jx_list_new(E);
      Jx_list_append(E, unshift, jx_builtin_new_from_selector(JX_SELECTOR_UNSHIFT));
      {
        jx_ob tmp1 = Jx_list_new(E);
        Jx_list_append(E, tmp1, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
        Jx_list_append(E, tmp1, jx_ob_from_ident("_"));
        Jx_list_append(E, unshift, tmp1);
      }
      {
        jx_ob tmp1 = Jx_list_new(E);
        Jx_list_append(E, tmp1, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
        Jx_list_append(E, tmp1, jx_ob_from_ident("self"));
        Jx_list_append(E, unshift, tmp1);
      }
      Jx_list_append(E, code, unshift);
    }
    {
      jx_ob apply = Jx_list_new(E);

      Jx_list_append(E, apply, jx_builtin_new_from_selector(JX_SELECTOR_APPLY));
      {
        jx_ob tmp1 = Jx_list_new(E);
        Jx_list_append(E, tmp1, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
        Jx_list_append(E, tmp1, jx_ob_from_ident("func"));
        Jx_list_append(E, apply, tmp1);
      }
      {
        jx_ob tmp1 = Jx_list_new(E);
        Jx_list_append(E, tmp1, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
        Jx_list_append(E, tmp1, jx_ob_from_ident("_"));
        Jx_list_append(E, apply, tmp1);
      }
      Jx_list_append(E, code, apply);
    }
    return jx_function_new_with_def(jx_ob_from_null(),
                                    names, code, JX_FUNCTION_MODE_EXEC);
  }
}

static jx_ob jx__code_close_with_args(jx_env * E, jx_ob code, jx_ob args)
{
  jx_ob unbound = jx_code_unbound_from_code(code);
  if(jx_list_check(args)) {
    jx_ob closed = jx_hash_new();
    jx_int i, size = jx_list_size(unbound);
    for(i = 0; i < size; i++) {
      jx_ob ident = jx_list_borrow(unbound, i);
      jx_ob value = jx_ob_from_null();
      if(Jx_peek_resolved(E, &value, ident)) {

        /* one important thing to be aware of: shared and synchronized
           containers are closed by reference.  Delete the container
           and you invalidate the reference, which will generate a segfault.

           JUST SO YOU KNOW!

           By the way, you should seriously consider implementing
           reference counting for shared and synchronized containers.
         */

        Jx_hash_set(E, closed, Jx_ob_copy(E, ident), Jx_ob_copy(E, value));

#if 0
        WE NO LONGER CLOSE ON ENTITY DEFS {
          jx_ob ent_def;
          while(jx_builtin_entity_check(value)) {
            /* close on chained entity hashes as well */
            if(Jx_peek_resolved(E, &ent_def, value)) {
              Jx_hash_set(E, closed, Jx_ob_copy(E, value), Jx_ob_copy(E, ent_def));
              value = jx_list_borrow(ent_def, 0);
            } else {
              break;
            }
          }
        }
#endif

      }
    }
    {
      jx_ob kw_args = jx_list_borrow(args, 1);
      if(!jx_hash_check(kw_args)) {
        jx_ob new_args = Jx_list_new(E);
        Jx_list_append(E, new_args, args);
        Jx_list_append(E, new_args, closed);
        args = new_args;
      } else {
        jx_ob iter = jx_list_new_from_hash(kw_args);
        jx_int i, size = jx_list_size(iter);
        for(i = 0; i < size; i += 2) {
          Jx_hash_set(E, closed,
                      jx_list_swap_with_null(iter, i),
                      jx_list_swap_with_null(iter, i + 1));
        }
        Jx_ob_free(E, iter);
        Jx_list_replace(E, args, 1, closed);
      }
    }
  }
  Jx_ob_free(E, unbound);
  return args;
}

static jx_ob Jx__code_eval_to_weak(jx_env * E, jx_int flags, jx_ob expr);
JX_INLINE jx_ob Jx_code_eval_to_weak(jx_env * E, jx_int flags, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    Jx__code_eval_to_weak(E, flags, expr) : expr;
}

static jx_ob Jx__code_eval_to_weak(jx_env * E, jx_int flags, jx_ob expr)
{
  /* E may NOT be NULL at this point */
  if(flags & JX_EVAL_DEBUG_MASK) {
    if(flags & JX_EVAL_DEBUG_TRACE) {
      fprintf(stderr, "# trace: eval_to_weak(E %p, flags 0x%x, ...)\n",
              (void *) E, (int) flags);
    }
    if(flags & (JX_EVAL_DEBUG_DUMP_NODE)) {
      jx_jxon_dump_in_node(stderr, "#   node", E->node, E->node);
    }
    if(flags & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
      jx_jxon_dump_in_node(stderr, "#   expr", E->node, expr);
    }
  }
  switch (expr.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_LIST:
    {
      jx_list *expr_list = expr.data.io.list;
      if(expr_list->packed_meta_bits || !jx__list_size(expr_list)) {
        /* empty and/packed lists cannot contain any expressions */
        return Jx_ob_copy(E, expr);
      } else {
        jx_ob *expr_vla = expr_list->data.ob_vla;
        jx_fast_bits bits = 0;
        /* first we check for a special form in slot 0 */
        if(jx_builtin_callable_check(expr_vla[0]) &&
           ((bits = expr_vla->meta.bits) & JX_META_BIT_BUILTIN_SELECTOR) &&
           (expr_vla->data.io.int_ < JX_SELECTOR_SPECIAL_FORMS_LIMIT) &&
           ((!(flags & JX_EVAL_DEFER_INVOCATION)) ||
            (expr_vla->data.io.int_ == JX_SELECTOR_RESOLVE))) {
          jx_int size = jx_vla_size(&expr_vla);
          jx_int selector;
          if((selector = expr_vla->data.io.int_) == JX_SELECTOR_RESOLVE) {
            /* [*resolve* ident] */
            jx_ob symbol = (size > 1) ? expr_vla[1] : jx_ob_from_null();
            jx_ob container = Jx_scope_borrow(E);
            jx_ob target = symbol;
            if(JX_OK(jx__resolve_container(E, &container, &target))) {
              /* looked-up objects are always returned as weak
                 references...  it's EVAL's job to make sure they
                 don't leak into persistent containers */
              jx_ob weak = jx_ob_take_weak_ref(container);
              if(flags & JX_EVAL_DEBUG_TRACE) {
                jx_jxon_dump_in_node(stderr, "# rsolvd", E->node, weak);
              }
              return weak;
            } else {
              if(jx_list_check(symbol)) {
                return jx_ob_from_null();
                //return Jx_ident_new_from_dotted(E, symbol);
              } else
                return Jx_ob_copy(E, symbol);
            }
          } else {
            //          printf("found special form %d\n",expr_vla->data.io.int_);
            switch (expr_vla->data.io.int_) {
            case JX_SELECTOR_BUILTINS: /* return available builtin functions */
              return jx_hash_get(E->node, jx_builtins());
              break;
            case JX_SELECTOR_QUOTE:
            case JX_SELECTOR_RAW:
              {
                jx_ob payload = (size > 1) ? expr_vla[1] : jx_ob_from_null();
                return Jx_ob_copy(E, payload);
              }
              break;
            case JX_SELECTOR_BIND:
              {
                jx_ob source = (size > 1) ?
                  Jx_code_eval_to_weak(E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob builtins = jx_hash_borrow(E->node, jx_builtins());
                return jx_node_code_bind_with_source(E->node, builtins, source);
              }
              break;
            case JX_SELECTOR_PASS:
              {
                return (size > 1) ?
                  Jx_code_eval_to_weak(E, flags, expr_vla[1]) : jx_ob_from_null();
              }
              break;
            case JX_SELECTOR_EVAL:
              {
                jx_ob ob = (size > 1) ?
                  Jx_code_eval_to_weak(E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob result = Jx_code_eval_to_weak(E, flags, ob);
                Jx_ob_free(E, ob);
                return result;
              }
              break;
            case JX_SELECTOR_DEBUG:
              {
                jx_int new_flags =
                  flags | JX_EVAL_DEBUG_DUMP_SUBEX | JX_EVAL_DEBUG_TRACE;
                jx_ob result = (size > 1) ? Jx_code_eval_to_weak(E, new_flags,
                                                                 expr_vla[1]) :
                  jx_ob_from_null();
                return result;
              }
              break;
            case JX_SELECTOR_EXEC:
              return ((size > 1) ? Jx_code_exec(E, flags, expr_vla[1]) :
                      jx_ob_from_null());
              break;
            case JX_SELECTOR_IF:       /* [*if* expr code expr code ... code] */
              {
                int i = 1;
                while(i < size) {
                  jx_ob cond = ((size > 1) ?
                                Jx_code_eval_to_weak(E, flags, expr_vla[i]) :
                                jx_ob_from_null());
                  jx_bool cond_bool = jx_ob_as_bool(cond);
                  Jx_ob_free(E, cond);
                  i += 2;
                  if(cond_bool) {
                    return ((size > 2) ? Jx_code_exec(E, flags, expr_vla[i - 1]) :
                            jx_ob_from_null());
                  } else if(size == i) {
                    break;
                  } else if(size == i + 1) {
                    return Jx_code_exec(E, flags, expr_vla[i]);
                  }
                }
                return jx_ob_from_null();
              }
              break;
            case JX_SELECTOR_TEST:     /* [*test* expr expr expr] */
              {
                jx_ob cond = (size > 1) ? Jx_code_eval_to_weak(E, flags,
                                                               expr_vla[1]) :
                  jx_ob_from_null();
                jx_bool cond_bool = jx_ob_as_bool(cond);
                Jx_ob_free(E, cond);
                if(cond_bool) {
                  return (size > 2) ? Jx_code_eval_to_weak(E, flags,
                                                           expr_vla[2]) :
                    jx_ob_from_null();
                } else {
                  return (size > 3) ? Jx_code_eval_to_weak(E, flags,
                                                           expr_vla[3]) :
                    jx_ob_from_null();
                }
              }
              break;
            case JX_SELECTOR_DEF:      /* [def name node code] */
            case JX_SELECTOR_DEFUN:    /* [defun name node expr] */
              {
                jx_ob name = (size > 1) ?
                  Jx_ob_copy(E, expr_vla[1]) : jx_ob_from_null();
                jx_ob args = (size > 2) ?
                  Jx_code_eval(E, flags, expr_vla[2]) : jx_ob_from_null();
                jx_ob body = (size > 3) ?
                  Jx_ob_copy(E, expr_vla[3]) : jx_ob_from_null();
                jx_ob key = jx_ident_split_from_dotted(E, name);

                args = jx__code_close_with_args(E, body, args);

                {
                  jx_ob function = jx_function_new_with_def
                    (name, args, body, expr_vla->data.io.int_ == JX_SELECTOR_DEF);
                  if(!jx_ok
                     (jx_set_from_path_with_value
                      (E, Jx_scope_borrow(E), key, function))) {
                    Jx_ob_free(E, function);
                  }
                }
                Jx_ob_free(E, key);
                return jx_ob_from_null();
              }
              break;
            case JX_SELECTOR_CODE:     /* [code args code] */
            case JX_SELECTOR_LAMBDA:   /* [expr args expr] */
              {
                jx_ob name = jx_ob_from_null();
                jx_ob args =
                  (size > 1) ? Jx_ob_copy(E, expr_vla[1]) : jx_ob_from_null();
                jx_ob body =
                  (size > 2) ? Jx_ob_copy(E, expr_vla[2]) : jx_ob_from_null();

                args = jx__code_close_with_args(E, body, args);

                return jx_function_new_with_def
                  (name, args, body, expr_vla->data.io.int_ == JX_SELECTOR_CODE);
              }
              break;
            case JX_SELECTOR_DEFMAC:   /* [defmac name node expr] */
              {
                jx_ob name =
                  (size > 1) ? Jx_ob_copy(E, expr_vla[1]) : jx_ob_from_null();
                jx_ob args =
                  (size > 2) ? Jx_ob_copy(E, expr_vla[2]) : jx_ob_from_null();
                jx_ob body =
                  (size > 3) ? Jx_ob_copy(E, expr_vla[3]) : jx_ob_from_null();
                jx_ob key = jx_ident_split_from_dotted(E, name);
                jx_ob function = jx_macro_new_with_def(name, args, body);
                //jx_ob_dump(stdout,"macro",function);
                if(!jx_ok
                   (jx_set_from_path_with_value
                    (E, Jx_scope_borrow(E), key, function))) {
                  Jx_ob_free(E, function);
                }
                Jx_ob_free(E, key);
                return jx_ob_from_null();
              }
              break;
            case JX_SELECTOR_DISPATCH: /* [dispatch index [callable0 ...] payload ] */
              {
                jx_ob index = (size > 1) ?
                  Jx_code_eval_to_weak(E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob table = (size > 2) ? Jx_code_eval_to_weak(E,
                                                                flags |
                                                                JX_EVAL_ALLOW_NESTED_WEAK_REFS,
                                                                expr_vla[2]) :
                  jx_ob_from_null();
                jx_ob payload = (size > 3) ? Jx_code_eval_to_weak(E, flags,
                                                                  expr_vla[3]) :
                  jx_ob_from_null();
                //              jx_jxon_dump(stdout,"table",table);
                jx_ob callable = jx_list_swap_with_null(table, jx_ob_as_int(index));
                jx_ob result = jx_ob_from_null();
                if(jx_builtin_callable_check(callable)) {
                } else {
                  Jx_ob_free(E, callable);
                  Jx_ob_free(E, payload);
                }
                Jx_ob_free(E, index);
                Jx_ob_free(E, table);
                return result;
              }
              break;
            case JX_SELECTOR_SWITCH:   /* [switch index [code0 code1 code2]] */
              {
                jx_ob index = (size > 1) ?
                  Jx_code_eval_to_weak(E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob table = (size > 2) ? expr_vla[2] : jx_ob_from_null();
                jx_int idx = jx_ob_as_int(index);
                Jx_ob_free(E, index);
                return Jx_code_exec(E, flags, jx_list_borrow(table, idx));
              }
              break;
            case JX_SELECTOR_SELECT:   /* [select index [expr0 expr1 expr2]] */
              {
                jx_ob index = (size > 1) ?
                  Jx_code_eval_to_weak(E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob table = (size > 2) ? expr_vla[2] : jx_ob_from_null();
                jx_int idx = jx_ob_as_int(index);
                Jx_ob_free(E, index);
                return Jx_code_eval_to_weak(E, flags, jx_list_borrow(table, idx));
              }
              break;
            case JX_SELECTOR_WHILE:
              {
                jx_ob cond = (size > 1) ? expr_vla[1] : jx_ob_from_null();
                jx_ob body = (size > 2) ? expr_vla[2] : jx_ob_from_null();
                jx_ob result = jx_ob_from_null();
                E->break_seen = JX_FALSE;
                while(!(E->break_seen || E->leave)) {
                  jx_ob test = Jx_code_eval_to_weak(E, flags, cond);
                  jx_int tst = jx_ob_as_bool(test);
                  Jx_ob_free(E, test);
                  if(tst) {
                    Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                  } else
                    break;
                }
                E->break_seen = JX_FALSE;
                return result;
              }
              break;
            case JX_SELECTOR_DO:
              {
                jx_ob body = (size > 1) ? expr_vla[1] : jx_ob_from_null();
                jx_ob cond = (size > 2) ? expr_vla[2] : jx_ob_from_null();
                jx_ob result = jx_ob_from_null();
                jx_int tst = 0;
                do {
                  Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                  jx_ob test = Jx_code_eval_to_weak(E, flags, cond);
                  tst = jx_ob_as_bool(test);
                  Jx_ob_free(E, test);
                } while(tst);
                return result;
              }
              break;
            case JX_SELECTOR_FOR:
              {
                jx_ob init = (size > 1) ? expr_vla[1] : jx_ob_from_null();
                jx_ob cond = (size > 2) ? expr_vla[2] : jx_ob_from_null();
                jx_ob step = (size > 3) ? expr_vla[3] : jx_ob_from_null();
                jx_ob body = (size > 4) ? expr_vla[4] : jx_ob_from_null();
                jx_ob result = jx_ob_from_null();
                jx_int tst = JX_TRUE;
                Jx_ob_free(E, Jx_code_eval_to_weak(E, flags, init));
                while(tst) {
                  jx_ob test = Jx_code_eval_to_weak(E, flags, cond);
                  if(jx_opcode_check(test))
                    break;
                  tst = jx_ob_as_bool(test);
                  Jx_ob_free(E, test);
                  if(tst) {
                    Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                  }
                  Jx_ob_free(E, Jx_code_eval(E, flags, step));
                }
                return result;
              }
              break;
            case JX_SELECTOR_FOREACH:  /* does not currently support break... */
              {
                jx_ob expr =
                  (size > 1) ? jx_ob_take_weak_ref(expr_vla[1]) : jx_ob_from_null();
                jx_ob list = (size > 2) ? Jx_code_eval_to_weak(E, flags,
                                                               expr_vla[2]) :
                  jx_ob_from_null();
                jx_ob body = (size > 3) ? expr_vla[3] : jx_ob_from_null();
                jx_ob result = jx_ob_from_null();
                jx_list *inp_list = list.data.io.list;
                jx_int i, size = jx_list_size(list);
                jx_ob scope = Jx_scope_borrow(E);
                if(size) {
                  if(jx_list_check(expr)) {     /* compound */
                    jx_int j, expr_size = jx_list_size(expr);
                    jx_ob *ob = inp_list->data.ob_vla;
                    for(i = 0; i < size; i++) {
                      for(j = 0; j < expr_size; j++) {
                        Jx_hash_set(E, scope,
                                    Jx_list_get(E, expr, j),
                                    Jx_list_get(E, *ob, j));
                      }
                      ob++;
                      Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                    }
                  } else {
                    if(!inp_list->packed_meta_bits) {   /* normal object input */
                      jx_ob *ob = inp_list->data.ob_vla;
                      for(i = 0; i < size; i++) {
                        Jx_hash_set(E, scope, expr, jx_ob_copy(*(ob++)));
                        Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                      }
                    } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) {
                      /* packed integer input */
                      jx_ob ob = jx_ob_from_int_zero();
                      jx_int *inp_int = inp_list->data.int_vla;
                      for(i = 0; i < size; i++) {
                        ob.data.io.int_ = *(inp_int++);
                        Jx_hash_set(E, scope, expr, ob);
                        Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                      }
                    } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) {
                      jx_ob ob = jx_ob_from_float_zero();
                      jx_float *inp_float = inp_list->data.float_vla;
                      for(i = 0; i < size; i++) {
                        ob.data.io.float_ = *(inp_float++);
                        Jx_hash_set(E, scope, expr, ob);
                        Jx_ob_replace(E, &result, Jx_code_exec(E, flags, body));
                      }
                    }
                  }
                }
                Jx_ob_free(E, list);
                return result;
              }
              break;
            case JX_SELECTOR_APPLY:    /* [apply callable payload] TODO: keyword arg list */
              {
                jx_ob callable = (size > 1) ? Jx_code_eval_to_weak
                  (E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob payload = (size > 2) ? Jx_code_eval(E, flags, expr_vla[2]) :
                  jx_ob_from_null();
                if(jx_builtin_callable_check(callable)) {
                  jx_ob result = jx__code_apply_callable(E, callable, payload);
                  //jx_jxon_dump(stdout,"apply_result",result);
                  return result;
                } else {
                  Jx_ob_free(E, callable);
                  Jx_ob_free(E, payload);
                  return jx_ob_from_null();
                }
              }
              break;
            case JX_SELECTOR_PAREVAL:  /* [expr_list ...] */
              {
                jx_ob src_list = (size > 1) ? Jx_code_eval_to_weak
                  (E, flags | JX_EVAL_DEFER_INVOCATION, expr_vla[1]) :
                  jx_ob_from_null();
                return jx__code_pareval(E, flags, src_list);
              }
              break;
            case JX_SELECTOR_PARASET:  /* [path_list expr_list ...] */
              {
                jx_ob path_list = (size > 1) ? Jx_code_eval_to_weak
                  (E, flags, expr_vla[1]) : jx_ob_from_null();
                jx_ob src_list = (size > 2) ? Jx_code_eval_to_weak
                  (E, flags | JX_EVAL_DEFER_INVOCATION, expr_vla[2]) :
                  jx_ob_from_null();
                jx_ob value_list = jx__code_pareval(E, flags, src_list);
                jx_ob container = Jx_scope_borrow(E);
                jx_int i, size = jx_list_size(path_list);
                jx_status status = JX_SUCCESS;
                for(i = 0; i < size; i++) {
                  status = jx_set_from_path_with_value(E, container,
                                                       jx_list_borrow(path_list, i),
                                                       jx_list_swap_with_null(value_list,
                                                                              i));
                }
                Jx_ob_free(E, path_list);
                Jx_ob_free(E, value_list);
                return jx_ob_from_status(status);
              }
              break;
            case JX_SELECTOR_PARAMAP:  /* [map callable list] */
              {
                jx_ob callable = (size > 1) ? Jx_code_eval_to_weak
                  (E, flags, expr_vla[1]) : jx_ob_from_null();
                if(size > 2) {
                  if(size == 3) {
                    jx_ob src_list = (size > 2) ? Jx_code_eval_to_weak
                      (E, flags, expr_vla[2]) : jx_ob_from_null();
                    return jx__code_paramap1(E, flags, callable, src_list);
                  } else if(0) {
                    /* zipping while mapping -- slower */
                    jx_int i, n = size - 2;
                    jx_ob src_list = Jx_list_new_with_size(E, n);
                    for(i = 0; i < n; i++) {
                      Jx_list_replace(E, src_list, i,
                                      Jx_code_eval_to_weak
                                      (E, flags, expr_vla[i + 2]));
                    }
                    return jx__code_mapN(E, flags, callable, src_list);
                  }
                }
              }
              return jx_ob_from_null();
              break;
            case JX_SELECTOR_MAP:      /* [map callable list] */
              {
                jx_ob callable = (size > 1) ? Jx_code_eval_to_weak
                  (E, flags, expr_vla[1]) : jx_ob_from_null();
                if(size > 2) {
                  if(size == 3) {
                    jx_ob src_list = (size > 2) ? Jx_code_eval_to_weak
                      (E, flags, expr_vla[2]) : jx_ob_from_null();
                    return jx__code_map1(E, flags, callable, src_list);
                  } else {
                    /* zipping while mapping -- slower */
                    jx_int i, n = size - 2;
                    jx_ob src_list = Jx_list_new_with_size(E, n);
                    for(i = 0; i < n; i++) {
                      Jx_list_replace(E, src_list, i,
                                      Jx_code_eval_to_weak
                                      (E, flags, expr_vla[i + 2]));
                    }
                    return jx__code_mapN(E, flags, callable, src_list);
                  }
                }
              }
              return jx_ob_from_null();
              break;
            case JX_SELECTOR_REDUCE:   /* [reduce callable list] */
              {
                jx_ob callable = (size > 1) ? Jx_code_eval_to_weak
                  (E, flags, expr_vla[1]) : jx_ob_from_null();
                if(size > 2) {
                  if(size == 3) {
                    jx_ob src_list = (size > 2) ? Jx_code_eval_to_weak
                      (E, flags, expr_vla[2]) : jx_ob_from_null();
                    return jx__code_reduce(E, flags, callable, src_list);
                  }
                }
              }
              return jx_ob_from_null();
              break;
            case JX_SELECTOR_CONTINUE:
              return jx_ob_from_opcode(JX_OPCODE_CONTINUE, 0);
              break;
            case JX_SELECTOR_BREAK:
              E->break_seen = JX_TRUE;
              return jx_ob_from_opcode(JX_OPCODE_BREAK, 0);
              break;
            }
          }
          return jx_ob_from_null();
          /* ================= END OF SPECIAL FORMS ================= */
        } else {
          /* not a special form, so continue... */
          jx_int size = jx__list_size(expr_list);
          jx_ob result = Jx_list_new_with_size(E, size);

          //jx_jxon_dump(stdout,"not special",expr);

          if(!(result.meta.bits & JX_META_BIT_LIST)) {
            /* memory exhausted? */
            return result;
          } else {
            /* first we evaluate all containers */

            //jx_jxon_dump(stdout,"pre-eval",expr);
            jx_list *result_list = result.data.io.list;
            jx_ob *expr_ob = expr_list->data.ob_vla;
            jx_ob *result_vla = result_list->data.ob_vla;
            register jx_ob *result_ob = result_vla;
            jx_ob method = jx_ob_from_null();
            jx_bool macro_flag = JX_FALSE;
            jx_bool method_flag = JX_FALSE;
            E->have_method = JX_FALSE;

            if(flags & JX_EVAL_RESUME_INVOCATION) {
              jx_int i;
              for(i = 0; i < size; i++) {
                *(result_ob++) = jx_ob_swap_with_null(expr_ob++);
              }
            } else {

              /* zeroth pass */

              if(expr_ob->meta.bits & (JX_META_BIT_HASH | JX_META_BIT_LIST)) {
                *(result_ob++) = Jx__code_eval_to_weak(E, flags, *(expr_ob++));
              } else {          /* not a container */
                *(result_ob++) = Jx_ob_copy(E, *(expr_ob++));
              }

              if(E->have_method) {
                /* implicit method call? */
                method_flag = JX_TRUE;
                method = E->method;
                E->have_method = JX_FALSE;
              } else {
                jx_fast_bits bits;
                if((bits = result_ob[-1].meta.bits) & JX_META_BIT_BUILTIN) {
                  switch (bits & JX_META_MASK_BUILTIN_TYPE) {
                  case JX_META_BIT_BUILTIN_MACRO:
                    macro_flag = JX_TRUE;
                    break;
                  case JX_META_BIT_BUILTIN_SELECTOR:
                    switch (result_ob[-1].data.io.int_) {
                    case JX_SELECTOR_ENTITY:
                      /* create a receiver scope for catching newly-defined symbols */
                      Jx_scope_push_new(E);
                      break;
                    }
                    break;
                  }
                }
              }
              /* remaining passes */
              if(macro_flag) {
                jx_int i;
                for(i = 1; i < size; i++) {
                  *(result_ob++) = Jx_ob_copy(E, *(expr_ob++));
                }
              } else {
                jx_int i;
                for(i = 1; i < size; i++) {
                  if(expr_ob->meta.bits & (JX_META_BIT_HASH | JX_META_BIT_LIST)) {
                    *(result_ob++) = Jx__code_eval_to_weak(E, flags, *(expr_ob++));
                    if(E->have_method) {
                      Jx_ob_replace(E, result_ob - 1,
                                    jx__code_curry_method(E, result_ob[-1],
                                                          E->method));
                      E->have_method = JX_FALSE;
                    }
                  } else {      /* not a container or we're processing a macro */
                    *(result_ob++) = Jx_ob_copy(E, *(expr_ob++));
                  }
                }
              }

              /* at this point, all subexpressions should have been evaluated */

              if(flags & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
                jx_jxon_dump_in_node(stderr, "# sub-ex", E->node, result);
              }
            }

            if(macro_flag) {
              jx_ob macro = jx__list_remove(result_list, 0);
              // printf("expand macro\n");
              return jx__macro_call(E, macro, result);
            }

            if(method_flag) {   /* transform implict method
                                   call into a standard evaluation where
                                   the function is the first entry
                                   [method-fn self args] */

              if(jx_builtin_callable_check(method)) {
                jx__list_insert(E, result_list, 0, method);
                size++;
                result_vla = result_list->data.ob_vla;
              } else {
                if(size)
                  Jx_ob_replace(E, result_vla, method);
              }
              //jx_jxon_dump(stdout,"result",result);
            }

            /* WARNING / REMINDER: we may now have some weak
               references at the root level of the result list --
               these will need to be eliminated or made strong (deep
               copied) before we return the frame to the caller */

            if(flags & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
              jx_jxon_dump(stdout, "# post-eval", result);
            }
            //            printf("flags %d\n",flags);

            /* ================= END OF STRAIGHT EVAL ================= */

            if(flags & JX_EVAL_DEFER_INVOCATION) {    /* expanding a macro... */

              if(!(flags & JX_EVAL_ALLOW_NESTED_WEAK_REFS)) {
                jx__code_result_copy_weak_transients(E, result_vla, size);
              }

              return result;
            } else if(!jx_builtin_callable_check(result_vla[0])) {

              /* strengthen result unless weak-nesting is permitted
                 (transient weak references become real copies)  */

              if(!(flags & JX_EVAL_ALLOW_NESTED_WEAK_REFS)) {
                jx__code_result_copy_weak_transients(E, result_vla, size);
              }

              if(flags & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
                jx_jxon_dump_in_node(stderr, "# return", E->node, result);
              }

              /* return the evaluated value */
              return result;
            } else {
              /* known callable */
              switch (result_vla->meta.bits & JX_META_MASK_BUILTIN_TYPE) {
              case JX_META_BIT_BUILTIN_NATIVE_FN:
                {
                  jx_native_fn native_fn = result_vla->data.io.native_fn;
                  //                  jx_jxon_dump(stdin,"result",result);
                  if(native_fn) {
                    jx_ob function = jx__list_remove(result_list, 0);   /* strip function pointer */
                    result = native_fn(E, result);
                    Jx_ob_free(E, function);
                    return result;
                  } else {
                    Jx_ob_free(E, result);
                    return jx_ob_from_null();
                  }
                }
                break;
              case JX_META_BIT_BUILTIN_FUNCTION:
                {
                  jx_ob fn = jx__list_remove(result_list, 0);
                  return jx__function_call(E, fn, result);
                }
                break;
              case JX_META_BIT_BUILTIN_SELECTOR:
                /* builtin safe functions do not consume their inputs  */
                {
                  jx_ob callable = jx__list_remove(result_list, 0);
                  return jx__code_apply_callable(E, callable, result);
                }
                break;
              default:
                jx__code_result_copy_weak_transients
                  (E, result_list->data.ob_vla + 1, size - 1);
                {
                  jx_ob callable = jx__list_remove(result_list, 0);
                  return jx__code_apply_callable(E, callable, result);
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
        jx_ob key = jx_list_take(list, 0);
        jx_ob value = jx_list_take(list, 0);
        size = size - 2;
        Jx_hash_set(E, result, key, Jx_code_eval(E, flags, value));
        Jx_ob_free(E, value);
      }
      Jx_ob_free(E, list);
      return result;
    }
    break;
  default:
    return Jx_ob_copy(E, expr);
    break;
  }
}

static jx_ob Jx__code_exec_to_weak(jx_env * E, jx_int flags, jx_ob code)
{
  /* on entry, code is known to be a list.
     if code is [ fn, ...] then evaluate & return result
     if code is [ [fn, ...], [fn, ...], ... ] then
     evaluate all members of a list, returning only the last result */
  //jx_jxon_dump(stdout,"Jx__code_exec_to_weak entered with code",node,code);
  //jx_jxon_dump(stdout,"                               and node",node,node);

  jx_ob result = jx_ob_from_null();
  jx_int size = jx_list_size(code);
  if(size) {
    jx_list *code_list = code.data.io.list;
    jx_ob inst = jx__list_borrow(code_list, 0);
    /* code = [ *fn*, ...] */
    if(jx_builtin_callable_check(inst)) {       /* code block with only a single instruction */
      inst = Jx_code_eval_to_weak(E, flags, code);
      if(!jx_opcode_check(inst))
        Jx_ob_replace(E, &result, inst);
      if(E->leave) {
        if(E->have_result) {
          Jx_ob_replace(E, &result, E->result);
          E->have_result = JX_FALSE;
        }
        E->result = jx_ob_from_null();
        E->leave--;
      }
    } else {
      /* code = [ [ ... ]  [ ... ] .... ] */
      jx_int pc = 0;
      E->leave = 0;
      while(pc < size) {
        inst = jx__list_borrow(code_list, pc);
        if(!jx_opcode_check(inst)) {
          inst = Jx_code_eval_to_weak(E, flags, inst);
          if(!jx_opcode_check(inst)) {
            Jx_ob_replace(E, &result, inst);
            pc++;
            if(E->break_seen) {
              break;
            }
            if(E->leave) {
              if(E->have_result) {
                Jx_ob_replace(E, &result, E->result);
                E->have_result = JX_FALSE;
              }
              E->result = jx_ob_from_null();
              E->leave--;
              break;
            }
            continue;
          }
        }
        switch (inst.meta.bits & JX_META_MASK_OPCODE_INST) {
        case JX_OPCODE_RETURN:
          E->leave = -1;
          return result;        /* return current (last evaluated) */
          break;
        case JX_OPCODE_TAIL_CALL:
          E->leave = -1;
          E->tail_call = JX_TRUE;
          return result;        /* return current (last evaluated) */
          break;
        case JX_OPCODE_BREAK:
        case JX_OPCODE_CONTINUE:
          return result;        /* return current (last evaluated) */
          break;
        case JX_OPCODE_JUMP_RELATIVE:
          pc += inst.data.io.int_;
          if((pc < 0) || (pc >= size)) {        /* range checked */
            return jx_null_with_ob(result);
          }
          break;
        }
      }
    }
  }
  return result;
}

jx_ob Jx__code_eval(jx_env * E, jx_int flags, jx_ob code)
{
  jx_ob result;
  //  jx_jxon_dump(stdout,"Jx__code_eval entered",code);
  if(E) {
    result = JX_OWN(E, Jx_ob_not_weak_with_ob
                    (E, Jx__code_eval_to_weak(E, flags, code)));
  } else {
    result = jx_ob_from_null();
  }
  return result;
}

jx_ob Jx__code_exec(jx_env * E, jx_int flags, jx_ob code)
{
  //jx_jxon_dump(stdout,"Jx__code_exec entered with code",node,code);
  //jx_jxon_dump(stdout,"                       and node",node,node);
  jx_ob result;
  if(E) {
    result = JX_OWN(E, Jx_ob_not_weak_with_ob
                    (E, Jx__code_exec_to_weak(E, flags, code)));
  } else {
    result = jx_ob_from_null();
  }
  //jx_jxon_dump(stdout,"Jx__code_exec existing with result",node,result);
  return result;
}
