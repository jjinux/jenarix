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

/* Depending on compliation settings, Jenarix objects are either 64,
   96, or 160 bits wide (that's 48, 80, or 160 bits of data plus 16
   bits of meta information).  Within the available space, the data
   field can contain either 32 bit or 64 bit C primitives, and tiny
   embedded strings of 6, 10, or 18 bytes including the sentinel */

/* adapt to tiny string size (a least as large as a machine
   pointer + 2 bytes) */

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif


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
typedef struct jx__tls jx_tls;

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


/* unfortunately, older compilers will warn on the type-checked
   form of the following typedef... */
#ifndef __GCC4___
typedef jx_ob (*jx_native_fn)(); /* o{e}, [ma] */
#else
typedef jx_ob (*jx_native_fn)(jx_ob, jx_ob); /* o{e}, [ma] */
#endif

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

#define JX_META_BIT_WEAK_REF           0x8000
/* set in weak references */

#define JX_META_BIT_GC                 0x4000
/* set if object has garbage collected resourcese */

/* Jenarix primitive types (note that null has none of the type bits
   set and corresponds to zero'd memory */

#define JX_META_BIT_BOOL               0x2000
#define JX_META_BIT_INT                0x1000
#define JX_META_BIT_FLOAT              0x0800
#define JX_META_BIT_STR                0x0400
#define JX_META_BIT_LIST               0x0200
#define JX_META_BIT_HASH               0x0100

/* JXON identifiers */

#define JX_META_BIT_IDENT              0x0080

/* JXON VM opcodes */

#define JX_META_BIT_OPCODE             0x0040

/* bit pattern for memory matching no object */

#define JX_META_NOT_AN_OB              0x8000

/* pointers to built-in entities (runtime-only, not serializable) */

#define JX_META_BIT_BUILTIN            0x0020

#define JX_META_MASK_TINY_STR_SIZE     0x001F

#define JX_META_MASK_OPCODE_INST       0x001F

#define JX_META_MASK_TYPE_BITS         0x3FE0

#define JX_META_MASK_FOR_HASH          0x7FFF

/* tiny_str size bits are re-used by builtins*/
#define JX_META_MASK_BUILTIN_TYPE      0x001F

/* note: we made need more builtins and will need to switch
   from-bit masked to indexed builtin types */

#define JX_META_BIT_BUILTIN_ENTITY     0x0000
#define JX_META_BIT_BUILTIN_OPAQUE_OB  0x0001
#define JX_META_BIT_BUILTIN_SELECTOR   0x0002
#define JX_META_BIT_BUILTIN_MACRO      0x0004
#define JX_META_BIT_BUILTIN_NATIVE_FN  0x0008
#define JX_META_BIT_BUILTIN_FUNCTION   0x0010

/* tiny tuple idea...

  jx_int  tiny_tuple_int[JX_TINY_LIST_SIZE];
  jx_float tiny_tuple_float[JX_TINY_LIST_SIZE];

#define JX_META_MASK_TINY_TUPLE_SIZE    0x0007

#define JX_META_BIT_TINY_TUPLE_INT      0x0008
#define JX_META_BIT_TINY_TUPLE_FLOAT    0x0010

*/

/* object initializers */

#define JX_OB_NULL      { JX_DATA_INIT, {0                    }}
#define JX_OB_INT       { JX_DATA_INIT, {JX_META_BIT_INT      }}
#define JX_OB_FLOAT     { JX_DATA_INIT, {JX_META_BIT_FLOAT    }}
#define JX_OB_BOOL      { JX_DATA_INIT, {JX_META_BIT_BOOL     }} 

#define JX_OB_LIST      { JX_DATA_INIT, {JX_META_BIT_GC | JX_META_BIT_LIST  }}
#define JX_OB_HASH      { JX_DATA_INIT, {JX_META_BIT_GC | JX_META_BIT_HASH  }}

#define JX_OB_OPCODE    { JX_DATA_INIT, {JX_META_BIT_OPCODE   }} 

/* VM opcodes (of 32 possibilities, only 2 are assigned) */

#define JX_OPCODE_BREAK             0   /* exit current block */
#define JX_OPCODE_CONTINUE          1   /* next iteration in loop */
#define JX_OPCODE_JUMP_RELATIVE     2   /* jump within block */
#define JX_OPCODE_RETURN            3   /* return from current function */
#define JX_OPCODE_TAIL_CALL         4   /* invoke tail recursion on function */

struct jx__list {
  jx_gc gc;
  jx_bits packed_meta_bits;
  jx_bool synchronized;
  jx_os_spinlock lock;
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
  jx_ob args;
  jx_ob body;
  jx_int mode; 
};

typedef struct {
  jx_gc gc;
} jx_str;

/* thread-local state */

typedef struct jx__tls_chain jx_tls_chain;

struct jx__tls {
  jx_int n_hash,n_list,n_vla;
  jx_tls_chain *hash_chain;
  jx_tls_chain *list_chain;
  jx_tls_chain *vla_chain;
  jx_ob builtins;
  jx_ob method;
  jx_bool have_method;
  jx_bool break_seen;
  jx_bool tail_call;
  jx_ob   result;
  jx_bool leave, have_result;
};

/* status checking */

#define JX_OK(s) ((s)>=0)

#define JX_ERR(s) ((s)<0)

#define JX_POS(s) ((s)>0)

/* convenience macros */

#define JX_PTR(p) ( (p) ? JX_SUCCESS : JX_STATUS_NULL_PTR )

#define JX_NULLIFY(x) jx_os_memset((void*) &(x), 0, sizeof(x));

/* prototypes */

void jx_tls_free(jx_tls *tls);
jx_tls *jx_tls_new(void);

jx_ob jx_tls_hash_new(jx_tls *tls);
jx_ob jx_tls_hash_new_with_assoc(jx_tls *tls,jx_ob key,jx_ob value);

jx_hash *jx_tls_hash_calloc(jx_tls *tls);
void jx_tls_hash_free(jx_tls *tls,jx_hash *hash);

jx_ob jx_tls_list_new(jx_tls *tls);
jx_ob jx_tls_list_new_with_size(jx_tls *tls,jx_int size);
jx_list *jx_tls_list_calloc(jx_tls *tls);
void jx_tls_list_free(jx_tls *tls,jx_list *list);

jx_ob jx__tls_ob_copy(jx_tls *tls, jx_ob ob);
JX_INLINE jx_ob jx_tls_ob_copy(jx_tls *tls, jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__tls_ob_copy(tls,ob) : ob;
}

#define   jx_tls_vla_new(t,r,s,z)           jx__tls_vla_new(t,r,s,z)
void *jx__tls_vla_new(jx_tls *tls, jx_int rec_size, jx_int size,jx_bool zero);

#define   jx_tls_vla_free(t,r)            jx__tls_vla_free(t,(void**)(void*)(r))
jx_status jx__tls_vla_free(jx_tls *tls,void **ref);

jx_char *jx_ob_as_str(jx_ob * ob);
jx_ob jx_ob_from_str(jx_char * str);
jx_ob jx_ob_from_ident(jx_char * ident);

jx_ob jx__str__concat(jx_char *left,jx_int left_len, jx_char *right, jx_int right_len);
jx_ob jx__ob_add(jx_ob left, jx_ob right);
jx_status jx__ob_free(jx_tls *tls, jx_ob ob);


/* free */

JX_INLINE jx_status jx_ob_free(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_GC) {
    if(bits & JX_META_BIT_WEAK_REF) {
      return JX_STATUS_FREED_WEAK;
    } else {
      /* object has resources which need to be garbage collected */
      return jx__ob_free(JX_NULL,ob);   /* call non-inline routine */
    }
  }
  return JX_SUCCESS;
}

JX_INLINE jx_status jx_tls_ob_free(jx_tls *tls,jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_GC) {
    if(bits & JX_META_BIT_WEAK_REF) {
      return JX_FAILURE;
    } else {
      /* object has resources which need to be garbage collected */
      return jx__ob_free(tls,ob);   /* call non-inline routine */
    }
  }
  return JX_SUCCESS;
}

jx_ob jx_ob_to_jxon_with_flags(jx_ob node, jx_ob ob, jx_int flags, jx_int indent, 
                               jx_int width, jx_int space_left);

JX_INLINE jx_ob jx_ob_to_jxon(jx_ob node, jx_ob ob)
{
  return jx_ob_to_jxon_with_flags(node, ob, 0, 0, 0, 0);
}

/* debugging */

void jx__hash_dump(FILE *file,jx_hash *I);

JX_INLINE void jx_ob_dump(FILE *f, char *prefix, jx_ob ob)
{
#if (JX_TINY_STR_SIZE == 6)
#ifndef JX_64_BIT
  /* jx_ob = 64 bits */
  fprintf(f,"%s: %08x%04x %04x\n",prefix, 
          (unsigned int)ob.data.raw.word,
          (unsigned int)ob.data.raw.bits, (unsigned int)ob.meta.bits);
#else
  /* jx_ob = 96 bits */
  fprintf(f,"%s: %08x%08x%04x %04x\n",prefix, 
          (unsigned int)(ob.data.raw.word), (unsigned int)(ob.data.raw.word>>32),
          (unsigned int)ob.data.raw.bits, (unsigned int)ob.meta.bits);
#endif
#else
#if (JX_TINY_STR_SIZE == 10)
  /* jx_ob = 96 bits */
  fprintf(f,"%s: %08x%08x%04x %04x\n",prefix, 
          (unsigned int)(ob.data.raw.word), (unsigned int)(ob.data.raw.word>>32),
          (unsigned int)ob.data.raw.bits, (unsigned int)ob.meta.bits);
#else
  /* jx_ob = 160 bits */
  fprintf(f,"%s: %08x%80x%08x%08x%04x %04x\n",prefix, 
          (unsigned int)(ob.data.raw.word[0]), (unsigned int)(ob.data.raw.word[0]>>32),
          (unsigned int)(ob.data.raw.word[1]), (unsigned int)(ob.data.raw.word[1]>>32),
          (unsigned int)ob.data.raw.bits, (unsigned int)ob.meta.bits);

#endif
#endif
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
#define   jx_vla_new_from_subset(r,i,c) jx__vla_new_from_subset((void**)(void*)r,i,c)
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
#define   jx_vla_rezero(r)          jx__vla_rezero((void**)(void*)(r))
#define   jx_vla_reset(r)           jx__vla_reset((void**)(void*)(r))
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
jx_status jx__vla_rezero(void **ref);
jx_status jx__vla_reset(void **ref);
jx_status jx__vla_free(void **ref);

JX_INLINE jx_int *jx_int_vla_new(jx_int size)
{
  return (jx_int*)jx_vla_new(sizeof(jx_int), size);
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
  return (jx_float*)jx_vla_new(sizeof(jx_float), size);
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

JX_INLINE jx_ob jx_null_with_ob(jx_ob ob)
{
  jx_ob result = JX_OB_NULL;
  jx_ob_free(ob);
  return result;
}

JX_INLINE jx_ob jx_ob_from_null(void)
{
  jx_ob result = JX_OB_NULL;
  return result;
}

JX_INLINE jx_ob jx_ob_not_weak_with_ob(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_WEAK_REF) ?
    jx_ob_copy(ob) : ob;
}

jx_ob jx__ob_make_strong_with_ob(jx_ob ob);
JX_INLINE jx_ob jx_ob_make_strong_with_ob(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__ob_make_strong_with_ob(ob) : ob;
}

jx_ob jx__ob_only_strong_with_ob(jx_ob ob);
JX_INLINE jx_ob jx_ob_only_strong_with_ob(jx_ob ob)
{
   return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__ob_only_strong_with_ob(ob) : ob;
}

JX_INLINE jx_ob jx_ob_from_opcode(jx_int inst, jx_int operand)
{
  jx_ob result = JX_OB_OPCODE;
  result.meta.bits |= (inst & JX_META_MASK_OPCODE_INST);
  result.data.io.bool_ = operand;
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

jx_bool jx__ob_as_bool(jx_ob ob);
JX_INLINE jx_bool jx_ob_as_bool(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BOOL) ? ob.data.io.bool_ : 
    ((bits & JX_META_BIT_INT) ? (ob.data.io.int_ ? JX_TRUE : JX_FALSE) :
     ((bits & JX_META_BIT_FLOAT) ? (ob.data.io.float_ ? JX_TRUE : JX_FALSE) : 
      ((bits & (JX_META_BIT_STR|JX_META_BIT_LIST|JX_META_BIT_HASH)) ? jx__ob_as_bool(ob) :
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

JX_INLINE jx_bool jx_opcode_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_OPCODE);
}

JX_INLINE jx_bool jx_gc_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC);
}

JX_INLINE jx_bool jx_weak_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_WEAK_REF);
}

JX_INLINE jx_bool jx_null_check(jx_ob ob)
{
  return !ob.meta.bits;
}

JX_INLINE jx_bool jx_bool_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_BOOL);
}

JX_INLINE jx_bool jx_int_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_INT);
}

JX_INLINE jx_bool jx_float_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_FLOAT);
}

JX_INLINE jx_bool jx_str_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_STR);
}

JX_INLINE jx_bool jx_ident_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_IDENT);
}

JX_INLINE jx_bool jx_list_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_LIST);
}

JX_INLINE jx_bool jx_hash_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_HASH);
}


/* builtin fn objects are a means through which jenarix can be extended */

JX_INLINE jx_ob jx_builtin_new_from_selector(jx_int selector)
{
  jx_ob result = JX_OB_NULL;
  result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR;
  result.data.io.int_ = selector;
  return result;
}

JX_INLINE jx_ob jx_builtin_new_entity(jx_ob name)
{
  if(jx_ident_check(name)) { /* an entity is simply a cloaked identifier */
    jx_ob result = jx_ob_copy(name);
    result.meta.bits = (JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_ENTITY |
                        (JX_META_BIT_GC & result.meta.bits));
    return result;
  } else {
    return jx_ob_from_null();
  }
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

JX_INLINE jx_bool jx_builtin_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_BUILTIN) && JX_TRUE;
}

JX_INLINE jx_bool jx_builtin_entity_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return ((bits & JX_META_BIT_BUILTIN) &&
          ((bits & JX_META_MASK_BUILTIN_TYPE)==
           JX_META_BIT_BUILTIN_ENTITY));
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

JX_INLINE jx_bool jx_builtin_callable_check(jx_ob ob)
{
  register jx_bits bits = ob.meta.bits;
  return (bits & JX_META_BIT_BUILTIN) && 
    ((bits & 
      (JX_META_BIT_BUILTIN_MACRO |      
       JX_META_BIT_BUILTIN_SELECTOR |      
       JX_META_BIT_BUILTIN_NATIVE_FN |      
       JX_META_BIT_BUILTIN_FUNCTION)));
#if 0
  ((JX_META_MASK_BUILTIN_TYPE & bits) ==  /* entities could be callable...*/
   JX_META_BIT_BUILTIN_ENTITY);
#endif
}

JX_INLINE jx_bool jx_function_check(jx_ob ob)
{
  return jx_builtin_check(ob) && (ob.meta.bits & JX_META_BIT_BUILTIN_FUNCTION);
}

JX_INLINE jx_bool jx_macro_check(jx_ob ob)
{
  return jx_builtin_check(ob) && (ob.meta.bits & JX_META_BIT_BUILTIN_MACRO);
}

/* convenience function */

JX_INLINE jx_ob jx_ob_swap(jx_ob *ob_ptr,jx_ob ob)
{
  jx_ob result = *ob_ptr;
  *ob_ptr = ob;
  return result;
}

JX_INLINE jx_ob jx_ob_swap_with_null(jx_ob *ob_ptr)
{
  jx_ob result = *ob_ptr;
  *ob_ptr = jx_ob_from_null();
  return result;
}


JX_INLINE jx_status jx_ob_replace_with_null(jx_ob *ob_ptr)
{
  jx_status status = jx_ob_free(*ob_ptr);
  *ob_ptr = jx_ob_from_null();
  return status;
}

JX_INLINE jx_status jx_ob_replace(jx_ob *ob_ptr, jx_ob ob)
{
  jx_status status = jx_ob_free(*ob_ptr);
  *ob_ptr = ob;
  return status;
}

JX_INLINE jx_status jx_tls_ob_replace(jx_tls *tls, jx_ob *ob_ptr, jx_ob ob)
{
  jx_status status = jx_tls_ob_free(tls,*ob_ptr);
  *ob_ptr = ob;
  return status;
}

JX_INLINE jx_bool jx_ok(jx_status status)
{
  return JX_OK(status);
}

JX_INLINE jx_bool jx_err(jx_status status)
{
  return JX_ERR(status);
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
jx_status jx__ob_set_synchronized(jx_ob ob, jx_bool synchronized, jx_bool recursive);
JX_INLINE jx_status jx_ob_set_synchronized(jx_ob ob, jx_bool synchronized, jx_bool recursive)
{
  if(ob.meta.bits & JX_META_BIT_GC) {
    return jx__ob_set_synchronized(ob, synchronized,recursive);
  }
  return JX_SUCCESS;
}

jx_bool jx__ob_synchronized(jx_ob ob);
JX_INLINE jx_bool jx_ob_synchronized(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_GC) {
    return jx__ob_synchronized(ob);
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

JX_INLINE jx_bool jx_ob_same(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_FOR_HASH;
  jx_bits right_bits = right.meta.bits & JX_META_MASK_FOR_HASH;

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
    return ((left.data.raw.word == right.data.raw.word) &&
            (left.data.raw.bits == right.data.raw.bits));
#else
#if (JX_TINY_STR_SIZE == 18)
    return ((left.data.raw.word[0] == right.data.raw.word[0]) &&
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
      if((left_bits && !right_bits) ||
         (right_bits && !left_bits))
        return JX_FALSE;
      return jx__ob_non_gc_equal(left, right);
    }
  } else if(left_bits & JX_META_BIT_GC) {
    return jx__ob_gc_equal(left, right);
  } else {
    return jx_ob_identical(left, right);
  }
}

/* lists */

JX_INLINE jx_int jx__list_size_locked(jx_list * I)
{
  return jx_vla_size(&I->data.vla);
}

JX_INLINE jx_int jx__list_size(jx_list * I)
{
  jx_int result = 0;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    result = jx__list_size_locked(I);
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
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
  jx_status status = JX_FAILURE;
  jx_bits bits = list.meta.bits;
  if(bits & JX_META_BIT_LIST) {
    status = jx__list_resize(list.data.io.list, size, fill);
    if(jx_ok(status)) {
      jx_ob_free(fill);
    }
  }
  return status;
}

JX_INLINE jx_status jx_list_reset(jx_ob list)
{
  return jx_list_resize(list,0,jx_ob_from_null());
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

JX_INLINE void jx__list_set_packed_data_locked(jx_list * list, jx_int index, jx_ob ob)
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

JX_INLINE void jx__list_set_packed_data(jx_list * I, jx_int index, jx_ob ob)
{
  jx_bool synchronized = I->synchronized;
  jx_status status = synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx__list_set_packed_data_locked(I,index,ob);
    if(synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
}

jx_status jx__list_repack_data_locked(jx_list * list);
JX_INLINE jx_status jx__list_repack_data(jx_list * I)
{
  jx_bool synchronized = I->synchronized;
  jx_status status = synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = jx__list_repack_data_locked(I);
    if(synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

JX_INLINE jx_status jx_list_repack(jx_ob list)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_repack_data(list.data.io.list) :
    JX_FAILURE;
}

jx_status jx__list_unpack_data_locked(jx_list * I);
JX_INLINE jx_status jx__list_unpack_data(jx_list * I)
{
  jx_bool synchronized = I->synchronized;
  jx_status status = synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = jx__list_unpack_data_locked(I);
    if(synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

JX_INLINE jx_status jx__list_replace(jx_list * I, jx_int index, jx_ob ob)
{
  jx_bool synchronized = I->synchronized;
  jx_status status = synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if((!I->gc.shared) && (index >=0) && (index < jx_vla_size(&I->data.vla))) {
      if(!I->packed_meta_bits) {
        jx_ob_replace(I->data.ob_vla + index, ob);
        status = JX_SUCCESS;
      } else {
        if(I->data.vla && (I->packed_meta_bits != ob.meta.bits)) {
          if(!jx_ok(jx__list_unpack_data(I)))
            goto unlock;
        }
        if(I->data.vla) {
          if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
            jx__list_set_packed_data(I, index, ob);
          } else if(!I->packed_meta_bits) {
            jx_ob_replace(I->data.ob_vla + index, ob);
          }
          status = JX_SUCCESS;
        }
      }
    }
  unlock:
    if(synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
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

JX_INLINE jx_ob jx__list_get_packed_data_locked(jx_list * list, jx_int index)
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

JX_INLINE jx_ob jx__list_get_packed_data(jx_list * I, jx_int index)
{
  jx_ob result = JX_OB_NULL;
  jx_bool synchronized = I->synchronized;
  jx_status status = synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    result = jx__list_get_packed_data_locked(I,index);
    if(synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

JX_INLINE jx_ob jx__list_borrow_locked(jx_list * I, jx_int index)
{
  jx_int size = jx_vla_size(&I->data.vla);
  index = (index<0) ? size+index : index;
  if((index >= 0) && (index < size) ) {
    if(I->packed_meta_bits) {
      return jx__list_get_packed_data(I, index);
    } else {
      return I->data.ob_vla[index];
    }
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx__list_borrow(jx_list * I, jx_int index)
{
  jx_ob result = JX_OB_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    result = jx__list_borrow_locked(I,index);
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

JX_INLINE jx_ob jx_list_borrow(jx_ob list, jx_int index)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_borrow(list.data.io.list, index) :
    jx_ob_from_null();
}

JX_INLINE jx_ob jx__list_swap_locked(jx_list * I, jx_int index, jx_ob ob)
{
  if((!I->gc.shared) && (index >=0) && (index < jx_vla_size(&I->data.vla))) {
    if(!I->packed_meta_bits) {
      return jx_ob_swap(I->data.ob_vla + index, ob);
    } else {
      if(I->data.vla && (I->packed_meta_bits != ob.meta.bits)) {
        if(!jx_ok(jx__list_unpack_data(I)))
          return jx_ob_from_null();
      }
      if(I->data.vla) {
        if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
          jx_ob result = jx__list_get_packed_data(I, index);
          jx__list_set_packed_data(I, index, ob);
          return result;
        } else if(!I->packed_meta_bits) {
          return jx_ob_swap(I->data.ob_vla + index, ob);
        }
      }
    }
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx__list_swap(jx_list * I, jx_int index, jx_ob ob)
{
  jx_ob result = JX_OB_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    result = jx__list_swap_locked(I,index,ob);
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

JX_INLINE jx_ob jx__list_swap_with_null_locked(jx_list *list, jx_int index)
{
  return jx__list_swap_locked(list, index, jx_ob_from_null());
}

JX_INLINE jx_ob jx__list_swap_with_null(jx_list *list, jx_int index)
{
  return jx__list_swap(list, index, jx_ob_from_null());
}

JX_INLINE jx_ob jx_list_swap_with_null(jx_ob list, jx_int index)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_swap(list.data.io.list, index, jx_ob_from_null()) :
    jx_ob_from_null();
}

JX_INLINE jx_ob jx_list_swap(jx_ob list, jx_int index, jx_ob ob)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_swap(list.data.io.list, index, ob) :
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

jx_ob jx__list_new_from_slice(jx_list * I, jx_int start,jx_int stop);
JX_INLINE jx_ob jx_list_new_from_slice(jx_ob list, jx_int start,jx_int stop)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_new_from_slice(list.data.io.list, start, stop) : 
    jx_ob_from_null();
}

jx_ob jx__list_new_with_cutout(jx_list * I, jx_int start,jx_int stop);
JX_INLINE jx_ob jx_list_new_with_cutout(jx_ob list, jx_int start,jx_int stop)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_new_with_cutout(list.data.io.list, start, stop) : 
    jx_ob_from_null();
}

JX_INLINE jx_ob jx_list_shift(jx_ob list)
{
  return jx_list_remove(list,0);
}
JX_INLINE jx_status jx_list_unshift(jx_ob list, jx_ob ob)
{
  return jx_list_insert(list,0,ob);
}
JX_INLINE jx_status jx_list_push(jx_ob list, jx_ob ob)
{
  return jx_list_append(list,ob);
}

JX_INLINE jx_ob jx_list_pop(jx_ob list)
{
  jx_int size = jx_list_size(list);
  if(size) 
    return jx__list_remove(list.data.io.list, size-1);
  else
    return jx_ob_from_null();
}

jx_int jx__list_index(jx_list * I,jx_ob ob);
JX_INLINE jx_int jx_list_index(jx_ob list,jx_ob ob)
{
  return (list.meta.bits & JX_META_BIT_LIST) ?
    jx__list_index(list.data.io.list, ob) :
    -1;
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

JX_INLINE jx_bool jx_hash_take(jx_ob * result, jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_remove(result, hash.data.io.hash, key);
  }
  return JX_FALSE;
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
    case JX_META_BIT_LIST:
      {
        jx_ob result = jx_ob_copy(left);
        if(!jx_ok( jx_list_combine( result, jx_ob_copy(right)) ) ) {
          jx_ob_replace_with_null(&result);
        }
        return result;
      }
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
#undef JX_MATH_OP

JX_INLINE jx_ob jx_ob_idiv(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS; 
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS; 
  if(left_bits == right_bits) { 
    switch(left_bits) {
    case JX_META_BIT_INT: 
      return jx_ob_from_int( left.data.io.int_ / right.data.io.int_);
      break; 
    case JX_META_BIT_FLOAT: 
      return jx_ob_from_float( (float)(((int)left.data.io.float_) / 
                                       (int)right.data.io.float_ ));
      break; 
    case JX_META_BIT_BOOL: 
      return jx_ob_from_float( left.data.io.bool_ / right.data.io.bool_ );
      break; 
    }
  } else {
    jx_bits merge_bits = left.meta.bits | right.meta.bits;
    if(merge_bits & JX_META_BIT_FLOAT) {
      return jx_ob_from_float((jx_float)(jx_ob_as_int(left) / jx_ob_as_int(right)));
    } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
      return jx_ob_from_int(jx_ob_as_int(left) / jx_ob_as_int(right));
    }
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_ob_div(jx_ob left, jx_ob right)
{
  jx_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS; 
  jx_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS; 
  if(left_bits == right_bits) { 
    switch(left_bits) {
    case JX_META_BIT_INT: 
      return jx_ob_from_float( ((float)left.data.io.int_) / right.data.io.int_);
      break; 
    case JX_META_BIT_FLOAT: 
      return jx_ob_from_float( left.data.io.float_ / right.data.io.float_ ); 
      break; 
    case JX_META_BIT_BOOL: 
      return jx_ob_from_float( ((float)left.data.io.bool_) / right.data.io.bool_ );
      break; 
    }
  } else {
    jx_bits merge_bits = left.meta.bits | right.meta.bits;
    if(merge_bits & JX_META_BIT_FLOAT) {
      return jx_ob_from_float(jx_ob_as_float(left) / jx_ob_as_float(right));
    } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
      return jx_ob_from_float(jx_ob_as_float(left) / jx_ob_as_float(right));
    }
 }
  return jx_ob_from_null();
}


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
  case JX_META_BIT_LIST:  
    return jx_ob_from_bool( !jx_list_size(ob));
    break; 
  case JX_META_BIT_HASH:  
    return jx_ob_from_bool( !jx_hash_size(ob));
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
    jx_ob result = jx__str_join_with_list(list.data.io.list,JX_NULL);
    jx_ob_free(list);
    return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_str_join_with_list_sep(jx_ob list, jx_ob sep)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_char *sep_str = JX_NULL;
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

JX_INLINE jx_ob jx_str_join_from_list(jx_ob list)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_ob result = jx__str_join_with_list(list.data.io.list,JX_NULL);
    return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_ob jx_str_join_from_list_sep(jx_ob list, jx_ob sep)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_char *sep_str = JX_NULL;
    jx_ob result;
    if(sep.meta.bits & JX_META_BIT_STR) {
      sep_str = jx_ob_as_str(&sep);
    }
    result = jx__str_join_with_list(list.data.io.list,sep_str);
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
  return jx__ob_lt(left,right);
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

JX_INLINE jx_status jx_ob_type(jx_ob ob)
{
  switch(ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case 0:
    return JX_OB_TYPE_NULL;
    break;
  case JX_META_BIT_BOOL:
    return JX_OB_TYPE_BOOL;
    break;
  case JX_META_BIT_INT:
    return JX_OB_TYPE_INT;
    break;
  case JX_META_BIT_FLOAT:
    return JX_OB_TYPE_FLOAT;
    break;
  case JX_META_BIT_LIST:
    return JX_OB_TYPE_LIST;
    break;
  case JX_META_BIT_HASH:
    return JX_OB_TYPE_HASH;
    break;
  case JX_META_BIT_STR:
    return JX_OB_TYPE_STR;
    break;
  case JX_META_BIT_IDENT:
    return JX_OB_TYPE_IDENT;
    break;
  case JX_META_BIT_OPCODE:
    return JX_OB_TYPE_OPCODE;
    break;
  default:
    return 0;
    break;
  }
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
    return jx_ob_from_int(0);
    break;
  }
}

#define JX_EVAL_ALLOW_NESTED_WEAK_REFS  0x01
#define JX_EVAL_DEFER_INVOCATION        0x02

#define JX_EVAL_DEBUG_DUMP_SUBEX        0x04
#define JX_EVAL_DEBUG_TRACE             0x08
#define JX_EVAL_DEBUG_DUMP_NODE         0x10

jx_ob jx__code_eval(jx_tls *tls, jx_int flags, jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_eval(jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_eval(JX_NULL,0,node,expr) : expr;
}

jx_ob jx__code_exec(jx_tls *tls, jx_int flags, jx_ob node, jx_ob expr);
JX_INLINE jx_ob jx_code_exec(jx_ob node, jx_ob code)
{
  return (code.meta.bits & JX_META_BIT_LIST) ?
    jx__code_exec(JX_NULL,0,node,code) : jx_ob_copy(code);
}

JX_INLINE jx_ob jx_code_eval_tls(jx_tls *tls, jx_int flags, jx_ob node, jx_ob expr)
{
  return (expr.meta.bits & JX_META_BIT_GC) ?
    jx__code_eval(tls, flags, node,expr) : expr;
}

JX_INLINE jx_ob jx_code_exec_tls(jx_tls *tls, jx_int flags, jx_ob node, jx_ob code)
{
  return (code.meta.bits & JX_META_BIT_LIST) ?
    jx__code_exec(tls,flags,node,code) : jx_ob_copy(code);
}

jx_ob jx__hash_copy(jx_hash * hash);

JX_INLINE jx_ob jx__function_call(jx_tls *tls, jx_ob node, jx_ob function, jx_ob payload)
{
  jx_ob result = JX_OB_NULL;
  //  jx_jxon_dump(stdout,"jx_function_call function",function);
  //  jx_jxon_dump(stdout,"jx_function_call payload", payload);

  if(jx_function_check(function)) {
    jx_function *fn = function.data.io.function;
    jx_ob args = fn->args;
    while(1) {
      if(jx_null_check(args)) {
        /* inner functions run within the host node namespace */
        jx_ob payload_ident = jx_ob_from_ident("_");
        jx_ob saved_payload = JX_OB_NULL;
        jx_bool saved = jx_hash_take(&saved_payload, node, payload_ident);
        if(jx_ok( jx_hash_set(node, payload_ident, payload) ) ) { 
          payload = jx_ob_from_null();
          /* call */
          result = fn->mode ? jx_code_exec_tls(tls, 0, node, fn->body) :
            jx_code_eval_tls(tls, 0, node,fn->body);
          if(saved) 
            jx_hash_set(node, payload_ident, saved_payload);
          else
            jx_hash_delete(node, payload_ident);
        } else {
          if(saved) 
            jx_hash_set(node, payload_ident, saved_payload);
          else
            jx_hash_delete(node, payload_ident);
        }
      } else if(jx_hash_check(args)) { /* simple namespace -- no processing */
        /* standard functions run inside their own node namespace (and
           thus can potentially be concurrent) */
        jx_ob payload_ident = jx_ob_from_ident("_");
        jx_ob inv_node = jx_ob_copy(args);
        /* expose fn to itself */
        jx_hash_set(inv_node,jx_ob_take_weak_ref(fn->name),jx_ob_take_weak_ref(function));
        //jx_jxon_dump(stdout,"inv_node",jx_ob_from_null(),inv_node);
        //jx_jxon_dump(stdout,"payload",jx_ob_from_null(),payload);
        if(jx_ok( jx_hash_set(inv_node,payload_ident,payload))) {
          /* call */
          result = fn->mode ? jx_code_exec_tls(tls,0, inv_node,fn->body) : 
            jx_code_eval_tls(tls, 0, inv_node,fn->body);
        }
        jx_tls_ob_free(tls,inv_node);
        payload = jx_ob_from_null();
      } else if(jx_list_check(args)) { /* parameter list exists */
        jx_ob ob_null = JX_OB_NULL;
        jx_ob inv_node;
        jx_list *args_list = args.data.io.list;
        jx_ob sub_list = jx__list_borrow(args_list,0);
        jx_ob kwd_hash = JX_OB_NULL;
        if(jx_list_check(sub_list)) { /* have both positional and keyword defaults */
          jx_ob kwds = jx__list_borrow(args_list,1);
          if(jx_hash_check(kwds)) {
            inv_node = jx__hash_copy(kwds.data.io.hash);
          } else {
            inv_node = jx_tls_hash_new(tls);
          }
          if(jx_list_check(payload)) {
            jx_list *args_list2 = sub_list.data.io.list;
            jx_list *payload_list = payload.data.io.list;
            jx_int i,size = jx_list_size(sub_list);
            jx_int size2 = jx_list_size(payload);
            if(size2<size) size = size2;
            for(i=0;i<size;i++) {
              jx_hash_set(inv_node,jx__list_borrow(args_list2,i),
                          jx__list_swap(payload_list,i, ob_null));
            }
            if(size2>size) { /* keyword args also provided in payload? */
              kwd_hash = jx__list_borrow(payload_list,size);
            }
          } else if(jx_list_size(sub_list)) { /* payload primitive -> [x] */
            jx_hash_set(inv_node, jx_list_borrow(sub_list,0), payload);
            payload = jx_ob_from_null();
          }
        } else { /* only positional arguments */
          inv_node = jx_tls_hash_new(tls);
          if(jx_list_check(payload)) {
            jx_list *args_list = args.data.io.list;
            jx_list *payload_list = payload.data.io.list;
            jx_int i,size = jx_list_size(args);
            jx_int size2 = jx_list_size(payload);
            for(i=0;i<size;i++) {
              jx_hash_set(inv_node,jx__list_borrow(args_list,i),
                          jx__list_swap(payload_list,i, ob_null));
            }
            if(size2>size) { /* keyword args also provided in payload? */
              kwd_hash = jx__list_borrow(payload_list,size);
            }
          } else if(jx_list_size(args)) { /* payload primitive -> [x] */
            jx_hash_set(inv_node, jx_list_borrow(args,0), payload);
          }
        }
        /* process keyword argument hash, if present (THIS STRATEGY WILL CHANGE) */
        if(jx_hash_check(kwd_hash)) {
          jx_ob kwd_list = jx_list_new_from_hash(kwd_hash);
          if(jx_list_check(kwd_list)) {
            jx_list *args_list3 = kwd_list.data.io.list;
            jx_int i,size3 = jx__list_size(args_list3);
            for(i=0;i<size3;i+=2) {
              jx_hash_set(inv_node,jx__list_swap(args_list3,i, ob_null),
                          jx__list_swap(args_list3,i+1, ob_null));
            }
          }
          jx_tls_ob_free(tls,kwd_list);
        }
        /* expose function to itself */
        jx_hash_set(inv_node,jx_ob_take_weak_ref(fn->name),jx_ob_take_weak_ref(function));
        /* call */
        result = fn->mode ? jx_code_exec_tls(tls,0, inv_node,fn->body) : 
          jx_code_eval_tls(tls, 0, inv_node,fn->body);
        jx_tls_ob_free(tls, inv_node);
      } else {
        /* args declaration is a primitive */
        jx_ob inv_node = jx_tls_hash_new_with_assoc(tls, args, payload);
        //      jx_jxon_dump(stdout,"args",args);
        jx_hash_set(inv_node,jx_ob_take_weak_ref(fn->name),
                    jx_ob_take_weak_ref(function));
        //jx_jxon_dump(stdout,"hash",jx_ob_from_null(),inv_node);
        result = fn->mode ? jx_code_exec_tls(tls,0, inv_node,fn->body) : 
          jx_code_eval_tls(tls, 0, inv_node,fn->body);
        jx_tls_ob_free(tls,inv_node);
      }
      if(tls->tail_call) {
        if(tls->have_result) {
          jx_ob_replace(&payload, tls->result);
          tls->result = jx_ob_from_null();
          tls->have_result = JX_FALSE;
          jx_tls_ob_free(tls,result);
        } else {
          jx_ob_replace(&payload, result);
        }
        result = jx_ob_from_null();
        tls->tail_call = JX_FALSE;
      } else {
        break;
      }
    }
  }
  jx_tls_ob_free(tls,payload);
  jx_tls_ob_free(tls,function);
  return result;
}

JX_INLINE jx_ob jx_function_call(jx_ob node, jx_ob function, jx_ob payload)
{
  jx_ob result;
  jx_tls *tls = jx_tls_new();
  result = jx__function_call(tls,node,function,payload);
  jx_tls_free(tls);
  return result;
}

JX_INLINE jx_ob jx__macro_call(jx_tls *tls, jx_ob node, jx_ob macro, jx_ob payload)
{
  //  jx_jxon_dump(stdout,"jx_macro_call macro",macro);
  //  jx_jxon_dump(stdout,"jx_macro_call payload", payload);
  if(jx_macro_check(macro)) {
    jx_function *fn = macro.data.io.function;
    jx_ob args = fn->args;

    if(jx_null_check(args)) { /* inner functions run within the host node namespace */
      jx_ob payload_ident = jx_ob_from_ident("_");
      jx_ob saved_payload = JX_OB_NULL;
      jx_bool saved = jx_hash_take(&saved_payload,node,payload_ident);
      if(jx_ok( jx_hash_set(node,payload_ident,payload))) {
        jx_ob ob = jx_code_eval_tls(tls, JX_EVAL_DEFER_INVOCATION, node, fn->body);
        if(saved) 
          jx_hash_set(node, payload_ident, saved_payload);
        else
          jx_hash_delete(node, payload_ident);
        {
          jx_ob result = jx_code_eval_tls(tls, 0, node, ob);
          jx_tls_ob_free(tls,ob);
          jx_tls_ob_free(tls,payload);
          jx_ob_free(macro);
          return result;
        }
      } else {
        if(saved) 
          jx_hash_set(node, payload_ident, saved_payload);
        else
          jx_hash_delete(node, payload_ident);
        jx_tls_ob_free(tls,payload);
        jx_ob_free(macro);
        return jx_ob_from_null();
      }
    } else if(jx_hash_check(args)) { /* simple namespace -- no processing */
      /* standard functions run inside their own node namespace (and
         thus can potentially be concurrent) */
      jx_ob payload_ident = jx_ob_from_ident("_");
      jx_ob inv_node = jx_ob_copy(args);
      jx_ob result = JX_OB_NULL;
      
      if(jx_ok( jx_hash_set(inv_node,payload_ident,payload))) {
        jx_ob ob = jx_code_eval_tls(tls, JX_EVAL_DEFER_INVOCATION, inv_node, fn->body);
        jx_tls_ob_free(tls,payload);
        jx_tls_ob_free(tls, inv_node);
        jx_ob_free(macro);
        {
          jx_ob result = jx_code_eval_tls(tls, 0, node, ob);
          jx_tls_ob_free(tls, ob);
          return result;
        }
      }
      return result;
    } else if(jx_list_check(args)) { /* parameter list exists */
      jx_ob ob_null = JX_OB_NULL;
      jx_ob result;
      jx_ob inv_node;
      jx_list *args_list = args.data.io.list;
      jx_ob sub_list = jx__list_borrow(args_list,0);
      jx_ob kwd_hash = JX_OB_NULL;
      if(jx_list_check(sub_list)) { /* have both positional and keyword defaults */
        jx_ob kwds = jx__list_borrow(args_list,1);
        if(jx_hash_check(kwds)) {
          inv_node = jx__hash_copy(kwds.data.io.hash);
        } else {
          inv_node = jx_tls_hash_new(tls);
        }
        if(jx_list_check(payload)) {
          jx_list *args_list2 = sub_list.data.io.list;
          jx_list *payload_list = payload.data.io.list;
          jx_int i,size = jx_list_size(sub_list);
          jx_int size2 = jx_list_size(payload);
          if(size2<size) size = size2;
          for(i=0;i<size;i++) {
            jx_hash_set(inv_node,jx__list_borrow(args_list2,i),
                        jx__list_swap(payload_list,i, ob_null));
          }
          if(size2>size) { /* keyword args also provided in payload? */
            kwd_hash = jx__list_borrow(payload_list,size);
          }
        } else if(jx_list_size(sub_list)) { /* payload primitive -> [x] */
          jx_hash_set(inv_node, jx_list_borrow(sub_list,0), payload);
        }
      } else { /* only positional arguments */
        inv_node = jx_tls_hash_new(tls);
        if(jx_list_check(payload)) {
          jx_list *args_list = args.data.io.list;
          jx_list *payload_list = payload.data.io.list;
          jx_int i,size = jx_list_size(args);
          jx_int size2 = jx_list_size(payload);
          for(i=0;i<size;i++) {
            jx_hash_set(inv_node,jx__list_borrow(args_list,i),
                        jx__list_swap(payload_list,i, ob_null));
          }
          if(size2>size) { /* keyword args also provided in payload? */
            kwd_hash = jx__list_borrow(payload_list,size);
          }
        } else if(jx_list_size(args)) { /* payload primitive -> [x] */
          jx_hash_set(inv_node, jx_list_borrow(args,0), payload);
        }
      }
      /* process keyword argument hash, if present (THIS STRATEGY WILL CHANGE) */
      if(jx_hash_check(kwd_hash)) {
        jx_ob kwd_list = jx_list_new_from_hash(kwd_hash);
        if(jx_list_check(kwd_list)) {
          jx_list *args_list3 = kwd_list.data.io.list;
          jx_int i,size3 = jx__list_size(args_list3);
          for(i=0;i<size3;i+=2) {
            jx_hash_set(inv_node,jx__list_swap(args_list3,i, ob_null),
                        jx__list_swap(args_list3,i+1, ob_null));
          }
        }
        jx_tls_ob_free(tls,kwd_list);
      }
      //      jx_jxon_dump(stdout,"inv_node",inv_node);
      //      jx_jxon_dump(stdout,"body",fn->body);
      {
        jx_ob ob = jx_code_eval_tls(tls, JX_EVAL_DEFER_INVOCATION, inv_node, fn->body);
        //        jx_jxon_dump(stdout,"node",node);
        //        jx_jxon_dump(stdout,"ob",ob);
        result = jx_code_eval_tls(tls, 0, node, ob);
        jx_tls_ob_free(tls, ob);
        jx_tls_ob_free(tls, payload);
        jx_tls_ob_free(tls, inv_node);
        jx_ob_free(macro);
        return result;
      }
    } else {
      /* args declaration is a primitive */
      jx_ob inv_node = jx_tls_hash_new_with_assoc(tls, args, payload);
      jx_ob result;
      {
        jx_ob ob = jx_code_eval_tls(tls, JX_EVAL_DEFER_INVOCATION, inv_node, fn->body);
        jx_ob_free(macro);
        result = jx_code_eval_tls(tls, 0, node, ob);
        jx_tls_ob_free(tls,inv_node);
        jx_tls_ob_free(tls,ob);
        jx_tls_ob_free(tls,payload);
        return result;
      }
    }
  } else {
    jx_tls_ob_free(tls,payload);
    jx_ob_free(macro);
    return jx_ob_from_null();
  }
}

JX_INLINE jx_status jx__create_path(jx_ob *container,jx_ob *target)
{
  if(jx_list_check(*target)) { /* compound indentifier */
    jx_int i,path_size = jx_list_size(*target);
    jx_ob path = *target;
    if(!path_size) 
      return JX_NO;
    for(i=0;i<path_size;i++) {
      *target = jx_list_borrow(path,i);
      if((1+i)<path_size) {
        switch(container->meta.bits & JX_META_MASK_TYPE_BITS) {
        case JX_META_BIT_LIST:
          *container = jx_list_borrow(*container,jx_ob_as_int(*target));
          break;
        case JX_META_BIT_HASH:
          {
            jx_ob tmp = JX_OB_NULL;
            if(!jx_hash_peek(&tmp,*container,*target)) {
              tmp = jx_hash_new();
              jx_hash_set(*container,*target,tmp);
            }
            *container = tmp;
          }
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

JX_INLINE jx_status jx__resolve_path(jx_ob *container,jx_ob *target)
{
  if(jx_list_check(*target)) { /* compound indentifier */
    jx_int i,path_size = jx_list_size(*target);
    jx_ob path = *target;
    if(!path_size)
      return JX_NO;
    for(i=0;i<path_size;i++) {
      *target = jx_list_borrow(path,i);
      if((1+i)<path_size) {
        switch(container->meta.bits & JX_META_MASK_TYPE_BITS) {
        case JX_META_BIT_LIST:
          *container = jx_list_borrow(*container,jx_ob_as_int(*target));
          break;
        case JX_META_BIT_HASH:
          *container = jx_hash_borrow(*container,*target);
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

JX_INLINE jx_status jx__resolve_container(jx_tls *tls, jx_ob *container,jx_ob *target)
{
  if(jx_weak_check(*target)) {
    *container = *target;
    return JX_YES;
  }
  jx_status status = jx__resolve_path(container,target);
  if(JX_POS(status)) {
    switch(container->meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      if(jx_int_check(*target)) {
        *container = jx_list_borrow(*container,jx_ob_as_int(*target));
      } else if(tls && jx_hash_peek(&tls->method, tls->builtins, *target)) {
        tls->have_method = JX_TRUE;
      } else {
        status =JX_STATUS_INVALID_CONTAINER;
      }
        break;
    case JX_META_BIT_HASH:
      {
        jx_ob tmp = JX_OB_NULL;
        if(jx_hash_peek(&tmp,*container,*target))
          *container = tmp;
        else if(tls && jx_hash_peek(&tls->method, tls->builtins, *target)) {
          tls->have_method = JX_TRUE;
        } else {
          status =JX_STATUS_INVALID_CONTAINER;
        }
      }
      break;
    default:
      *container = jx_ob_from_null();
      break;
      }
    }
  return status;
}

/* enable C++ mangling */
#ifdef __cplusplus
#if 0
{
#endif
}
#endif


#endif
