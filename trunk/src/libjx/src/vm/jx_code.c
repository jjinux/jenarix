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

  ok = jx_declare(ok, names, "parmap", JX_SELECTOR_PARMAP);

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

static jx_ob jx__code_bind_with_source(jx_ob prebind, jx_ob source, jx_int unresol_depth)
{
  if(unresol_depth<0) unresol_depth = 0;
  switch (JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else if(unresol_depth) {
      jx_int slot = jx_list_size(source);
      while(slot--) {
        jx_ob entry = jx_list_swap_with_null(source,slot);
        entry = jx__code_bind_with_source(prebind,entry,unresol_depth-1);
        jx_list_replace(source, slot, entry);
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
            switch(builtin.data.io.int_) {
            case JX_SELECTOR_NOP: /* [nop # ...] -> bind(#) */
              jx_ob_replace(&source, jx_list_remove(source,1));
              return jx__code_bind_with_source(prebind, source, unresol_depth-1);
              break;
            case JX_SELECTOR_RAW: /* [raw # ...] -> [*raw* # ...] */
              jx_list_replace(source, 0, builtin);
              return source;
              break;
            case JX_SELECTOR_QUOTE: /* [quote # ...] -> [*quote* bind(#) ...] */
              jx_list_replace(source, 0, builtin);
              return jx__code_bind_with_source(prebind,source,unresol_depth-1);
              break;
            case JX_SELECTOR_ENTITY:
              unresolved = 2;
              break;
            case JX_SELECTOR_SET:
            case JX_SELECTOR_INCR:
            case JX_SELECTOR_DECR:
            case JX_SELECTOR_APPEND:
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
            }
          }
          jx_list_replace(source, 0, builtin);
        }
      } else if(jx_builtin_selector_check(ident) &&
                ident.data.io.int_ == JX_SELECTOR_NOP) {
        /* [*nop* # ...] -> bind(#) */
        jx_ob_replace(&source, jx_list_remove(source,1));
        return jx__code_bind_with_source(prebind,source,unresol_depth-1);
      }
      //      jx_jxon_dump(stdout,"post-sub",source);
      /* now process the source list, introducing symbol resolution where needed */
      {
        jx_int slot = size;
        while((slot>unresolved)&&(slot--)) {
          jx_ob entry = jx__list_swap_with_null(source_list,slot);
          if(jx_ident_check(entry)) { /* # -> [*res* #] */
            jx_ob builtin = jx_hash_borrow(prebind, entry);
            if(jx_builtin_selector_check(builtin)) { 
              switch(builtin.data.io.int_) {              
              case JX_SELECTOR_BREAK:
                entry = jx_ob_from_opcode(JX_OPCODE_BREAK,0);
                break;
              case JX_SELECTOR_CONTINUE:
                entry = jx_ob_from_opcode(JX_OPCODE_CONTINUE,0);
                break;
              case JX_SELECTOR_RETURN: 
                entry = jx_ob_from_opcode(JX_OPCODE_RETURN,0);
                break;
              case JX_SELECTOR_TAIL: 
                entry = jx_ob_from_opcode(JX_OPCODE_TAIL_CALL,0);
                break;
              default:
                {
                  jx_ob new_entry = jx_list_new_with_size(2);
                  jx_list_replace(new_entry, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
                  jx_list_replace(new_entry, 1, jx_ident_split_with_dotted(entry));
                  entry = new_entry;
                }
                break;
              }
            } else {
              jx_ob new_entry = jx_list_new_with_size(2);
              jx_list_replace(new_entry, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
              jx_list_replace(new_entry, 1, jx_ident_split_with_dotted(entry));
              entry = new_entry;
            }
            
          } else if(entry.meta.bits & JX_META_BIT_GC) {
            entry = jx__code_bind_with_source(prebind,entry,unresol_depth-1);
          }
          jx__list_replace(source_list, slot, entry);
        }
        /* special handling for "unresolved" slots */
        while((slot--)>1) {
          jx_ob entry = jx__list_swap_with_null(source_list,slot);
          switch(resolution) {
          case JX_RESOLUTION_LVALUE:  /* (set (lit lit (expr))) value) */
          case JX_RESOLUTION_RVALUE:  /* (get (lit (expr) lit (expr) (expr))) */
            switch(slot) {
            case 1:
              if(jx_ident_check(entry)) {
                entry = jx_ident_split_with_dotted(entry);
              } else {
                entry = jx__code_bind_with_source(prebind,entry,1);
              }
              break;
            default:
              entry = jx__code_bind_with_source(prebind,entry,0);
              break;
            }
            break;
          case JX_RESOLUTION_LAMDEF:
            switch(slot) {
            case 1:
              entry = jx__code_bind_with_source(prebind,entry,2); /* arg list */
              break;
            default:
              entry = jx__code_bind_with_source(prebind,entry,1);
              break;
            }
            break;
          case JX_RESOLUTION_FUNDEF: 
            switch(slot) {
            case 2:
              entry = jx__code_bind_with_source(prebind,entry,2); /* arg list */
              break;
            case 1:
              entry = jx__code_bind_with_source(prebind,entry,1); /* fn name */
              break;
            default:
              entry = jx__code_bind_with_source(prebind,entry,1);
              break;
            }
            break;
          case JX_RESOLUTION_NORMAL: 
          default:
            entry = jx__code_bind_with_source(prebind,entry,0);
            break;
          }
          jx__list_replace(source_list, slot, entry);
        }
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
        if(jx_ident_check(value)&&(!unresol_depth)) {
          jx_ob new_entry = jx_list_new_with_size(2);
          jx_list_replace(new_entry, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
          jx_list_replace(new_entry, 1, jx_ident_split_with_dotted(value));
          jx_hash_set(result, key, new_entry);
        } else {
          jx_hash_set(result, key, jx__code_bind_with_source(prebind, value, unresol_depth-1));
        }
      }
      jx_ob_free(list);
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
    jx_ob_free(source);
    return jx_ob_from_null();
    break;
  }
}

jx_ob jx_code_bind_with_source(jx_ob prebind, jx_ob source)
{
  if(jx_ident_check(source)) {  /* if code consists solely of a top-level identifier */
    jx_ob builtin = jx_hash_borrow(prebind, source);
    jx_ob result;
    if(jx_builtin_check(builtin)) { /* known builtin function (early / fixed binding) */
      result = jx_list_new_with_size(1);
      jx_list_replace(result, 0, builtin);
      jx_ob_free(source);
    } else {                    /* unknown identifier (fall back upon late binding) */
      result = jx_list_new_with_size(2);
      jx_list_replace(result, 0, jx_builtin_new_from_selector(JX_SELECTOR_RESOLVE));
      jx_list_replace(result, 1, jx_ident_split_with_dotted(source));
    }
    return result;
  } else
    return jx__code_bind_with_source(prebind, source, 0);
}

static jx_status jx__code_unbound_from_code(jx_ob unbound, jx_ob code)
{
  switch (JX_META_MASK_TYPE_BITS & code.meta.bits) {
  case JX_META_BIT_LIST:
    if(!code.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      jx_int size = jx_list_size(code);
      if(size) {
        jx_ob first = jx_list_borrow(code,0);
        if(jx_builtin_selector_check(first)) { 
          switch(first.data.io.int_) {
            /* all functions which accept symbols need to be included here */
          case JX_SELECTOR_APPEND:
          case JX_SELECTOR_RESOLVE:
            {
              jx_ob sym = jx_list_get(code,1);
              if(jx_list_check(sym)) {
                jx_ob_replace(&sym, jx_list_get(sym,0));
              }
              jx_list_unshift(unbound,sym);
            }
            break;
          }
        }
        while(size--) {
          jx__code_unbound_from_code(unbound,jx_list_borrow(code,size));
        }
      }
    }
    break;
  case JX_META_BIT_HASH:
    { 
      jx_ob list = jx_list_new_from_hash(code);       /* copies code */
      jx_int size = jx_list_size(list);
      while(size) {
        size = size - 2;
        jx__code_unbound_from_code(unbound, jx_list_borrow(list, size+1));
      }
      jx_ob_free(list);
    }
    break;
  }
  //  jx_jxon_dump(stdout,"unbound",jx_ob_from_null(),unbound);
  return JX_SUCCESS;
}

jx_ob jx_code_unbound_from_code(jx_ob code)
{
  jx_ob unbound = jx_list_new();
  jx__code_unbound_from_code(unbound,code);
  return unbound;
}

static jx_ob jx__code_eval_to_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_eval_to_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_eval_to_weak(tls,flags, node, expr) : expr;
}

static jx_ob jx__code_exec_to_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_exec_to_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_LIST) ?
    jx__code_exec_to_weak(tls,flags, node, expr) : jx_ob_copy(expr);
}

static void jx__code_filter_weak_transients(jx_ob *ob,jx_int size)
{
  while(size--) {
    if(ob->meta.bits & JX_META_BIT_WEAK_REF) 
      *ob = jx_ob_copy(*ob); /* NOTE: shared/synchronized objects remain weak */
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
      /* NOTE for performance reasons, the "safe" builtin
         selector functions pull from but do not free the
         payload object, hence the use of jx_replace below */
      switch (callable.data.io.int_) {
      case JX_SELECTOR_RETURN:
        tls->leave = -1; 
        if(jx_list_size(payload)) {
          tls->have_result = JX_TRUE;
          jx_tls_ob_replace(tls, &tls->result, jx_ob_not_weak_with_ob
                            (jx_list_remove(payload,0)));
        } else {
          jx_tls_ob_replace(tls, &payload, jx_ob_from_opcode(JX_OPCODE_RETURN,0));
        }
        break;
      case JX_SELECTOR_TAIL:
        tls->leave = -1; /* break out of all blocks until we find a function */
        tls->tail_call = JX_TRUE;
        if(jx_list_size(payload)) {
          tls->have_result = JX_TRUE;
          jx_tls_ob_replace(tls, &tls->result, jx_ob_not_weak_with_ob(payload));
        } else {
          jx_tls_ob_replace(tls, &payload, jx_ob_from_opcode(JX_OPCODE_RETURN,0));
        }
        payload = jx_ob_from_null();
        break;
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
      case JX_SELECTOR_IDIV:
        jx_tls_ob_replace(tls, &payload, jx_safe_idiv(node, payload));
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
      case JX_SELECTOR_REVERSE:
        jx_tls_ob_replace(tls, &payload, jx_safe_reverse(node, payload));
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
      case JX_SELECTOR_INT:
        jx_tls_ob_replace(tls, &payload, jx_safe_int(node, payload));
        break;
      case JX_SELECTOR_FLOAT:
        jx_tls_ob_replace(tls, &payload, jx_safe_float(node, payload));
        break;
      case JX_SELECTOR_BOOL:
        jx_tls_ob_replace(tls, &payload, jx_safe_bool(node, payload));
        break;
      case JX_SELECTOR_SYNCHRONIZE:
        jx_tls_ob_replace(tls, &payload, jx_safe_synchronize(node, payload));
        break;
      case JX_SELECTOR_SYNCHRONIZED:
        jx_tls_ob_replace(tls, &payload, jx_safe_synchronized(node, payload));
        break;
      case JX_SELECTOR_SHARE:
        jx_tls_ob_replace(tls, &payload, jx_safe_share(node, payload));
        break;
      case JX_SELECTOR_SHARED:
        jx_tls_ob_replace(tls, &payload, jx_safe_shared(node, payload));
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
  jx_ob out = jx_ob_from_null();
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

typedef struct {
  jx_int id;
  jx_ob callable;
  jx_ob node;
  jx_ob payload;
} jx_thread_info;

void *thread_fn(void *id_ptr)
{
  jx_thread_info *info = (jx_thread_info*)id_ptr;
  jx_tls *tls = jx_tls_new(); // use stack not heap?

  //  printf("thread %d started\n",info->id);

  tls->builtins = jx_hash_borrow(info->node,jx_ob_from_null());


  info->payload = jx__code_apply_callable
    (tls, info->node, info->callable, info->payload);
  
  jx_tls_free(tls);
  //  printf("thread %d complete\n",info->id);
  return NULL;
}

JX_INLINE jx_ob jx__code_parmap1(jx_tls *tls, jx_int flags, jx_ob node, jx_ob callable, jx_ob inp) 
{
  jx_ob result = jx_ob_from_null();
  jx_int size = jx_list_size(inp);
  if(jx_builtin_callable_check(callable) && size) {
    result = jx_tls_list_new_with_size(tls, size);
    jx_os_thread *thread_array = NULL;        
    jx_thread_info *thread_info =  (jx_thread_info*)jx_vla_new(sizeof(jx_thread_info),size);
    if(jx_ok( jx_os_thread_array_new( &thread_array,size ))) {
      jx_int i;
      for(i=0; i<size; i++) {
        jx_os_thread *thread = jx_os_thread_array_entry( thread_array, i);
        thread_info[i].id = i;
        thread_info[i].node = node;
        thread_info[i].callable = callable;
        thread_info[i].payload = jx_list_swap_with_null(inp,i);
        jx_os_thread_start(thread, thread_fn, thread_info + i);
      }
      {
        jx_int i;
        for(i=0; i<size; i++) {
          jx_os_thread *thread = jx_os_thread_array_entry( thread_array, i);
          jx_os_thread_join(thread);
          jx_list_replace(result,i,thread_info[i].payload);
        }
      }
      jx_os_thread_array_free( &thread_array );
      jx_vla_free(&thread_info);
    }
  }
  jx_tls_ob_free(tls, inp);
  jx_tls_ob_free(tls, callable);
  return result;
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
        jx_ob inp_ob = jx_ob_from_int_zero();
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
        jx_ob inp_ob = jx_ob_from_float_zero();
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
          jx_list_replace(arg_ob, j, jx_ob_take_weak_ref
                          (jx_list_borrow(jx_list_borrow(src_list,j),i)));
        }
        jx_list_replace(out,i,jx__code_apply_callable
                        (tls, node, callable_weak_ref, 
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

static jx_ob jx__code_close_with_args(jx_ob node, jx_ob code, jx_ob args)
{
  jx_ob unbound = jx_code_unbound_from_code(code);
  
  if(jx_list_check(args)) {
    jx_ob closed = jx_hash_new();
    jx_int i,size = jx_list_size(unbound);
    for(i=0;i<size;i++) {
      jx_ob ident = jx_list_borrow(unbound,i);
      jx_ob value = jx_ob_from_null();
      if(jx_hash_peek(&value,node,ident)) {
        jx_ob_dump(stdout,"closing on",value);
        jx_hash_set(closed,jx_ob_copy(ident),jx_ob_copy(value));
        
        while(jx_builtin_entity_check(value)) { /* close on chained entity hashes as well */
          jx_ob ent_def = jx_hash_get(node, value); 
          jx_hash_set(closed, jx_ob_copy(value), ent_def);
          value = jx_list_borrow(ent_def,0);
        }
      }
    }
    {
      jx_ob kw_args = jx_list_borrow(args,1);
      if(!jx_hash_check(kw_args)) {
        jx_ob new_args = jx_list_new();
        jx_list_append(new_args,args);
        jx_list_append(new_args,closed);
        args = new_args;
      } else {
        jx_ob iter = jx_list_new_from_hash(kw_args);
        jx_int i,size = jx_list_size(iter);
        for(i=0;i<size;i+=2) {
          jx_hash_set(closed, 
                      jx_list_swap_with_null(iter,i),
                      jx_list_swap_with_null(iter,i+1));
        }
        jx_ob_free(iter);
        jx_list_replace(args,1,closed);
      }
    }
  }
  jx_ob_free(unbound);
  return args;
}

jx_ob jx__code_eval_to_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob expr)
{
  register jx_int flags_ = flags;
  if(flags_ & JX_EVAL_DEBUG_TRACE) {
    fprintf(stderr, "# trace: eval_to_weak(tls %p, flags 0x%x, ...)\n",
            (void*)tls, (int)flags_);
  }
  if(flags_ & (JX_EVAL_DEBUG_DUMP_NODE)) {
    jx_jxon_dump_in_node(stderr,"#   node",node, node);
  }
  if(flags_ & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
    jx_jxon_dump_in_node(stderr,"#   expr",node, expr);
  }
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
           ((!(flags_ & JX_EVAL_DEFER_INVOCATION)) || 
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
              jx_ob ob = (size>1) ? jx_code_eval_tls(tls, flags_, node, expr_vla[1]) : 
                jx_ob_from_null();
              jx_ob result = jx_code_eval_tls(tls, flags_, node, ob);
              jx_tls_ob_free(tls, ob);
              return result;
            }
            break;
          case JX_SELECTOR_DEBUG:                             
            {
              jx_int new_flags = flags_ | JX_EVAL_DEBUG_DUMP_SUBEX | JX_EVAL_DEBUG_TRACE;
              jx_ob result = (size>1) ? jx_code_eval_tls(tls, new_flags, node, expr_vla[1]) : 
                jx_ob_from_null();
              return result;
            }
            break;
          case JX_SELECTOR_EXEC:                             
            return  ((size>1) ? jx_code_exec_tls(tls, flags_, node, expr_vla[1]) : 
                     jx_ob_from_null());
            break;
          case JX_SELECTOR_IF: /* [*if* expr code expr code ... code] */
            {
              int i = 1;
              while(i<size) {
                jx_ob cond = ((size>1) ? jx_code_eval_tls(tls, flags_, node, expr_vla[i]) : 
                              jx_ob_from_null());
                jx_bool cond_bool = jx_ob_as_bool(cond);
                jx_ob_free(cond);
                i+=2;
                if(cond_bool) {
                  return ((size>2) ? jx_code_exec_tls(tls,flags_, node, expr_vla[i-1]) :
                          jx_ob_from_null());
                } else if(size == i) {
                  break;
                } else if(size == i+1) {
                  return jx_code_exec_tls(tls,flags_, node, expr_vla[i]);
                }
              }
              return jx_ob_from_null();
            }
            break;
          case JX_SELECTOR_TEST: /* [*test* expr expr expr] */
            {
              jx_ob cond = (size>1) ? jx_code_eval_tls(tls, flags_, node, expr_vla[1]) : 
                jx_ob_from_null();
              jx_bool cond_bool = jx_ob_as_bool(cond);
              jx_ob_free(cond);
              if(cond_bool) {
                return (size>2) ? jx_code_eval_to_weak(tls,flags_,node, expr_vla[2]) : 
                  jx_ob_from_null();
              } else {
                return (size>3) ? jx_code_eval_to_weak(tls,flags_,node, expr_vla[3]) : 
                  jx_ob_from_null();
              }
            }
            break;
          case JX_SELECTOR_RESOLVE: /* [*resolve* ident] */
            {
              jx_ob symbol = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob container = node;
              jx_ob target = symbol;
              if(JX_POS(jx__resolve_container(tls,&container,&target))) {
                /* looked-up objects are always returned as weak references...
                   it's EVAL's job to make sure they don't leak into end-user code */
                jx_ob weak = jx_ob_take_weak_ref(container);
                if(flags_ & (JX_EVAL_DEBUG_TRACE)) {
                  jx_jxon_dump_in_node(stderr,"# rsolvd",node, weak);
                }
                return weak;
              } else {
                if(jx_list_check(symbol)) 
                  return jx_ident_new_from_dotted(symbol);
                else
                  return jx_tls_ob_copy(tls,symbol);
              }
            }
            break;
          case JX_SELECTOR_DEF:   /* [def name node code] */
          case JX_SELECTOR_DEFUN:  /* [defun name node expr] */
            {
              jx_ob name = (size > 1) ? jx_tls_ob_copy(tls,expr_vla[1]) : jx_ob_from_null();
              jx_ob args = (size > 2) ? jx_code_eval_tls(tls, flags_, node, expr_vla[2]) : 
                              jx_ob_from_null();
              jx_ob body = (size > 3) ? jx_tls_ob_copy(tls,expr_vla[3]) : jx_ob_from_null();
              jx_ob key = jx_ident_split_from_dotted(name);
              
              args = jx__code_close_with_args(node,body,args);

              {
                jx_ob function = jx_function_new_with_def
                  (name, args, body, expr_vla->data.io.int_ == JX_SELECTOR_DEF);
                if(!jx_ok( jx_set_from_path_with_value(node,key,function))) {
                  jx_ob_free(function);
                }
              }
              jx_ob_free(key);
              return jx_ob_from_null();
            }
            break;
          case JX_SELECTOR_CODE: /* [code args code] */
          case JX_SELECTOR_LAMBDA: /* [expr args expr] */
            {
              jx_ob name = jx_ob_from_null();
              jx_ob args = (size > 1) ? jx_tls_ob_copy(tls,expr_vla[1]) : jx_ob_from_null();
              jx_ob body = (size > 2) ? jx_tls_ob_copy(tls,expr_vla[2]) : jx_ob_from_null();

              args = jx__code_close_with_args(node,body,args);

              return jx_function_new_with_def
                (name,args,body,expr_vla->data.io.int_ == JX_SELECTOR_CODE);
            }
            break;
          case JX_SELECTOR_DEFMAC:  /* [defmac name node expr] */
            {
              jx_ob name = (size > 1) ? jx_tls_ob_copy(tls,expr_vla[1]) : jx_ob_from_null();
              jx_ob args = (size > 2) ? jx_tls_ob_copy(tls,expr_vla[2]) : jx_ob_from_null();
              jx_ob body = (size > 3) ? jx_tls_ob_copy(tls,expr_vla[3]) : jx_ob_from_null();
              jx_ob key = jx_ident_split_from_dotted(name);
              jx_ob function = jx_macro_new_with_def
                (name, args, body );
              //jx_ob_dump(stdout,"macro",function);
              if(!jx_ok( jx_set_from_path_with_value(node,key,function))) {
                jx_ob_free(function);
              }
              jx_ob_free(key);
              return jx_ob_from_null();
            }
            break;
          case JX_SELECTOR_DISPATCH: /* [dispatch index [callable0 ...] payload ] */
            {
              jx_ob index = (size > 1) ?  jx_code_eval_tls(tls, flags_, node, expr_vla[1]) :
                jx_ob_from_null();
              jx_ob table = (size > 2) ? 
                jx_code_eval_to_weak(tls,flags_ | JX_EVAL_ALLOW_NESTED_WEAK_REFS,
                                        node, expr_vla[2]) :
                jx_ob_from_null();
              jx_ob payload = (size > 3) ? jx_code_eval_tls(tls, flags_, node, expr_vla[3]) :
                jx_ob_from_null();
              //              jx_jxon_dump(stdout,"table",table);
              jx_ob callable = jx_list_swap_with_null(table,jx_ob_as_int(index));
              jx_ob result = jx_ob_from_null();
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
              jx_ob index = (size > 1) ?  jx_code_eval_tls(tls, flags_, node, expr_vla[1]) :
                jx_ob_from_null();
              jx_ob table = (size > 2) ? expr_vla[2] : jx_ob_from_null();
              jx_int idx = jx_ob_as_int(index);
              jx_ob_free(index);
              return jx_code_exec_tls(tls,flags_,node, jx_list_borrow(table,idx));
            }
            break;
          case JX_SELECTOR_SELECT:  /* [select index [expr0 expr1 expr2]] */
            {
              jx_ob index = (size > 1) ?  jx_code_eval_tls(tls, flags_, node, expr_vla[1]) :
                jx_ob_from_null();
              jx_ob table = (size > 2) ? expr_vla[2] : jx_ob_from_null();
              jx_int idx = jx_ob_as_int(index);
              jx_ob_free(index);
              return jx_code_eval_tls(tls, flags_, node, jx_list_borrow(table,idx));
            }
            break;
          case JX_SELECTOR_WHILE:
            {
              jx_ob cond = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob body = (size>2) ? expr_vla[2] : jx_ob_from_null();
              jx_ob result = jx_ob_from_null();
              tls->break_seen = JX_FALSE;
              while(!(tls->break_seen || tls->leave)) {
                jx_ob test = jx_code_eval_to_weak(tls, flags_, node, cond); 
                jx_int tst = jx_ob_as_bool(test);
                jx_ob_free(test);
                if(tst) {
                  jx_ob_replace(&result, jx_code_exec_tls(tls,flags_,node, body));
                } else
                  break;
              }
              tls->break_seen = JX_FALSE;
              return result;
            }
            break;
          case JX_SELECTOR_DO:
            {
              jx_ob body = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob cond = (size>2) ? expr_vla[2] : jx_ob_from_null();
              jx_ob result = jx_ob_from_null();
              jx_int tst = 0;
              do {
                jx_ob_replace(&result, jx_code_exec_tls(tls,flags_,node, body));
                jx_ob test = jx_code_eval_tls(tls, flags_, node, cond);
                tst = jx_ob_as_bool(test);
                jx_ob_free(test);
              } while(tst);
              return result;
            }
            break;
          case JX_SELECTOR_FOR:
            {
              jx_ob init = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob cond = (size>2) ? expr_vla[2] : jx_ob_from_null();
              jx_ob step = (size>3) ? expr_vla[3] : jx_ob_from_null();
              jx_ob body = (size>4) ? expr_vla[4] : jx_ob_from_null();
              jx_ob result = jx_ob_from_null();
              jx_int tst = JX_TRUE;
              jx_ob_free(jx_code_eval_tls(tls, flags_, node, init));
              while(tst) {
                jx_ob test = jx_code_eval_tls(tls, flags_, node, cond);
                if(!jx_opcode_check(test))
                  break;
                tst = jx_ob_as_bool(test);
                jx_ob_free(test);
                if(tst) {
                  jx_ob_replace(&result, jx_code_exec_tls(tls,flags_,node, body));
                }
                jx_ob_free(jx_code_eval_tls(tls, flags_, node, step));
              }
              return result;
            }
            break;
          case JX_SELECTOR_FOREACH: /* does not currently support break... */
            {
              jx_ob expr = (size>1) ? expr_vla[1] : jx_ob_from_null();
              jx_ob list = (size>2) ? 
                jx_code_eval_to_weak(tls,flags_,node, expr_vla[2]) :
                jx_ob_from_null();
              jx_ob body = (size>3) ? expr_vla[3] : jx_ob_from_null();
              jx_ob result = jx_ob_from_null();
              jx_list *inp_list = list.data.io.list;
              jx_int i,size = jx_list_size(list);
              if(size) {
                if(!inp_list->packed_meta_bits) { /* normal object input */
                  jx_ob *ob = inp_list->data.ob_vla;
                  for(i=0;i<size;i++) {
                    jx_hash_set(node, expr, jx_ob_copy(*(ob++)));
                    jx_ob_replace(&result, jx_code_exec_tls(tls,flags_,node,body));
                  } 
                } else if(inp_list->packed_meta_bits == JX_META_BIT_INT) { 
                    /* packed integer input */
                  jx_ob ob = jx_ob_from_int_zero();
                  jx_int *inp_int = inp_list->data.int_vla;
                  for(i=0;i<size;i++) {
                    ob.data.io.int_ = *(inp_int++);
                    jx_hash_set(node, expr, ob);
                    jx_ob_replace(&result, jx_code_exec_tls(tls,flags_,node,body));
                  }
                } else if(inp_list->packed_meta_bits == JX_META_BIT_FLOAT) { 
                  jx_ob ob = jx_ob_from_float_zero();
                  jx_float *inp_float = inp_list->data.float_vla;
                  for(i=0;i<size;i++) {
                    ob.data.io.float_ = *(inp_float++);
                    jx_hash_set(node, expr, ob);
                    jx_ob_replace(&result, jx_code_exec_tls(tls,flags_,node,body));
                  }
                }
              }
              jx_tls_ob_free(tls,list);
              return result;
            }
            break;
          case JX_SELECTOR_APPLY: /* [apply callable payload] TODO: keyword arg list*/
            {
              jx_ob callable = (size>1) ? jx_code_eval_to_weak
                (tls, flags_, node, expr_vla[1]) : jx_ob_from_null();
              jx_ob payload = (size>2) ? jx_code_eval_tls(tls, flags_, node, expr_vla[2]) :
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
          case JX_SELECTOR_PARMAP: /* [map callable list] */
            {
              jx_ob callable = (size>1) ? jx_code_eval_to_weak
                (tls, flags_, node, expr_vla[1]) : jx_ob_from_null();
              if(size > 2) {
                if(size == 3) {
                  jx_ob src_list = (size>2) ? jx_code_eval_to_weak
                    (tls, flags_, node, expr_vla[2]) : 
                    jx_ob_from_null();
                  return jx__code_parmap1(tls,flags_,node,callable,src_list);
                } else if(0) { 
                  /* zipping while mapping -- slower */
                  jx_int i,n = size - 2;
                  jx_ob src_list = jx_tls_list_new_with_size(tls,n);
                  for(i=0;i<n;i++) {
                    jx_list_replace(src_list, i, jx_code_eval_to_weak
                                    (tls, flags_, node, expr_vla[i+2]));
                  }
                  return jx__code_mapN(tls,flags_,node,callable,src_list);
                }
              }
            }
            return jx_ob_from_null();
            break;
          case JX_SELECTOR_MAP: /* [map callable list] */
            {
              jx_ob callable = (size>1) ? jx_code_eval_to_weak
                (tls, flags_, node, expr_vla[1]) : jx_ob_from_null();
              if(size > 2) {
                if(size == 3) {
                  jx_ob src_list = (size>2) ? jx_code_eval_to_weak
                    (tls, flags_, node, expr_vla[2]) : 
                    jx_ob_from_null();
                  return jx__code_map1(tls,flags_,node,callable,src_list);
                } else { 
                  /* zipping while mapping -- slower */
                  jx_int i,n = size - 2;
                  jx_ob src_list = jx_tls_list_new_with_size(tls,n);
                  for(i=0;i<n;i++) {
                    jx_list_replace(src_list, i, jx_code_eval_to_weak
                                    (tls, flags_, node, expr_vla[i+2]));
                  }
                  return jx__code_mapN(tls,flags_,node,callable,src_list);
                }
              }
            }
            return jx_ob_from_null();
            break;
          case JX_SELECTOR_REDUCE: /* [reduce callable list] */
            {
              jx_ob callable = (size>1) ? jx_code_eval_to_weak
                (tls, flags_, node, expr_vla[1]) : jx_ob_from_null();
              if(size > 2) {
                if(size == 3) {
                  jx_ob src_list = (size>2) ? jx_code_eval_to_weak
                    (tls, flags_, node, expr_vla[2]) : 
                    jx_ob_from_null();
                  return jx__code_reduce(tls,flags_,node,callable,src_list);
                }
              }
            }
            return jx_ob_from_null();
            break;
          case JX_SELECTOR_CONTINUE:
            return jx_ob_from_opcode(JX_OPCODE_CONTINUE,0);
            break;
          case JX_SELECTOR_BREAK:
            tls->break_seen = JX_TRUE;
            return jx_ob_from_opcode(JX_OPCODE_BREAK,0);
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
            jx_ob method_ob = jx_ob_from_null();
            jx_bool macro_flag = JX_FALSE;
            jx_bool method_flag = JX_FALSE;

            tls->have_method = JX_FALSE;
            for(i=0;i<size;i++) {
              if((!macro_flag) && (expr_ob->meta.bits &
                                   (JX_META_BIT_LIST|JX_META_BIT_HASH))) {
                *(result_ob++) = jx_code_eval_to_weak(tls, flags_, node, *expr_ob);
                if((!i)&&tls->have_method) { /* implicit method call? */
                  method_flag = JX_TRUE;
                  method_ob = tls->method;
                  //jx_jxon_dump(stdout,"method_ob",node,tls->method);
                }
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
            /* at this point, all subexpressions should have been evaluated */

            if(flags_ & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
              jx_jxon_dump_in_node(stderr,"# sub-ex",node, result);
            }

            if(macro_flag) {
              jx_ob macro = jx__list_remove(result_list,0);
              // printf("expand macro\n");
              return jx__macro_call(tls,node,macro,result);
            }

            if(method_flag) { /* transform implict method 
                                 call into a standard evaluation */
              jx__list_insert(result_list,0,method_ob);
              //              jx_jxon_dump(stdout,"result",node,result);
              result_vla = result_list->data.ob_vla;
            }
            /* WARNING / REMINDER: we may now have some weak
               references at the root level of the result list --
               these will need to be eliminated or made strong (deep
               copied) before we return the frame to the caller */

            //jx_jxon_dump(stdout,"# post-eval",node,result);
            //            printf("flags %d\n",flags);

            if(flags_ & JX_EVAL_DEFER_INVOCATION) { /* expanding a macro... */
              return result;
            } else if(!jx_builtin_callable_check(result_vla[0])) {
              /* first entry in list is not callable...but it is an Jenaric Entity? */
              if(jx_list_check(result_vla[0])) {
                jx_int entity_size;
                jx_list *list = result_vla->data.io.list;
                if((!list->packed_meta_bits) && 
                   (entity_size = jx_vla_size(&list->data))) { 
                  jx_ob *instance_ob = list->data.ob_vla;
                  /* non-zero, unpacked => vla valid */
                  if(jx_builtin_entity_check(instance_ob[0])) {
                    /* first entry in result contains an entity (a class / instance, etc.) */
                    if(size<2) { /* constructor */
                      jx_ob handle = jx_ob_copy(instance_ob[0]);
                      jx_ob content = jx_entity_resolve_content(node,handle);
                      jx_ob attrs = jx_entity_resolve_attrs(node,handle);
                      if(jx_null_check(content) && jx_null_check(attrs)) {
                        jx_ob_replace(&result, jx_list_new_with_size(1));
                        jx_list_replace(result, 0, handle);
                      } else {
                        jx_ob_replace(&result, jx_list_new_with_size(3));
                        jx_list_replace(result, 0, handle);
                        jx_list_replace(result, 1, content);
                        jx_list_replace(result, 2, attrs);
                      }
                      return result;
                    } else {
                      /* the entry following the entity -- what is it? */
                      switch(result_vla[1].meta.bits & JX_META_MASK_TYPE_BITS) {
                      case 0: /* null method == constructor? */
                        break;
                      case JX_META_BIT_IDENT: /* identifier? -> 
                                                  method/attr resolution */
                        {
                          jx_ob method_name = result_vla[1];
                          jx_ob method;
                          if(entity_size>2) {
                          /* first we look in the member */
                            method = jx_hash_borrow(instance_ob[2],method_name);
                          } else 
                            method = jx_ob_from_null();
                          if(jx_null_check(method)) {
                            /* no luck? resolve class attr / methods */
                            jx_ob handle = instance_ob[0];
                            while(!jx_null_check(handle)) {
                              jx_ob def = jx_hash_borrow(node,handle);
                              jx_ob attr = jx_list_borrow(def,2);
                              method = jx_hash_borrow(attr, method_name);
                              if(!jx_null_check(method))
                                break;
                              handle = jx_list_borrow(def,0);
                            }
                          }
                          method = jx_ob_take_weak_ref(method);
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
                              jx__code_filter_weak_transients(result_list->data.ob_vla + 1,size - 1);
                              return jx__code_apply_callable(tls, node, method, result);
                              break;
                            }
                          } else { /* simply an attribute */
                            jx_ob_free(result);
                            return method;
                          }
                        }
                        break;
                      case JX_META_BIT_INT: /* entity access (0=ident,1=content,2=attr) */
                        {
                          jx_int index = jx_ob_as_int(result_vla[1]);
                          if((index<entity_size)&&(index>=0)) {
                            jx_ob member = instance_ob[index];
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
              if(!(flags_ & JX_EVAL_ALLOW_NESTED_WEAK_REFS)) {
                jx__code_filter_weak_transients(result_vla,size);
              }
              
              if(flags_ & (JX_EVAL_DEBUG_DUMP_SUBEX)) {
                jx_jxon_dump_in_node(stderr,"# return",node, result);
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
                jx__code_filter_weak_transients(result_list->data.ob_vla + 1,size - 1);
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
        jx_hash_set(result, key, jx_code_eval_tls(tls, flags_, node, value));
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

static jx_ob jx__code_exec_to_weak(jx_tls *tls,jx_int flags, jx_ob node, jx_ob code)
{
  /* on entry, code is known to be a list.
     if code is [ fn, ...] then evaluate & return result
     if code is [ [fn, ...], [fn, ...], ... ] then
     evaluate all members of a list, returning only the last result */
  //jx_jxon_dump(stdout,"jx__code_exec_to_weak entered with code",node,code);
  //jx_jxon_dump(stdout,"                               and node",node,node);

  jx_ob result = jx_ob_from_null();
  jx_int size = jx_list_size(code);
  if(size) {
    jx_list *code_list = code.data.io.list;
    jx_ob inst = jx__list_borrow(code_list, 0);
    /* code = [ *fn*, ...] */
    if(jx_builtin_callable_check(inst)) { /* code block with only a single instruction */
      inst = jx_code_eval_to_weak(tls,flags, node, code);
      if(!jx_opcode_check(inst)) 
        jx_ob_replace(&result,inst);
      if(tls->leave) {
        if(tls->have_result) {
          jx_ob_replace(&result,tls->result);
          tls->have_result = JX_FALSE;
        }
        tls->result = jx_ob_from_null();
        tls->leave--;
      }        
    } else {
      /* code = [ [ ... ]  [ ... ] .... ] */
      jx_int pc = 0;
      tls->leave = 0;
      while( pc<size ) {
        inst = jx__list_borrow(code_list, pc);
        if(!jx_opcode_check(inst)) {
          inst = jx_code_eval_to_weak(tls, flags, node, inst);
          if(!jx_opcode_check(inst)) {
            jx_ob_replace(&result, inst);
            pc++;
            if(tls->break_seen) {
              break;
            }
            if(tls->leave) {
              if(tls->have_result) {
                jx_ob_replace(&result,tls->result);
                tls->have_result = JX_FALSE;
              }
              tls->result = jx_ob_from_null();
              tls->leave--;
              break;
            }
            continue;
          }
        }
        switch(inst.meta.bits & JX_META_MASK_OPCODE_INST) {
        case JX_OPCODE_RETURN:
          tls->leave = -1;
          return result; /* return current (last evaluated) */
          break;
        case JX_OPCODE_TAIL_CALL:
          tls->leave = -1;
          tls->tail_call = JX_TRUE;
          return result; /* return current (last evaluated) */
          break;
        case JX_OPCODE_BREAK:
        case JX_OPCODE_CONTINUE:
          return result; /* return current (last evaluated) */
          break;
        case JX_OPCODE_JUMP_RELATIVE:
          pc += inst.data.io.int_; 
          if((pc<0)||(pc>=size)) { /* range checked */
            return jx_null_with_ob(result);
          }
          break;
        }
      }
    }
  }
  return result;
}

jx_ob jx__code_eval(jx_tls *tls, jx_int flags, jx_ob node, jx_ob code)
{
  jx_ob result;
  //  jx_jxon_dump(stdout,"jx__code_eval entered",code);
  if(tls) {
    result = jx_ob_not_weak_with_ob(jx__code_eval_to_weak(tls,flags,node,code));
  } else {
    jx_tls *tls = jx_tls_new(); // use stack not heap?
    tls->builtins = jx_hash_borrow(node,jx_builtins());
    result = jx_ob_not_weak_with_ob(jx__code_eval_to_weak(tls,flags,node,code));    
    jx_tls_free(tls);
  }
  return result;
}

jx_ob jx__code_exec(jx_tls *tls, jx_int flags, jx_ob node, jx_ob code)
{ 
  //jx_jxon_dump(stdout,"jx__code_exec entered with code",node,code);
  //jx_jxon_dump(stdout,"                       and node",node,node);
  jx_ob result;
  if(tls) {
    result = jx_ob_not_weak_with_ob(jx__code_exec_to_weak(tls,flags,node,code));
  } else {
    jx_tls *tls = jx_tls_new(); // ditto w/ above
    tls->builtins = jx_hash_borrow(node,jx_ob_from_null());
    result = jx_ob_not_weak_with_ob(jx__code_exec_to_weak(tls,flags,node,code));
    jx_tls_free(tls);
  }
  //jx_jxon_dump(stdout,"jx__code_exec existing with result",node,result);
  return result;
}
                  
