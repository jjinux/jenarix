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

/* by default, Jenarix objects are 96 bits wide (32 bits of meta
   information and 64 bits of data, including zero-terminated tiny
   strings of up to 7 characters) */

#define JX_TINY_STR_SIZE 6
/* may only be 6, 10, or 18 */

#if (JX_TINY_STR_SIZE < JX_TINY_STR_MIN_SIZE)
#undef JX_TINY_STR_SIZE
#define JX_TINY_STR_SIZE JX_TINY_STR_MIN_SIZE
#endif

/* adapt to tiny string size (must be at least as wide as a machine
 * pointer) */

#if (JX_TINY_STR_SIZE == 6)
typedef jx_int32      jx_data;
#define JX_DATA_INIT  {0}
#else
#if (JX_TINY_STR_SIZE == 10)
typedef jx_int64      jx_data;
#define JX_DATA_INIT  {0}
#else
#if (JX_TINY_STR_SIZE == 18)
typedef jx_int64      jx_data[2];
#define JX_DATA_INIT  { {0,0} }
#endif
#endif
#endif

typedef struct jx__list jx_list;
typedef struct jx__hash jx_hash;

typedef  jx_uint16 jx_bits;

typedef struct {
  jx_bits  pad; 
  jx_bits  bits;
} jx_meta;

struct jx__ob {
  union {
    jx_data raw; /* must cover full width of union (for bit-zeroing purposes) */
    jx_bool bool_;
    jx_int int_;
    jx_float float_;
    jx_char tiny_str[JX_TINY_STR_SIZE-2]; 
    jx_char *str;
    jx_list *list;
    jx_hash *hash;
  } data;
  jx_meta meta;
};

typedef struct {
  jx_int size;
  jx_int rec_size;
} jx_vla; 

/* meta flag bits */

#define JX_META_NOT_AN_OB       0x8000

#define JX_META_BIT_GC          0x4000
/* set if object has garbage collected resourcese */

#define JX_META_BIT_BOOL        0x2000
#define JX_META_BIT_INT         0x1000
#define JX_META_BIT_FLOAT       0x0800
#define JX_META_BIT_STR         0x0400
#define JX_META_BIT_LIST        0x0200
#define JX_META_BIT_HASH        0x0100

#define JX_META_MASK_TYPE_BITS  0x3F00

#define JX_META_MASK_TINY_SIZE  0x00FF

/* object initializers */

#define JX_OB_NULL     { JX_DATA_INIT, {0,0}                    }
#define JX_OB_INT      { JX_DATA_INIT, {0,JX_META_BIT_INT}      }
#define JX_OB_FLOAT    { JX_DATA_INIT, {0,JX_META_BIT_FLOAT}    }
#define JX_OB_BOOL     { JX_DATA_INIT, {0,JX_META_BIT_BOOL}     }
#define JX_OB_TINY_STR { JX_DATA_INIT, {0,JX_META_BIT_TINY_STR} }

#define JX_OB_STR      { JX_DATA_INIT, {0, JX_META_BIT_GC | JX_META_BIT_STR } }
#define JX_OB_LIST     { JX_DATA_INIT, {0, JX_META_BIT_GC | JX_META_BIT_LIST} }
#define JX_OB_HASH     { JX_DATA_INIT, {0, JX_META_BIT_GC | JX_META_BIT_HASH} }

/* inline methods */

#define JX_INLINE __inline__ static

/* variable length array (vla) functions provide untyped, auto-zeroed,
   null-protected, size and record-length aware variable length arrays   
   ALWAYS PASSED BY REFERENCE (since pointer location may change) */

void     *jx_vla_new(jx_int rec_size, jx_int size);
void     *jx_vla_new_with_content(jx_int rec_size, jx_int size, void *content);
/* macros to avoid annoying type mismatch warnings with (void**) parameter */
#define   jx_vla_copy(r)            jx__vla_copy((void**)(r))
#define   jx_vla_size(r)            jx__vla_size((void**)(r))
#define   jx_vla_resize(r,s)        jx__vla_resize((void**)(r),(s))
#define   jx_vla_grow_check(r,i)    jx__vla_grow_check((void**)(r),(i))
#define   jx_vla_append(r,c)        jx__vla_append((void**)(r),(c))
#define   jx_vla_append_c_str(r,s)  jx__vla_append_c_str((void**)(r),(s))
#define   jx_vla_append_ob_str(r,o) jx__vla_append_ob_str((void**)(r),(o))
#define   jx_vla_insert(r,i,c)      jx__vla_insert((void**)(r),(i),(c))
#define   jx_vla_extend(r1,r2)      jx__vla_extend((void**)(r1),((void**)(r2)))
#define   jx_vla_remove(r,i,c)      jx__vla_remove((void**)(r),(i),(c))
#define   jx_vla_free(r)            jx__vla_free((void**)(r))
void *jx__vla_copy(void **ref);
JX_INLINE jx_int jx__vla_size(void **ref)
{
  if(*ref) {
    return ((jx_vla*)(*ref))[-1].size;
  } else {
    return 0;
  }
}
jx_status jx__vla_resize(void **ref, jx_int new_size);
JX_INLINE jx_status jx__vla_grow_check(void **ref, jx_int index)
{
  if(((jx_vla*)(*ref))[-1].size > index) {
    return JX_TRUE;
  } else {
    return (jx_vla_resize(ref, index+1) == JX_SUCCESS);
  }
}
jx_status jx__vla_append(void **ref, jx_int count);
jx_status jx__vla_append_c_str(void **ref, jx_char *str);
jx_status jx__vla_append_ob_str(void **ref, jx_ob ob);
jx_status jx__vla_insert(void **ref, jx_int index, jx_int count);
jx_status jx__vla_extend(void **ref1, void **ref2);
jx_status jx__vla_remove(void **ref, jx_int index, jx_int count);
jx_status jx__vla_free(void **ref);

JX_INLINE jx_int *jx_int_vla_new(jx_int size) 
{
  return jx_vla_new(sizeof(jx_int),size);
}
JX_INLINE jx_status jx_int_vla_resize(jx_int **ref,jx_int size) 
{
  return jx_vla_resize((void**)ref,size);
}
JX_INLINE jx_status jx_int_vla_free(jx_int *vla)
{
  return jx_vla_free(&vla);
}
JX_INLINE jx_float *jx_float_vla_new(jx_float size) 
{
  return jx_vla_new(sizeof(jx_float),size);
}
JX_INLINE jx_status jx_float_vla_resize(jx_float **ref,jx_int size) 
{
  return jx_vla_resize((void**)ref,size);
}
JX_INLINE jx_status jx_float_vla_free(jx_float *vla)
{
  return jx_vla_free(&vla);
}

jx_status jx__ob_free(jx_ob ob);

JX_INLINE jx_status jx_ob_free(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_GC) { 
    /* object has resources which need to be garbage collected */
    return jx__ob_free(ob); /* call non-inline routine */
  }
  return JX_SUCCESS;
}

JX_INLINE jx_ob jx_ob_from_null(void) 
{ 
  jx_ob result = JX_OB_NULL;
  return result;
}

JX_INLINE jx_ob jx_ob_from_bool(jx_bool bool_)
{
  jx_ob result = JX_OB_BOOL;
  result.data.bool_ = bool_;
  return result;
}

JX_INLINE jx_ob jx_ob_from_int(jx_int int_) 
{ 
  jx_ob result = JX_OB_INT;
  result.data.int_ = int_;
  return result;
}

JX_INLINE jx_ob jx_ob_from_float(jx_float float_)
{ 
  jx_ob result = JX_OB_FLOAT;
  result.data.float_ = float_;
  return result;
}

JX_INLINE jx_bool jx_ob_as_bool(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_BOOL) ? ob.data.bool_ : JX_FALSE;
}
JX_INLINE jx_int jx_ob_as_int(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_INT) ? ob.data.int_ : 0;
}
JX_INLINE jx_float jx_ob_as_float(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_FLOAT) ? ob.data.float_ : 0.0F;
}
JX_INLINE jx_status jx_null_check(jx_ob ob)
{
  return !ob.meta.bits;
}
JX_INLINE jx_status jx_bool_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_BOOL) ? JX_TRUE : JX_FALSE;
}
JX_INLINE jx_status jx_int_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_INT) ? JX_TRUE : JX_FALSE;
}
JX_INLINE jx_status jx_float_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_FLOAT) ? JX_TRUE : JX_FALSE;;
}
JX_INLINE jx_status jx_str_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_STR) ? JX_TRUE : JX_FALSE;;
}
JX_INLINE jx_status jx_list_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_LIST) ? JX_TRUE : JX_FALSE;;
}
JX_INLINE jx_status jx_hash_check(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_HASH) ? JX_TRUE : JX_FALSE;;
}
JX_INLINE jx_bool jx_ok(jx_status status)
{
  return (status>=0);
}

JX_INLINE jx_int jx_str_len(jx_ob ob)
{
  return ((ob.meta.bits & JX_META_BIT_STR) ?
          ((ob.meta.bits & JX_META_BIT_GC) ? jx_vla_size(&ob.data.str)-1 : 
           ob.meta.bits & JX_META_MASK_TINY_SIZE)
          : 0);
}

jx_ob jx__ob_copy(jx_ob ob);
JX_INLINE jx_ob jx_ob_copy(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_GC) {
    return jx__ob_copy(ob);
  }
  return ob;
}

jx_bool jx__ob_gc_identical(jx_ob left, jx_ob right);

JX_INLINE jx_bool jx_ob_identical(jx_ob left, jx_ob right)
{
  if(left.meta.bits != right.meta.bits) { 
    return JX_FALSE;
  } else if(left.meta.bits & JX_META_BIT_GC) {
    return jx__ob_gc_identical(left,right);
  } else {
#if (JX_TINY_STR_SIZE == 6) ||  (JX_TINY_STR_SIZE == 10)
    return ((left.meta.bits == right.meta.bits) && 
            (left.meta.pad  == right.meta.pad) && 
            (left.data.raw  == right.data.raw));
#else
#if (JX_TINY_STR_SIZE == 18)
    return ((left.meta.bits == right.meta.bits) && 
            (left.meta.pad  == right.meta.pad) && 
            (left.data.raw[0] == right.data.raw[0]) &&
            (left.data.raw[1] == right.data.raw[1]));
#endif
#endif
  }
}

/* lists */

jx_status jx__list_append(jx_list *I, jx_ob ob);
JX_INLINE jx_status jx_list_append(jx_ob list, jx_ob ob)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    return jx__list_append(list.data.list, ob);
  }
  return JX_FAILURE;
}
jx_status jx__list_insert(jx_list *I, jx_int index, jx_ob ob);
JX_INLINE jx_status jx_list_insert(jx_ob list, jx_int index, jx_ob ob)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    return jx__list_insert(list.data.list, index, ob);
  }
  return JX_FAILURE;
}
jx_status jx__list_replace(jx_list *I, jx_int index, jx_ob ob);
JX_INLINE jx_status jx_list_replace(jx_ob list, jx_int index, jx_ob ob)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    return jx__list_replace(list.data.list, index, ob);
  }
  return JX_FAILURE;
}

jx_status jx__list_combine(jx_list *list1,jx_list *list2);
JX_INLINE jx_status jx_list_combine(jx_ob list1, jx_ob list2)
{
  if((list1.meta.bits & JX_META_BIT_LIST) && 
     (list2.meta.bits & JX_META_BIT_LIST)) {
    return jx__list_combine(list1.data.list, list2.data.list);
  }
  return JX_FAILURE;
}

jx_ob jx__list_borrow(jx_list *I, jx_int index);
JX_INLINE jx_ob jx_list_borrow(jx_ob list, jx_int index)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    return jx__list_borrow(list.data.list, index);
  }
  return jx_ob_from_null();
}
#define JX__HASH_COPY_KEYS   0x1
#define JX__HASH_COPY_VALUES 0x2
jx_ob jx__hash_copy_members(jx_hash *I,jx_int flags);
JX_INLINE jx_ob jx_list_new_from_hash(jx_ob hash)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_copy_members(hash.data.hash,
                                 JX__HASH_COPY_KEYS | 
                                 JX__HASH_COPY_VALUES);
  }
  return jx_ob_from_null();
}
jx_ob jx_list_new(void);
jx_status jx__list_with_hash(jx_list *list, jx_hash *hash);
JX_INLINE jx_ob jx_list_new_with_hash(jx_ob hash)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result = jx_list_new();
    if(result.meta.bits & JX_META_BIT_LIST) {
      if(jx_ok(jx__list_with_hash(result.data.list, hash.data.hash))) {
        jx_ob_free(hash);
      } else {
        jx_ob_free(result);
        result = hash;
      }
    }
    return result;
  }
  return jx_ob_from_null();
}

/* hashing */

JX_INLINE jx_uint32 jx__ob_hash_code(jx_meta meta, jx_data data)
{
#if (JX_TINY_STR_SIZE == 6)
  register jx_uint32 a = (((jx_uint32)meta.bits)^
                          ((jx_uint32)meta.pad)^
                          ((jx_uint32)data));
#else
#if (JX_TINY_STR_SIZE == 10)
  register jx_uint32 a = (((jx_uint32)meta.bits)^
                          ((jx_uint32)meta.pad)^
                          ((jx_uint32)data)^
                          ((jx_uint32)(data>>32)));
#else
#if (JX_TINY_STR_SIZE == 18)
  register jx_uint32 a = (((jx_uint32)meta.bits)^
                          ((jx_uint32)meta.pad)^
                          ((jx_uint32)data[0])^
                          ((jx_uint32)(data[0]>>32))^
                          ((jx_uint32)data[1])^
                          ((jx_uint32)(data[1]>>32)));
#endif
#endif
#endif
  a += ~(a<<15);
  a ^=  (a>>10);
  a +=  (a<<3);
  a ^=  (a>>6);
  a += ~(a<<11);
  a ^=  (a>>16);
  return (a ? a : 1); /* zero is reserved as the hash code of an
                         unhashable object */
}

jx_uint32 jx__ob_gc_hash_code(jx_ob);
JX_INLINE jx_uint32 jx_ob_hash_code(jx_ob ob)
{
  return (ob.meta.bits & JX_META_BIT_GC) ?
    jx__ob_gc_hash_code(ob) :
    jx__ob_hash_code(ob.meta, ob.data.raw);
  
}

jx_ob jx_hash_new(void);
jx_status jx__hash_from_list(jx_hash *hast, jx_list *list);
JX_INLINE jx_ob jx_hash_new_from_list(jx_ob list)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_ob result = jx_hash_new();
    if(result.meta.bits & JX_META_BIT_HASH) {
      if(!jx_ok(jx__hash_from_list(result.data.hash,list.data.list))) {
        jx_ob_free(result);
        result = jx_ob_from_null();
      }
    }
    return result;
  }
  return jx_ob_from_null();
}
jx_status jx__hash_with_list(jx_hash *hash, jx_list *list);
JX_INLINE jx_ob jx_hash_new_with_list(jx_ob list)
{
  if(list.meta.bits & JX_META_BIT_LIST) {
    jx_ob result = jx_hash_new();
    if(result.meta.bits & JX_META_BIT_HASH) {
      if(jx_ok(jx__hash_with_list(result.data.hash,list.data.list))) {
        jx_ob_free(list);
      } else {
        jx_ob_free(result);
        result = list;
      }
    }
    return result;
  }
  return jx_ob_from_null();
}

jx_int jx__hash_size(jx_hash *I);
JX_INLINE jx_int jx_hash_size(jx_ob hash)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_size(hash.data.hash);
  }
  return 0;
}

jx_status jx__hash_set(jx_hash *I, jx_ob key, jx_ob value);
JX_INLINE jx_status jx_hash_set(jx_ob hash, jx_ob key, jx_ob value)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_set(hash.data.hash, key, value);
  }
  return JX_FAILURE;
}

jx_bool jx__hash_has_key(jx_hash *I, jx_ob key);
JX_INLINE jx_bool jx_hash_has_key(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_has_key(hash.data.hash, key);
  }
  return JX_FALSE;
}

JX_INLINE jx_ob jx_hash_keys(jx_ob hash)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_copy_members(hash.data.hash, JX__HASH_COPY_KEYS);
  }
  return jx_ob_from_null();
}
JX_INLINE jx_ob jx_hash_values(jx_ob hash)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    return jx__hash_copy_members(hash.data.hash, JX__HASH_COPY_VALUES);
  }
  return jx_ob_from_null();
}

jx_bool jx__hash_borrow(jx_ob *result, jx_hash *I, jx_ob key);
JX_INLINE jx_ob jx_hash_borrow(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_borrow(&result, hash.data.hash, key))
      return result;
  }
  return jx_ob_from_null();
}
JX_INLINE jx_ob jx_hash_get(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_borrow(&result, hash.data.hash, key))
      return jx_ob_copy(result);
  }
  return jx_ob_from_null();
}

jx_bool jx__hash_remove(jx_ob *result, jx_hash *I, jx_ob key);
JX_INLINE jx_ob jx_hash_remove(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_remove(&result, hash.data.hash, key))
      return result;
  }
  return jx_ob_from_null();
}

JX_INLINE jx_status jx_hash_delete(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_remove(&result, hash.data.hash, key)) {
      jx_ob_free(result);
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
}

jx_bool jx__hash_borrow_key(jx_ob *result, jx_hash *I, jx_ob key);
JX_INLINE jx_ob jx_hash_borrow_key(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_borrow_key(&result, hash.data.hash, key))
      return result;
  }
  return jx_ob_from_null();
}
JX_INLINE jx_ob jx_hash_get_key(jx_ob hash, jx_ob key)
{
  if(hash.meta.bits & JX_META_BIT_HASH) {
    jx_ob result;
    if(jx__hash_borrow_key(&result, hash.data.hash, key))
      return jx_ob_copy(result);
  }
  return jx_ob_from_null();
}

jx_ob     jx_ob_to_json_with_flags(jx_ob ob,jx_int flags);
JX_INLINE jx_ob jx_ob_to_json(jx_ob ob)
{
  return jx_ob_to_json_with_flags(ob,0);
}

#endif


