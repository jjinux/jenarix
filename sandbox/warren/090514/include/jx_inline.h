#ifndef _H_jx_inline
#define _H_jx_inline

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

#include <math.h>

/* adapt to tiny string size (a least as large as a machine
   pointer + 2 bytes) */

#if (JX_TINY_STR_SIZE < JX_TINY_STR_MIN_SIZE)
#undef JX_TINY_STR_SIZE
#define JX_TINY_STR_SIZE JX_TINY_STR_MIN_SIZE
#endif

#if (JX_TINY_STR_SIZE == 6)
typedef jx_int32 jx_data_word;
#define JX_DATA_INIT  {{0,0}}
#else
#if (JX_TINY_STR_SIZE == 10)
typedef jx_int64 jx_data_word;
#define JX_DATA_INIT  {{0,0}}
#else
#if (JX_TINY_STR_SIZE == 18)
typedef jx_int64 jx_data_word[2];
#define JX_DATA_INIT  {{{0,0},0}}
#endif
#endif
#endif

typedef struct jx__list jx_list;
typedef struct jx__hash jx_hash;
typedef struct jx__opaque_ob jx_opaque_ob;
typedef struct jx__function jx_function;

typedef jx_uint16 jx_bits;

/* header record for gc'd objects */

typedef struct {
  jx_bool shared; /* shared access flag (read_only, immortal, etc.) */
} jx_gc;

typedef struct { /* get rid of this later on */
  jx_bits bits;
} jx_meta;

typedef struct { /* for fast initialization / comparison */
  jx_data_word word;
  jx_bits bits;
} jx_data_raw;

/* opaque objects -- runtime objects inside jenarix containers */

typedef jx_status (*jx_opaque_free_fn)(jx_opaque_ob*); 

typedef jx_ob (*jx_native_fn)(jx_ob, jx_ob); /* o{e}, [ma] */

typedef union {
  jx_bool bool_;
  jx_int int_;
  jx_float float_;
  jx_char tiny_str[JX_TINY_STR_SIZE];
  jx_char *str; /* NEVER ACCESS DIRECTLY!
                   note: vla ptr to jx_str header, not first char */
  jx_list *list;
  jx_hash *hash; 
  void *vla; /* builtin */
  jx_function  *function; /* builtin */
  jx_native_fn native_fn; /* builtin */
  jx_opaque_ob *opaque_ob; /* builtin */
} jx_data_io;

typedef union {
  jx_data_raw raw; 
  jx_data_io io;
} jx_data;

struct jx__ob { 
  jx_data data; 
  jx_meta meta;
};

/* meta flag bits */

#define JX_META_NOT_AN_OB              0x8000

#define JX_META_BIT_GC                 0x4000
/* set if object has garbage collected resourcese */

#define JX_META_BIT_BOOL               0x2000
#define JX_META_BIT_INT                0x1000
#define JX_META_BIT_FLOAT              0x0800
#define JX_META_BIT_STR                0x0400
#define JX_META_BIT_LIST               0x0200
#define JX_META_BIT_HASH               0x0100

/* identifiers (for use in JSON-based code) */
#define JX_META_BIT_IDENT              0x0080

/* pointers to built-in entities (runtime-only, not serializable) */

#define JX_META_BIT_BUILTIN            0x0040

#define JX_META_MASK_TINY_STR_SIZE     0x001F

#define JX_META_MASK_TYPE_BITS         0x3FC0

#define JX_META_MASK_FOR_HASH          0xFFDF

/* this bit is set in weak references */
#define JX_META_BIT_WEAK_REF           0x0020

/* tiny_str size bits are used by builtins */

#define JX_META_BIT_BUILTIN_VLA        0x0001
#define JX_META_BIT_BUILTIN_SELECTOR   0x0002
#define JX_META_BIT_BUILTIN_OPAQUE_OB  0x0004
#define JX_META_BIT_BUILTIN_NATIVE_FN  0x0008
#define JX_META_BIT_BUILTIN_FUNCTION   0x0010

/* object initializers */

#define JX_OB_NULL     { JX_DATA_INIT, {0                    }}
#define JX_OB_INT      { JX_DATA_INIT, {JX_META_BIT_INT      }}
#define JX_OB_FLOAT    { JX_DATA_INIT, {JX_META_BIT_FLOAT    }}
#define JX_OB_BOOL     { JX_DATA_INIT, {JX_META_BIT_BOOL     }}

#define JX_OB_LIST     { JX_DATA_INIT, {JX_META_BIT_GC | JX_META_BIT_LIST  }}
#define JX_OB_HASH     { JX_DATA_INIT, {JX_META_BIT_GC | JX_META_BIT_HASH  }}

/* inline methods */

#define JX_INLINE __inline__ static

struct jx__list {
  jx_bits packed_meta_bits;
  jx_gc gc;
  union {
    jx_ob *ob_vla;     
    jx_float *float_vla;
    jx_int *int_vla;
    void *vla;
  } data;
};

struct jx__opaque_ob {
  jx_gc gc;
  jx_opaque_free_fn free_fn;
  jx_ob magic;
};

struct jx__function {
  jx_gc gc;
  jx_ob name;
  jx_ob node; 
  jx_ob code;
};

typedef struct {
  jx_gc gc;
} jx_str;

/* prototypes */


jx_char *jx_ob_as_str(jx_ob * ob);
jx_ob jx_ob_from_str(jx_char * str);
jx_ob jx_ob_from_ident(jx_char * ident);

jx_ob jx__str__concat(jx_char *left,jx_int left_len, jx_char *right, jx_int right_len);
jx_ob jx__ob_add(jx_ob left, jx_ob right);
jx_status jx__ob_free(jx_ob ob);

JX_INLINE jx_status jx_ob_free(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_GC) {
    if(bits & JX_META_BIT_WEAK_REF) {
      return JX_FAILURE;
    } else {
      /* object has resources which need to be garbage collected */
      return jx__ob_free(ob);   /* call non-inline routine */
    }
  }
  return JX_SUCCESS;
}

jx_ob jx_ob_to_jxon_with_flags(jx_ob ob, jx_int flags);

JX_INLINE jx_ob jx_ob_to_jxon(jx_ob ob)
{
  return jx_ob_to_jxon_with_flags(ob, 0);
}

/* debugging */

JX_INLINE void jx_ob_dump(FILE *f, char *prefix, jx_ob ob)
{
  fprintf(f,"%s: %08x%04x %04x\n",prefix, 
          (unsigned int)ob.data.raw.word,
          (unsigned int)ob.data.raw.bits, (unsigned int)ob.meta.bits);
}


JX_INLINE void jx_jxon_dump(FILE *f, char *prefix, jx_ob ob)
{
  jx_ob jxon = jx_ob_to_jxon(ob);
  jx_ob_as_str(&jxon);
  fprintf(f,"%s: %s\n",prefix, jx_ob_as_str(&jxon));
  jx_ob_free(jxon);
}

/* builtin fn objects are a means through which jenarix can be extended */

JX_INLINE jx_ob jx_builtin_new_from_selector(jx_int selector)
{
  jx_ob result = JX_OB_NULL;
  result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR;
  result.data.io.int_ = selector;
  return result;
}

JX_INLINE jx_ob jx_builtin_new_with_vla(void **ref)
{
  jx_ob result = JX_OB_NULL;
  result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_VLA | JX_META_BIT_GC;
  result.data.io.vla = (*ref);
  return result;
}

JX_INLINE jx_ob jx_builtin_new_from_native_fn(jx_native_fn fn)
{
  jx_ob result = JX_OB_NULL;
  result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_NATIVE_FN | JX_META_BIT_WEAK_REF;
  result.data.io.native_fn = fn;
  return result;
}

JX_INLINE jx_ob jx_builtin_new_with_opaque_ob(jx_opaque_ob *opaque_ob)
{
  jx_ob result = JX_OB_NULL;
  result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_OPAQUE_OB | JX_META_BIT_GC;
  result.data.io.opaque_ob = opaque_ob;
  return result;
}

/* variable length array (vla) functions provide untyped, auto-zeroed,
   size and record-length aware variable length arrays NOTE: ALWAYS
   PASSED BY REFERENCE (since pointer location may change due to
   reallocation) */

typedef struct jx__vla jx_vla;

struct jx__vla {
  jx_vla *ptr;
  jx_int alloc;
  jx_int rec_size, size;
};

void *jx__vla_new(jx_int rec_size, jx_int size);

void *jx__vla_new_with_content(jx_int rec_size, jx_int size, void *content);
/* macros to avoid annoying type mismatch and aliasing warnings with (void**) parameter */
#define   jx_vla_new(r,s)           jx__vla_new(r,s)
#define   jx_vla_new_with_content(r,s,c) jx__vla_new_with_content(r,s,c)
#define   jx_vla_copy(r)            (void*)jx__vla_copy((void**)(void*)(r))
#define   jx_vla_size(r)            jx__vla_size((void**)(void*)(r))
#define   jx_vla_resize(r,s)        jx__vla_resize((void**)(void*)(r),(s))
#define   jx_vla_grow_check(r,i)    jx__vla_grow_check((void**)(void*)(r),(i))
#define   jx_vla_append(r,c)        jx__vla_append((void**)(void*)(r),(c))
#define   jx_vla_append_c_str(r,s)  jx__vla_append_c_str((void**)(void*)(r),(s))
#define   jx_vla_append_ob_str(r,o) jx__vla_append_ob_str((void**)(void*)(r),(o))
#define   jx_vla_insert(r,i,c)      jx__vla_insert((void**)(void*)(r),(i),(c))
#define   jx_vla_extend(r1,r2)      jx__vla_extend((void**)(void*)(r1),((void**)(void*)(r2)))
#define   jx_vla_remove(r,i,c)      jx__vla_remove((void**)(void*)(r),(i),(c))
#define   jx_vla_free(r)            jx__vla_free((void**)(void*)(r))
void *jx__vla_copy(void **ref);
JX_INLINE jx_int jx__vla_size(void **ref)
{
  if(*ref) {
    return ((jx_vla *) (*ref))[-1].size;
  } else {
    return 0;
  }
}

jx_status jx__vla_resize(void **ref, jx_int new_size);
JX_INLINE jx_bool jx__vla_grow_check(void **ref, jx_int index)
{
  if(((jx_vla *) (*ref))[-1].size > index) {
    return JX_TRUE;
  } else {
    return (jx_vla_resize(ref, index + 1) == JX_SUCCESS);
  }
}

jx_status jx__vla_append(void **ref, jx_int count);
jx_status jx__vla_append_c_str(void **ref, jx_char * str);
jx_status jx__vla_append_ob_str(void **ref, jx_ob ob);
jx_status jx__vla_insert(void **ref, jx_int index, jx_int count);
jx_status jx__vla_extend(void **ref1, void **ref2);
jx_status jx__vla_remove(void **ref, jx_int index, jx_int count);
jx_status jx__vla_free(void **ref);

JX_INLINE jx_int *jx_int_vla_new(jx_int size)
{
  return jx_vla_new(sizeof(jx_int), size);
}

JX_INLINE jx_status jx_int_vla_resize(jx_int ** ref, jx_int size)
{
  return jx_vla_resize((void **) ref, size);
}

JX_INLINE jx_status jx_int_vla_free(jx_int ** ref)
{
  return jx_vla_free(ref);
}

JX_INLINE jx_float *jx_float_vla_new(jx_float size)
{
  return jx_vla_new(sizeof(jx_float), size);
}

JX_INLINE jx_status jx_float_vla_resize(jx_float ** ref, jx_int size)
{
  return jx_vla_resize((void **) ref, size);
}

JX_INLINE jx_status jx_float_vla_free(jx_float ** ref)
{
  return jx_vla_free(ref);
}

jx_ob jx__ob_copy(jx_ob ob);
JX_INLINE jx_ob jx_ob_copy(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__ob_copy(ob) : ob;
}

jx_ob jx__ob_strong_with_ob(jx_ob ob);
JX_INLINE jx_ob jx_ob_strong_with_ob(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__ob_strong_with_ob(ob) : ob;
}

JX_INLINE jx_ob jx_ob_from_null(void)
{
  jx_ob result = JX_OB_NULL;
  return result;
}

JX_INLINE jx_ob jx_ob_from_bool(jx_bool bool_)
{
  jx_ob result = JX_OB_BOOL;
  result.data.io.bool_ = bool_;
  return result;
}

JX_INLINE jx_ob jx_ob_from_int(jx_int int_)
{
  jx_ob result = JX_OB_INT;
  result.data.io.int_ = int_;
  return result;
}

JX_INLINE jx_ob jx_ob_from_status(jx_status status)
{
  if(status == 0) {
    return jx_ob_from_null();
  } else {
    return jx_ob_from_int(status);
  }
}

JX_INLINE jx_ob jx_ob_from_float(jx_float float_)
{
  jx_ob result = JX_OB_FLOAT;
  result.data.io.float_ = float_;
  return result;
}

JX_INLINE jx_ob jx_null_with_ob(jx_ob ob)
{
  jx_ob result = JX_OB_NULL;
  jx_ob_free(ob);
  return result;
}

jx_bool jx__ob_as_bool(jx_ob ob);
JX_INLINE jx_bool jx_ob_as_bool(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BOOL) ? ob.data.io.bool_ : 
    ((bits & JX_META_BIT_INT) ? (ob.data.io.int_ ? JX_TRUE : JX_FALSE) :
     ((bits & JX_META_BIT_FLOAT) ? (ob.data.io.float_ ? JX_TRUE : JX_FALSE) : 
      ((bits & JX_META_BIT_STR) ? jx__ob_as_bool(ob) :
       JX_FALSE)));
}

jx_int jx__ob_as_int(jx_ob ob);
JX_INLINE jx_int jx_ob_as_int(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_INT) ? ob.data.io.int_ :
    ((bits & JX_META_BIT_FLOAT) ? (jx_int)ob.data.io.float_ :
     ((bits & JX_META_BIT_BOOL) ? (jx_int)ob.data.io.bool_ :
      ((bits & JX_META_BIT_STR) ? jx__ob_as_int(ob) :
       0)));
}

jx_float jx__ob_as_float(jx_ob ob);
JX_INLINE jx_float jx_ob_as_float(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_FLOAT) ? ob.data.io.float_ : 
    ((bits & JX_META_BIT_INT) ? (jx_float)ob.data.io.int_ :
     ((bits & JX_META_BIT_BOOL) ? (ob.data.io.bool_ ? 1.0 : 0.0) :
      ((bits & JX_META_BIT_STR) ? jx__ob_as_float(ob) :
       0.0F)));
}

jx_ob jx__ob_to_bool(jx_ob ob);
JX_INLINE jx_ob jx_ob_to_bool(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_BOOL)
    return ob;
  else if(!bits || (bits & ( JX_META_BIT_INT |
                        JX_META_BIT_FLOAT )))
    return jx_ob_from_bool(jx_ob_as_bool(ob));
  else
    return jx__ob_to_bool(ob);
}

jx_ob jx__ob_to_int(jx_ob ob);
JX_INLINE jx_ob jx_ob_to_int(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  if( bits & JX_META_BIT_INT )
    return ob;
  else if(!bits || (bits & ( JX_META_BIT_BOOL |
                             JX_META_BIT_FLOAT )))
    return jx_ob_from_int(jx_ob_as_int(ob));
  else
    return jx__ob_to_int(ob);
}

jx_ob jx__ob_to_float(jx_ob ob);
JX_INLINE jx_ob jx_ob_to_float(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_FLOAT)
    return ob;
  if(!bits || (bits & (JX_META_BIT_BOOL |
                       JX_META_BIT_INT )))
    return jx_ob_from_float(jx_ob_as_float(ob));
  else
    return jx__ob_to_float(ob);
}

jx_ob jx__ob_to_str(jx_ob ob);
JX_INLINE jx_ob jx_ob_to_str(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_STR) ?
    jx_ob_copy(ob) : jx__ob_to_str(ob);
}

jx_ob jx__ob_to_ident(jx_ob ob);
JX_INLINE jx_ob jx_ob_to_ident(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_IDENT) ?
    jx_ob_copy(ob) : jx__ob_to_ident(ob);
}

JX_INLINE jx_bool jx_null_check(jx_ob ob)
{
  return !ob.meta.bits;
}

JX_INLINE jx_bool jx_bool_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_BOOL) && JX_TRUE;
}

JX_INLINE jx_bool jx_int_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_INT) && JX_TRUE;
}

JX_INLINE jx_bool jx_float_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_FLOAT) && JX_TRUE;
}

JX_INLINE jx_bool jx_str_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_STR) && JX_TRUE;
}

JX_INLINE jx_bool jx_ident_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_IDENT) && JX_TRUE;
}

JX_INLINE jx_bool jx_list_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_LIST) && JX_TRUE;
}

JX_INLINE jx_bool jx_hash_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_HASH) && JX_TRUE;
}

JX_INLINE jx_bool jx_builtin_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_BUILTIN) && JX_TRUE;
}

JX_INLINE jx_bool jx_builtin_vla_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BUILTIN) && 
    (bits & JX_META_BIT_BUILTIN_VLA);
}

JX_INLINE jx_bool jx_builtin_selector_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BUILTIN) && 
    (bits & JX_META_BIT_BUILTIN_SELECTOR);
}

JX_INLINE jx_bool jx_builtin_opaque_ob_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BUILTIN) && 
    (bits & JX_META_BIT_BUILTIN_OPAQUE_OB);
}

JX_INLINE jx_bool jx_builtin_native_fn_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BUILTIN) && 
    (bits & JX_META_BIT_BUILTIN_NATIVE_FN);
}

JX_INLINE jx_bool jx_builtin_any_fn_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BUILTIN) && 
    (bits & 
     (JX_META_BIT_BUILTIN_SELECTOR |      
      JX_META_BIT_BUILTIN_NATIVE_FN |      
      JX_META_BIT_BUILTIN_FUNCTION));
}

JX_INLINE jx_bool jx_function_check(jx_ob ob)
{
  return jx_builtin_check(ob) && (ob.meta.bits & JX_META_BIT_BUILTIN_FUNCTION);
}

/* convenience function */

JX_INLINE jx_status jx_ob_replace(jx_ob *ob_ptr,jx_ob ob)
{
  jx_status result = jx_ob_free(*ob_ptr);
  *ob_ptr = ob;
  return result;
}

JX_INLINE jx_bool jx_ok(jx_status status)
{
  return (status >= 0);
}

JX_INLINE jx_int jx_str_len(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  return ((bits & JX_META_BIT_STR) ? 
          ((bits & JX_META_BIT_GC) ? 
           (ob.data.io.str ? 
            jx_vla_size(&ob.data.io.str) - (1 + sizeof(jx_str)) : 0)
           : bits & JX_META_MASK_TINY_STR_SIZE)
          : 0);
}

JX_INLINE jx_int jx_ident_len(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  return ((bits & JX_META_BIT_IDENT) ? 
          ((bits & JX_META_BIT_GC) ? 
           (ob.data.io.str ? jx_vla_size(&ob.data.io.str) 
            - (1 + sizeof(jx_str)) : 0)
           : bits & JX_META_MASK_TINY_STR_SIZE)
          : 0);
}

jx_status jx__ob_set_shared(jx_ob ob, jx_bool shared);
JX_INLINE jx_status jx_ob_set_shared(jx_ob ob, jx_bool shared)
{
  if(ob.meta.bits & JX_META_BIT_GC) {
    return jx__ob_set_shared(ob, shared);
  }
  return JX_SUCCESS;
}

jx_bool jx__ob_shared(jx_ob ob);
JX_INLINE jx_bool jx_ob_shared(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_GC) {
    return jx__ob_shared(ob);
  }
  return JX_FALSE;
}

JX_INLINE jx_ob jx_ob_take_weak_ref(jx_ob ob)
{
  jx_bits bits = ob.meta.bits; 
  if(bits & JX_META_BIT_GC) {
    ob.meta.bits = bits | JX_META_BIT_WEAK_REF;
  }
  return ob;
}

jx_bool jx__ob_gc_identical(jx_ob left, jx_ob right);

JX_INLINE jx_bool jx_ob_identical(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_FOR_HASH;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_FOR_HASH;

  if(left_bits != right_bits) {
    return JX_FALSE;
  } else if(left_bits & JX_META_BIT_GC) {
    return jx__ob_gc_identical(left, right);
  } else {
#if (JX_TINY_STR_SIZE == 6) ||  (JX_TINY_STR_SIZE == 10)
    return ((left_bits == right_bits) &&
            (left.data.raw.word == right.data.raw.word) &&
            (left.data.raw.bits == right.data.raw.bits));
#else
#if (JX_TINY_STR_SIZE == 18)
    return ((left_bits == right_bits) &&
            (left.data.raw.word[0] == right.data.raw.word[0]) &&
            (left.data.raw.word[1] == right.data.raw.word[1]) &&
            (left.data.raw.bits    == right.data.raw.bits   ));
#endif
#endif
  }
}

jx_bool jx__ob_gc_equal(jx_ob left, jx_ob right);
jx_bool jx__ob_non_gc_equal(jx_ob left, jx_ob right);

JX_INLINE jx_bool jx_ob_equal(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_FOR_HASH;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_FOR_HASH;

  if(left_bits != right_bits) {
    if((left_bits | right_bits) & JX_META_BIT_GC) {
      return JX_FALSE;
    } else {
      return jx__ob_non_gc_equal(left, right);
    }
  } else if(left_bits & JX_META_BIT_GC) {
    return jx__ob_gc_equal(left, right);
  } else {
    return jx_ob_identical(left, right);
  }
}

/* lists */

JX_INLINE jx_int jx__list_size(jx_list * I)
{
  return jx_vla_size(&I->data.vla);
}

JX_INLINE jx_int jx_list_size(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_LIST) {
    return jx__list_size(ob.data.io.list);
  } else
    return 0;
}

jx_status jx__list_resize(jx_list * I, jx_int size, jx_ob fill);
JX_INLINE jx_status jx_list_resize(jx_ob list, jx_int size, jx_ob fill)
{
  jx_bits bits = list.meta.bits;
  if(bits & JX_META_BIT_LIST) {
    if(jx_ok(jx__list_resize(list.data.io.list, size, fill))) {
      jx_ob_free(fill);
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
}

jx_ob jx_list_new(void);

JX_INLINE jx_ob jx_list_new_with_fill(jx_int size, jx_ob fill)
{
  jx_ob result = jx_list_new();
  if(!jx_ok(jx_list_resize(result, size, fill))) {
    jx_ob_free(result);
    result = jx_ob_from_null();
  }
  return result;
}

JX_INLINE jx_ob jx_list_new_with_first(jx_ob fill)
{
  jx_ob result = jx_list_new();
  if(!jx_ok(jx_list_resize(result, 1, fill))) {
    jx_ob_free(result);
    result = jx_ob_from_null();
  }
  return result;
}

jx_status jx__list_append(jx_list * I, jx_ob ob);
JX_INLINE jx_status jx_list_append(jx_ob list, jx_ob ob)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_append(list.data.io.list, ob) : 
    JX_FAILURE;
}

jx_status jx__list_insert(jx_list * I, jx_int index, jx_ob ob);
JX_INLINE jx_status jx_list_insert(jx_ob list, jx_int index, jx_ob ob)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_insert(list.data.io.list, index, ob) : 
    JX_FAILURE;
}

JX_INLINE void jx__list_set_packed_data(jx_list * list, jx_int index, jx_ob ob)
{
  switch (list->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_INT:
    list->data.int_vla[index] = ob.data.io.int_;
    break;
  case JX_META_BIT_FLOAT:
    list->data.float_vla[index] = ob.data.io.float_;
    break;
  }
}

jx_status jx__list_unpack_data(jx_list * list);
JX_INLINE jx_status jx__list_replace(jx_list * I, jx_int index, jx_ob ob)
{
  if((!I->gc.shared) && (index >=0) && (index < jx_vla_size(&I->data.vla))) {
    if(!I->packed_meta_bits) {
      jx_ob_replace(I->data.ob_vla + index, ob);
      return JX_SUCCESS;
    } else {
      if(I->data.vla && (I->packed_meta_bits != ob.meta.bits)) {
        if(!jx_ok(jx__list_unpack_data(I)))
          return JX_FAILURE;
      }
      if(I->data.vla) {
        if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
          jx__list_set_packed_data(I, index, ob);
        } else if(!I->packed_meta_bits) {
          jx_ob_replace(I->data.ob_vla + index, ob);
        }
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

JX_INLINE jx_status jx_list_replace(jx_ob list, jx_int index, jx_ob ob)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_replace(list.data.io.list, index, ob) : 
    JX_FAILURE;
}

jx_status jx__list_combine(jx_list * list1, jx_list * list2);
JX_INLINE jx_status jx_list_combine(jx_ob list1, jx_ob list2)
{
  jx_bits bits1 = list1.meta.bits;
  jx_bits bits2 = list2.meta.bits;
  if((bits1 & JX_META_BIT_LIST) && (bits2 & JX_META_BIT_LIST)) {
    if((bits2 & JX_META_BIT_WEAK_REF) || (jx_ob_shared(list2))) {
      /* copy list2 if weak or shared */
      list2 = jx_ob_copy(list2);
      if(!jx_ok(jx__list_combine(list1.data.io.list, list2.data.io.list)))
        jx_ob_free(list2);
      else
        return JX_SUCCESS;
    } else {
      return jx__list_combine(list1.data.io.list, list2.data.io.list);
    }
  }
  return JX_FAILURE;
}

JX_INLINE jx_ob jx__list_get_packed_data(jx_list * list, jx_int index)
{
  switch (list->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_INT:
    return jx_ob_from_int(list->data.int_vla[index]);
    break;
  case JX_META_BIT_FLOAT:
    return jx_ob_from_float(list->data.float_vla[index]);
    break;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx__list_borrow(jx_list * I, jx_int index)
{
  if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
    if(I->packed_meta_bits) {
      return jx__list_get_packed_data(I, index);
    } else {
      return I->data.ob_vla[index];
    }
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_list_borrow(jx_ob list, jx_int index)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_borrow(list.data.io.list, index) :
    jx_ob_from_null();
}

JX_INLINE jx_ob jx_list_get(jx_ob list, jx_int index)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx_ob_copy(jx__list_borrow(list.data.io.list, index)) :
    jx_ob_from_null();
}

jx_ob jx__list_remove(jx_list * I, jx_int index);
JX_INLINE jx_ob jx_list_remove(jx_ob list, jx_int index)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_remove(list.data.io.list, index) : 
    jx_ob_from_null();
}

jx_status jx__list_delete(jx_list * I, jx_int index);
JX_INLINE jx_status jx_list_delete(jx_ob list, jx_int index)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_delete(list.data.io.list, index) : 
    JX_FAILURE;
}

jx_status jx__list_set_int_vla(jx_list * list, jx_int ** ref);
JX_INLINE jx_status jx_list_set_int_vla(jx_ob list, jx_int ** ref)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_set_int_vla(list.data.io.list, ref) : 
    JX_FAILURE;
}

jx_status jx__list_set_float_vla(jx_list * list, jx_float ** ref);
JX_INLINE jx_status jx_list_set_float_vla(jx_ob list, jx_float ** ref)
{
  jx_bits bits = list.meta.bits;
  if(bits & JX_META_BIT_LIST) {
    return jx__list_set_float_vla(list.data.io.list, ref);
  }
  return JX_FAILURE;
}

#define JX__HASH_COPY_KEYS   0x1
#define JX__HASH_COPY_VALUES 0x2
jx_ob jx__hash_copy_members(jx_hash * I, jx_int flags);
JX_INLINE jx_ob jx_list_new_from_hash(jx_ob hash)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_copy_members(hash.data.io.hash,
                                 JX__HASH_COPY_KEYS | JX__HASH_COPY_VALUES);
  }
  return jx_ob_from_null();
}

jx_status jx__list_with_hash(jx_list * list, jx_hash * hash);
JX_INLINE jx_ob jx_list_new_with_hash(jx_ob hash)
{
  jx_bits bits = hash.meta.bits;
  if(bits & JX_META_BIT_HASH) {
    if(bits & JX_META_BIT_WEAK_REF) {
      return jx_list_new_from_hash(hash);
    } else {
      jx_ob result = jx_list_new();
      if(result.meta.bits & JX_META_BIT_LIST) {
        if(jx_ok(jx__list_with_hash(result.data.io.list, hash.data.io.hash))) {
          jx_ob_free(hash);
        } else {
          jx_ob_free(result);
          result = hash;
        }
      }
      return result;
    }
  }
  return jx_ob_from_null();
}

/* hashing */

JX_INLINE jx_uint32 jx__ob_hash_code(jx_ob ob)
{
#if (JX_TINY_STR_SIZE == 6)
  register jx_uint32 a = (((jx_uint32) (ob.meta.bits & JX_META_MASK_FOR_HASH)) ^
                          ((jx_uint32) ob.data.raw.bits) ^ ((jx_uint32) ob.data.raw.word));
#else
#if (JX_TINY_STR_SIZE == 10)
  register jx_uint32 a = (((jx_uint32) (ob.meta.bits & JX_META_MASK_FOR_HASH)) ^
                          ((jx_uint32) ob.data.raw.bits) ^
                          ((jx_uint32) ob.data.raw.word) ^ 
                          ((jx_uint32) (ob.data.raw.word >> 32)));
#else
#if (JX_TINY_STR_SIZE == 18)
  register jx_uint32 a = (((jx_uint32) (ob.meta.bits & JX_META_MASK_FOR_HASH)) ^
                          ((jx_uint32) ob.data.bits) ^
                          ((jx_uint32) ob.data.raw.word[0]) ^
                          ((jx_uint32) (ob.data.raw.word[0] >> 32)) ^
                          ((jx_uint32) ob.data.raw.word[1]) ^ 
                          ((jx_uint32) (ob.data.raw.word[1] >> 32)));
#endif
#endif
#endif
  a += ~(a << 15);
  a ^= (a >> 10);
  a += (a << 3);
  a ^= (a >> 6);
  a += ~(a << 11);
  a ^= (a >> 16);
  return (a ? a : 1);           /* zero is reserved as the hash code of an
                                   unhashable object */
}

jx_uint32 jx__ob_gc_hash_code(jx_ob);
JX_INLINE jx_uint32 jx_ob_hash_code(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__ob_gc_hash_code(ob) : jx__ob_hash_code(ob);
}

jx_ob jx_hash_new(void);
jx_status jx__hash_from_list(jx_hash * hast, jx_list * list);
JX_INLINE jx_ob jx_hash_new_from_list(jx_ob list)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_ob result = jx_hash_new();
    if(result.meta.bits & JX_META_BIT_HASH) {
      if(!jx_ok(jx__hash_from_list(result.data.io.hash, list.data.io.list))) {
        jx_ob_free(result);
        result = jx_ob_from_null();
      }
    }
    return result;
  }
  return jx_ob_from_null();
}

jx_status jx__hash_with_list(jx_hash * hash, jx_list * list);
JX_INLINE jx_ob jx_hash_new_with_list(jx_ob list)
{
  jx_bits bits = list.meta.bits;
  if(bits & JX_META_BIT_LIST) {
    if(bits & JX_META_BIT_WEAK_REF) {
      return jx_hash_new_from_list(list);
    } else {
      jx_ob result = jx_hash_new();
      if(result.meta.bits & JX_META_BIT_HASH) {
        if(jx_ok(jx__hash_with_list(result.data.io.hash, list.data.io.list))) {
          jx_ob_free(list);
        } else {
          jx_ob_free(result);
          result = list;
        }
      }
      return result;
    }
  }
  return list;
}

jx_int jx__hash_size(jx_hash * I);
JX_INLINE jx_int jx_hash_size(jx_ob hash)
{
  return (hash.meta.bits & JX_META_BIT_HASH) ?
    jx__hash_size(hash.data.io.hash) : 0;
}

jx_status jx__hash_set(jx_hash * I, jx_ob key, jx_ob value);
JX_INLINE jx_status jx_hash_set(jx_ob hash, jx_ob key, jx_ob value)
{
  return (hash.meta.bits & JX_META_BIT_HASH) ?
    jx__hash_set(hash.data.io.hash, key, value) : JX_FAILURE;
}

jx_bool jx__hash_has_key(jx_hash * I, jx_ob key);
JX_INLINE jx_bool jx_hash_has_key(jx_ob hash, jx_ob key)
{
  return (hash.meta.bits & JX_META_BIT_HASH) ?
    jx__hash_has_key(hash.data.io.hash, key) : JX_FALSE;
}

JX_INLINE jx_ob jx_hash_keys(jx_ob hash)
{
  return (hash.meta.bits & JX_META_BIT_HASH) ?
    jx__hash_copy_members(hash.data.io.hash, JX__HASH_COPY_KEYS) : 
    jx_ob_from_null();
}

JX_INLINE jx_ob jx_hash_values(jx_ob hash)
{
  return (hash.meta.bits & JX_META_BIT_HASH) ?
    jx__hash_copy_members(hash.data.io.hash, JX__HASH_COPY_VALUES) : 
    jx_ob_from_null();
}

jx_bool jx__hash_peek(jx_ob * result, jx_hash * I, jx_ob key);
JX_INLINE jx_bool jx_hash_peek(jx_ob * result, jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_peek(result, hash.data.io.hash, key);
  }
  return JX_FALSE;
}

JX_INLINE jx_ob jx_hash_borrow(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_peek(&result, hash.data.io.hash, key))
      return result;
  }
  return jx_ob_from_null();
}


JX_INLINE jx_ob jx_hash_get(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_peek(&result, hash.data.io.hash, key))
      return jx_ob_copy(result);
  }
  return jx_ob_from_null();
}

jx_bool jx__hash_remove(jx_ob * result, jx_hash * I, jx_ob key);
JX_INLINE jx_ob jx_hash_remove(jx_ob hash, jx_ob key)
{
  jx_bits bits = hash.meta.bits;
  if(bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_remove(&result, hash.data.io.hash, key))
      return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_status jx_hash_delete(jx_ob hash, jx_ob key)
{
  jx_bits bits = hash.meta.bits;
  if(bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_remove(&result, hash.data.io.hash, key)) {
      jx_ob_free(result);
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
}

jx_bool jx__hash_peek_key(jx_ob * result, jx_hash * I, jx_ob value);
JX_INLINE jx_ob jx_hash_borrow_key(jx_ob hash, jx_ob value)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_peek_key(&result, hash.data.io.hash, value))
      return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_hash_get_key(jx_ob hash, jx_ob value)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_peek_key(&result, hash.data.io.hash, value))
      return jx_ob_copy(result);
  }
  return jx_ob_from_null();
}


JX_INLINE jx_ob jx_ob_add(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if(left_bits == right_bits) {
    switch(left_bits) {
    case JX_META_BIT_BOOL:
      return jx_ob_from_int( left.data.io.bool_ + right.data.io.bool_ );
      break;
    case JX_META_BIT_INT:
      return jx_ob_from_int(  left.data.io.int_ + right.data.io.int_ );
      break;
    case JX_META_BIT_FLOAT:
      return jx_ob_from_float( left.data.io.float_ + right.data.io.float_ );
      break;
    case JX_META_BIT_STR:
      return jx__str__concat(jx_ob_as_str(&left), jx_str_len(left),
                             jx_ob_as_str(&right), jx_str_len(right));
      break;
    }
  } 
  return jx__ob_add(left,right);
}

#define JX_MATH_OP(SUFFIX,OPER) \
jx_ob jx__ob_ ## SUFFIX(jx_ob left, jx_ob right); \
JX_INLINE jx_ob jx_ob_ ## SUFFIX(jx_ob left, jx_ob right) \
{ \
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS; \
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS; \
  if(left_bits == right_bits) { \
    switch(left_bits) { \
    case JX_META_BIT_INT: \
      return jx_ob_from_int(  left.data.io.int_ OPER right.data.io.int_ ); \
      break; \
    case JX_META_BIT_FLOAT: \
      return jx_ob_from_float( left.data.io.float_ OPER right.data.io.float_ ); \
      break; \
    case JX_META_BIT_BOOL: \
      return jx_ob_from_int( left.data.io.bool_ OPER right.data.io.bool_ ); \
      break; \
    }\
  }\
  return jx__ob_ ## SUFFIX(left,right);\
}

JX_MATH_OP(sub,-)
JX_MATH_OP(mul,*)
JX_MATH_OP(div,/)
#undef JX_MATH_OP

#define JX_LOG_OP(SUFFIX,OPER) \
jx_ob jx__ob_ ## SUFFIX(jx_ob left, jx_ob right); \
JX_INLINE jx_ob jx_ob_ ## SUFFIX(jx_ob left, jx_ob right) \
{ \
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS; \
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS; \
  if(left_bits == right_bits) { \
    switch(left_bits) { \
    case JX_META_BIT_INT: \
      return jx_ob_from_bool(  left.data.io.int_ OPER right.data.io.int_ ); \
      break; \
    case JX_META_BIT_FLOAT: \
      return jx_ob_from_bool( left.data.io.float_ OPER right.data.io.float_ ); \
      break; \
    case JX_META_BIT_BOOL: \
      return jx_ob_from_bool( left.data.io.bool_ OPER right.data.io.bool_ ); \
      break; \
    }\
  }\
  return jx__ob_ ## SUFFIX(left,right);\
}

JX_LOG_OP(and,&&)
JX_LOG_OP(or,||)

#undef JX_LOG_OP

jx_ob jx__ob_mod(jx_ob left, jx_ob right); 
JX_INLINE jx_ob jx_ob_mod(jx_ob left, jx_ob right) 
{ 
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS; 
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS; 
  if(left_bits == right_bits) { 
    switch(left_bits) { 
    case JX_META_BIT_INT: 
      return jx_ob_from_int(  left.data.io.int_ % right.data.io.int_ ); 
      break; 
    case JX_META_BIT_FLOAT: 
      return jx_ob_from_float( fmod( left.data.io.float_, right.data.io.float_ ));
      break; 
    case JX_META_BIT_BOOL: 
      return jx_ob_from_int( left.data.io.bool_ % right.data.io.bool_ ); 
      break; 
    }
  }
  return jx__ob_mod(left,right);\
}

JX_INLINE jx_ob jx_ob_neg(jx_ob ob)
{ 
  switch(ob.meta.bits) {  
  case JX_META_BIT_INT:                                                 
    return jx_ob_from_int( -ob.data.io.int_ ); 
    break;                                                              
  case JX_META_BIT_FLOAT: 
    return jx_ob_from_float( -ob.data.io.float_ );
    break; 
  case JX_META_BIT_BOOL:  
    return jx_ob_from_int( -ob.data.io.bool_ );
    break; 
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_ob_not(jx_ob ob)
{ 
  switch(ob.meta.bits & JX_META_MASK_TYPE_BITS) {  
  case JX_META_BIT_STR:
    return jx_ob_from_bool( !jx_str_len(ob) );
    break;
  case JX_META_BIT_INT:                                                 
    return jx_ob_from_bool( !ob.data.io.int_ ); 
    break;                                                              
  case JX_META_BIT_FLOAT: 
    return jx_ob_from_bool( !ob.data.io.float_ );
    break; 
  case JX_META_BIT_BOOL:  
    return jx_ob_from_bool( !ob.data.io.bool_ );
    break; 
  }
  return jx_ob_from_null();
}


jx_ob jx__str_concat(jx_ob left, jx_ob right);
JX_INLINE jx_ob jx_str_concat(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if((left_bits == right_bits) && (left_bits & JX_META_BIT_STR)) {
    return jx__str__concat(jx_ob_as_str(&left), jx_str_len(left),
                           jx_ob_as_str(&right), jx_str_len(right));
  } else {
    return jx__str_concat(left,right);
  }
}

jx_int jx__str__compare(jx_char *left, jx_char *right);
jx_int jx__str_compare(jx_ob left, jx_ob right);
JX_INLINE jx_int jx_str_compare(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if((left_bits == right_bits) && (left_bits & JX_META_BIT_STR)) {
    return jx__str__compare(jx_ob_as_str(&left),jx_ob_as_str(&right));
  } else {
    return jx__str_compare(left,right);
  }
}

JX_INLINE jx_ob jx_str_concat_with_both(jx_ob left, jx_ob right)
{
  jx_ob result = jx_str_concat(left, right);
  jx_ob_free(left);
  jx_ob_free(right);
  return result;
}

jx_ob jx__str_join_with_list(jx_list * list, jx_char *sep);
JX_INLINE jx_ob jx_str_join_with_list(jx_ob list)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_ob result = jx__str_join_with_list(list.data.io.list,NULL);
    jx_ob_free(list);
    return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_str_join_with_list_sep(jx_ob list, jx_ob sep)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_char *sep_str = NULL;
    jx_ob result;
    if(sep.meta.bits & JX_META_BIT_STR) {
      sep_str = jx_ob_as_str(&sep);
    }
    result = jx__str_join_with_list(list.data.io.list,sep_str);
    jx_ob_free(sep);
    jx_ob_free(list);
    return result;
  }
  return jx_ob_from_null();
}

/* comparison operators */

jx_bool jx__ob_lt(jx_ob left, jx_ob right);
JX_INLINE jx_bool jx_ob_lt(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if(left_bits == right_bits) {
    switch(left_bits) {
    case JX_META_BIT_BOOL:
      return left.data.io.bool_ < right.data.io.bool_;
      break;
    case JX_META_BIT_INT:
      return left.data.io.int_ < right.data.io.int_;
      break;
    case JX_META_BIT_FLOAT:
      return left.data.io.float_ < right.data.io.float_;
      break;
    case JX_META_BIT_STR:
      return jx__str__compare(jx_ob_as_str(&left),jx_ob_as_str(&right)) < 0;
      break;
    }
  } 
return JX_FALSE;
}

jx_bool jx__ob_gt(jx_ob left, jx_ob right);
JX_INLINE jx_bool jx_ob_gt(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if(left_bits == right_bits) {
    switch(left_bits) {
    case JX_META_BIT_BOOL:
      return left.data.io.bool_ > right.data.io.bool_;
      break;
    case JX_META_BIT_INT:
      return left.data.io.int_ > right.data.io.int_;
      break;
    case JX_META_BIT_FLOAT:
      return left.data.io.float_ > right.data.io.float_;
      break;
    case JX_META_BIT_STR:
      return jx__str__compare(jx_ob_as_str(&left),jx_ob_as_str(&right)) > 0;
      break;
    }
  } 
  return jx__ob_gt(left,right);
}

jx_bool jx__ob_le(jx_ob left, jx_ob right);
JX_INLINE jx_bool jx_ob_le(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if(left_bits == right_bits) {
    switch(left_bits) {
    case JX_META_BIT_BOOL:
      return left.data.io.bool_ <= right.data.io.bool_;
      break;
    case JX_META_BIT_INT:
      return left.data.io.int_ <= right.data.io.int_;
      break;
    case JX_META_BIT_FLOAT:
      return left.data.io.float_ <= right.data.io.float_;
      break;
    case JX_META_BIT_STR:
      return jx__str__compare(jx_ob_as_str(&left),jx_ob_as_str(&right)) <= 0;
      break;
    }
  } 
return JX_FALSE;
}

jx_bool jx__ob_ge(jx_ob left, jx_ob right);
JX_INLINE jx_bool jx_ob_ge(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  if(left_bits == right_bits) {
    switch(left_bits) {
    case JX_META_BIT_BOOL:
      return left.data.io.bool_ >= right.data.io.bool_;
      break;
    case JX_META_BIT_INT:
      return left.data.io.int_ >= right.data.io.int_;
      break;
    case JX_META_BIT_FLOAT:
      return left.data.io.float_ >= right.data.io.float_;
      break;
    case JX_META_BIT_STR:
      return jx__str__compare(jx_ob_as_str(&left),jx_ob_as_str(&right)) >= 0;
      break;
    }
  } 
  return jx__ob_ge(left,right);
}

/* size measurement */

jx_int jx_list_size(jx_ob);
JX_INLINE jx_ob jx_ob_size(jx_ob ob)
{
  switch(ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_LIST:
    return jx_ob_from_int( jx_list_size(ob));
    break;
  case JX_META_BIT_HASH:
    return jx_ob_from_int( jx_hash_size(ob));
    break;
  case JX_META_BIT_STR:
    return jx_ob_from_int( jx_str_len(ob));
    break;
  case JX_META_BIT_IDENT:
    return jx_ob_from_int( jx_ident_len(ob));
    break;
  default:
    return jx_ob_from_int(1);
    break;
  }
}


jx_ob jx__code_eval(jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_eval(jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_eval(node,expr) : expr;
}

jx_ob jx__code_exec(jx_ob node, jx_ob code);
JX_INLINE jx_ob jx_code_exec(jx_ob node, jx_ob code)
{
  return (code.meta.bits & JX_META_BIT_LIST) ?
    jx__code_exec(node,code) : jx_ob_copy(code);
}

JX_INLINE jx_ob jx_function_call(jx_function *fn, jx_ob node, jx_ob payload)
{
  jx_ob payload_ident = jx_ob_from_ident("_");
  if(jx_hash_check(fn->node)) {
    /* standard functions run inside their own node namespace (and
       thus can potentially be concurrent) */
    jx_ob inv_node = jx_ob_copy(fn->node);
    jx_ob result = JX_OB_NULL;
    if(jx_ok( jx_hash_set(inv_node,jx_ob_from_ident("_"),payload))) {
      result = jx_code_exec(inv_node,fn->code);
    }
    jx_ob_free(inv_node);
    return result;
  } else {
    /* inner functions run within the host node namespace */
    jx_ob saved_payload = jx_hash_remove(node, payload_ident);
    if(jx_ok( jx_hash_set(node,payload_ident, payload) ) ) { 
      jx_ob result = jx_code_exec(node,fn->code);
      if(!jx_ok(jx_hash_set(node,payload_ident,saved_payload)))
        jx_ob_free(saved_payload);
      return result;
    } else {
      jx_ob_free(payload);
      jx_hash_set(node, payload_ident, saved_payload);
      return jx_ob_from_null();
    }
  }
}
#endif
