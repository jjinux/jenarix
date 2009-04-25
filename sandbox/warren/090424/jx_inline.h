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

typedef  jx_int32     jx_meta;

#define JX_TINY_STR_SIZE 16

/* adapt to tiny string size (must be at least as wide as a machine
 * pointer) */

#if (JX_TINY_STR_SIZE == 4)
typedef jx_int32      jx_data;
#define JX_DATA_INIT  {0}
#else
#if (JX_TINY_STR_SIZE == 8)
typedef jx_int64      jx_data;
#define JX_DATA_INIT  {0}
#else
#if (JX_TINY_STR_SIZE == 16)
typedef jx_int64      jx_data[2];
#define JX_DATA_INIT  { {0,0} }
#endif
#endif
#endif

typedef struct jx__list jx_list;
typedef struct jx__hash jx_hash;

struct jx__ob {
  jx_meta meta;
  union {
    jx_data raw; /* must cover full width of union (for bit-zeroing purposes) */
    jx_bool bool_;
    jx_int int_;
    jx_float float_;
    jx_char tiny_str[JX_TINY_STR_SIZE]; 
    jx_char *str;
    jx_list *list;
    jx_hash *hash;
  } data;
};

typedef struct {
  jx_int size;
  jx_int rec_size;
} jx_vla; 

/* meta flag bits */

#define JX_META_BIT_GC          0x80000000 
/* set if object has garbage collected resourcese */

#define JX_META_BIT_STR         0x40000000
#define JX_META_BIT_FLOAT       0x20000000
#define JX_META_BIT_INT         0x10000000
#define JX_META_BIT_BOOL        0x08000000
#define JX_META_BIT_LIST        0x04000000
#define JX_META_BIT_HASH        0x02000000

#define JX_META_MASK_TYPE_BITS  0x7FFF0000

#define JX_META_MASK_TINY_SIZE  0x000000FF

/* object initializers */

#define JX_OB_NULL     { 0, JX_DATA_INIT }
#define JX_OB_INT      { JX_META_BIT_INT, JX_DATA_INIT }
#define JX_OB_FLOAT    { JX_META_BIT_FLOAT, JX_DATA_INIT }
#define JX_OB_BOOL     { JX_META_BIT_BOOL, JX_DATA_INIT }
#define JX_OB_TINY_STR { JX_META_BIT_TINY_STR, JX_DATA_INIT }

#define JX_OB_STR      { JX_META_BIT_GC | JX_META_BIT_STR,  JX_DATA_INIT }
#define JX_OB_LIST     { JX_META_BIT_GC | JX_META_BIT_LIST, JX_DATA_INIT }
#define JX_OB_HASH     { JX_META_BIT_GC | JX_META_BIT_HASH, JX_DATA_INIT }

/* inline methods */

#define JX_INLINE __inline__ static

jx_status jx_vla_resize(void **vla, jx_int new_size);

JX_INLINE jx_int jx_vla_get_size(void *vla)
{
  if(vla) {
    return ((jx_vla*)vla)[-1].size;
  } else {
    return 0;
  }
}

JX_INLINE jx_status jx_vla_grow_check(void **vla, jx_int index)
{
  if(((jx_vla*)(*vla))[-1].size > index) {
    return JX_TRUE;
  } else {
    return (jx_vla_resize(vla, index+1) == JX_SUCCESS);
  }
}

jx_status jx__ob_free(jx_ob ob);

JX_INLINE jx_status jx_ob_free(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_GC) { 
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
  return (ob.meta & JX_META_BIT_BOOL) ? ob.data.bool_ : JX_FALSE;
}
JX_INLINE jx_int jx_ob_as_int(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_INT) ? ob.data.int_ : 0;
}
JX_INLINE jx_float jx_ob_as_float(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_FLOAT) ? ob.data.float_ : 0.0F;
}
JX_INLINE jx_status jx_ob_is_null(jx_ob ob)
{
  return !ob.meta;
}
JX_INLINE jx_status jx_ob_is_int(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_INT) ? JX_TRUE : JX_FALSE;
}
JX_INLINE jx_status jx_ob_is_float(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_FLOAT) ? JX_TRUE : JX_FALSE;
}
JX_INLINE jx_status jx_ob_is_str(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_STR) ? JX_TRUE : JX_FALSE;
}
JX_INLINE jx_status jx_ob_is_list(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_LIST) ? JX_TRUE : JX_FALSE;
}
JX_INLINE jx_status jx_ob_is_hash(jx_ob ob)
{
  return (ob.meta & JX_META_BIT_HASH) ? JX_TRUE : JX_FALSE;
}
#endif


