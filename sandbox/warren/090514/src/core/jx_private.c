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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_private.h"
#include "jx_mem_wrap.h"

/* jx_vla routines */

void *jx_vla_new(jx_int rec_size, jx_int size)
{
  jx_vla *vla = (jx_vla *) jx_calloc(1, sizeof(jx_vla) + rec_size * size);
  if(vla) {
    vla->size = size;
    vla->rec_size = rec_size;
    return (void *) (vla + 1);
  } else
    return NULL;
}

void *jx_vla_new_with_content(jx_int rec_size, jx_int size, void *content)
{
  jx_vla *vla = jx_malloc(sizeof(jx_vla) + rec_size * size);
  if(vla) {
    jx_char *base = (jx_char *) (vla + 1);
    vla->size = size;
    vla->rec_size = rec_size;
    memcpy(base, content, rec_size * size);
    return (void *) base;
  } else
    return NULL;
}

void *jx__vla_copy(void **ref)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    return jx_vla_new_with_content(vla->rec_size, vla->size, (*ref));
  } else {
    return NULL;
  }
}

jx_status jx__vla_resize(void **ref, jx_int new_size)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if(new_size != vla->size) {
      vla = jx_realloc(vla, sizeof(jx_vla) + vla->rec_size * new_size);
      if(vla) {
        *ref = (void *) (vla + 1);
        if(vla->size < new_size) {      /* zero new memory */
          jx_char *base = (jx_char *) (*ref);
          memset(base + vla->size * vla->rec_size, 0,
                 (new_size - vla->size) * vla->rec_size);
        }
        vla->size = new_size;
        return JX_SUCCESS;
      } else {
        return JX_FAILURE;
      }
    } else {
      return JX_SUCCESS;
    }
  } else {
    return JX_FAILURE;
  }
}

jx_status jx__vla_append(void **ref, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if(count >= 0) {
      jx_int new_size = vla->size + count;
      vla = jx_realloc(vla, sizeof(jx_vla) + vla->rec_size * new_size);
      if(vla) {
        jx_char *base = (jx_char *) (vla + 1);
        (*ref) = base;
        memset(base + vla->rec_size * vla->size, 0, vla->rec_size * count);
        vla->size = new_size;
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_append_c_str(void **ref, jx_char * str)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if(vla->rec_size == 1) {
      jx_int str_len = strlen(str);
      jx_int size = vla->size;
      jx_status status = jx_vla_append(ref, (size ? str_len : str_len + 1));
      if(jx_ok(status)) {
        jx_char *base = (jx_char *) (*ref);
        memcpy(base + (size ? size - 1 : 0), str, str_len + 1);
      } else {
        return status;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_append_ob_str(void **ref, jx_ob ob)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if(vla->rec_size == 1) {
      jx_int str_len = jx_str_len(ob);
      jx_int size = vla->size;
      jx_status status = jx_vla_append(ref, (size ? str_len : str_len + 1));
      if(jx_ok(status)) {
        jx_char *base = (jx_char *) (*ref);
        memcpy(base + (size ? size - 1 : 0), jx_ob_as_str(&ob), str_len + 1);
      } else {
        return status;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_insert(void **ref, jx_int index, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if((index >= 0) && (count >= 0) && (index <= vla->size)) {
      jx_int new_size = vla->size + count;
      vla = jx_realloc(vla, sizeof(jx_vla) + vla->rec_size * new_size);
      if(vla) {
        jx_char *base = (jx_char *) (vla + 1);
        (*ref) = base;
        memmove(base + (index + count) * vla->rec_size,
                base + index * vla->rec_size, (vla->size - index) * vla->rec_size);
        vla->size = new_size;
        memset(base + index * vla->rec_size, 0, vla->rec_size * count);
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_extend(void **ref1, void **ref2)
{
  if(*ref1 && *ref2) {
    jx_vla *vla1 = ((jx_vla *) (*ref1)) - 1;
    jx_vla *vla2 = ((jx_vla *) (*ref2)) - 1;
    if(vla1->rec_size == vla2->rec_size) {
      jx_int new_size = vla1->size + vla2->size;
      vla1 = jx_realloc(vla1, sizeof(jx_vla) + vla1->rec_size * new_size);
      if(vla1) {
        jx_char *base1 = (jx_char *) (vla1 + 1);
        jx_char *base2 = (jx_char *) (vla2 + 1);
        (*ref1) = base1;
        memcpy(base1 + vla1->rec_size * vla1->size, base2, vla2->rec_size * vla2->size);
        vla1->size = new_size;
        return JX_SUCCESS;
      }
    }
  } else if(*ref2) {
    jx_vla *vla2 = ((jx_vla *) (*ref2)) - 1;
    jx_vla *vla1 = jx_vla_new(vla2->rec_size, vla2->size);
    if(vla1) {
      jx_char *base1 = (jx_char *) (vla1 + 1);
      jx_char *base2 = (jx_char *) (vla2 + 1);
      memcpy(base1, base2, vla2->rec_size * vla2->size);
      (*ref1) = base1;
      return JX_SUCCESS;
    }
  } else if(!*ref2) {
    return JX_SUCCESS;
  }
  return JX_FAILURE;
}

jx_status jx__vla_remove(void **ref, jx_int index, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if((index >= 0) && (count >= 0) &&
       (index < vla->size) && ((count + index) <= vla->size)) {
      jx_int new_size = vla->size - count;
      jx_char *base = (jx_char *) (*ref);
      memmove(base + index * vla->rec_size,
              base + (count + index) * vla->rec_size,
              ((vla->size - index) - count) * vla->rec_size);
      return jx_vla_resize(ref, new_size);
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_free(void **ref)
{
  if(*ref) {
    jx_free(((jx_vla *) (*ref)) - 1);
    (*ref) = NULL;
  }
  return JX_SUCCESS;
}

/* strings */

jx_ob jx_ob_from_str(jx_char * str)
{
  jx_int size = strlen(str);
  jx_ob result = JX_OB_NULL;

  if(size < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | size;
    memcpy(result.data.io.tiny_str, str, size+1);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = jx_vla_new(1, size + 1);
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      memcpy(result.data.io.str, str, size+1);
    }
  }
  return result;
}

jx_ob jx_ob_with_str_vla(jx_char ** ref)
{
  jx_int size = jx_vla_size(ref);
  jx_ob result = JX_OB_NULL;
  if(size)
    size--;
  if(size < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | size;
    memcpy(result.data.io.tiny_str, *ref, size+1);
    jx_vla_free(ref);
  } else {
    result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
    result.data.io.str = *ref;
  }
  return result;
}

jx_char *jx_ob_as_str(jx_ob * ob)
{
  jx_bits meta = ob->meta.bits;
  if(meta & JX_META_BIT_STR) {
    if(meta & JX_META_BIT_GC) {
      return ob->data.io.str;
    } else {
      return ob->data.io.tiny_str;
    }
  }
  return NULL;
}

/* lists */

jx_ob jx_list_new(void)
{
  jx_ob result = JX_OB_LIST;
  if((result.data.io.list = (jx_list *) jx_calloc(1, sizeof(jx_list)))) {
    return result;
  } else {
    return jx_ob_from_null();
  }
}

jx_ob jx_list_new_from_int_array(jx_int * array, jx_int size)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->packed_meta_bits = JX_META_BIT_INT;
    result.data.io.list->data.vla = jx_vla_new_with_content(sizeof(jx_int), size, array);
  }
  return result;
}

jx_ob jx_list_new_with_int_vla(jx_int ** ref)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->packed_meta_bits = JX_META_BIT_INT;
    result.data.io.list->data.vla = *ref;
  }
  return result;
}

jx_ob jx_list_new_from_float_array(jx_float * array, jx_float size)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->packed_meta_bits = JX_META_BIT_FLOAT;
    result.data.io.list->data.vla = jx_vla_new_with_content(sizeof(jx_float), size, array);
  }
  return result;
}

jx_ob jx_list_new_with_float_vla(jx_float ** ref)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->packed_meta_bits = JX_META_BIT_FLOAT;
    result.data.io.list->data.vla = *ref;
  }
  return result;
}

jx_ob jx__list_copy(jx_list * I)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    jx_list *new_I = result.data.io.list;
    (*new_I) = (*I);
    new_I->data.vla = jx_vla_copy(&I->data.vla);
    if(!new_I->packed_meta_bits) {      /* need to recursively copy all GC content */
      jx_int i, size = jx_vla_size(&new_I->data.ob_vla);
      jx_ob *ob = new_I->data.ob_vla;
      for(i = 0; i < size; i++) {
        if(ob->meta.bits & JX_META_BIT_GC) {
          *ob = jx_ob_copy(*ob);
        }
        ob++;
      }
    }
  }
  return result;
}

jx_status jx__list_set_read_only(jx_list * I, jx_bool read_only)
{
  if(!I->packed_meta_bits) {    /* no need to scan packed structures */
    jx_int i, size = jx_vla_size(&I->data.ob_vla);
    jx_ob *ob = I->data.ob_vla;
    if(read_only) {
      for(i = 0; i < size; i++) {
        if(ob->meta.bits & JX_META_BIT_GC)
          ob->meta.bits |= JX_META_BIT_READ_ONLY;
        ob++;
      }
    } else {
      for(i = 0; i < size; i++) {
        if(ob->meta.bits & JX_META_BIT_GC)
          ob->meta.bits &= JX_META_BIT_READ_ONLY;
        ob++;
      }
    }
  }
  return JX_SUCCESS;
}

static jx_status jx__list_free(jx_list * list)
{
  if(!list->packed_meta_bits) {
    jx_int i, size = jx_vla_size(&list->data.vla);
    jx_ob *ob = list->data.ob_vla;
    for(i = 0; i < size; i++) {
      jx_ob_free(*(ob++));
    }
  }
  jx_vla_free(&list->data.vla);
  jx_free(list);
  return JX_SUCCESS;
}

JX_INLINE jx_int jx__list_size(jx_list * I)
{
  return jx_vla_size(&I->data.vla);
}

jx_int jx_list_size(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_LIST) {
    return jx__list_size(ob.data.io.list);
  } else
    return 0;
}

static jx_status jx_meta_get_packed_size(jx_bits bits)
{
  switch (bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_INT:
    return sizeof(jx_int);
    break;
  case JX_META_BIT_FLOAT:
    return sizeof(jx_float);
    break;
  default:
    return 0;
  }
}

jx_bool jx__list_equal(jx_list *left, jx_list *right)
{
  jx_int left_size = jx__list_size(left);
  jx_int right_size = jx__list_size(right);
  if(left_size == right_size) {
    if(!left_size)
      return JX_TRUE;
    else if(left->packed_meta_bits != right->packed_meta_bits)
      /* to do */
      return JX_FALSE;
    else if(left->packed_meta_bits) {
      jx_int i;
      switch (left->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_INT:
        {
          jx_int *left_int = left->data.int_vla;
          jx_int *right_int = right->data.int_vla;
          for(i = 0; i < left_size; i++) {
            if( *(left_int++) != *(right_int++))
              return JX_FALSE;
          }
        }
        return JX_TRUE;
        break;
      case JX_META_BIT_FLOAT:
        {
          jx_float *left_float = left->data.float_vla;
          jx_float *right_float = right->data.float_vla;
          for(i = 0; i < left_size; i++) {
            if( *(left_float++) != *(right_float++))
              return JX_FALSE;
          }
        }
        return JX_TRUE;
        break;
      }
    } else { /* standard objects */
      jx_int i;
      jx_ob *left_ob = left->data.ob_vla;
      jx_ob *right_ob = right->data.ob_vla;
      for(i = 0; i < left_size; i++) {
        if(!jx_ob_equal(*(left_ob++),*(right_ob++)))
          return JX_FALSE;
      }
      return JX_TRUE;
    }
  }
  return JX_FALSE;
}

static jx_status jx__list_repack_data(jx_list * list)
{
  /* re-homogenize (if possible) */
  jx_int i, size = jx_vla_size(&list->data.vla);
  if(!size) {
    list->packed_meta_bits = 0;
    if(list->data.vla) {
      jx_vla_free(&list->data.vla);
    }
    return JX_TRUE;
  }
  jx_bits meta_or = 0, meta_and = -1;
  {
    jx_ob *ob = list->data.ob_vla;
    for(i = 0; i < size; i++) {
      meta_or |= ob->meta.bits;
      meta_and &= (ob++)->meta.bits;
    }
  }
  if(meta_and == meta_or) {     /* all meta bits identical in all objects */
    switch (meta_or) {
    case JX_META_BIT_INT:
    case JX_META_BIT_FLOAT:
      {
        void *new_vla = NULL;
        switch (meta_or) {
        case JX_META_BIT_INT:
          new_vla = jx_vla_new(sizeof(jx_int), size);
          if(new_vla) {
            jx_ob *src = list->data.ob_vla;
            jx_int *dst = (jx_int *) new_vla;
            for(i = 0; i < size; i++) {
              *(dst++) = jx_ob_as_int(*(src++));
            }
          }
          break;
        case JX_META_BIT_FLOAT:
          new_vla = jx_vla_new(sizeof(jx_float), size);
          if(new_vla) {
            jx_ob *src = list->data.ob_vla;
            jx_float *dst = (jx_float *) new_vla;
            for(i = 0; i < size; i++) {
              *(dst++) = jx_ob_as_int(*(src++));
            }
          }
          break;
        }
        if(new_vla) {
          jx_vla_free(&list->data.vla);
          list->data.vla = new_vla;
          list->packed_meta_bits = meta_or;
          return JX_TRUE;
        }
      }
      break;
    default:
      break;
    }
  }
  return JX_FALSE;
}

static jx_status jx__list_unpack_data(jx_list * list)
{
  /* homogenous list becoming hetergeneous */
  jx_int size = jx_vla_size(&list->data.vla);
  if(!size) {
    list->packed_meta_bits = 0;
    if(list->data.vla) {
      jx_vla_free(&list->data.vla);
    }
    return JX_SUCCESS;
  }
  jx_ob *new_vla = jx_vla_new(sizeof(jx_ob), size);
  if(new_vla) {
    switch (list->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_INT:
      {
        jx_int i;
        jx_int *src = list->data.int_vla;
        jx_ob *dst = new_vla;
        for(i = 0; i < size; i++) {
          *(dst++) = jx_ob_from_int(*(src++));
        }
      }
      break;
    case JX_META_BIT_FLOAT:
      {
        jx_int i;
        jx_float *src = list->data.float_vla;
        jx_ob *dst = new_vla;
        for(i = 0; i < size; i++) {
          *(dst++) = jx_ob_from_float(*(src++));
        }
      }
      break;
    }
    jx_vla_free(&list->data.vla);
    list->data.vla = new_vla;
    list->packed_meta_bits = 0;
    return JX_SUCCESS;
  } else
    return JX_FAILURE;
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

/* mental / debuggin aids */

#define JX_OWN(ob) ob
#define JX_BORROW(ob) ob

jx_status jx__list_resize(jx_list * I, jx_int size, jx_ob fill)
{
  if(size >= 0) {
    jx_int old_size = jx__list_size(I);
    if(I->packed_meta_bits != fill.meta.bits) {
      if(I->packed_meta_bits) {
        if(!jx_ok(jx__list_unpack_data(I)))
          return JX_FAILURE;
      }
      if(!old_size)
        jx_vla_free(&I->data.vla);
    }
    if(!I->data.vla) {          /* filling an empty list */
      jx_int packed_size = jx_meta_get_packed_size(fill.meta.bits);
      if(packed_size) {
        /* filling with packed data */
        I->packed_meta_bits = fill.meta.bits;
        I->data.vla = jx_vla_new(packed_size, size);
        if(I->data.vla) {
          switch (I->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
          case JX_META_BIT_INT:
            {
              jx_int *int_ = I->data.int_vla + old_size;
              jx_int int_fill = fill.data.io.int_;
              jx_int i;
              for(i = 0; i < size; i++) {
                *(int_++) = int_fill;
              }
            }
            break;
          case JX_META_BIT_FLOAT:
            {
              jx_float *float_ = I->data.float_vla + old_size;
              jx_float float_fill = fill.data.io.float_;
              jx_int i;
              for(i = 0; i < size; i++) {
                *(float_++) = float_fill;
              }
            }
            break;
          }
          return JX_SUCCESS;
        }
      } else {
        /* filling with normal objects */
        I->data.vla = jx_vla_new(sizeof(jx_ob), size);
        if(I->data.vla) {
          jx_int i;
          jx_ob *ob = I->data.ob_vla;
          if(fill.meta.bits & JX_META_BIT_GC) {
            for(i = 0; i < size; i++) {
              *(ob++) = jx_ob_copy(fill);
            }
          } else {
            for(i = 0; i < size; i++) {
              *(ob++) = fill;
            }
          }
          return JX_SUCCESS;
        }
      }
    } else {                    /* existing list is not empty, but data sizes are known to match */
      if(!(fill.meta.bits & JX_META_BIT_GC)) {
        /* filling with a non GC object, possibly packed */
        if(!I->packed_meta_bits) {      /* not packed data */
          if(size < old_size) { /* must freee some existing objects */
            jx_int i;
            jx_ob *ob = I->data.ob_vla + size;
            for(i = size; i < old_size; i++) {
              jx_ob_free(*(ob++));
            }
          }
          if(jx_ok(jx_vla_resize(&I->data.vla, size))) {
            if(size > old_size) {
              jx_int i;
              jx_ob *ob = I->data.ob_vla + old_size;
              for(i = old_size; i < size; i++) {
                *(ob++) = fill;
              }
            }
            return JX_SUCCESS;
          }
        } else {                /* implies I->packed_meta_bits == fill.meta.bits */
          if(jx_ok(jx_vla_resize(&I->data.vla, size))) {
            if(size > old_size) {
              switch (I->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
              case JX_META_BIT_INT:
                {
                  jx_int *int_ = I->data.int_vla + old_size;
                  jx_int int_fill = fill.data.io.int_;
                  jx_int i;
                  for(i = old_size; i < size; i++) {
                    *(int_++) = int_fill;
                  }
                }
                break;
              case JX_META_BIT_FLOAT:
                {
                  jx_float *float_ = I->data.float_vla + old_size;
                  jx_float float_fill = fill.data.io.float_;
                  jx_int i;
                  for(i = old_size; i < size; i++) {
                    *(float_++) = float_fill;
                  }
                }
                break;
              }
            }
            return JX_SUCCESS;
          }
        }
      } else {
        /* filling with a GC object, never packed  */
        if(size < old_size) {   /* must freee some existing objects */
          jx_int i;
          jx_ob *ob = I->data.ob_vla + size;
          for(i = size; i < old_size; i++) {
            jx_ob_free(*(ob++));
          }
        }
        if(jx_ok(jx_vla_resize(&I->data.vla, size))) {
          if(size > old_size) {
            jx_int i;
            jx_ob *ob = I->data.ob_vla + old_size;
            for(i = old_size; i < size; i++) {
              *(ob++) = jx_ob_copy(fill);
            }
          }
          return JX_SUCCESS;
        }
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_append(jx_list * I, jx_ob ob)
{
  if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
    if(!jx_ok(jx__list_unpack_data(I)))
      return JX_FAILURE;
  }
  if(I->data.vla) {
    jx_int size = jx_vla_size(&I->data.vla);
    if(jx_vla_grow_check(&I->data.vla, size)) {
      if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
        jx__list_set_packed_data(I, size, ob);
      } else if(!I->packed_meta_bits) {
        I->data.ob_vla[size] = JX_OWN(ob);
      }
      return JX_SUCCESS;
    }
  } else {
    jx_int packed_size = jx_meta_get_packed_size(ob.meta.bits);
    if(packed_size) {
      I->packed_meta_bits = ob.meta.bits;
      I->data.vla = jx_vla_new(packed_size, 1);
      if(I->data.vla) {
        jx__list_set_packed_data(I, 0, ob);
        return JX_SUCCESS;
      }
    } else {
      I->data.vla = jx_vla_new(sizeof(jx_ob), 1);
      if(I->data.vla) {
        I->data.ob_vla[0] = JX_OWN(ob);
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_insert(jx_list * I, jx_int index, jx_ob ob)
{
  if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
    if(!jx_ok(jx__list_unpack_data(I)))
      return JX_FAILURE;
  }
  if(I->data.vla) {
    if(!jx_ok(jx_vla_insert(&I->data.vla, index, 1)))
      return JX_FAILURE;
    else if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
      jx__list_set_packed_data(I, index, ob);
    } else if(!I->packed_meta_bits) {
      I->data.ob_vla[index] = JX_OWN(ob);
    }
    return JX_SUCCESS;
  } else if(index == 0) {
    jx_int packed_size = jx_meta_get_packed_size(ob.meta.bits);
    if(packed_size) {
      I->packed_meta_bits = ob.meta.bits;
      I->data.vla = jx_vla_new(packed_size, 1);
      if(I->data.vla) {
        jx__list_set_packed_data(I, 0, ob);
        return JX_SUCCESS;
      }
    } else {
      I->data.vla = jx_vla_new(sizeof(jx_ob), 1);
      if(I->data.vla) {
        I->data.ob_vla[0] = JX_OWN(ob);
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_replace(jx_list * I, jx_int index, jx_ob ob)
{
  jx_int size = jx_vla_size(&I->data.vla);
  if(index < size) {
    if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
      if(!jx_ok(jx__list_unpack_data(I)))
        return JX_FAILURE;
    }
    if(I->data.vla) {
      if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
        jx__list_set_packed_data(I, index, ob);
      } else if(!I->packed_meta_bits) {
        jx_ob_free(I->data.ob_vla[index]);
        I->data.ob_vla[index] = JX_OWN(ob);
      }
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_combine(jx_list * list1, jx_list * list2)
{
  if(list1 == list2) {
    jx_ob ob = jx__list_copy(list2);
    if(jx_ok(jx__list_combine(list1, ob.data.io.list))) {
      return JX_SUCCESS;
    } else {
      jx_ob_free(ob);
    }
  } else {
    jx_int list1_size = jx__list_size(list1);
    jx_int list2_size = jx__list_size(list2);
    if(!list1_size) {
      list1->packed_meta_bits = 0;
      jx_vla_free(&list1);
    }
    if(!list2_size) {
      list2->packed_meta_bits = 0;
      jx_vla_free(&list2);
    }
    if(list1_size && list2_size) {
      if(list1->packed_meta_bits != list2->packed_meta_bits) {
        if(list1->packed_meta_bits) {
          if(!jx_ok(jx__list_unpack_data(list1)))
            return JX_FAILURE;
        }
        if(list2->packed_meta_bits) {
          if(!jx_ok(jx__list_unpack_data(list2)))
            return JX_FAILURE;
        }
      }
    }
    if(list1->data.vla && list2->data.vla) {
      if(jx_ok(jx__vla_extend(&list1->data.vla, &list2->data.vla))) {
        jx_vla_free(&list2->data.vla);
        jx__list_free(list2);
        return JX_SUCCESS;
      }
    } else if(list2->data.vla) {
      (*list1) = (*list2);
      list2->data.vla = NULL;
      jx__list_free(list2);
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
}

jx_ob jx__list_borrow(jx_list * I, jx_int index)
{
  if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
    if(I->packed_meta_bits) {
      return jx__list_get_packed_data(I, index);
    } else {
      return JX_BORROW( I->data.ob_vla[index] );
    }
  }
  return jx_ob_from_null();
}

jx_ob jx__list_remove(jx_list * I, jx_int index)
{
  if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
    if(I->packed_meta_bits) {
      return jx__list_get_packed_data(I, index);
    } else {
      jx_ob result = I->data.ob_vla[index];
      if(jx_ok(jx_vla_remove(&I->data.vla, index, 1))) {
        return result;
      } else
        return jx_ob_from_null();
    }
  }
  return jx_ob_from_null();
}

jx_status jx__list_delete(jx_list * I, jx_int index)
{
  if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
    if(!I->packed_meta_bits) {
      jx_ob_free(I->data.ob_vla[index]);
    }
    return jx_vla_remove(&I->data.vla, index, 1);
  }
  return JX_FAILURE;
}

jx_int *jx_list_as_int_vla(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_LIST) {
    jx_list *list = ob.data.io.list;
    if(list->packed_meta_bits & JX_META_BIT_INT) {
      return list->data.int_vla;
    } else if(jx__list_repack_data(list)) {
      if(list->packed_meta_bits & JX_META_BIT_INT) {
        return list->data.int_vla;
      } else if(!list->data.vla) {
        list->packed_meta_bits = JX_META_BIT_INT;
        list->data.int_vla = (jx_int *) jx_vla_new(sizeof(int), 0);
        return list->data.int_vla;
      }
    }
  }
  return NULL;
}

jx_status jx__list_set_int_vla(jx_list * I, jx_int ** ref)
{
  if(I->packed_meta_bits & JX_META_BIT_INT) {
    I->data.int_vla = *ref;
    return JX_SUCCESS;
  }
  return JX_FAILURE;
}

jx_float *jx_list_as_float_vla(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_LIST) {
    jx_list *list = ob.data.io.list;
    if(list->packed_meta_bits & JX_META_BIT_FLOAT) {
      return list->data.float_vla;
    } else if(jx__list_repack_data(list)) {
      if(list->packed_meta_bits & JX_META_BIT_FLOAT) {
        return list->data.float_vla;
      } else if(!list->data.vla) {
        list->packed_meta_bits = JX_META_BIT_FLOAT;
        list->data.float_vla = (jx_float *) jx_vla_new(sizeof(float), 0);
        return list->data.float_vla;
      }
    }
  }
  return NULL;
}

jx_status jx__list_set_float_vla(jx_list * I, jx_float ** ref)
{
  if(I->packed_meta_bits & JX_META_BIT_FLOAT) {
    I->data.float_vla = *ref;
    return JX_SUCCESS;
  }
  return JX_FAILURE;
}

/* hashing */

JX_INLINE jx_uint32 jx__c_str_hash(jx_char * str)
{
  register unsigned char *p = (unsigned char *) str;
  register jx_uint32 x, len = 0;
  register unsigned char c;

  x = *p << 7;
  while((c = *(p++))) {
#if 0
    /* PYTHON (time: G5 = 3.2, P3 = 19.3, P4=11.1) */
    x = (1000003 * x) + c;
#endif
#if 0
    /* aho (G5 = 3.6 sec, P3 = 15.2?!, P4=3.6) */
    x = (x << 6) + (x << 16) - x + c;
#endif
#if 1
    /*  djb2 (G5 = 2.8, P3 = 18.7, P4=2.9) FASTEST OVERALL */
    x = (x << 5) + x + c;
#endif
    len++;
  }
  x ^= len;
  return x ? x : 1;             /* zero is reserved as the hash code of an
                                   unhashable object */
}

jx_uint32 jx__ob_gc_hash_code(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_STR) {  /* right now, we only hash GC strings */
    return jx__c_str_hash(jx_ob_as_str(&ob));
  } else {
    return 0;                   /* unhashable */
  }
}

/* hash objects */

jx_ob jx_hash_new(void)
{
  jx_ob result = JX_OB_HASH;
  if((result.data.io.hash = (jx_hash *) jx_calloc(1, sizeof(jx_hash)))) {
    return result;
  } else {
    return jx_ob_from_null();
  }
}


static jx_bool jx__hash_recondition(jx_hash * I, jx_int mode, jx_bool pack);

jx_ob jx_hash_new_with_flags(jx_int flags)
{
  jx_ob result = jx_hash_new();
  if(result.meta.bits & JX_META_BIT_HASH) {
    if(flags & JX_HASH_FLAG_BIDIRECTIONAL) {
      if(!jx__hash_recondition(result.data.io.hash, JX_HASH_ONE_TO_ONE, JX_TRUE)) {
        jx_ob_free(result);
        result = jx_ob_from_null();
      }
    }
  }
  return result;
}

static jx_ob jx__hash_copy(jx_hash * hash)
{
  jx_ob result = jx_hash_new();
  if(result.meta.bits & JX_META_BIT_HASH) {
    jx_hash *I = result.data.io.hash;
    *I = *hash;
    I->info = jx_vla_copy(&hash->info);
    I->key_value = jx_vla_copy(&hash->key_value);
    {
      jx_int i, size = jx_vla_size(&I->key_value);
      jx_ob *ob = I->key_value;
      for(i = 0; i < size; i++) {
        if(ob->meta.bits & JX_META_BIT_GC) {
          *ob = jx_ob_copy(*ob);
        }
        ob++;
      }
    }
  }
  return result;
}

static jx_status jx__hash_free(jx_hash * I)
{
  jx_int size = jx_vla_size(&I->key_value);
  if(size) {
    jx_ob *ob = I->key_value;
    jx_int i;
    for(i = 0; i < size; i++) {
      jx_ob_free(*(ob++));
    }
  }
  jx_vla_free(&I->key_value);
  jx_vla_free(&I->info);
  jx_free(I);
  return JX_SUCCESS;
}

jx_int jx__hash_size(jx_hash * I)
{
  jx_int result = 0;
  if(I->key_value) {
    if(!I->info) {              /* no info mode -- search & match objects directly  */
      result = jx_vla_size(&I->key_value) >> 1;
    } else {
      jx_hash_info *info = (jx_hash_info *) I->info;
      result = info->usage;
    }
  }
  return result;
}

JX_INLINE jx_uint32 jx__new_mask_from_min_size(jx_uint32 min_size)
{
  jx_uint32 new_mask = 0;
  jx_uint32 tmp_usage = ((3 * min_size) >> 1);
  while(tmp_usage) {
    new_mask = (new_mask << 1) + 1;
    tmp_usage = (tmp_usage >> 1);
  }
  while(new_mask < min_size) {
    new_mask = (new_mask << 1);
  }
  return new_mask;
}

static jx_bool jx__hash_recondition(jx_hash * I, jx_int mode, jx_bool pack)
{
  /* note that on recondition we assume that there are no two
     identical keys present in the hash */

  jx_bool result = JX_TRUE;
  jx_uint32 usage = 0;
  jx_uint32 old_mode = JX_HASH_RAW;
  jx_uint32 min_size = 0;

  if(!I->info) {
    usage = jx_vla_size(&I->key_value) >> 1;
    if(min_size < usage)
      min_size = usage;
    if(mode > JX_HASH_LINEAR) {
      I->info = jx_vla_new(sizeof(jx_uint32), (2 * min_size) + JX_HASH_INFO_SIZE);
      if(I->info) {
        jx_hash_info *info = (jx_hash_info *) I->info;
        info->mode = mode;
      }
    }
  } else {
    jx_hash_info *info = (jx_hash_info *) I->info;
    old_mode = info->mode;
    usage = info->usage;
    if(min_size < usage)
      min_size = usage;
  }

  switch (mode) {
  case JX_HASH_ONE_TO_ANY:
  case JX_HASH_ONE_TO_ONE:
  case JX_HASH_ONE_TO_NIL:
    min_size = usage + 1;       /* always make sure there is space for a new entry */
    break;
  }

  if(!I->key_value) {
    if(min_size) {
      I->key_value = jx_vla_new(sizeof(jx_ob), 0);
    }
  }
  if(!min_size) {
    /* empty table stays empty */
    if(I->key_value) {
      jx_vla_resize(&I->key_value, 0);
    }
    /* free info field unless we've selected non-default behaviors */
    switch (mode) {
    case JX_HASH_ONE_TO_ONE:
    case JX_HASH_ONE_TO_NIL:
      break;
    default:
      jx_vla_free(&I->info);
      break;
    }
  } else {
    switch (mode) {
    case JX_HASH_RAW:          /* to */
      switch (old_mode) {
      case JX_HASH_LINEAR:     /* from */
        if(I->key_value) {
          jx_vla_resize(&I->key_value, (usage << 1));
        }
        jx_vla_free(&I->info);
        break;
      case JX_HASH_ONE_TO_ANY: /* from */
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_hash_info *old_info = (jx_hash_info *) I->info;
          jx_ob *new_key_value = jx_vla_new(sizeof(jx_ob), (usage << 1));
          if(new_key_value) {
            jx_ob *old_key_value = I->key_value, *new_kv = new_key_value;
            jx_uint32 *old_hash_entry = old_info->table;
            if(old_info->mask) {
              jx_uint32 i = old_info->mask + 1;
              while(i--) {
                if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* only process actives */
                  jx_uint32 old_kv_offset =
                    old_hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                  /* copy object into new key_value table */
                  *(new_kv++) = old_key_value[old_kv_offset];
                  *(new_kv++) = old_key_value[old_kv_offset + 1];       /* for ONE_TO_NIL, these will be null */
                }
                old_hash_entry += 2;
              }
            }
            jx_vla_free(&I->key_value);
            I->key_value = new_key_value;
            jx_vla_free(&I->info);
          }
        }
        break;
      }
      break;
    case JX_HASH_LINEAR:       /* to */
      switch (old_mode) {
      case JX_HASH_RAW:        /* from */
        {
          I->info = jx_vla_new(sizeof(jx_uint32), usage + JX_HASH_INFO_SIZE);
          if(I->info) {
            jx_hash_info *info = (jx_hash_info *) I->info;
            jx_uint32 i = usage;
            jx_ob *ob = I->key_value;
            jx_uint32 *hash_entry = info->table;
            while(i--) {
              *(hash_entry) = jx_ob_hash_code(ob[0]);
              hash_entry++;
              ob += 2;
            }
            info->mode = mode;
            info->usage = usage;
          }
        }
        break;
      case JX_HASH_LINEAR:     /* from */
        /* do nuttin' */
        break;
      case JX_HASH_ONE_TO_ANY: /* from */
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_hash_info *old_info = (jx_hash_info *) I->info;
          jx_ob *new_key_value = jx_vla_new(sizeof(jx_ob), (usage << 1));
          jx_hash_info *new_info =
            (jx_hash_info *) jx_vla_new(sizeof(jx_uint32), usage + JX_HASH_INFO_SIZE);
          if(new_key_value && new_info) {
            jx_ob *old_key_value = I->key_value, *new_kv = new_key_value;
            jx_uint32 *old_hash_entry = old_info->table;
            jx_uint32 *new_hash_entry = new_info->table;
            if(old_info->mask) {
              jx_uint32 i = old_info->mask + 1;
              jx_uint32 new_kv_offset = 0;
              new_info->mode = mode;
              new_info->usage = usage;
              while(i--) {
                if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* only process actives */
                  jx_uint32 old_kv_offset =
                    old_hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                  /* copy object into new key_value table */
                  *(new_kv++) = old_key_value[old_kv_offset];
                  *(new_kv++) = old_key_value[old_kv_offset + 1];
                  *(new_hash_entry++) = old_hash_entry[0];
                  new_kv_offset += 2;
                }
                old_hash_entry += 2;
              }
            }
            jx_vla_free(&I->key_value);
            I->key_value = new_key_value;
            jx_vla_free(&I->info);
            I->info = (jx_uint32 *) new_info;
          } else {
            jx_vla_free(&new_key_value);
            jx_vla_free(&new_info);
          }
        }
        break;
      }
      break;
    case JX_HASH_ONE_TO_ANY:   /* to */
    case JX_HASH_ONE_TO_NIL:
      {
        jx_uint32 new_mask = jx__new_mask_from_min_size(min_size);
        {
          /* prepare new info block */
          jx_hash_info *new_info = (jx_hash_info *) jx_vla_new(sizeof(jx_uint32),
                                                               2 * (new_mask + 1) +
                                                               JX_HASH_INFO_SIZE);
          if(new_info) {
            new_info->mode = mode;
            new_info->usage = usage;
            new_info->mask = new_mask;
            switch (old_mode) {
            case JX_HASH_RAW:  /* from */
              {
                jx_ob *ob = I->key_value;
                jx_uint32 *hash_table = new_info->table;
                jx_uint32 kv_offset = 0;
                jx_uint32 size = (usage << 1);
                while(kv_offset < size) {
                  jx_uint32 hash_code = jx_ob_hash_code(ob[0]);
                  jx_uint32 index = new_mask & hash_code;
                  jx_uint32 sentinel = index;
                  do {
                    jx_uint32 *hash_entry = hash_table + (index << 1);
                    if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                      hash_entry[0] = hash_code;
                      hash_entry[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                      break;
                    }
                    index = (index + 1) & new_mask;
                  } while(index != sentinel);
                  kv_offset += 2;
                  ob += 2;
                }
                jx_vla_free(&I->info);
                I->info = (jx_uint32 *) new_info;
                new_info = NULL;
              }
              break;
            case JX_HASH_LINEAR:       /* from */
              {
                jx_hash_info *old_info = (jx_hash_info *) I->info;
                jx_uint32 *old_hash_entry = old_info->table;
                jx_uint32 *hash_table = new_info->table;
                jx_uint32 kv_offset = 0;
                jx_uint32 size = (usage << 1);
                while(kv_offset < size) {
                  jx_uint32 hash_code = *(old_hash_entry++);
                  jx_uint32 index = new_mask & hash_code;
                  jx_uint32 sentinel = index;
                  do {
                    jx_uint32 *hash_entry = hash_table + (index << 1);
                    if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                      hash_entry[0] = hash_code;
                      hash_entry[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                      break;
                    }
                    index = (index + 1) & new_mask;
                  } while(index != sentinel);
                  kv_offset += 2;
                }
                jx_vla_free(&I->info);
                I->info = (jx_uint32 *) new_info;
                new_info = NULL;
              }
              break;
            case JX_HASH_ONE_TO_ANY:   /* from */
            case JX_HASH_ONE_TO_ONE:
            case JX_HASH_ONE_TO_NIL:
              {
                jx_hash_info *old_info = (jx_hash_info *) I->info;
                if(pack || (new_mask < old_info->mask)) {
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value = jx_vla_new(sizeof(jx_ob), (usage << 1));
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash_entry = old_info->table;
                    jx_uint32 *hash_table = new_info->table;
                    jx_uint32 i = old_info->mask + 1;
                    jx_uint32 new_kv_offset = 0;
                    while(i--) {
                      if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {    /* only process actives */
                        jx_uint32 hash_code = old_hash_entry[0];
                        jx_uint32 index = new_mask & hash_code;
                        jx_uint32 old_kv_offset =
                          old_hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                        jx_uint32 sentinel = index;
                        /* copy object into new key_value table */
                        new_key_value[new_kv_offset] = old_key_value[old_kv_offset];
                        new_key_value[new_kv_offset + 1] =
                          old_key_value[old_kv_offset + 1];
                        do {
                          jx_uint32 *hash_entry = hash_table + (index << 1);
                          if(!hash_entry[1]) {
                            hash_entry[0] = hash_code;
                            hash_entry[1] = (new_kv_offset | JX_HASH_ENTRY_ACTIVE);
                            break;
                          }
                          index = (index + 1) & new_mask;
                        } while(index != sentinel);
                        new_kv_offset += 2;
                      }
                      old_hash_entry += 2;
                    }
                    jx_vla_free(&I->key_value);
                    I->key_value = new_key_value;
                    jx_vla_free(&I->info);
                    I->info = (jx_uint32 *) new_info;
                    new_info = NULL;
                  }
                } else if(new_mask > old_info->mask) {
                  /* we copy deleted entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash_entry = old_info->table;
                  jx_uint32 *hash_table = new_info->table;
                  new_info->stale_usage = old_info->stale_usage;
                  if(old_info->mask) {
                    jx_uint32 i = old_info->mask + 1;
                    while(i--) {
                      if(old_hash_entry[1]) {   /* only skip virgin entries */
                        jx_uint32 hash_code = old_hash_entry[0];
                        jx_uint32 index = new_mask & hash_code;
                        jx_uint32 sentinel = index;
                        do {
                          jx_uint32 *hash_entry = hash_table + (index << 1);
                          if(!hash_entry[1]) {
                            hash_entry[0] = hash_code;
                            hash_entry[1] = old_hash_entry[1];
                            break;
                          }
                          index = (index + 1) & new_mask;
                        } while(index != sentinel);
                      }
                      old_hash_entry += 2;
                    }
                  }
                  jx_vla_free(&I->info);
                  I->info = (jx_uint32 *) new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            }
            /* if something went wrong, then we simply free new new info and keep the old */
            jx_vla_free(&new_info);
          }
        }
      }
      break;
    case JX_HASH_ONE_TO_ONE:   /* to */
      {
        jx_uint32 new_mask = jx__new_mask_from_min_size(min_size);
        {
          /* prepare new info block */
          jx_hash_info *new_info =
            jx_vla_new(sizeof(jx_uint32), 4 * (new_mask + 1) + JX_HASH_INFO_SIZE);
          if(new_info) {
            new_info->mode = mode;
            new_info->usage = usage;
            new_info->mask = new_mask;
            switch (old_mode) {
            case JX_HASH_RAW:  /* from */
              {
                jx_ob *ob = I->key_value;
                jx_uint32 *hash_table = new_info->table;
                jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                jx_uint32 hash_code;
                jx_uint32 kv_offset = 0;
                jx_uint32 size = (usage << 1);

                while(kv_offset < size) {
                  /* forward */
                  hash_code = jx_ob_hash_code(ob[0]);
                  {
                    jx_uint32 index = new_mask & hash_code;
                    jx_uint32 sentinel = index;
                    do {
                      jx_uint32 *hash_entry = hash_table + (index << 1);
                      if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                        hash_entry[0] = hash_code;
                        hash_entry[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                        break;
                      }
                      index = (index + 1) & new_mask;
                    } while(index != sentinel);
                  }
                  /* reverse */
                  hash_code = jx_ob_hash_code(ob[1]);
                  if(!hash_code) {      /* value not hashable */
                    result = JX_FALSE;
                    break;
                  } else {
                    jx_uint32 index = new_mask & hash_code;
                    jx_ob *key_value = I->key_value;
                    jx_uint32 sentinel = index;
                    do {
                      jx_uint32 *hash_entry = rev_hash_table + (index << 1);
                      if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                        hash_entry[0] = hash_code;
                        hash_entry[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                        break;
                      } else if((hash_entry[0] == hash_code) &&
                                jx_ob_identical(key_value
                                                [(hash_entry[1] &
                                                  JX_HASH_ENTRY_KV_OFFSET_MASK) + 1],
                                                ob[1])) {
                        result = JX_FALSE;
                        break;
                      }
                      index = (index + 1) & new_mask;
                    } while(index != sentinel);
                  }
                  kv_offset += 2;
                  ob += 2;
                }
                if(result) {
                  jx_vla_free(&I->info);
                  I->info = (jx_uint32 *) new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            case JX_HASH_LINEAR:       /* from */
              {
                jx_hash_info *old_info = (jx_hash_info *) I->info;
                jx_uint32 *old_hash_entry = old_info->table;
                jx_ob *ob = I->key_value;
                jx_uint32 *hash_table = new_info->table;
                jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                jx_uint32 hash_code;
                jx_uint32 kv_offset = 0;
                jx_uint32 size = (usage << 1);
                while(kv_offset < size) {
                  /* forward */
                  hash_code = *(old_hash_entry++);
                  {
                    jx_uint32 index = new_mask & hash_code;
                    jx_uint32 sentinel = index;
                    do {
                      jx_uint32 *hash_entry = hash_table + (index << 1);
                      if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                        hash_entry[0] = hash_code;
                        hash_entry[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                        break;
                      }
                      index = (index + 1) & new_mask;
                    } while(index != sentinel);
                  }
                  /* reverse */
                  hash_code = jx_ob_hash_code(ob[1]);
                  if(!hash_code) {
                    result = JX_FALSE;
                    break;
                  } else {
                    jx_uint32 index = new_mask & hash_code;
                    jx_ob *key_value = I->key_value;
                    jx_uint32 sentinel = index;
                    do {
                      jx_uint32 *hash_entry = rev_hash_table + (index << 1);
                      if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                        hash_entry[0] = hash_code;
                        hash_entry[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                        break;
                      } else if((hash_entry[0] == hash_code) &&
                                jx_ob_identical(key_value
                                                [(hash_entry[1] &
                                                  JX_HASH_ENTRY_KV_OFFSET_MASK) + 1],
                                                ob[1])) {
                        result = JX_FALSE;
                        break;
                      }
                      index = (index + 1) & new_mask;
                    } while(index != sentinel);
                  }
                  kv_offset += 2;
                  ob += 2;
                }
                if(result) {
                  jx_vla_free(&I->info);
                  I->info = (jx_uint32 *) new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            case JX_HASH_ONE_TO_ANY:   /* from */
            case JX_HASH_ONE_TO_NIL:
              {
                jx_hash_info *old_info = (jx_hash_info *) I->info;
                if(pack || (new_mask < old_info->mask)) {
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value = jx_vla_new(sizeof(jx_ob), (usage << 1));
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash_entry = old_info->table;
                    jx_uint32 *hash_table = new_info->table;
                    jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                    jx_uint32 i = old_info->mask + 1;
                    jx_uint32 new_kv_offset = 0;
                    while(i--) {
                      if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {    /* only process actives */
                        jx_uint32 old_kv_offset =
                          old_hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;

                        {       /* forward */
                          jx_uint32 hash_code = old_hash_entry[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;
                          /* copy object into new key_value table */
                          new_key_value[new_kv_offset] = old_key_value[old_kv_offset];
                          new_key_value[new_kv_offset + 1] =
                            old_key_value[old_kv_offset + 1];
                          do {
                            jx_uint32 *hash_entry = hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = (new_kv_offset | JX_HASH_ENTRY_ACTIVE);
                              break;
                            }
                            index = (index + 1) & new_mask;
                          } while(index != sentinel);
                        }
                        {       /* reverse */
                          jx_uint32 hash_code =
                            jx_ob_hash_code(old_key_value
                                            [(old_hash_entry[1] &
                                              JX_HASH_ENTRY_KV_OFFSET_MASK) + 1]);
                          if(!hash_code) {
                            result = JX_FALSE;
                            break;
                          } else {
                            jx_uint32 index = new_mask & hash_code;
                            jx_uint32 sentinel = index;
                            do {
                              jx_uint32 *hash_entry = rev_hash_table + (index << 1);
                              if(!hash_entry[1]) {
                                hash_entry[0] = hash_code;
                                hash_entry[1] = (new_kv_offset | JX_HASH_ENTRY_ACTIVE);
                                break;
                              } else if((hash_entry[0] == hash_code) &&
                                        jx_ob_identical(new_key_value[(hash_entry[1]
                                                                       &&
                                                                       JX_HASH_ENTRY_KV_OFFSET_MASK)
                                                                      + 1],
                                                        new_key_value[new_kv_offset +
                                                                      1])) {
                                result = JX_FALSE;
                                break;
                              }
                              index = (index + 1) & new_mask;
                            } while(index != sentinel);
                          }
                        }
                        new_kv_offset += 2;
                      }
                      old_hash_entry += 2;
                    }
                    if(result) {
                      jx_vla_free(&I->key_value);
                      I->key_value = new_key_value;
                      new_key_value = JX_NULL;
                      jx_vla_free(&I->info);
                      I->info = (jx_uint32 *) new_info;
                      new_info = JX_NULL;
                    }
                    jx_vla_free(&new_key_value);
                  }
                } else if(new_mask > old_info->mask) {
                  /* we copy stale entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash_entry = old_info->table;
                  jx_uint32 *hash_table = new_info->table;
                  jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                  jx_ob *key_value = I->key_value;
                  new_info->stale_usage = old_info->stale_usage;
                  if(old_info->mask) {
                    jx_uint32 i = old_info->mask + 1;
                    while(i--) {
                      if(old_hash_entry[1]) {   /* only skip virgin entries */
                        {       /* forward */
                          jx_uint32 hash_code = old_hash_entry[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;
                          do {
                            jx_uint32 *hash_entry = hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = old_hash_entry[1];
                              break;
                            }
                            index = (index + 1) & new_mask;
                          } while(index != sentinel);
                        }
                        if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* reverse for actives only */
                          jx_uint32 hash_code =
                            jx_ob_hash_code(key_value
                                            [(old_hash_entry[1] &
                                              JX_HASH_ENTRY_KV_OFFSET_MASK) + 1]);
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;
                          do {
                            jx_uint32 *hash_entry = rev_hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = old_hash_entry[1];
                              break;
                            }
                          } while(index != sentinel);
                        }
                      }
                      old_hash_entry += 2;
                    }
                  }
                  if(result) {
                    jx_vla_free(&I->info);
                    I->info = (jx_uint32 *) new_info;
                    new_info = JX_NULL;
                  }
                }
              }
              break;
            case JX_HASH_ONE_TO_ONE:   /* from */
              {
                jx_hash_info *old_info = (jx_hash_info *) I->info;
                if(pack || (new_mask < old_info->mask)) {
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value = jx_vla_new(sizeof(jx_ob), (usage << 1));
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash_entry = old_info->table;
                    jx_uint32 *hash_table = new_info->table;
                    jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                    jx_uint32 i = old_info->mask + 1;
                    jx_uint32 new_kv_offset = 0;
                    while(i--) {
                      if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {    /* only process actives */
                        jx_uint32 old_kv_offset =
                          old_hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                        /* forward */
                        {
                          jx_uint32 hash_code = old_hash_entry[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;

                          /* copy object into new key_value table */
                          new_key_value[new_kv_offset] = old_key_value[old_kv_offset];
                          new_key_value[new_kv_offset + 1] =
                            old_key_value[old_kv_offset + 1];
                          do {
                            jx_uint32 *hash_entry = hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = (new_kv_offset | JX_HASH_ENTRY_ACTIVE);
                              break;
                            }
                            index = (index + 1) & new_mask;
                          } while(index != sentinel);
                        }
                        /* reverse */
                        {
                          jx_uint32 hash_code =
                            jx_ob_hash_code(old_key_value[old_kv_offset + 1]);
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;
                          do {
                            jx_uint32 *hash_entry = rev_hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = (new_kv_offset | JX_HASH_ENTRY_ACTIVE);
                              break;
                            }
                            index = (index + 1) & new_mask;
                          } while(index != sentinel);
                        }
                        new_kv_offset += 2;
                      }
                      old_hash_entry += 2;
                    }
                    jx_vla_free(&I->key_value);
                    I->key_value = new_key_value;
                    jx_vla_free(&I->info);
                    I->info = (jx_uint32 *) new_info;
                    new_info = JX_NULL;
                  }
                } else if(new_mask > old_info->mask) {
                  /* we copy stale entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash_entry = old_info->table;
                  jx_uint32 *hash_table = new_info->table;
                  jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                  jx_ob *key_value = I->key_value;
                  new_info->stale_usage = old_info->stale_usage;
                  if(old_info->mask) {
                    jx_uint32 i = old_info->mask + 1;
                    while(i--) {
                      if(old_hash_entry[1]) {   /* only skip virgin entries */

                        {       /* forward */
                          jx_uint32 hash_code = old_hash_entry[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;
                          do {
                            jx_uint32 *hash_entry = hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = old_hash_entry[1];
                              break;
                            }
                            index = (index + 1) & new_mask;
                          } while(index += sentinel);
                        }

                        if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* reverse for actives only */
                          jx_uint32 hash_code =
                            jx_ob_hash_code(key_value
                                            [(old_hash_entry[1] &
                                              JX_HASH_ENTRY_KV_OFFSET_MASK) + 1]);
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 sentinel = index;
                          do {
                            jx_uint32 *hash_entry = rev_hash_table + (index << 1);
                            if(!hash_entry[1]) {
                              hash_entry[0] = hash_code;
                              hash_entry[1] = old_hash_entry[1];
                              break;
                            }
                            index = (index + 1) & new_mask;
                          } while(index != sentinel);
                        }
                      }
                      old_hash_entry += 2;
                    }
                  }
                  jx_vla_free(&I->info);
                  I->info = (jx_uint32 *) new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            }
            /* if something went wrong, then we simply free new new info and keep the old */
            jx_vla_free(&new_info);
          }
        }
      }
      break;
    }
    if(I->info && I->key_value) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      jx_uint32 n_ent = jx_vla_size(&I->key_value);
      if(n_ent / 2 != (info->usage + info->stale_usage)) {
        printf("mismatch %d != %d+%d = %d\n",
               (int) n_ent / 2, (int) info->usage, (int) info->stale_usage,
               (int) info->usage + (int) info->stale_usage);
      }
    }
  }
  return result;
}

jx_status jx__hash_set(jx_hash * I, jx_ob key, jx_ob value)
{
  jx_status result = JX_FAILURE;
  jx_uint32 hash_code = jx_ob_hash_code(key);
  if(!hash_code) {              /* unhashable key */
    return JX_FAILURE;
  } else {
    if(!I->info) {              /* "no info" mode -- search & match objects directly  */
      if(!I->key_value) {
        /* new table, first entry  */
        I->key_value = jx_vla_new(sizeof(jx_ob), 2);
        if(I->key_value) {
          I->key_value[0] = JX_OWN(key);        /* takes ownership */
          I->key_value[1] = JX_OWN(value);      /* takes ownership */
          result = JX_SUCCESS;
        }
      } else {
        jx_uint32 size = jx_vla_size(&I->key_value);
        if(1) {
          jx_bool found = JX_FALSE;
          register jx_uint32 i = (size >> 1);
          register jx_ob *ob = I->key_value;
          while(i--) {
            if(jx_ob_identical(ob[0], key)) {
              found = JX_TRUE;
              jx_ob_free(I->key_value[0]);
              jx_ob_free(I->key_value[1]);
              I->key_value[0] = JX_OWN(key);    /* takes ownership */
              I->key_value[1] = JX_OWN(value);  /* takes ownership */
              result = JX_SUCCESS;
              break;
            }
            ob += 2;
          }
          if(!found) {
            if(jx_ok(jx_vla_grow_check(&I->key_value, size + 1))) {
              I->key_value[size] = key;
              I->key_value[size + 1] = value;
              result = JX_SUCCESS;
              if(size > 8) {    /* switch to linear hash once we have more than four elements */
                jx__hash_recondition(I, JX_HASH_LINEAR, JX_FALSE);
              }
            }
          }
        }
      }
    } else {                    /* we have an info record */
      if(!I->key_value) {
        I->key_value = jx_vla_new(sizeof(jx_ob), 0);
      }
      if(I->key_value && I->info) {
        jx_hash_info *info = (jx_hash_info *) I->info;
        switch (info->mode) {
        case JX_HASH_LINEAR:
          {
            register jx_uint32 i = info->usage;
            register jx_uint32 *hash_entry = info->table;
            register jx_ob *ob = I->key_value;

            jx_bool found = JX_FALSE;
            while(i--) {
              if(*hash_entry == hash_code) {
                if(jx_ob_identical(ob[0], key)) {
                  found = JX_TRUE;
                  break;
                }
              }
              hash_entry++;
              ob += 2;
            }
            if(!found) {
              jx_uint32 usage = info->usage;
              if(jx_ok(jx_vla_grow_check(&I->info, usage + JX_HASH_INFO_SIZE)) &&
                 jx_ok(jx_vla_grow_check(&I->key_value, (usage << 1) + 1))) {
                info = (jx_hash_info *) I->info;
                hash_entry = info->table + usage;
                ob = I->key_value + (usage << 1);
                info->usage++;
                *hash_entry = hash_code;
                ob[0] = JX_OWN(key);
                ob[1] = JX_OWN(value);
                result = JX_SUCCESS;
                if(info->usage > 8) {
                  jx__hash_recondition(I, JX_HASH_ONE_TO_ANY, JX_FALSE);        /* switch to true hash */
                }
              }
            } else {
              jx_ob_free(ob[0]);
              jx_ob_free(ob[1]);
              ob[0] = JX_OWN(key);
              ob[1] = JX_OWN(value);
              result = JX_SUCCESS;
            }
          }
          break;
        case JX_HASH_ONE_TO_ANY:
        case JX_HASH_ONE_TO_NIL:
          if((info->usage + 1) > (3 * info->mask) >> 2) {       /* more than ~3/4'rs full */
            jx__hash_recondition(I, info->mode, JX_FALSE);
            info = (jx_hash_info *) I->info;
          }
          {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash_table = info->table;
            jx_uint32 usage = info->usage;
            jx_ob *key_value = I->key_value;
            jx_bool found = JX_FALSE;
            jx_uint32 *dest_ptr = JX_NULL;
            jx_bool dest_virgin = JX_FALSE;
            jx_uint32 index = mask & hash_code;
            jx_uint32 sentinel = index;
            do {
              jx_uint32 *hash_entry = hash_table + (index << 1);
              if(!hash_entry[1]) {
                /* virgin slot terminates probe... */
                if(!dest_ptr) {
                  dest_ptr = hash_entry;
                  dest_virgin = JX_TRUE;
                }
                break;
              } else if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                /* deleted slot -- save for future insertion */
                if(!dest_ptr)
                  dest_ptr = hash_entry;
              } else if((hash_entry[0] == hash_code) &&
                        jx_ob_identical(key_value
                                        [hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK],
                                        key)) {
                /* matched key, so we must replace */
                dest_ptr = hash_entry;
                found = JX_TRUE;
                break;
              }
              index = (index + 1) & mask;
            } while(index != sentinel);
            if(dest_ptr) {
              if(!found) {
                jx_uint32 kv_offset;
                if(dest_virgin) {
                  kv_offset = ((info->stale_usage + usage) << 1);       /* allocate new key_value */
                } else {
                  kv_offset = (dest_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);     /* use existing key_value */
                }
                if(jx_ok(jx_vla_grow_check(&I->key_value, kv_offset + 1))) {
                  key_value = I->key_value;
                  if(!dest_virgin)
                    info->stale_usage--;
                  dest_ptr[0] = hash_code;
                  dest_ptr[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                  key_value[kv_offset] = JX_OWN(key);
                  key_value[kv_offset + 1] = JX_OWN(value);
                  result = JX_SUCCESS;
                  info->usage++;
                } else {
                  result = JX_FAILURE;
                }
              } else {
                jx_uint32 kv_offset = (dest_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                jx_ob_free(key_value[kv_offset]);
                jx_ob_free(key_value[kv_offset + 1]);
                key_value[kv_offset] = JX_OWN(key);
                key_value[kv_offset + 1] = JX_OWN(value);
                result = JX_SUCCESS;
              }
            }
          }
          break;
        case JX_HASH_ONE_TO_ONE:
          {
            jx_uint32 reverse_hash_code = jx_ob_hash_code(value);
            if(!reverse_hash_code) {
              result = JX_FAILURE;
            } else {
              if((info->usage + 1) > (3 * info->mask) >> 2) {   /* more than ~3/4'rs full */
                jx__hash_recondition(I, info->mode, JX_FALSE);
                info = (jx_hash_info *) I->info;
              }
              {
                jx_uint32 mask = info->mask;
                jx_uint32 usage = info->usage;
                jx_ob *key_value = I->key_value;
                jx_uint32 *dest_forward_ptr = JX_NULL;
                jx_uint32 *dest_reverse_ptr = JX_NULL;
                jx_bool forward_found = JX_FALSE;
                jx_bool reverse_found = JX_FALSE;
                jx_bool virgin_forward = JX_FALSE;
                jx_bool virgin_reverse = JX_FALSE;
                {
                  jx_uint32 *hash_table = info->table;
                  jx_uint32 index = mask & hash_code;
                  jx_uint32 sentinel = index;
                  do {
                    jx_uint32 *hash_entry = hash_table + (index << 1);
                    if(!hash_entry[1]) {
                      if(!dest_forward_ptr) {
                        dest_forward_ptr = hash_entry;
                        virgin_forward = JX_TRUE;
                      }
                      break;
                    } else if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                      if(!dest_forward_ptr)
                        dest_forward_ptr = hash_entry;
                    } else if((hash_entry[0] == hash_code) &&
                              jx_ob_identical(key_value
                                              [hash_entry[1] &
                                               JX_HASH_ENTRY_KV_OFFSET_MASK], key)) {
                      forward_found = JX_TRUE;
                      break;
                    }
                    index = (index + 1) & mask;
                  } while(index != sentinel);
                }
                {
                  jx_uint32 *hash_table = info->table + ((mask + 1) << 1);
                  jx_uint32 index = mask & reverse_hash_code;
                  jx_uint32 sentinel = index;
                  do {
                    jx_uint32 *hash_entry = hash_table + (index << 1);
                    if(!hash_entry[1]) {
                      if(!dest_reverse_ptr) {
                        dest_reverse_ptr = hash_entry;
                        virgin_reverse = JX_TRUE;
                      }
                      break;
                    } else if(!(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
                      if(!dest_reverse_ptr)
                        dest_reverse_ptr = hash_entry;
                    } else if((hash_entry[0] == reverse_hash_code) &&
                              jx_ob_identical(key_value
                                              [(hash_entry[1] &
                                                JX_HASH_ENTRY_KV_OFFSET_MASK) + 1],
                                              value)) {
                      reverse_found = JX_TRUE;
                      break;
                    }
                    index = (index + 1) & mask;
                  } while(index != sentinel);
                }
                if(!(forward_found || reverse_found)) {
                  jx_uint32 kv_offset;
                  if(virgin_forward && virgin_reverse) {
                    kv_offset = ((info->stale_usage + usage) << 1);
                  } else if(!virgin_forward) {
                    kv_offset = (dest_forward_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                  } else {
                    kv_offset = (dest_reverse_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                  }
                  if(jx_ok(jx_vla_grow_check(&I->key_value, kv_offset + 1))) {
                    key_value = I->key_value;

                    if(!(virgin_forward || virgin_reverse))
                      info->stale_usage--;

                    dest_forward_ptr[0] = hash_code;
                    dest_forward_ptr[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);

                    dest_reverse_ptr[0] = reverse_hash_code;
                    dest_reverse_ptr[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);

                    key_value[kv_offset] = JX_OWN(key);
                    key_value[kv_offset + 1] = JX_OWN(value);

                    result = JX_SUCCESS;
                    info->usage++;
                  }
                }
              }
            }
          }
          break;
        }
      }
    }
  }
  return result;
}

jx_ob jx__hash_copy_members(jx_hash * I, jx_int flags)
{
  jx_ob result = jx_list_new();
  jx_uint32 size = jx_vla_size(&I->key_value);
  if(size) {
    jx_hash_info *info = (jx_hash_info *) I->info;
    if((!info) || (info->mode == JX_HASH_LINEAR)) {
      register jx_int i = (info ? info->usage : (size >> 1));
      register jx_ob *ob = I->key_value;
      while(i--) {
        if(flags & JX__HASH_COPY_KEYS)
          jx_list_append(result, jx_ob_copy(ob[0]));
        if(flags & JX__HASH_COPY_VALUES)
          jx_list_append(result, jx_ob_copy(ob[1]));
        ob += 2;
      }
    } else {
      switch (info->mode) {
      case JX_HASH_ONE_TO_ANY:
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_uint32 mask = info->mask;
          jx_uint32 *hash_table = info->table;
          jx_ob *key_value = I->key_value;
          jx_uint32 index = 0;
          do {
            jx_uint32 *hash_entry = hash_table + (index << 1);
            if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* active slot with matching hash code */
              jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
              if(flags & JX__HASH_COPY_KEYS)
                jx_list_append(result, jx_ob_copy(kv_ob[0]));
              if(flags & JX__HASH_COPY_VALUES)
                jx_list_append(result, jx_ob_copy(kv_ob[1]));
            }
            index++;
          } while(index <= mask);
        }
        break;
      }
    }
  }
  return result;
}

jx_status jx__hash_set_read_only(jx_hash * I, jx_bool read_only)
{
  jx_uint32 size = jx_vla_size(&I->key_value);
  if(size) {
    jx_hash_info *info = (jx_hash_info *) I->info;
    if((!info) || (info->mode == JX_HASH_LINEAR)) {
      register jx_int i = (info ? info->usage : (size >> 1));
      register jx_ob *kv_ob = I->key_value;
      if(read_only) {
        while(i--) {
          if(kv_ob[0].meta.bits & JX_META_BIT_GC)
            kv_ob[0].meta.bits |= JX_META_BIT_READ_ONLY;
          if(kv_ob[1].meta.bits & JX_META_BIT_GC)
            kv_ob[1].meta.bits |= JX_META_BIT_READ_ONLY;
          kv_ob += 2;
        }
      } else {
        while(i--) {
          if(kv_ob[0].meta.bits & JX_META_BIT_GC)
            kv_ob[0].meta.bits &= JX_META_BIT_READ_ONLY;
          if(kv_ob[1].meta.bits & JX_META_BIT_GC)
            kv_ob[1].meta.bits &= JX_META_BIT_READ_ONLY;
          kv_ob += 2;
        }
      }
    } else {
      switch (info->mode) {
      case JX_HASH_ONE_TO_ANY:
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_uint32 mask = info->mask;
          jx_uint32 *hash_table = info->table;
          jx_ob *key_value = I->key_value;
          jx_uint32 index = 0;
          do {
            jx_uint32 *hash_entry = hash_table + (index << 1);
            if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* active slot with matching hash code */
              jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
              if(read_only) {
                if(kv_ob[0].meta.bits & JX_META_BIT_GC)
                  kv_ob[0].meta.bits |= JX_META_BIT_READ_ONLY;
                if(kv_ob[1].meta.bits & JX_META_BIT_GC)
                  kv_ob[1].meta.bits |= JX_META_BIT_READ_ONLY;
              } else {
                if(kv_ob[0].meta.bits & JX_META_BIT_GC)
                  kv_ob[0].meta.bits &= JX_META_BIT_READ_ONLY;
                if(kv_ob[1].meta.bits & JX_META_BIT_GC)
                  kv_ob[1].meta.bits &= JX_META_BIT_READ_ONLY;
              }
            }
            index++;
          } while(index <= mask);
        }
        break;
      }
    }
  }
  return JX_SUCCESS;
}

jx_bool jx__hash_has_key(jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_uint32 size = jx_vla_size(&I->key_value);
  if(size) {
    if(I->key_value) {
      if(!I->info) {            /* JX_HASH_RAW */
        register jx_uint32 i = (size >> 1);
        jx_ob *ob = I->key_value;
        while(i--) {
          if(jx_ob_identical(ob[0], key)) {
            found = JX_TRUE;
            break;
          }
          ob += 2;
        }
      } else {
        register jx_uint32 hash_code = jx_ob_hash_code(key);
        if(hash_code) {
          jx_hash_info *info = (jx_hash_info *) I->info;
          switch (info->mode) {
          case JX_HASH_LINEAR:
            {
              register jx_uint32 i = info->usage;
              register jx_uint32 *hash_entry = info->table;
              register jx_ob *ob = I->key_value;
              while(i--) {
                if(*hash_entry == hash_code) {
                  if(jx_ob_identical(ob[0], key)) {
                    found = JX_TRUE;
                    break;
                  }
                }
                hash_entry++;
                ob += 2;
              }
            }
            break;
          case JX_HASH_ONE_TO_ANY:
          case JX_HASH_ONE_TO_ONE:
          case JX_HASH_ONE_TO_NIL:
            {
              jx_uint32 mask = info->mask;
              jx_uint32 *hash_table = info->table;
              jx_ob *key_value = I->key_value;
              jx_uint32 index = mask & hash_code;
              jx_uint32 sentinel = index;
              do {
                jx_uint32 *hash_entry = hash_table + (index << 1);
                if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE) && (hash_entry[0] == hash_code)) {
                  /* active slot with matching hash code */
                  jx_ob *kv_ob =
                    key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                  if(jx_ob_identical(*kv_ob, key)) {
                    found = JX_TRUE;
                    break;
                  }
                }
                index = (index + 1) & mask;
              } while(index != sentinel);
            }
            break;
          }
        }
      }
    }
  }
  return found;
}

jx_bool jx__hash_borrow(jx_ob * result, jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_uint32 size = jx_vla_size(&I->key_value);
  if(size) {
    if(!I->info) {              /* JX_HASH_RAW */
      register jx_int i = (size >> 1);
      register jx_ob *ob = I->key_value;
      while(i--) {
        if(jx_ob_identical(ob[0], key)) {
          found = JX_TRUE;
          *result = JX_BORROW( ob[1]; );
          break;
        }
        ob += 2;
      }
    } else {
      register jx_uint32 hash_code = jx_ob_hash_code(key);
      if(hash_code) {
        jx_hash_info *info = (jx_hash_info *) I->info;
        switch (info->mode) {
        case JX_HASH_LINEAR:
          {
            register jx_uint32 i = info->usage;
            register jx_uint32 *hash_entry = info->table;
            register jx_ob *ob = I->key_value;
            while(i--) {
              if(*hash_entry == hash_code) {
                if(jx_ob_identical(ob[0], key)) {
                  found = JX_TRUE;
                  *result = JX_BORROW( ob[1]; );
                  break;
                }
              }
              hash_entry++;
              ob += 2;
            }
          }
          break;
        case JX_HASH_ONE_TO_ANY:
        case JX_HASH_ONE_TO_ONE:
        case JX_HASH_ONE_TO_NIL:
          {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash_table = info->table;
            jx_ob *key_value = I->key_value;
            jx_uint32 index = mask & hash_code;
            jx_uint32 sentinel = index;
            do {
              jx_uint32 *hash_entry = hash_table + (index << 1);
              if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE) && (hash_entry[0] == hash_code)) {
                jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                /* active slot with matching hash code */
                if(jx_ob_identical(kv_ob[0], key)) {
                  *result = JX_BORROW( kv_ob[1]; );
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index + 1) & mask;
            } while(index != sentinel);
          }
          break;
        }
      }
    }
  }
  return found;
}

jx_bool jx__hash_remove(jx_ob * result, jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_uint32 size = jx_vla_size(&I->key_value);
  if(size) {
    if(!I->info) {              /* JX_HASH_RAW */
      register jx_int i = (size >> 1);
      jx_ob *ob = I->key_value;
      while(i--) {
        if(jx_ob_identical(ob[0], key)) {
          found = JX_TRUE;
          jx_ob_free(ob[0]);    /* key */
          *result = ob[1];      /* value ownership returned */
          if(size > 2) {
            ob[0] = I->key_value[size - 2];
            ob[1] = I->key_value[size - 1];
          }
          if(!jx_ok(jx_vla_resize(&I->key_value, size - 2))) {
            memset(ob + size - 2, 0, sizeof(jx_ob) * 2);
          }
          break;
        }
        ob += 2;
      }
    } else {
      register jx_uint32 hash_code = jx_ob_hash_code(key);
      if(hash_code) {

        jx_hash_info *info = (jx_hash_info *) I->info;
        switch (info->mode) {
        case JX_HASH_LINEAR:
          {
            register jx_uint32 i = info->usage;
            register jx_uint32 *hash_entry = info->table;
            register jx_ob *ob = I->key_value;
            while(i--) {
              if(*hash_entry == hash_code) {
                if(jx_ob_identical(ob[0], key)) {
                  jx_uint32 size = ((info->usage - 1) << 1);
                  found = JX_TRUE;
                  jx_ob_free(ob[0]);
                  *result = ob[1];      /* value ownership returned */
                  if(size) {
                    ob[0] = I->key_value[size];
                    ob[1] = I->key_value[size + 1];
                    *hash_entry = info->table[info->usage - 1];
                  }
                  if(!jx_ok(jx_vla_resize(&I->key_value, size))) {
                    memset(ob + size, 0, sizeof(jx_ob) * 2);
                  }
                  info->usage--;
                  break;
                }
              }
              hash_entry++;
              ob += 2;
            }
          }
          break;
        case JX_HASH_ONE_TO_ANY:
        case JX_HASH_ONE_TO_NIL:
          {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash_table = info->table;
            jx_ob *key_value = I->key_value;
            jx_uint32 index = mask & hash_code;
            jx_uint32 sentinel = index;
            do {
              jx_uint32 *hash_entry = hash_table + (index << 1);
              if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE) && (hash_entry[0] == hash_code)) {
                /* active slot with matching hash code */
                jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                if(jx_ob_identical(key_value[kv_offset], key)) {
                  jx_ob *ob = key_value + kv_offset;
                  hash_entry[1] = (kv_offset | JX_HASH_ENTRY_DELETED);
                  jx_ob_free(ob[0]);
                  *result = ob[1];
                  memset(ob, 0, sizeof(jx_ob) * 2);
                  info->usage--;
                  info->stale_usage++;
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index + 1) & mask;
            } while(index != sentinel);

            {
              jx_uint32 usage = info->usage;
              if(!usage) {
                jx__hash_recondition(I, JX_HASH_RAW, JX_TRUE);  /* purge empty hash table */
              } else if(found && (info->stale_usage > usage) &&
                        (usage + info->stale_usage) > (info->mask) >> 1) {
                jx__hash_recondition(I, info->mode, JX_TRUE);   /* pack & (possibly) shrink */
              }
            }
          }
          break;
        case JX_HASH_ONE_TO_ONE:
          {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash_table = info->table;
            jx_ob *key_value = I->key_value;
            jx_uint32 index = mask & hash_code;
            jx_uint32 sentinel = index;
            do {
              jx_uint32 *hash_entry = hash_table + (index << 1);
              if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE) && (hash_entry[0] == hash_code)) {
                jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                if(jx_ob_identical(key_value[kv_offset], key)) {
                  jx_ob *ob = key_value + kv_offset;
                  hash_entry[1] = (kv_offset | JX_HASH_ENTRY_DELETED);
                  {
                    /* reverse */
                    jx_uint32 rev_hash_code = jx_ob_hash_code(ob[1]);
                    jx_uint32 *rev_hash_table = info->table + ((mask + 1) << 1);
                    jx_uint32 rev_index = mask & rev_hash_code;
                    jx_uint32 rev_sentinel = rev_index;
                    do {
                      jx_uint32 *rev_hash_entry = rev_hash_table + (rev_index << 1);
                      if((rev_hash_entry[1] & JX_HASH_ENTRY_ACTIVE)
                         && (rev_hash_entry[0] == hash_code)
                         && jx_ob_identical(key_value[rev_hash_entry[1]], ob[1])) {
                        rev_hash_entry[1] = (kv_offset | JX_HASH_ENTRY_DELETED);
                        break;
                      }
                      rev_index = (rev_index + 1) & mask;
                    } while(rev_index != rev_sentinel);
                  }
                  jx_ob_free(ob[0]);
                  *result = ob[1];
                  memset(ob, 0, sizeof(jx_ob) * 2);
                  info->usage--;
                  info->stale_usage++;
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index + 1) & mask;
            } while(index != sentinel);
            {
              jx_uint32 usage = info->usage;
              if(found && (info->stale_usage > usage) &&
                 ((usage + info->stale_usage) > ((info->mask) >> 1))) {
                jx__hash_recondition(I, info->mode, JX_TRUE);   /* pack & (possibly) shrink */
              }
            }
          }
          break;
        }
      }
    }
  }
  return found;
}

jx_bool jx__hash_borrow_key(jx_ob * result, jx_hash * I, jx_ob value)
{
  jx_bool found = JX_FALSE;
  jx_uint32 size = jx_vla_size(&I->key_value);
  if(size) {
    jx_hash_info *info = (jx_hash_info *) I->info;
    if((!info) || (info->mode == JX_HASH_LINEAR)) {
      register jx_int i = (info ? info->usage : (size >> 1));
      jx_ob *ob = I->key_value;
      while(i--) {              /* brute-force table scan */
        if(jx_ob_identical(ob[1], value)) {
          found = JX_TRUE;
          *result = JX_BORROW( ob[0] );
          break;
        }
        ob += 2;
      }
    } else {                    /* not JX_HASH_RAW */
      switch (info->mode) {
      case JX_HASH_ONE_TO_ANY:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_uint32 mask = info->mask;
          jx_uint32 *hash_table = info->table;
          jx_ob *key_value = I->key_value;
          jx_uint32 index = 0;
          do {
            jx_uint32 *hash_entry = hash_table + (index << 1);
            if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
              jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
              if(jx_ob_identical(key_value[kv_offset + 1], value)) {
                found = JX_TRUE;
                *result = JX_BORROW( key_value[kv_offset] );
                break;
              }
            }
            index++;
          } while(index <= mask);
        }
        break;
      case JX_HASH_ONE_TO_ONE:
        {
          register jx_uint32 hash_code = jx_ob_hash_code(value);
          if(hash_code) {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash_table = info->table + ((mask + 1) << 1);
            jx_ob *key_value = I->key_value;
            jx_uint32 index = mask & hash_code;
            jx_uint32 sentinel = index;
            do {
              jx_uint32 *hash_entry = hash_table + (index << 1);
              if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE) && (hash_entry[0] == hash_code)) {
                jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                if(jx_ob_identical(key_value[kv_offset + 1], value)) {
                  *result = JX_BORROW( key_value[kv_offset] );
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index + 1) & mask;
            } while(index != sentinel);
          }
          break;
        }
      }
    }
  }
  return found;
}

jx_status jx__hash_from_list(jx_hash * hash, jx_list * list)
{
  jx_int size = jx__list_size(list);
  if(0x1 & size) {
    return JX_FAILURE;
  } else if(size) {
    jx_int i;
    for(i = 0; i < size; i += 2) {
      jx_ob key = jx_ob_copy(jx__list_borrow(list, i));
      jx_ob value = jx_ob_copy(jx__list_borrow(list, i + 1));
      if(!jx_ok(jx__hash_set(hash, key, value))) {
        jx_ob_free(key);
        jx_ob_free(value);
        return JX_FAILURE;
      }
    }
  }
  return JX_SUCCESS;
}

jx_status jx__list_with_hash(jx_list * list, jx_hash * hash)
{
  jx_int size = jx_vla_size(&hash->key_value);
  if(size) {
    jx_hash_info *info = (jx_hash_info *) hash->info;
    if((!info) || (info->mode == JX_HASH_LINEAR)) {
      jx_vla_free(&list->data.vla);
      list->data.vla = hash->key_value;
      hash->key_value = JX_NULL;
    } else {
      switch (info->mode) {
      case JX_HASH_ONE_TO_ANY:
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_uint32 mask = info->mask;
          jx_uint32 *table_base = info->table;
          jx_ob *key_value = hash->key_value;
          jx_uint32 index = 0;
          do {
            jx_uint32 *hash_entry = table_base + (index << 1);
            if(hash_entry[1] && !(hash_entry[1] & JX_HASH_ENTRY_ACTIVE)) {
              jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
              key_value[kv_offset].meta.bits = JX_META_NOT_AN_OB;       /* mark inactive entries */
            }
            index++;
          } while(index <= mask);
          {
            jx_ob *ob = key_value;
            jx_int i, count = 0;
            for(i = 0; i < size; i += 2) {
              if(ob[0].meta.bits != JX_META_NOT_AN_OB) {        /* purge the inactives */
                if(count != i) {
                  memcpy(key_value + count, ob, sizeof(jx_ob) * 2);
                }
                count += 2;
              }
              ob += 2;
            }
            jx_vla_resize(&hash->key_value, count);
            if(count) {
              jx_vla_free(&list->data.vla);
              list->data.vla = hash->key_value;
              hash->key_value = JX_NULL;
            }
            jx_vla_free(&hash->info);
          }
        }
      }
    }
  }
  return JX_SUCCESS;
}

jx_status jx__hash_with_list(jx_hash * hash, jx_list * list)
{
  jx_int size = jx__list_size(list);
  if(0x1 & size) {
    return JX_FAILURE;
  } else if(size) {
    jx_status status = JX_SUCCESS;
    jx_int i;
    for(i = 0; i < size; i += 2) {
      jx_ob key = jx__list_borrow(list, i);
      jx_ob value = jx__list_borrow(list, i + 1);
      if(!jx_ok(jx__hash_set(hash, key, value))) {
        status = JX_FAILURE;
        break;
      }
    }
    /* must empty contents of one container or the other */
    if(jx_ok(status)) {
      jx_vla_free(&list->data.vla);
    } else {
      jx_vla_free(&hash->key_value);
      jx_vla_free(&hash->info);
    }
    return status;
  } else {
    return JX_SUCCESS;
  }
}

/* read only */

jx_status jx__ob_set_read_only(jx_ob ob, jx_bool read_only)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    if(read_only)
      ob.meta.bits |= JX_META_BIT_READ_ONLY;
    else
      ob.meta.bits &= ~(JX_META_BIT_READ_ONLY);
    break;
  case JX_META_BIT_LIST:
    return jx__list_set_read_only(ob.data.io.list, read_only);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_set_read_only(ob.data.io.hash, read_only);
    break;
  }
  return JX_SUCCESS;
}

/* copying */

jx_ob jx__ob_copy(jx_ob ob)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    ob.data.io.str = (jx_char *) jx_vla_copy(&ob.data.io.str);
    return ob;
    break;
  case JX_META_BIT_LIST:
    return jx__list_copy(ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_copy(ob.data.io.hash);
    break;
  }
  return jx_ob_from_null();
}

/* comparison */

jx_bool jx__ob_gc_identical(jx_ob left, jx_ob right)
{
  /* on entry, we know left.meta.bits == right.meta.bits and that
     both objects are GC'd */
  switch (left.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    {
      jx_char *left_st = jx_ob_as_str(&left);
      jx_char *right_st = jx_ob_as_str(&right);
      if(left_st && right_st) {
        return (!strcmp(left_st, right_st));
      }
    }
    break;
  case JX_META_BIT_LIST:
    break;
  case JX_META_BIT_HASH:
    break;
  }
  return JX_FALSE;
}

jx_bool jx__ob_non_gc_equal(jx_ob left, jx_ob right)
{
  return JX_FALSE;
}

jx_bool jx__ob_gc_equal(jx_ob left, jx_ob right)
{
  /* on entry, we know left.meta.bits == right.meta.bits and that
     both objects are GC'd */
  switch (left.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    {
      jx_char *left_st = jx_ob_as_str(&left);
      jx_char *right_st = jx_ob_as_str(&right);
      if(left_st && right_st) {
        return (!strcmp(left_st, right_st));
      }
    }
    break;
  case JX_META_BIT_LIST:
    return jx__list_equal(left.data.io.list, right.data.io.list);
    break;
  case JX_META_BIT_HASH:
    // to do
    // return jx__hash_equal(left.data.io.list, right.data.io.list);
    break;
  }
  return JX_FALSE;
}

/* freeing */

jx_status jx__ob_free(jx_ob ob)
{
  if(ob.meta.bits & JX_META_BIT_GC) {
    if(ob.meta.bits & JX_META_BIT_READ_ONLY) {
      return JX_FAILURE;
    } else {
      switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_STR:
        return jx_vla_free(&ob.data.io.str);
        break;
      case JX_META_BIT_LIST:
        return jx__list_free(ob.data.io.list);
        break;
      case JX_META_BIT_HASH:
        return jx__hash_free(ob.data.io.hash);
        break;
      }
    }
  }
  return JX_SUCCESS;
}
