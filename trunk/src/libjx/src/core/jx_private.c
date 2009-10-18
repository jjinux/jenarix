

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


/* having a minimum allocation wastes memory but can provide more
   effective reuse of small containers while saving calls to
   realloc */

#define JX_VLA_MIN_ALLOC_CUTOFF 96
#define JX_VLA_MIN_ALLOC 4


/* constants for tuning hash-table performance */

#define JX_HASH_RAW_CUTOFF 2
#define JX_HASH_LINEAR_CUTOFF 4


/* global constants (improve performance) */

jx_ob jx_ob_null = JX_OB_NULL;
jx_ob jx_ob_int_zero = JX_OB_INT;
jx_ob jx_ob_float_zero = JX_OB_FLOAT;
jx_ob jx_ob_bool_false = JX_OB_BOOL;
jx_ob jx_ob_builtins = JX_OB_BUILTINS;

#if (JX_TINY_STR_SIZE <= 6)
#define JX_OB_RESOLVE  {{{JX_SELECTOR_RESOLVE}}, \
      {0,JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR}}
#endif
#if (JX_TINY_STR_SIZE == 10)
#define JX_OB_RESOLVE  {{{JX_SELECTOR_RESOLVE}}, \
      {0,JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR}}
#endif
#if (JX_TINY_STR_SIZE == 12)
#define JX_OB_RESOLVE  {{{JX_SELECTOR_RESOLVE}}, \
      {0,JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR}}
#endif
#if (JX_TINY_STR_SIZE == 14)
#define JX_OB_RESOLVE  {{{{JX_SELECTOR_RESOLVE,0,0}}}, \
      {0,JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR}}
#endif
#if (JX_TINY_STR_SIZE == 20)
#define JX_OB_RESOLVE  {{{{JX_SELECTOR_RESOLVE,0}}}, \
      {0,JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_SELECTOR}}
#endif

jx_ob jx_ob_resolve = JX_OB_RESOLVE;

jx_char jx_ob_as_ident_error[] = "jx_ob_as_ident-Error: object not an identifier.";
jx_char jx_ob_as_str_error[] = "jx_ob_as_str-Error: object not a string.";

JX_INLINE void jx__gc_init(jx_gc * gc)
{
  memset(gc, 0, sizeof(jx_gc));
}


/* jx_vla routines */

JX_INLINE jx_int jx__vla_min_alloc(jx_int rec_size, jx_int size)
{
  return (((size * rec_size) > JX_VLA_MIN_ALLOC_CUTOFF) ? size :
          (size > JX_VLA_MIN_ALLOC) ? size : JX_VLA_MIN_ALLOC);
}

void *jx__vla_new(jx_int rec_size, jx_int size)
{
  jx_int alloc = jx__vla_min_alloc(rec_size, size);
  jx_vla *vla = (jx_vla *) jx_calloc(1, sizeof(jx_vla) + rec_size * alloc);
  if(vla) {
    vla->ptr = vla;
    vla->alloc = alloc;
    vla->rec_size = rec_size;
    vla->size = size;
    return (void *) (vla + 1);
  } else
    return NULL;
}

void *jx__vla_new_with_content(jx_int rec_size, jx_int size, void *content)
{
  jx_int alloc = jx__vla_min_alloc(rec_size, size);
  jx_vla *vla = jx_malloc(sizeof(jx_vla) + rec_size * alloc);
  if(vla) {
    jx_char *base = (jx_char *) (vla + 1);
    vla->ptr = vla;
    vla->alloc = alloc;
    vla->rec_size = rec_size;
    vla->size = size;
    jx_os_memcpy(base, content, rec_size * size);
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

void *jx__vla_new_from_subset(void **ref, jx_int index, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if((index >= 0) && (count > 0) &&
       (index < vla->size) && ((count + index) <= vla->size)) {
      jx_char *ptr = (jx_char *) (*ref);
      return jx_vla_new_with_content(vla->rec_size, count, ptr + vla->rec_size * index);
    }
  }
  return NULL;
}

static jx_status jx__vla__resize(jx_vla ** vla_ptr, jx_int new_size,
                                 jx_bool zero, int force_padding)
{
  jx_vla *vla = *vla_ptr;
  jx_int old_size = vla->size;
  if(new_size != old_size) {
    jx_int old_alloc = vla->alloc;
    if((new_size > old_alloc) || (new_size && force_padding)) {
      jx_int new_alloc = new_size + (new_size >> 1);    /* 50% margin for growth */
      jx_int new_bytes = (new_alloc - old_alloc) * vla->rec_size;
      jx_int padding = (((char *) vla) - ((char *) vla->ptr));

      if(new_bytes <= padding) {        /* we have enough space in front */
        jx_vla *new_vla = (jx_vla *) (((char *) vla) - new_bytes);
        jx_os_memmove(new_vla, vla, sizeof(jx_vla) + vla->rec_size * old_size);
        vla = new_vla;
        vla->alloc = new_alloc;
        new_vla->size = new_size;
        *(vla_ptr) = vla;
      } else {
        jx_vla *new_ptr = jx_realloc(vla->ptr, sizeof(jx_vla) +
                                     padding + vla->rec_size * new_alloc);
        if(!new_ptr) {
          fprintf(stderr, "MEMORY REALLOCATION ERROR -- are you being too greedy?\n");
          return JX_FAILURE;
        } else {
          vla = (jx_vla *) (((char *) new_ptr) + padding);
          if(force_padding && (!padding) && (new_alloc > (new_size + 1))) {
            /* leave some room up front */
            jx_int rec_size = vla->rec_size;
            jx_int less_alloc = ((new_alloc - new_size) >> 1);  /* half & half */
            jx_int padding = less_alloc * rec_size;
            jx_vla *new_vla = (jx_vla *) (((char *) vla) + padding);
            jx_os_memmove(new_vla, vla, sizeof(jx_vla) + rec_size * new_size);
            vla = new_vla;
            new_alloc -= less_alloc;
          }
          vla->ptr = new_ptr;
          vla->alloc = new_alloc;
          vla->size = new_size;
          (*vla_ptr) = vla;
        }
      }
    } else if(new_size) {
      vla->size = new_size;
      {
        jx_int padding = (((char *) vla) - ((char *) vla->ptr));
        if(padding && ((padding >> 1) > (new_size * vla->rec_size))) {
          /* twice the space upfront as currently used */
          jx_int rec_size = vla->rec_size;
          jx_int addl_alloc = (padding >> 1) / rec_size;
          jx_int new_bytes = rec_size * addl_alloc;
          jx_int new_alloc = vla->alloc + addl_alloc;
          jx_vla *new_vla = (jx_vla *) (((char *) vla) - new_bytes);
          jx_os_memmove(new_vla, vla, sizeof(jx_vla) + rec_size * new_size);
          vla = new_vla;
          vla->alloc = new_alloc;
          *(vla_ptr) = vla;
        }
      }
      if((vla->alloc > 4) && ((new_size << 1) < vla->alloc)) {
        jx_int new_alloc = vla->size;
        jx_int padding = (((char *) vla) - ((char *) vla->ptr));
        jx_vla *new_ptr = jx_realloc(vla->ptr, sizeof(jx_vla) +
                                     padding + vla->rec_size * new_alloc);
        if(new_ptr) {
          vla = (jx_vla *) (((char *) new_ptr) + padding);
          vla->alloc = new_alloc;
          vla->ptr = new_ptr;
          vla->size = new_size;
          (*vla_ptr) = vla;
        }
      }
    } else {                    /* new size is zero -- simply return insertion point to front of array */
      jx_int rec_size = vla->rec_size;
      vla = vla->ptr;
      vla->ptr = vla;
      vla->size = 0;
      vla->rec_size = rec_size;
      vla->alloc = old_alloc;
      *(vla_ptr) = vla;
    }
    if(old_size < new_size) {   /* zero new memory */
      jx_char *base = (jx_char *) (vla + 1);
      if(zero)
        memset(base + old_size * vla->rec_size, 0, (new_size - old_size) * vla->rec_size);
    }
    return JX_SUCCESS;
  }
  return JX_SUCCESS;
}

jx_status jx__vla_resize(void **ref, jx_int new_size)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    jx_status status = jx__vla__resize(&vla, new_size, JX_TRUE, JX_FALSE);
    (*ref) = (void *) (vla + 1);
    return status;
  } else {
    return JX_FAILURE;
  }
}

jx_status jx__vla_append(void **ref, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if(count > 0) {
      jx_int new_size = vla->size + count;
      jx_status status = jx__vla__resize(&vla, new_size, JX_TRUE, JX_FALSE);
      (*ref) = (void *) (vla + 1);
      return status;
    } else if(!count)
      return JX_SUCCESS;
  }
  return JX_FAILURE;
}

jx_status jx__vla_append_c_str(void **ref, jx_char * str)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    if(vla->rec_size == 1) {
      jx_int str_len = jx_strlen(str);
      jx_int size = vla->size;
      jx_status status = jx_vla_append(ref, (size ? str_len : str_len + 1));
      vla = ((jx_vla *) (*ref)) - 1;
      if(jx_ok(status)) {
        jx_char *base = (jx_char *) (*ref);
        jx_os_memcpy(base + (size ? size - 1 : 0), str, str_len + 1);
        return vla->size;
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
      jx_int old_size = vla->size;
      jx_status status = jx_vla_append(ref, (old_size ? str_len : str_len + 1));
      vla = ((jx_vla *) (*ref)) - 1;
      if(jx_ok(status)) {
        jx_char *base = (jx_char *) (*ref);
        jx_os_memcpy(base + (old_size ? old_size - 1 : 0), jx_ob_as_str(&ob),
                     str_len + 1);
        return vla->size;
      }
      return status;
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_insert(void **ref, jx_int index, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    jx_int old_size = vla->size;
    jx_bool front_half = (index < (old_size >> 1));
    if(front_half) {
      /* optimize insertion in front half if space available */
      jx_int padding = (((char *) vla) - ((char *) vla->ptr));
      if(padding >= (count * vla->rec_size)) {
        register jx_int new_alloc = vla->alloc + count;
        register jx_int new_size = vla->size + count;
        register jx_int rec_size = vla->rec_size;
        register jx_vla *ptr = vla->ptr;
        register jx_vla *new_vla = (jx_vla *) (((char *) vla) - rec_size * count);
        vla = new_vla;
        vla->alloc = new_alloc;
        vla->size = new_size;
        vla->rec_size = rec_size;
        vla->ptr = ptr;
        {
          jx_char *base = (jx_char *) (vla + 1);
          (*ref) = base;
          if(!index) {
            jx_os_memset(base, 0, rec_size * count);
          } else {
            jx_os_memmove(base, base + count * rec_size, index * rec_size);
            jx_os_memset(base + index * rec_size, 0, rec_size * count);
          }
        }
        return JX_SUCCESS;
      }
    }
    if((index >= 0) && (count >= 0) && (index <= old_size)) {
      jx_int new_size = old_size + count;
      jx_status status = jx__vla__resize(&vla, new_size, JX_FALSE, front_half);
      if(jx_ok(status)) {
        jx_char *base = (jx_char *) (vla + 1);
        (*ref) = base;
        jx_os_memmove(base + (index + count) * vla->rec_size,
                      base + index * vla->rec_size, (old_size - index) * vla->rec_size);
        jx_os_memset(base + index * vla->rec_size, 0, vla->rec_size * count);
      }
      return status;
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
      jx_int old_vla1_size = vla1->size;
      jx_int new_size = old_vla1_size + vla2->size;
      jx_int rec_size = vla1->rec_size;
      jx_status status = jx__vla__resize(&vla1, new_size, JX_TRUE, JX_FALSE);
      if(jx_ok(status)) {
        jx_char *base1 = (jx_char *) (vla1 + 1);
        jx_char *base2 = (jx_char *) (vla2 + 1);
        (*ref1) = base1;
        jx_os_memcpy(base1 + rec_size * old_vla1_size, base2, rec_size * vla2->size);
      }
      return status;
    }
  } else if(*ref2) {
    jx_vla *vla2 = ((jx_vla *) (*ref2)) - 1;
    jx_vla *vla1 = jx_vla_new(vla2->rec_size, vla2->size);
    if(vla1) {
      jx_char *base1 = (jx_char *) (vla1 + 1);
      jx_char *base2 = (jx_char *) (vla2 + 1);
      jx_os_memcpy(base1, base2, vla2->rec_size * vla2->size);
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
    if((index >= 0) && (count > 0) &&
       (index < vla->size) && ((count + index) <= vla->size)) {
      jx_int old_size = vla->size;
      jx_int new_size = old_size - count;
      jx_char *base = (jx_char *) (*ref);
      jx_bool from_front = (!index) || (index < (old_size - new_size));
      if(new_size && from_front) {      /* optimize removal of element(s) from start of array */
        register jx_int rec_size = vla->rec_size;
        if(index) {
          jx_char *base = (jx_char *) (vla + 1);
          jx_os_memmove(base + count * rec_size, base, index * rec_size);
        }
        {
          register jx_int new_alloc = vla->alloc - count;
          register jx_vla *ptr = vla->ptr;
          register jx_vla *new_vla = (jx_vla *) (((char *) vla) + rec_size * count);
          new_vla->alloc = new_alloc;
          new_vla->size = new_size;
          new_vla->rec_size = rec_size;
          new_vla->ptr = ptr;
          (*ref) = (void *) (new_vla + 1);
          return JX_SUCCESS;
        }
      } else {
        jx_os_memmove(base + index * vla->rec_size,
                      base + (count + index) * vla->rec_size,
                      ((vla->size - index) - count) * vla->rec_size);
        return jx_vla_resize(ref, new_size);
      }
    } else if(!count)
      return JX_SUCCESS;
  }
  return JX_FAILURE;
}

jx_status jx__vla_rezero(void **ref)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    memset((*ref), 0, vla->rec_size * vla->size);
    (*ref) = NULL;
  }
  return JX_SUCCESS;
}

jx_status jx__vla_reset(void **ref)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    vla->size = 0;
  }
  return JX_SUCCESS;
}

jx_status jx__vla_reverse(void **ref)
{
  if(jx_vla_size(ref) > 1) {
    if(jx_ok(jx_vla_append(ref, 1))) {
      jx_vla *vla = ((jx_vla *) (*ref)) - 1;
      {
        register jx_int rec_size = vla->rec_size;
        register jx_char *first = (jx_char *) (*ref);
        register jx_char *last = first + (rec_size * (vla->size - 2));
        register jx_char *tmp = first + (rec_size * (vla->size - 1));
        while(first < last) {
          jx_os_memcpy(tmp, first, rec_size);
          jx_os_memcpy(first, last, rec_size);
          jx_os_memcpy(last, tmp, rec_size);
          first += rec_size;
          last -= rec_size;
        }
      }
      jx__vla_resize(ref, vla->size - 1);
    } else {
      return JX_FAILURE;
    }
  }
  return JX_SUCCESS;
}

jx_status jx__vla_free(void **ref)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    jx_free(vla->ptr);
    (*ref) = NULL;
  }
  return JX_SUCCESS;
}


/* unboxing volatile cast subroutines */

jx_bool jx__ob_as_bool(jx_ob ob)
{
  return jx_ob_as_bool(jx__ob_to_bool(ob));
}

jx_int jx__ob_as_int(jx_ob ob)
{
  return jx_ob_as_int(jx__ob_to_int(ob));
}

jx_float jx__ob_as_float(jx_ob ob)
{
  return jx_ob_as_float(jx__ob_to_float(ob));
}


/* constructive cast subroutines */

jx_ob jx__ob_to_bool(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx_ob_from_bool(jx_str_len(ob) && JX_TRUE);
    break;
  case JX_META_BIT_LIST:
    //    printf("here %d\n",jx_list_size(ob));
    return jx_ob_from_bool(jx_list_size(ob) && JX_TRUE);
    break;
  case JX_META_BIT_HASH:
    return jx_ob_from_bool(jx_hash_size(ob) && JX_TRUE);
    break;
  case JX_META_BIT_IDENT:
    {
      jx_char *ident = jx_ob_as_ident(&ob);
      jx_char ch = ident[0];
      return jx_ob_from_bool((ch == 'T') || (ch == 't'));
    }
    break;
  default:
    return jx_ob_from_bool(jx_ob_as_bool(ob));
    break;
  }
  return jx_ob_from_null();
}

jx_ob jx__ob_to_int(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    {
      jx_char *buffer = jx_ob_as_str(&ob);
#ifdef JX_64_BIT
      jx_int icon;
      if(jx_os_sscanf(buffer, "%lli", &icon) != 1) {
        icon = 0;
      }
#else
      int icon;
      if(jx_os_sscanf(buffer, "%i", &icon) != 1) {
        icon = 0;
      }
#endif
      return jx_ob_from_int(icon);
    }
    break;
  case JX_META_BIT_IDENT:
    return jx_ob_from_int(0);
    break;
  }
  return jx_ob_from_null();
}

jx_ob jx__ob_to_float(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    {
      jx_char *buffer = jx_ob_as_str(&ob);
#ifdef JX_64_BIT
      double fcon;
      if(jx_os_sscanf(buffer, "%lf", &fcon) != 1) {     /* use strtof instead? */
        fcon = 0.0;
      }
#else
      float fcon;
      if(jx_os_sscanf(buffer, "%f", &fcon) != 1) {      /* use strtof instead? */
        fcon = 0.0F;
      }
#endif
      return jx_ob_from_float(fcon);
    }
    break;
  case JX_META_BIT_IDENT:
    return jx_ob_from_float(0.0);
    break;
  }
  return jx_ob_from_null();
}

#define JX_STR_TMP_BUF_SIZE 256

static void force_float(char *str)
{
  char ch, *p;
  p = str;
  while((ch = *(p++))) {
    switch (ch) {
    case '.':
    case 'e':
    case 'E':
      return;                   /* float implied */
      break;
    }
  }
  p[-1] = '.';
  p[0] = '0';
  p[1] = 0;
}

jx_ob jx__ob_to_str(jx_env * E, jx_ob ob)
{
  jx_fast_bits bits = ob.meta.bits;
  switch (bits & JX_META_MASK_TYPE_BITS) {
  case 0:                      /* NULL */
    return jx_ob_from_str("null");
    break;
  case JX_META_BIT_OPCODE:
    switch (bits & JX_META_MASK_OPCODE_INST) {
    case JX_OPCODE_BREAK:
      return jx_ob_from_str("@BREAK");
      break;
    case JX_OPCODE_CONTINUE:
      return jx_ob_from_str("@CONTINUE");
      break;
    case JX_OPCODE_RETURN:
      return jx_ob_from_str("@RETURN");
      break;
    case JX_OPCODE_TAIL_CALL:
      return jx_ob_from_str("@TAIL_CALL");
      break;
    case JX_OPCODE_JUMP_RELATIVE:
      {
        char buffer[JX_STR_TMP_BUF_SIZE];
        snprintf(buffer, sizeof(buffer), "@%d", (int) ob.data.io.int_);
        return jx_ob_from_str(buffer);
      }
    default:
      return jx_ob_from_str("@UNKNOWN");
      break;
    }
    break;
  case JX_META_BIT_BOOL:
    if(ob.data.io.bool_)
      return jx_ob_from_str("true");
    else
      return jx_ob_from_str("false");
    break;
  case JX_META_BIT_INT:
    {
      char buffer[JX_STR_TMP_BUF_SIZE];
#ifdef JX_64_BIT
      snprintf(buffer, sizeof(buffer), "%lld", ob.data.io.int_);
#else
      snprintf(buffer, sizeof(buffer), "%d", ob.data.io.int_);
#endif
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_FLOAT:
    {
      char buffer[JX_STR_TMP_BUF_SIZE];
#ifdef JX_64_BIT
      snprintf(buffer, sizeof(buffer), "%.17g", ob.data.io.float_);
#else
      snprintf(buffer, sizeof(buffer), "%.8g", ob.data.io.float_);
#endif
      force_float(buffer);
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_IDENT:
    {
      jx_ob result;
      if(JX_META_BIT_GC & bits) {
        result = jx_ob_from_str(jx_ob_as_ident(&ob));
      } else {
        result = ob;
        result.meta.bits = JX_META_BIT_STR | (JX_META_MASK_TINY_STR_SIZE & bits);
      }
      if(!jx_ob_as_str(&result)[0])     /* special case: the blank identifier */
        jx_ob_replace(&result, jx_ob_from_str("."));
      return result;
    }
    break;
  case JX_META_BIT_STR:
    if(JX_META_BIT_GC & bits) {
      return jx_ob_from_str(jx_ob_as_ident(&ob));
    } else {
      return ob;
    }
    break;
  case JX_META_BIT_BUILTIN:
    {
      jx_char buffer[JX_STR_TMP_BUF_SIZE];
      switch (bits & JX_META_MASK_BUILTIN_TYPE) {
      case JX_META_BIT_BUILTIN_ENTITY: /* a cloaked identifier */
        sprintf(buffer, "$");
        jx_os_strncpy(buffer + 1, jx_ob_as_ident(&ob), JX_STR_TMP_BUF_SIZE - 1);
        buffer[JX_STR_TMP_BUF_SIZE - 1] = 0;
        break;
      case JX_META_BIT_BUILTIN_SELECTOR:
        sprintf(buffer, "`op:%d", (int) ob.data.io.int_);
        break;
      case JX_META_BIT_BUILTIN_OPAQUE_OB:
        sprintf(buffer, "opaque`%p", ob.data.io.void_);
        break;
      case JX_META_BIT_BUILTIN_NATIVE_FN:
        sprintf(buffer, "native`%p", ob.data.io.void_);
        break;
      case JX_META_BIT_BUILTIN_FUNCTION:
        {
          jx_function *fn = ob.data.io.function;
          if(jx_ident_check(fn->name)) {
            sprintf(buffer, "fn`");
            jx_ob_into_strcat(buffer, JX_STR_TMP_BUF_SIZE, fn->name);
          } else {
            sprintf(buffer, "fn`%p", ob.data.io.void_);
          }
        }
        break;
      case JX_META_BIT_BUILTIN_MACRO:
        {
          jx_function *fn = ob.data.io.function;
          if(jx_ident_check(fn->name)) {
            sprintf(buffer, "`macro:");
            jx_ob_into_strcat(buffer, JX_STR_TMP_BUF_SIZE, fn->name);
          } else {
            sprintf(buffer, "`macro:%p", ob.data.io.void_);
          }
        }
        break;
      default:
        sprintf(buffer, "`unknown_builtin");
        break;
      }
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_LIST:       /* only containers may recurse */
  case JX_META_BIT_HASH:
    return jx_ob_to_jxon(ob);   /* JXON is our default encoding */
    break;
  default:
    return jx_ob_from_str("ERROR:invalid object");
    break;
  }
}

jx_status jx_ob_into_str(jx_char * buf, jx_int buf_size, jx_ob ob)
{
  jx_status status = JX_FAILURE;
  jx_ob st = jx_ob_to_str(ob);
  if(jx_str_check(st)) {
    jx_int len = jx_str_len(st);
    if(len >= buf_size) {
      len = buf_size - 1;
      status = JX_STATUS_OVERFLOW;
    } else {
      status = JX_SUCCESS;
    }
    jx_os_memcpy(buf, jx_ob_as_str(&st), len);
    buf[len] = 0;
  } else {
    buf[0] = 0;
  }
  jx_ob_free(st);               /* ok */
  return status;
}

jx_status jx_ob_into_strcat(jx_char * buf, jx_int buf_size, jx_ob ob)
{
  jx_size len = strlen(buf);
  if(len < buf_size) {
    jx_ob_into_str(buf + len, buf_size - len, ob);
    return JX_SUCCESS;
  } else {
    return JX_STATUS_OVERFLOW;
  }
}

jx_ob jx__ob_to_ident(jx_env * E, jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx_ob_from_ident(jx_ob_as_str(&ob)); /* E */
    break;
  }
  return jx_ob_from_null();
}


/* strings */

jx_ob Jx_ob_from_str(jx_env * E, jx_char * str)
{
  jx_int len = jx_strlen(str);
  jx_ob result = jx_ob_from_null();

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | len;
    jx_os_memcpy(result.data.io.tiny_str, str, len + 1);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = Jx_vla_new(E, 1, len + 1 + sizeof(jx__str), JX_TRUE);
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str->str, str, len + 1);
    }
  }
  return result;
}

jx_ob jx_ob_from_str(jx_char * str)
{
  return Jx_ob_from_str(JX_NULL, str);
}

jx_ob Jx_ob_from_str_with_len(jx_env * E, jx_char * str, jx_int len)
{
  jx_ob result = jx_ob_from_null();

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | len;
    jx_os_memcpy(result.data.io.tiny_str, str, len);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = Jx_vla_new(E, 1, len + 1 + sizeof(jx__str), JX_TRUE);
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str->str, str, len);
      result.data.io.str->str[len] = 0;
    }
  }
  return result;
}

jx_ob jx_ob_from_str_with_len(jx_char * str, jx_int len)
{
  return Jx_ob_from_str_with_len(JX_NULL, str, len);
}

jx_ob Jx_ob_from_ident_with_len(jx_env * E, jx_char * str, jx_int len)
{
  jx_ob result = jx_ob_from_null();

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_IDENT | len;
    jx_os_memcpy(result.data.io.tiny_str, str, len);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = Jx_vla_new(E, 1, len + 1 + sizeof(jx__str), JX_TRUE);
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_IDENT | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str->str, str, len);
      result.data.io.str->str[len] = 0;
    }
  }
  return result;
}

jx_ob jx_ob_from_ident_with_len(jx_char * str, jx_int len)
{
  return Jx_ob_from_ident_with_len(JX_NULL, str, len);
}

jx_ob jx_ob_with_str_vla(jx_char ** ref)
{
  jx_int size = jx_vla_size(ref);
  jx_ob result = jx_ob_from_null();
  if(size)
    size--;
  if(size < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | size;
    jx_os_memcpy(result.data.io.tiny_str, *ref, size + 1);
    jx_vla_free(ref);
  } else {
    /* string not tiny -- use heap */
    jx_int size = jx_vla_size(ref);
    if(jx_ok(jx_vla_resize(ref, size + sizeof(jx__str)))) {
      jx__str *str = (jx__str *)(void *)(*ref);
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      /* insert jx_str record in front of chars */
      jx_os_memmove(str->str, (* ref), size);
      jx__gc_init(&str->gc);
      result.data.io.str = str;
    } else {
      jx_vla_free(ref);
    }
  }
  return result;
}

static jx_ob jx_ob_with_ident_vla(jx_char ** ref)
{
  jx_int size = jx_vla_size(ref);
  jx_ob result = jx_ob_from_null();
  if(size)
    size--;
  if(size < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_IDENT | size;
    jx_os_memcpy(result.data.io.tiny_str, *ref, size + 1);
    jx_vla_free(ref);
  } else {
    /* string not tiny -- use heap */
    jx_int size = jx_vla_size(ref);
    if(jx_ok(jx_vla_resize(ref, size + sizeof(jx__str)))) {
      jx__str *str = (jx__str *)(void *)(*ref);
      result.meta.bits = JX_META_BIT_IDENT | JX_META_BIT_GC;
      /* insert jx_str record in front of chars */
      jx_os_memmove(str->str, (*ref), size);
      jx__gc_init(&str->gc);
      result.data.io.str = str;
    } else {
      jx_vla_free(ref);
    }
  }
  return result;
}

jx_ob jx__str__concat(jx_env * E, jx_char * left, jx_int left_len,
                      jx_char * right, jx_int right_len)
{
  jx_int total_len = left_len + right_len;
  if(total_len < JX_TINY_STR_SIZE) {
    jx_char buffer[JX_TINY_STR_SIZE];
    jx_os_memcpy(buffer, left, left_len);
    jx_os_memcpy(buffer + left_len, right, right_len);
    return jx_ob_from_str_with_len(buffer, total_len);
  } else {
    jx_ob result = jx_ob_from_null();
    jx_char *vla = Jx_vla_new(E, 1, total_len + sizeof(jx__str) + 1, JX_TRUE);
    /* will be zero initialized */
    if(vla) {
      jx__str *str = (jx__str *)(void *)vla;
      jx_os_memcpy(str->str, left, left_len);
      jx_os_memcpy(str->str + left_len, right, right_len);
      result.data.io.str = str;
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
    }
    return result;
  }
}

jx_ob jx__str_concat(jx_env * E, jx_ob left, jx_ob right)
{                               /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {    /* we have at least one string */
    jx_ob left_str = Jx_ob_to_str(E, left);
    jx_ob right_str = Jx_ob_to_str(E, right);
    jx_ob result = Jx_str_concat(E, left_str, right_str);
    Jx_ob_free(E, left_str);
    Jx_ob_free(E, right_str);
    return result;
  }
  return jx_ob_from_null();
}

jx_int jx__str__compare(jx_char * left, jx_char * right)
{
  if(left && right) {
    return strcmp(left, right);
  }
  return 0;
}

jx_int jx__str_compare(jx_env * E, jx_ob left, jx_ob right)
{                               /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {    /* we have at least one string */
    jx_ob left_str = Jx_ob_to_str(E, left);
    jx_ob right_str = Jx_ob_to_str(E, right);
    jx_int result = jx_str_compare(left_str, right_str);
    Jx_ob_free(E, left_str);  /* ok */
    Jx_ob_free(E, right_str); /* ok */
    return result;
  }
  return 0;
}

jx_ob jx__str_gc_copy_strong(jx_env * E, jx__str * str)
{
  jx_ob result = jx_ob_from_null();
  result.data.io.str = (jx__str *) Jx_vla_copy(E, &str);
  if(result.data.io.str) {
    jx__gc_init((jx_gc *) result.data.io.str);
    result.meta.bits = result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
  }
  return result;
}

jx_bool jx__str_free(jx__str * str)
{
  jx_gc *gc = &str->gc;
  if(gc->shared) {
    return JX_STATUS_FREED_SHARED;
  } else {
    jx_vla_free(&str);
    return JX_SUCCESS;
  }
}

jx_ob jx__ob_add(jx_env * E, jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {
    return jx__str_concat(E, left, right);
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_from_float(jx_ob_as_float(left) + jx_ob_as_float(right));
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_from_int(jx_ob_as_int(left) + jx_ob_as_int(right));
  }
  return jx_ob_from_null();
}

jx_ob jx__ob_pow(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_from_float(pow(jx_ob_as_float(left), jx_ob_as_float(right)));
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_from_int(pow(jx_ob_as_int(left), jx_ob_as_int(right)));
  }
  return jx_ob_from_null();
}

jx_ob jx__ob_mul(jx_env * E, jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_LIST) {
    if((left.meta.bits & JX_META_BIT_LIST) && (right.meta.bits & JX_META_BIT_INT)) {
      return Jx_list_new_with_repeat(E, jx_ob_as_int(right), Jx_ob_copy(E, left));
    } else if((right.meta.bits & JX_META_BIT_LIST) && (left.meta.bits & JX_META_BIT_INT)) {
      return Jx_list_new_with_repeat(E, jx_ob_as_int(left), Jx_ob_copy(E, right));
    }
    return jx_ob_from_null();
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_from_float(jx_ob_as_float(left) * jx_ob_as_float(right));
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_from_int(jx_ob_as_int(left) * jx_ob_as_int(right));
  }
  return jx_ob_from_null();
}

jx_int jx__ob_compare(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits left_bits = left.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_fast_bits right_bits = right.meta.bits & JX_META_MASK_TYPE_BITS;
  jx_fast_bits merge_bits = left_bits | right_bits;
  switch (merge_bits) {
  case JX_META_BIT_BOOL | JX_META_BIT_INT:
    return ((jx_ob_as_int(left) < jx_ob_as_int(right)) ? -1 :
            (jx_ob_as_int(left) != jx_ob_as_int(right)) ? 1 :
            ((left_bits < right_bits) ? -1 : (left_bits == right_bits) ? 0 : 1));
    break;
  case JX_META_BIT_BOOL | JX_META_BIT_FLOAT:
  case JX_META_BIT_INT | JX_META_BIT_FLOAT:
    return ((jx_ob_as_float(left) < jx_ob_as_float(right)) ? -1 :
            (jx_ob_as_float(left) != jx_ob_as_float(right)) ? 1 :
            ((left_bits < right_bits) ? -1 : (left_bits == right_bits) ? 0 : 1));
    break;
  default:                     /* uncomparable items: fall back on bit mask values */
    if((left_bits) && (!right_bits))
      return 1;
    if((!left_bits) && (right_bits))
      return -1;
    return ((left_bits < right_bits) ? -1 : (left_bits == right_bits) ? 0 : 1);
    break;
  }
  return JX_FALSE;
}

jx_bool jx__ob_lt(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {
    return jx_str_compare(left, right) < 0;
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_as_float(left) < jx_ob_as_float(right);
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_as_int(left) < jx_ob_as_int(right);
  }
  return JX_FALSE;
}

jx_bool jx__ob_gt(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {
    return jx_str_compare(left, right) > 0;
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_as_float(left) > jx_ob_as_float(right);
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_as_int(left) > jx_ob_as_int(right);
  }
  return JX_FALSE;
}

jx_bool jx__ob_le(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {
    return jx_str_compare(left, right) <= 0;
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_as_float(left) <= jx_ob_as_float(right);
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_as_int(left) <= jx_ob_as_int(right);
  }
  return JX_FALSE;
}

jx_bool jx__ob_ge(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {
    return jx_str_compare(left, right) >= 0;
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_as_float(left) >= jx_ob_as_float(right);
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_as_int(left) >= jx_ob_as_int(right);
  }
  return JX_FALSE;
}

#define JX_MATH_OP(SUFFIX,OPER) \
jx_ob jx__ob_ ## SUFFIX(jx_ob left, jx_ob right) \
{  /* on entry, we know the types aren't matched */ \
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits; \
  if( merge_bits & JX_META_BIT_FLOAT) { \
    return jx_ob_from_float( jx_ob_as_float(left) OPER jx_ob_as_float(right) ); \
  } else if( merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) { \
    return jx_ob_from_int( jx_ob_as_int(left) OPER jx_ob_as_int(right)); \
  } \
  return jx_ob_from_null(); \
}

JX_MATH_OP(sub, -)
#undef JX_MATH_OP
#define JX_LOG_OP(SUFFIX,OPER) \
jx_ob jx__ob_ ## SUFFIX(jx_ob left, jx_ob right) \
{  /* on entry, we know the types aren't matched */ \
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits; \
  if( merge_bits & JX_META_BIT_FLOAT) { \
    return jx_ob_from_bool( jx_ob_as_float(left) OPER jx_ob_as_float(right) ); \
  } else if( merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) { \
    return jx_ob_from_bool( jx_ob_as_int(left) OPER jx_ob_as_int(right)); \
  } else if( merge_bits & (JX_META_BIT_HASH | JX_META_BIT_LIST)) { \
    return jx_ob_from_bool( jx_ob_as_bool(left) OPER jx_ob_as_bool(right)); \
  } \
  return jx_ob_from_null(); \
}
  JX_LOG_OP(and, &&)
  JX_LOG_OP(or, ||)
#undef JX_LOG_OP
     jx_ob jx__ob_mod(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_from_float(fmod(jx_ob_as_float(left), jx_ob_as_float(right)));
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_from_int(jx_ob_as_int(left) % jx_ob_as_int(right));
  }
  return jx_ob_from_null();
}


/* identifers are effectively strings without quotes */

jx_ob jx_ob_from_ident(jx_char * ident)
{
  jx_int len = jx_strlen(ident);
  jx_ob result = jx_ob_from_null();

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_IDENT | len;
    jx_os_memcpy(result.data.io.tiny_str, ident, len + 1);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = jx_vla_new(1, len + 1 + sizeof(jx__str));
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_IDENT | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str->str, ident, len + 1);
    }
  }
  return result;
}

jx_ob jx__ident_gc_copy_strong(jx_env * E, jx__str * str)
{
  jx_ob result = jx_ob_from_null();
  result.data.io.str = (jx__str *) Jx_vla_copy(E, &str);
  if(result.data.io.str) {
    jx__gc_init( &result.data.io.str->gc);
    result.meta.bits = result.meta.bits = JX_META_BIT_IDENT | JX_META_BIT_GC;
  }
  return result;
}


/* lists */

jx_ob jx__list_new(void)
{
  jx_ob result = JX_OB_LIST;
  if((result.data.io.list = (jx_list *) jx_calloc(1, sizeof(jx_list)))) {
    return result;
  } else {
    return jx_ob_from_null();
  }
}

jx_ob jx_list_new(void)
{                               /* E? */
  jx_ob result = JX_OB_LIST;
  if((result.data.io.list = (jx_list *) jx_calloc(1, sizeof(jx_list)))) {
    return result;
  } else {
    return jx_ob_from_null();
  }
}

jx_ob jx_list_new_with_size(jx_int size)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->data.vla = jx_vla_new(sizeof(jx_ob), size);    /* zero'd memory */
  }
  return result;
}

jx_ob jx_list_new_with_range(jx_int start, jx_int stop, jx_int step, jx_float scale)
{
  jx_ob result = jx_list_new();
  jx_int delta = stop - start;
  if(step && ((delta > 0) == (step > 0))) {
    jx_int steps = (delta / step);
    if(delta % step)
      steps++;
    if(steps) {
      if(scale == JX_FLOAT_ZERO) {
        jx_int *ptr = jx_list_as_int_vla(result);
        if(jx_ok(jx_vla_resize(&ptr, steps))) {
          jx_list_set_int_vla(result, &ptr);
          while(steps--) {
            *(ptr++) = start;
            start += step;
          }
        }
      } else {
        jx_float *ptr = jx_list_as_float_vla(result);
        if(jx_ok(jx_vla_resize(&ptr, steps))) {
          jx_list_set_float_vla(result, &ptr);
          while(steps--) {
            *(ptr++) = start * scale;
            start += step;
          }
        }
      }
    }
  }
  return result;
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
    result.data.io.list->data.vla =
      jx_vla_new_with_content(sizeof(jx_float), size, array);
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


/* This public-domain C quick_sort implementation by Darel Rex Finley
 (DarelRex@gmail.com) with modifications by Warren L. DeLano */
#define  JX_QS_MAX_LEVELS  300
JX_INLINE void jx_quick_sort_int(jx_int * arr, jx_int elements)
{
  jx_int piv, beg[JX_QS_MAX_LEVELS], end[JX_QS_MAX_LEVELS], i = 0, L, R, swap;
  beg[0] = 0;
  end[0] = elements;
  while(i >= 0) {
    L = beg[i];
    R = end[i] - 1;
    if(L < R) {
      piv = arr[L];
      while(L < R) {
        while((arr[R] >= piv) && L < R)
          R--;
        if(L < R)
          arr[L++] = arr[R];
        while((arr[L] <= piv) && L < R)
          L++;
        if(L < R)
          arr[R--] = arr[L];
      }
      arr[L] = piv;
      beg[i + 1] = L + 1;
      end[i + 1] = end[i];
      end[i++] = L;
      if((end[i] - beg[i]) > (end[i - 1] - beg[i - 1])) {
        swap = beg[i];
        beg[i] = beg[i - 1];
        beg[i - 1] = swap;
        swap = end[i];
        end[i] = end[i - 1];
        end[i - 1] = swap;
      }
    } else {
      i--;
    }
  }
}

JX_INLINE void jx_quick_sort_float(jx_float * arr, jx_int elements)
{

#define  MAX_LEVELS  300
  jx_int beg[JX_QS_MAX_LEVELS], end[JX_QS_MAX_LEVELS], i = 0, L, R, swap;
  jx_float piv;
  beg[0] = 0;
  end[0] = elements;
  while(i >= 0) {
    L = beg[i];
    R = end[i] - 1;
    if(L < R) {
      piv = arr[L];
      while(L < R) {
        while((arr[R] >= piv) && L < R)
          R--;
        if(L < R)
          arr[L++] = arr[R];
        while((arr[L] <= piv) && L < R)
          L++;
        if(L < R)
          arr[R--] = arr[L];
      }
      arr[L] = piv;
      beg[i + 1] = L + 1;
      end[i + 1] = end[i];
      end[i++] = L;
      if((end[i] - beg[i]) > (end[i - 1] - beg[i - 1])) {
        swap = beg[i];
        beg[i] = beg[i - 1];
        beg[i - 1] = swap;
        swap = end[i];
        end[i] = end[i - 1];
        end[i - 1] = swap;
      }
    } else {
      i--;
    }
  }
}

JX_INLINE void jx_quick_sort_ob(jx_ob * arr, jx_int elements)
{
  jx_int beg[JX_QS_MAX_LEVELS], end[JX_QS_MAX_LEVELS], i = 0, L, R, swap;
  jx_ob piv;
  beg[0] = 0;
  end[0] = elements;
  while(i >= 0) {
    L = beg[i];
    R = end[i] - 1;
    if(L < R) {
      piv = arr[L];
      while(L < R) {
        while((jx_ob_compare(arr[R], piv) >= 0) && L < R)
          R--;
        if(L < R)
          arr[L++] = arr[R];
        while((jx_ob_compare(arr[L], piv) <= 0) && L < R)
          L++;
        if(L < R)
          arr[R--] = arr[L];
      }
      arr[L] = piv;
      beg[i + 1] = L + 1;
      end[i + 1] = end[i];
      end[i++] = L;
      if((end[i] - beg[i]) > (end[i - 1] - beg[i - 1])) {
        swap = beg[i];
        beg[i] = beg[i - 1];
        beg[i - 1] = swap;
        swap = end[i];
        end[i] = end[i - 1];
        end[i - 1] = swap;
      }
    } else {
      i--;
    }
  }
}

jx_status jx__list_sort_locked(jx_list * I)
{
  jx_int size = jx_vla_size(&I->data.vla);
  if(I->packed_meta_bits && (size > 1)) {
    switch (I->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_INT:
      jx_quick_sort_int(I->data.int_vla, size);
      return JX_SUCCESS;
      break;
    case JX_META_BIT_FLOAT:
      jx_quick_sort_float(I->data.float_vla, size);
      return JX_SUCCESS;
      break;
    default:
      return JX_FAILURE;
      break;
    }
  }
  jx_quick_sort_ob(I->data.ob_vla, size);
  return JX_SUCCESS;
}

jx_ob jx__list_copy(jx_env * E, jx_list * I)
{
  jx_ob result = Jx_list_new(E);
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_LIST) {
      jx_list *new_I = result.data.io.list;
      (*new_I) = (*I);
      new_I->gc.synchronized = JX_FALSE;
      jx__gc_init(&new_I->gc);
      new_I->data.vla = Jx_vla_copy(E, &I->data.vla);
      if(!new_I->packed_meta_bits) {    /* need to recursively copy all GC content */
        jx_int i, size = jx_vla_size(&new_I->data.ob_vla);
        jx_ob *ob = new_I->data.ob_vla;
        for(i = 0; i < size; i++) {
          if(ob->meta.bits & JX_META_BIT_GC) {
            *ob = Jx_ob_copy(E, *ob);
          }
          ob++;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_ob jx__list_copy_strong(jx_env * E, jx_list * I)
{
  jx_ob result = Jx_list_new(E);
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_LIST) {
      jx_list *new_I = result.data.io.list;
      (*new_I) = (*I);
      new_I->gc.synchronized = JX_FALSE;
      jx__gc_init(&new_I->gc);
      new_I->data.vla = jx_vla_copy(&I->data.vla);
      if(!new_I->packed_meta_bits) {    /* need to recursively copy all GC content */
        jx_int i, size = jx_vla_size(&new_I->data.ob_vla);
        jx_ob *ob = new_I->data.ob_vla;
        for(i = 0; i < size; i++) {
          if(ob->meta.bits & JX_META_BIT_GC) {
            *ob = Jx_ob_copy_strong(E, *ob);
          }
          ob++;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

#if 0

static void jx__list_make_strong(jx_list * I)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(!I->packed_meta_bits) {
      jx_int i, size = jx_vla_size(&I->data.ob_vla);
      jx_ob *ob = I->data.ob_vla;
      for(i = 0; i < size; i++) {
        if(ob->meta.bits & JX_META_BIT_GC) {
          *ob = jx_ob_make_strong_with_ob(*ob);
        }
        ob++;
      }
    }
    jx_gc_unlock(&I->gc);
  }
}

static void jx__list_only_strong(jx_list * I)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(!I->packed_meta_bits) {
      jx_int i, size = jx_vla_size(&I->data.ob_vla);
      jx_ob *ob = I->data.ob_vla;
      for(i = 0; i < size; i++) {
        if(ob->meta.bits & JX_META_BIT_GC) {
          *ob = jx_ob_not_weak_with_ob(*ob);
        }
        ob++;
      }
    }
    jx_gc_unlock(&I->gc);
  }
}
#endif

jx_status jx__list_set_shared(jx_list * I, jx_bool shared)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    I->gc.shared = shared;
    if(!I->packed_meta_bits) {  /* no need to scan packed structures */
      jx_int size = jx_vla_size(&I->data.ob_vla);
      jx_ob *ob = I->data.ob_vla;
      while(size--) {
        jx_ob_set_shared(*(ob++), shared);
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return status;
}

jx_status jx__list_set_synchronized(jx_list * I, jx_bool synchronized, jx_bool recursive)
{
  jx_bool synched_on_entry = I->gc.synchronized;
  jx_status status = synched_on_entry ? jx_gc_lock(&I->gc) : JX_YES;
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    I->gc.synchronized = synchronized;
    if(recursive && !I->packed_meta_bits) {
      jx_int size = jx_vla_size(&I->data.ob_vla);
      jx_ob *ob = I->data.ob_vla;

      while(size--) {
        jx_ob_set_synchronized(*(ob++), synchronized, recursive);
      }
    }
    if(synched_on_entry) {
      jx_gc_unlock(&I->gc);
    }
  }
  return status;
}

jx_bool jx__ob_shared(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    return ob.data.io.str->gc.shared;
    break;
  case JX_META_BIT_LIST:
    return ob.data.io.list->gc.shared;
    break;
  case JX_META_BIT_HASH:
    return ob.data.io.hash->gc.shared;
    break;
  }
  return JX_FALSE;
}

jx_int jx__list_index(jx_list * I, jx_ob ob)
{
  jx_int result = -1;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_int I_size = jx__list_size(I);
    if(I_size) {
      if(I->packed_meta_bits) {
        switch (I->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
        case JX_META_BIT_INT:
          {
            jx_int *I_int = I->data.int_vla;
            jx_int target = ob.data.io.int_;
            jx_int i;
            for(i = 0; i < I_size; i++) {
              if(*(I_int++) == target) {
                result = i;
                break;
              }
            }
          }
          break;
        case JX_META_BIT_FLOAT:
          {
            jx_int i;
            jx_float *I_float = I->data.float_vla;
            jx_float target = ob.data.io.float_;
            for(i = 0; i < I_size; i++) {
              if(*(I_float++) == target)
                result = i;
            }
          }
          break;
        }
      } else {                  /* standard objects */
        jx_int i;
        jx_ob *I_ob = I->data.ob_vla;
        for(i = 0; i < I_size; i++) {
          if(jx_ob_identical(*(I_ob++), ob)) {
            result = i;
            break;
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  //  printf("index %d\n",result);
  return result;
}

JX_INLINE void Jx__list_free(jx_env * E, jx_list * I)
{
  if(E && (E->n_list < JX_TLS_MAX)) {
    jx_mem_chain *chain = (jx_mem_chain *) I;
    chain->next = E->list_chain;
    E->list_chain = chain;
    E->n_list++;
  } else {
    jx_free(I);
  }
}

JX_INLINE jx_status jx__list_free(jx_env * E, jx_list * I)
{                               /* assumes not synchronized */
  jx_status status = JX_SUCCESS;
  if(I->gc.shared) {
    status = JX_STATUS_FREED_SHARED;
  } else {

    if(!I->packed_meta_bits) {
      jx_int i, size = jx_vla_size(&I->data.vla);
      jx_ob *ob = I->data.ob_vla;
      for(i = 0; i < size; i++) {
        Jx_ob_free(E, JX_DISOWN(E, *(ob++)));
      }
    }
    Jx_vla_free(E, &I->data.vla);
    Jx__list_free(E, I);
  }
  return status;
}

jx_ob jx__str_join_from_list(jx_env * E, jx_list * I, jx_char * sep)
{
  jx_ob result = jx_ob_from_null();
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_int sep_len = sep ? jx_strlen(sep) : 0;
    jx_int size = jx__list_size(I);
    jx_int total = size ? sep_len * (size - 1) : 0;
    if(!I->packed_meta_bits) {  /* TO DO: handle this */
      jx_int i = size;
      jx_ob *ob = I->data.ob_vla;
      while(i--) {
        total += jx_str_len(*(ob++));
      }
      {
        jx_char *vla = Jx_vla_new(E, 1, total + 1, JX_TRUE);
        if(vla) {
          jx_char *next = vla;
          ob = I->data.ob_vla;
          for(i = 0; i < size; i++) {
            jx_int len = jx_str_len(*ob);
            if(i && sep) {
              jx_os_memcpy(next, sep, sep_len);
              next += sep_len;
            }
            jx_os_memcpy(next, jx_ob_as_str(ob++), len);
            next += len;
          }
          next[0] = 0;
          result = jx_ob_with_str_vla(&vla);
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

static jx_ob jx__ident_join_with_list(jx_env * E, jx_list * I, jx_char * sep)
{
  jx_ob result = jx_ob_from_null();
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_int sep_len = sep ? jx_strlen(sep) : 0;
    jx_int size = jx__list_size(I);
    jx_int total = size ? sep_len * (size - 1) : 0;
    if(!I->packed_meta_bits) {  /* TO DO: handle this */
      if((size == 1) && (!jx_ob_as_ident(I->data.ob_vla)[0])) {
        /* special case -- zero-length identifier */
        return jx_ob_from_ident(".");
      } else {
        jx_int i = size;
        jx_ob *ob = I->data.ob_vla;
        while(i--) {
          total += jx_ident_len(*(ob++));
        }
        {
          jx_char *vla = Jx_vla_new(E, 1, total + 1, JX_TRUE);
          /* +1 for 0-terminator */
          if(vla) {
            jx_char *next = vla;
            ob = I->data.ob_vla;
            for(i = 0; i < size; i++) {
              jx_int len = jx_ident_len(*ob);
              if(i && sep) {
                jx_os_memcpy(next, sep, sep_len);
                next += sep_len;
              }
              jx_os_memcpy(next, jx_ob_as_ident(ob++), len);
              next += len;
            }
            next[0] = 0;
            result = jx_ob_with_ident_vla(&vla);
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_ob Jx_ident_new_from_dotted(jx_env * E, jx_ob ident)
{
  //  jx_jxon_dump(stdout,"ident",jx_ob_from_null(),ident);
  if(jx_list_check(ident)) {
    return jx__ident_join_with_list(JX_NULL, ident.data.io.list, ".");
  } else
    return jx_ob_copy(ident);
}


/* generally speaking, dotted identifiers are expanded into a nested
   list of identifiers */

jx_ob jx_ident_split_from_dotted(jx_env * E, jx_ob ident)
{
  if(jx_ident_check(ident)) {
    jx_char *st = jx_ob_as_ident(&ident);
    if((st[0] == '.') && (!st[1])) {    /* just dot? special case: zero-length identifier */
      jx_ob result = Jx_list_new(E);
      Jx_list_append(E, result, jx_ob_from_ident(""));
      return result;
    } else if(jx_strstr(st, ".")) {     /* dot present */
      jx_ob result = Jx_list_new(E);
      jx_char *next;
      while((next = jx_strstr(st, "."))) {
        jx_int len = (next - st);
        if(len >= 0) {
          Jx_list_append(E, result, jx_ob_from_ident_with_len(st, len));
        } else {
          Jx_ob_free(E, result);
          return jx_ob_from_null();
        }
        st = next + 1;
      }
      Jx_list_append(E, result, jx_ob_from_ident(st));
      return result;
    } else
      return Jx_ob_copy(E, ident);
  } else {
    return jx_ob_from_null();
  }
}

jx_ob jx_ident_split_with_dotted(jx_env * E, jx_ob ident)
{
  if(jx_ident_check(ident)) {
    jx_char *st = jx_ob_as_ident(&ident);
    if((st[0] == '.') && (!st[1])) {    /* just dot? special case: zero-length identifier */
      jx_ob result = Jx_list_new(E);
      Jx_list_append(E, result, jx_ob_from_ident(""));
      Jx_ob_free(E, ident);
      return result;
    } else if(jx_strstr(st, ".")) {     /* dot present */
      jx_ob result = Jx_list_new(E);
      jx_char *next;
      while((next = jx_strstr(st, "."))) {
        jx_int len = (next - st);
        if(len >= 0) {
          Jx_list_append(E, result, jx_ob_from_ident_with_len(st, len));
        } else {
          Jx_ob_free(E, result);
          return jx_null_with_ob(ident);
        }
        st = next + 1;
      }
      Jx_list_append(E, result, jx_ob_from_ident(st));
      Jx_ob_free(E, ident);
      return result;
    } else
      return ident;
  } else {
    return Jx_null_with_ob(E, ident);
  }
}

static jx_status jx_meta_get_packed_size(jx_fast_bits bits)
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

jx_bool jx__list_equal(jx_list * left, jx_list * right)
{
  jx_bool result = JX_FALSE;
  if(left > right) {
    jx_list *tmp = left;
    left = right;
    right = tmp;
  }
  {
    jx_status status = jx_gc_lock(&left->gc);
    if(JX_POS(status)) {
      status = jx_gc_lock(&right->gc);
      if(JX_POS(status)) {

        jx_int left_size = jx__list_size(left);
        jx_int right_size = jx__list_size(right);
        if(left_size != right_size) {
          goto unlock_both;
        } else {
          if(!left_size) {
            result = JX_TRUE;
            goto unlock_both;
          } else if(left->packed_meta_bits != right->packed_meta_bits) {
            /* rare circumstance  -- one packed, but not other */
            int i;
            for(i = 0; i < left_size; i++) {
              if(!jx_ob_equal(jx__list_borrow_locked(left, i),
                              jx__list_borrow_locked(right, i))) {
                goto unlock_both;
              }
            }
            result = JX_TRUE;
          } else if(left->packed_meta_bits) {
            jx_int i;
            switch (left->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
            case JX_META_BIT_INT:
              {
                jx_int *left_int = left->data.int_vla;
                jx_int *right_int = right->data.int_vla;
                for(i = 0; i < left_size; i++) {
                  if(*(left_int++) != *(right_int++))
                    goto unlock_both;
                }
              }
              result = JX_TRUE;
              break;
            case JX_META_BIT_FLOAT:
              {
                jx_float *left_float = left->data.float_vla;
                jx_float *right_float = right->data.float_vla;
                for(i = 0; i < left_size; i++) {
                  if(*(left_float++) != *(right_float++))
                    goto unlock_both;
                }
              }
              result = JX_TRUE;
              break;
            }
          } else {              /* standard objects */
            jx_int i;
            jx_ob *left_ob = left->data.ob_vla;
            jx_ob *right_ob = right->data.ob_vla;
            for(i = 0; i < left_size; i++) {
              if(!jx_ob_equal(*(left_ob++), *(right_ob++)))
                goto unlock_both;
            }
            result = JX_TRUE;
          }
        }
      unlock_both:
        jx_gc_unlock(&right->gc);
      }
      jx_gc_unlock(&left->gc);
    }
  }
  return result;
}

jx_bool jx__list_identical(jx_list * left, jx_list * right)
{
  jx_bool result = JX_FALSE;
  if(left > right) {
    jx_list *tmp = left;
    left = right;
    right = tmp;
  }
  {
    jx_status status = jx_gc_lock(&left->gc);
    if(JX_POS(status)) {
      status = jx_gc_lock(&right->gc);
      if(JX_POS(status)) {
        jx_int left_size = jx__list_size(left);
        jx_int right_size = jx__list_size(right);
        if(left_size != right_size) {
          goto unlock_both;
        } else {
          if(!left_size) {
            result = JX_TRUE;
          } else if(left->packed_meta_bits != right->packed_meta_bits) {
            /* rare circumstance  -- one packed, but not other */
            int i;
            for(i = 0; i < left_size; i++) {
              if(!jx_ob_identical(jx__list_borrow_locked(left, i),
                                  jx__list_borrow_locked(right, i))) {
                goto unlock_both;
              }
            }
            result = JX_TRUE;
          } else if(left->packed_meta_bits) {
            jx_int i;
            switch (left->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
            case JX_META_BIT_INT:
              {
                jx_int *left_int = left->data.int_vla;
                jx_int *right_int = right->data.int_vla;
                for(i = 0; i < left_size; i++) {
                  if(*(left_int++) != *(right_int++))
                    goto unlock_both;
                }
              }
              result = JX_TRUE;
              break;
            case JX_META_BIT_FLOAT:
              {
                jx_float *left_float = left->data.float_vla;
                jx_float *right_float = right->data.float_vla;
                for(i = 0; i < left_size; i++) {
                  if(*(left_float++) != *(right_float++))
                    goto unlock_both;
                }
              }
              result = JX_TRUE;
              break;
            }
          } else {              /* standard objects */
            jx_int i;
            jx_ob *left_ob = left->data.ob_vla;
            jx_ob *right_ob = right->data.ob_vla;
            for(i = 0; i < left_size; i++) {
              if(!jx_ob_identical(*(left_ob++), *(right_ob++)))
                goto unlock_both;
            }
            result = JX_TRUE;
          }
        }
      unlock_both:
        jx_gc_unlock(&right->gc);
      }
      jx_gc_unlock(&left->gc);
    }
  }
  return result;
}

jx_status Jx__list_repack_data_locked(jx_env * E, jx_list * I)
{
  /* re-homogenize (if possible) */
  register jx_int size = jx_vla_size(&I->data.vla);
  if(!size) {
    I->packed_meta_bits = 0;
    if(I->data.vla) {
      Jx_vla_free(E, &I->data.vla);
    }
    return JX_TRUE;
  } else if(I->packed_meta_bits) {
    return JX_SUCCESS;
  } else {
    jx_fast_bits meta_or = 0, meta_and = -1;
    {
      register jx_ob *ob = I->data.ob_vla;
      register jx_fast_bits abort_mask = (JX_META_BIT_BOOL |
                                          JX_META_BIT_LIST |
                                          JX_META_BIT_HASH | JX_META_BIT_STR);
      register jx_int i;
      for(i = 0; i < size; i++) {
        register jx_fast_bits bits = (ob++)->meta.bits;
        meta_or |= bits;
        meta_and &= bits;
        if((!meta_and) || (bits & abort_mask))
          return JX_FAILURE;
      }
    }
    if(meta_and == meta_or) {   /* all meta bits identical in all objects */
      switch (meta_or) {
      case JX_META_BIT_INT:
      case JX_META_BIT_FLOAT:
        {
          void *new_vla = NULL;
          switch (meta_or) {
          case JX_META_BIT_INT:
            new_vla = Jx_vla_new(E, sizeof(jx_int), size, JX_FALSE);
            if(new_vla) {
              jx_ob *src = I->data.ob_vla;
              jx_int *dst = (jx_int *) new_vla;
              jx_int i;
              for(i = 0; i < size; i++) {
                *(dst++) = jx_ob_as_int(*(src++));
              }
            }
            break;
          case JX_META_BIT_FLOAT:
            new_vla = Jx_vla_new(E, sizeof(jx_float), size, JX_FALSE);
            if(new_vla) {
              jx_ob *src = I->data.ob_vla;
              jx_float *dst = (jx_float *) new_vla;
              jx_int i;
              for(i = 0; i < size; i++) {
                *(dst++) = jx_ob_as_int(*(src++));
              }
            }
            break;
          }
          if(new_vla) {
            Jx_vla_free(E, &I->data.vla);
            I->data.vla = new_vla;
            I->packed_meta_bits = meta_or;
            return JX_SUCCESS;
          }
        }
        break;
      default:
        break;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_unpack_data_locked(jx_list * I)
{
  /* homogenous list becoming hetergeneous */
  jx_int size = jx_vla_size(&I->data.vla);
  if(!size) {
    I->packed_meta_bits = 0;
    if(I->data.vla) {
      jx_vla_free(&I->data.vla);
    }
    return JX_SUCCESS;
  }
  jx_ob *new_vla = jx_vla_new(sizeof(jx_ob), size);
  if(new_vla) {
    switch (I->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_INT:
      {
        jx_int i;
        jx_int *src = I->data.int_vla;
        jx_ob *dst = new_vla;
        for(i = 0; i < size; i++) {
          *(dst++) = jx_ob_from_int(*(src++));
        }
      }
      break;
    case JX_META_BIT_FLOAT:
      {
        jx_int i;
        jx_float *src = I->data.float_vla;
        jx_ob *dst = new_vla;
        for(i = 0; i < size; i++) {
          *(dst++) = jx_ob_from_float(*(src++));
        }
      }
      break;
    }
    jx_vla_free(&I->data.vla);
    I->data.vla = new_vla;
    I->packed_meta_bits = 0;
    return JX_SUCCESS;
  } else
    return JX_FAILURE;
}

jx_status jx__list_resize(jx_env * E, jx_list * I, jx_int size, jx_ob fill)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else {
      if(size >= 0) {
        jx_int old_size = jx__list_size(I);
        if(I->packed_meta_bits != fill.meta.bits) {
          if(I->packed_meta_bits) {
            if(!jx_ok(jx__list_unpack_data(I)))
              goto unlock;
          }
          if(!old_size)
            Jx_vla_free(E, &I->data.vla);
        }
        if(!I->data.vla) {      /* filling an empty list */
          jx_int packed_size = jx_meta_get_packed_size(fill.meta.bits);
          if(packed_size) {
            /* filling with packed data */
            I->packed_meta_bits = fill.meta.bits;
            I->data.vla = Jx_vla_new(E, packed_size, size, JX_FALSE);
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
              status = JX_SUCCESS;
              goto unlock;
            }
          } else {
            /* filling with normal objects */
            I->data.vla = Jx_vla_new(E, sizeof(jx_ob), size, JX_FALSE);
            if(I->data.vla) {
              jx_int i;
              jx_ob *ob = I->data.ob_vla;
              if(fill.meta.bits & JX_META_BIT_GC) {
                for(i = 0; i < size; i++) {
                  *(ob++) = Jx_ob_copy(E, fill);
                }
              } else {
                for(i = 0; i < size; i++) {
                  *(ob++) = fill;
                }
              }
              status = JX_SUCCESS;
              goto unlock;
            }
          }
        } else {
          /* existing list is not empty, but data sizes are known to match */
          if(!(fill.meta.bits & JX_META_BIT_GC)) {
            /* filling with a non GC object, possibly packed */
            if(!I->packed_meta_bits) {  /* not packed data */
              if(size < old_size) {     /* must freee some existing objects */
                jx_int i;
                jx_ob *ob = I->data.ob_vla + size;
                for(i = size; i < old_size; i++) {
                  Jx_ob_free(E, JX_DISOWN(E, *(ob++)));
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
                status = JX_SUCCESS;
                goto unlock;
              }
            } else {            /* implies I->packed_meta_bits == fill.meta.bits */
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
                status = JX_SUCCESS;
                goto unlock;
              }
            }
          } else {
            /* filling with a GC object, never packed  */
            if(size < old_size) {       /* must freee some existing objects */
              jx_int i;
              jx_ob *ob = I->data.ob_vla + size;
              for(i = size; i < old_size; i++) {
                Jx_ob_free(E, JX_DISOWN(E, *(ob++)));
              }
            }
            if(jx_ok(jx_vla_resize(&I->data.vla, size))) {
              if(size > old_size) {
                jx_int i;
                jx_ob *ob = I->data.ob_vla + old_size;
                for(i = old_size; i < size; i++) {
                  *(ob++) = Jx_ob_copy(E, fill);
                }
              }
              status = JX_SUCCESS;
              goto unlock;
            }
          }
        }
      }
    }
  unlock:
    jx_gc_unlock(&I->gc);
  }
  return status;
}

JX_INLINE jx_status jx__list__append_locked(jx_env * E, jx_list * I, jx_ob ob)
{
  if(I->gc.shared) {
    return JX_STATUS_PERMISSION_DENIED;
  }
  if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
    if(!jx_ok(jx__list_unpack_data_locked(I))) {
      return JX_FAILURE;
    }
  }
  if(I->data.vla) {
    jx_int size = jx_vla_size(&I->data.vla);
    if(jx_vla_grow_check(&I->data.vla, size)) {
      if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
        jx__list_set_packed_data_locked(I, size, ob);
      } else if(!I->packed_meta_bits) {
        I->data.ob_vla[size] = JX_OWN(E, ob);
      }
      return JX_SUCCESS;
    }
  } else {
    jx_int packed_size = jx_meta_get_packed_size(ob.meta.bits);
    if(packed_size) {
      I->packed_meta_bits = ob.meta.bits;
      I->data.vla = Jx_vla_new(E, packed_size, 1, JX_FALSE);
      if(I->data.vla) {
        jx__list_set_packed_data_locked(I, 0, ob);
        return JX_SUCCESS;
      }
    } else {
      I->data.vla = Jx_vla_new(E, sizeof(jx_ob), 1, JX_FALSE);
      if(I->data.vla) {
        I->data.ob_vla[0] = JX_OWN(E, ob);
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_append_locked(jx_env * E, jx_list * I, jx_ob ob)
{
  return jx__list__append_locked(E, I, ob);
}

jx_status jx__list_append(jx_env * E, jx_list * I, jx_ob ob)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = jx__list__append_locked(E, I, ob);
    jx_gc_unlock(&I->gc);
  }
  return status;
}

JX_INLINE jx_status jx__list__insert_locked(jx_env * E, jx_list * I, jx_int index, jx_ob ob)
{
  if(I->gc.shared) {
    return JX_STATUS_PERMISSION_DENIED;
  }
  if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
    if(!jx_ok(jx__list_unpack_data_locked(I))) {
      return JX_FAILURE;
    }
  }
  if(I->data.vla) {
    index = (index < 0) ? 1 + jx_vla_size(&I->data.vla) + index : index;
    if(!jx_ok(jx_vla_insert(&I->data.vla, index, 1)))
      return JX_FAILURE;
    else if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
      jx__list_set_packed_data_locked(I, index, ob);
    } else if(!I->packed_meta_bits) {
      I->data.ob_vla[index] = JX_OWN(E, ob);
    }
    return JX_SUCCESS;
  } else if(index == 0) {
    jx_int packed_size = jx_meta_get_packed_size(ob.meta.bits);
    if(packed_size) {
      I->packed_meta_bits = ob.meta.bits;
      I->data.vla = jx_vla_new(packed_size, 1);
      if(I->data.vla) {
        jx__list_set_packed_data_locked(I, 0, ob);
        return JX_SUCCESS;
      }
    } else {
      I->data.vla = jx_vla_new(sizeof(jx_ob), 1);
      if(I->data.vla) {
        I->data.ob_vla[0] = JX_OWN(E, ob);
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__list_insert_locked(jx_env * E, jx_list * I, jx_int index, jx_ob ob)
{
  return jx__list__insert_locked(E, I, index, ob);
}

jx_status jx__list_insert(jx_env * E, jx_list * I, jx_int index, jx_ob ob)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = jx__list__insert_locked(E,I,index,ob);    
    jx_gc_unlock(&I->gc);
  }
  return status;
}

jx_status jx__list_combine(jx_env * E, jx_list * list1, jx_list * list2)
{
  /* we're assuming on entry that list2 is not synchronized 
     (since we're consuming it)... */
  jx_status status = jx_gc_lock(&list1->gc);
  if(JX_POS(status)) {
    if(JX_POS(status)) {
      /* consumes list2 */
      if(list1->gc.shared || list2->gc.shared) {
        status = JX_STATUS_PERMISSION_DENIED;
      } else if(list1 == list2) {
        jx_gc_unlock(&list1->gc);
        {
          jx_ob ob = jx__list_copy_strong(E, list2);
          if(jx_ok(jx__list_combine(E, list1, ob.data.io.list))) {
            return JX_SUCCESS;
          } else {
            Jx_ob_free(E, JX_DISOWN(E, ob));
          }
        }
      } else {
        jx_int list1_size = jx__list_size(list1);
        jx_int list2_size = jx__list_size(list2);

        if(!list1_size) {
          list1->packed_meta_bits = 0;
          jx_vla_free(&list1->data.vla);
        }
        if(!list2_size) {
          list2->packed_meta_bits = 0;
          jx_vla_free(&list2->data.vla);
        }
        if(list1_size && list2_size) {
          if(list1->packed_meta_bits != list2->packed_meta_bits) {
            if(list1->packed_meta_bits) {
              if(!jx_ok(jx__list_unpack_data_locked(list1)))
                goto unlock;
            }
            if(list2->packed_meta_bits) {
              if(!jx_ok(jx__list_unpack_data_locked(list2)))
                goto unlock;
            }
          }
        }
        if(list1->data.vla && list2->data.vla) {
          if(jx_ok(jx__vla_extend(&list1->data.vla, &list2->data.vla))) {
            jx_vla_reset(&list2->data.vla);
            jx__list_free(NULL, list2);
            status = JX_SUCCESS;
            goto unlock;
          }
        } else if(list2->data.vla) {
          list1->data.vla = list2->data.vla;
          list1->packed_meta_bits = list2->packed_meta_bits;
          list2->data.vla = NULL;
          list2->packed_meta_bits = 0;
          jx__list_free(NULL, list2);
          status = JX_SUCCESS;
          goto unlock;
        }
      }
    }
  unlock:
    jx_gc_unlock(&list1->gc);
  }
  return status;

}

jx_ob jx__list_remove(jx_list * I, jx_int index)
{
  jx_ob result = jx_ob_from_null();
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(!I->gc.shared) {
      jx_int size = jx_vla_size(&I->data.vla);
      index = (index < 0) ? size + index : index;
      if((index >= 0) && (index < size)) {
        if(I->packed_meta_bits) {
          result = jx__list_get_packed_data_locked(I, index);
          jx_vla_remove(&I->data.vla, index, 1);
        } else {
          result = I->data.ob_vla[index];
          jx_vla_remove(&I->data.vla, index, 1);
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_ob jx__list_new_from_slice(jx_env * E, jx_list * I, jx_int start, jx_int stop)
{
  jx_ob result = jx_ob_from_null();
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_int size = jx_vla_size(&I->data.vla);
    if(start < 0)
      start = size + start + 1;
    if(stop < 0)
      stop = size + stop + 1;
    if(start < 0)
      start = 0;
    else if(start > size)
      start = size;
    if(stop < 0)
      stop = 0;
    if(stop > size)
      stop = size;
    {
      jx_int new_size = stop - start;
      result = Jx_list_new(E);
      {
        jx_list *new_I = result.data.io.list;
        *(new_I) = *I;
        jx__gc_init(&new_I->gc);
        new_I->data.vla = jx_vla_new_from_subset(&I->data.vla, start, new_size);
        if(!I->packed_meta_bits) {
          jx_ob *ob = new_I->data.vla;
          while(new_size--) {
            if(ob->meta.bits & JX_META_BIT_GC) {
              *ob = Jx_ob_copy(E, *ob);
            }
            ob++;
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_ob jx__list_new_with_cutout(jx_env * E, jx_list * I, jx_int start, jx_int stop)
{
  jx_ob result = jx_ob_from_null();
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_int size = jx_vla_size(&I->data.vla);
    if(start < 0)
      start = size + start + 1;
    if(stop < 0)
      stop = size + stop + 1;
    if(start < 0)
      start = 0;
    else if(start > size)
      start = size;
    if(stop < 0)
      stop = 0;
    if(stop > size)
      stop = size;
    {
      jx_int new_size = stop - start;
      result = Jx_list_new(E);
      {
        jx_list *new_I = result.data.io.list;
        *(new_I) = *I;
        jx__gc_init(&new_I->gc);
        new_I->data.vla = jx_vla_new_from_subset(&I->data.vla, start, new_size);
        jx_vla_remove(&I->data.vla, start, new_size);
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_status jx__list_delete(jx_env * E, jx_list * I, jx_int index)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
      if(!I->packed_meta_bits) {
        Jx_ob_free(E, JX_DISOWN(E, I->data.ob_vla[index]));
      }
      status = jx_vla_remove(&I->data.vla, index, 1);
    }
    jx_gc_unlock(&I->gc);
  }
  return status;
}

jx_int *jx_list_as_int_vla(jx_ob ob)
{
  jx_list *I = ob.data.io.list;
  jx_int *result = JX_NULL;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(ob.meta.bits & JX_META_BIT_LIST) {
      if(I->packed_meta_bits & JX_META_BIT_INT) {
        result = I->data.int_vla;
      } else if(jx_ok(Jx__list_repack_data_locked(JX_NULL, I))) {
        if(I->packed_meta_bits & JX_META_BIT_INT) {
          result = I->data.int_vla;
        } else if(!I->data.vla) {
          I->packed_meta_bits = JX_META_BIT_INT;
          I->data.int_vla = (jx_int *) jx_vla_new(sizeof(jx_int), 0);
          result = I->data.int_vla;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_status jx__list_set_int_vla(jx_list * I, jx_int ** ref)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared)
      status = JX_STATUS_PERMISSION_DENIED;
    else {
      if(I->packed_meta_bits & JX_META_BIT_INT) {
        I->data.int_vla = *ref;
        status = JX_SUCCESS;
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return status;
}

jx_float *jx_list_as_float_vla(jx_ob ob)
{
  jx_list *I = ob.data.io.list;
  jx_float *result = JX_NULL;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(ob.meta.bits & JX_META_BIT_LIST) {
      if(I->packed_meta_bits & JX_META_BIT_FLOAT) {
        result = I->data.float_vla;
      } else if(Jx__list_repack_data_locked(JX_NULL, I)) {
        if(I->packed_meta_bits & JX_META_BIT_FLOAT) {
          result = I->data.float_vla;
        } else if(!I->data.vla) {
          I->packed_meta_bits = JX_META_BIT_FLOAT;
          I->data.float_vla = (jx_float *) jx_vla_new(sizeof(jx_float), 0);
          result = I->data.float_vla;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_status jx__list_set_float_vla(jx_list * I, jx_float ** ref)
{
  if(I->gc.shared)
    return JX_FAILURE;
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
  jx_fast_bits bits = ob.meta.bits;
  if(bits & (JX_META_BIT_STR | JX_META_BIT_IDENT)) {
    if(bits & JX_META_BIT_STR)
      return jx__c_str_hash(jx_ob_as_str(&ob));
    else
      return jx__c_str_hash(jx_ob_as_ident(&ob));
  } else if(bits & JX_META_BIT_BUILTIN) {
    if((bits & JX_META_MASK_BUILTIN_TYPE) == JX_META_BIT_BUILTIN_ENTITY)
      return jx__c_str_hash(jx_ob_as_ident(&ob));       /* entity handles are merely cloaked identifiers */
    return jx__ob_non_gc_hash_code(ob);
  } else {
    return 0;                   /* unhashable */
  }
}


/* hash objects */

jx_ob jx_hash_new(void)
{
  jx_ob result = JX_OB_HASH;
  jx_hash *I = (jx_hash *) jx_calloc(1, sizeof(jx_hash));
  if(I) {
    result.data.io.hash = I;
    return result;
  }
  return jx_ob_from_null();
}

static jx_bool Jx__hash_recondition(jx_env * E, jx_hash * I, jx_int mode, jx_bool pack);

jx_ob Jx_hash_new_with_flags(jx_env * E, jx_int flags)
{
  jx_ob result = Jx_hash_new(E);
  if(result.meta.bits & JX_META_BIT_HASH) {
    if(flags & JX_HASH_FLAG_BIDIRECTIONAL) {
      if(!Jx__hash_recondition(E, result.data.io.hash, JX_HASH_ONE_TO_ONE, JX_TRUE)) {
        Jx_ob_free(E, result);
        result = jx_ob_from_null();
      }
    }
  }
  return result;
}

jx_ob jx_hash_new_with_flags(jx_int flags)
{
  return Jx_hash_new_with_flags(JX_NULL, flags);
}

jx_ob Jx__hash_copy(jx_env * E, jx_hash * hash)
{
  jx_ob result = Jx_hash_new(E);
  jx_status status = jx_gc_lock(&hash->gc);
  //  printf("hash copy %d %d\n",hash->info ? ((jx_hash_info*)hash->info)->usage : -1,
  //         hash->info ? -1 : jx_vla_size(&hash->key_value));
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_HASH) {
      jx_hash *I = result.data.io.hash;
      *I = *hash;
      jx__gc_init(&I->gc);
      I->info = Jx_vla_copy(E, &hash->info);
      I->key_value = Jx_vla_copy(E, &hash->key_value);
      {
        jx_hash_info *info = (jx_hash_info *) I->info;
        if((!info) || (info->mode == JX_HASH_LINEAR)) {
          jx_int i, size = jx_vla_size(&I->key_value);
          jx_ob *ob = I->key_value;
          for(i = 0; i < size; i++) {
            if(ob->meta.bits & JX_META_BIT_GC) {
              *ob = Jx_ob_copy(E, *ob);
            }
            ob++;
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
                if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                  jx_ob *kv_ob =
                    key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                  kv_ob[0] = Jx_ob_copy(E, kv_ob[0]);
                  kv_ob[1] = Jx_ob_copy(E, kv_ob[1]);
                }
                index++;
              } while(index <= mask);
            }
            break;
          }
        }
      }
    }
    jx_gc_unlock(&hash->gc);
  }
  return result;
}

jx_ob Jx__hash_copy_strong(jx_env * E, jx_hash * hash)
{
  jx_ob result = Jx_hash_new(E);
  jx_status status = jx_gc_lock(&hash->gc);
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_HASH) {
      jx_hash *I = result.data.io.hash;
      *I = *hash;
      jx__gc_init(&I->gc);
      I->info = Jx_vla_copy(E, &hash->info);
      I->key_value = Jx_vla_copy(E, &hash->key_value);

      {
        jx_hash_info *info = (jx_hash_info *) I->info;
        if((!info) || (info->mode == JX_HASH_LINEAR)) {
          jx_int i, size = jx_vla_size(&I->key_value);
          jx_ob *ob = I->key_value;
          for(i = 0; i < size; i++) {
            if(ob->meta.bits & JX_META_BIT_GC) {
              *ob = Jx_ob_copy_strong(E, *ob);
            }
            ob++;
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
                if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                  jx_ob *kv_ob =
                    key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                  kv_ob[0] = Jx_ob_copy_strong(E, kv_ob[0]);
                  kv_ob[1] = Jx_ob_copy_strong(E, kv_ob[1]);
                }
                index++;
              } while(index <= mask);
            }
            break;
          }
        }
      }
    }
    jx_gc_unlock(&hash->gc);
  }
  return result;
}

#if 0
static void jx__hash_make_strong(jx_hash * I)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_int i, size = jx_vla_size(&I->key_value);
    jx_ob *ob = I->key_value;
    for(i = 0; i < size; i++) {
      if(ob->meta.bits & JX_META_BIT_GC) {
        *ob = jx_ob_make_strong_with_ob(*ob);
      }
      ob++;
    }
    jx_gc_unlock(&I->gc);
  }
}

static void jx__hash_only_strong(jx_hash * I)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {

    jx_int i, size = jx_vla_size(&I->key_value);
    jx_ob *ob = I->key_value;
    for(i = 0; i < size; i++) {
      if(ob->meta.bits & JX_META_BIT_GC) {
        *ob = jx_ob_only_strong_with_ob(*ob);
      }
      ob++;
    }
    jx_gc_unlock(&I->gc);
  }
}
#endif

JX_INLINE void Jx__hash_free(jx_env * E, jx_hash * hash)
{
  if(E && (E->n_hash < JX_TLS_MAX)) {
    jx_mem_chain *chain = (jx_mem_chain *) hash;
    chain->next = E->hash_chain;
    E->hash_chain = chain;
    E->n_hash++;
  } else {
    jx_free(hash);
  }
}

JX_INLINE jx_status jx__hash_free(jx_env * E, jx_hash * I)
{                               /* assumes I not shared */
  jx_status status = JX_SUCCESS;
  jx_int size = jx_vla_size(&I->key_value);
  if(size) {
    jx_ob *ob = I->key_value;
    jx_int i;
    for(i = 0; i < size; i++) {
      Jx_ob_free(E, JX_DISOWN(E, *(ob++)));
    }
  }
  Jx_vla_free(E, &I->key_value);
  Jx_vla_free(E, &I->info);
  Jx__hash_free(E, I);
  return status;
}

jx_int jx__hash_size(jx_hash * I)
{
  jx_int result = 0;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(I->key_value) {
      if(!I->info) {            /* no info mode -- search & match objects directly  */
        result = jx_vla_size(&I->key_value) >> 1;
      } else {
        jx_hash_info *info = (jx_hash_info *) I->info;
        result = info->usage;
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

JX_INLINE jx_uint32 jx__new_mask_from_min_size(jx_uint32 min_size)
{
  jx_uint32 new_mask = 0;
  jx_uint32 threshold = ((3 * min_size) >> 1);
  jx_uint32 tmp_usage = threshold;
  while(tmp_usage) {
    new_mask = (new_mask << 1) + 1;
    tmp_usage = (tmp_usage >> 1);
  }
  while(new_mask < threshold) {
    new_mask = (new_mask << 1);
  }
  return new_mask;
}

JX_INLINE void jx__pair_dump(FILE * f, char *prefix, jx_ob key, jx_ob value)
{
#if (JX_TINY_STR_SIZE <= 6)
  fprintf(f, "%s: %08x%04x %04x -> %08x%04x %04x", prefix,
          (unsigned int) key.data.raw.word,
          (unsigned int) key.meta.fill, (unsigned int) key.meta.bits,
          (unsigned int) value.data.raw.word,
          (unsigned int) value.meta.fill, (unsigned int) value.meta.bits);
#endif

#if (JX_TINY_STR_SIZE == 10)
  fprintf(f, "%s: %08x%08x%04x %04x -> %08x%08x%04x %04x", prefix,
          (unsigned int) (key.data.raw.word), (unsigned int) (key.data.raw.word >> 32),
          (unsigned int) key.meta.fill, (unsigned int) key.meta.bits,
          (unsigned int) (value.data.raw.word),
          (unsigned int) (value.data.raw.word >> 32), (unsigned int) value.meta.fill,
          (unsigned int) value.meta.bits);
#endif

#if (JX_TINY_STR_SIZE == 12)
  fprintf(f, "%s: %08x%08x%08x %08x -> %08x%08x%08x %08x", prefix,
          (unsigned int) (key.data.raw.word), (unsigned int) (key.data.raw.word >> 32),
          (unsigned int) key.meta.fill, (unsigned int) key.meta.bits,
          (unsigned int) (value.data.raw.word),
          (unsigned int) (value.data.raw.word >> 32), (unsigned int) value.meta.fill,
          (unsigned int) value.meta.bits);
#endif

#if (JX_TINY_STR_SIZE == 14)
  fprintf(f, "%s: %08x%08x%08x%08x%08x %08x -> %08x%08x%08x%08x%08x %08x", prefix,
          (unsigned int) (key.data.raw.word[0]), (unsigned int) (key.data.raw.word[1]),
          (unsigned int) (key.data.raw.word[2]),
          (unsigned int) key.meta.fill, (unsigned int) key.meta.bits,
          (unsigned int) (value.data.raw.word[0]),
          (unsigned int) (value.data.raw.word[1]),
          (unsigned int) (value.data.raw.word[2]), (unsigned int) value.meta.fill,
          (unsigned int) value.meta.bits);
#endif

#if (JX_TINY_STR_SIZE == 20)
  fprintf(f, "%s: %08x%08x%08x%08x%08x %08x -> %08x%08x%08x%08x%08x %08x", prefix,
          (unsigned int) (key.data.raw.word[0]),
          (unsigned int) (key.data.raw.word[0] >> 32),
          (unsigned int) (key.data.raw.word[1]),
          (unsigned int) (key.data.raw.word[1] >> 32), (unsigned int) key.meta.fill,
          (unsigned int) key.meta.bits, (unsigned int) (value.data.raw.word[0]),
          (unsigned int) (value.data.raw.word[0] >> 32),
          (unsigned int) (value.data.raw.word[1]),
          (unsigned int) (value.data.raw.word[1] >> 32), (unsigned int) value.meta.fill,
          (unsigned int) value.meta.bits);
#endif
  {
    switch (key.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_INT:
    case JX_META_BIT_BOOL:
    case JX_META_BIT_BUILTIN:
      fprintf(f, " %d", (int) key.data.io.int_);
      break;
    case JX_META_BIT_STR:
      fprintf(f, " %s", jx_ob_as_str(&key));
      break;
    case JX_META_BIT_IDENT:
      fprintf(f, " %s", jx_ob_as_ident(&key));
      break;
    }
  }
  printf(" ->");
  {
    switch (value.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_INT:
    case JX_META_BIT_BOOL:
    case JX_META_BIT_BUILTIN:
      fprintf(f, " %d", (int) value.data.io.int_);
      break;
    case JX_META_BIT_STR:
      fprintf(f, " %s", jx_ob_as_str(&value));
      break;
    case JX_META_BIT_IDENT:
      fprintf(f, " %s", jx_ob_as_ident(&value));
      break;
    }
  }
  printf("\n");
}

void jx__hash_dump(FILE * file, jx_hash * I)
{
  if(!I->info) {
    jx_int usage = jx_vla_size(&I->key_value) >> 1;
    fprintf(file, "hash: mode RAW\n");
    fprintf(file, "hash: usage %d (from vla_size)\n", (int) usage);
    {
      int i;
      jx_ob *ob = I->key_value;
      char buffer[20];
      for(i = 0; i < usage; i++) {
        sprintf(buffer, "key->value %3d", i);
        jx__pair_dump(file, buffer, ob[0], ob[1]);
        ob += 2;
      }
    }
  } else {
    jx_hash_info *info = (jx_hash_info *) I->info;
    jx_int usage = info->usage;
    switch (info->mode) {
    case JX_HASH_LINEAR:
      fprintf(file, "hash: mode LINEAR\n");
      break;
    case JX_HASH_ONE_TO_ANY:
      fprintf(file, "hash: mode ONE-TO-ANY\n");
      break;
    case JX_HASH_ONE_TO_ONE:
      fprintf(file, "hash: mode ONE-TO-ONE\n");
      break;
    case JX_HASH_ONE_TO_NIL:
      fprintf(file, "hash: mode ONE-TO-NIL\n");
      break;
    }
    fprintf(file, "hash: usage %d (from info)\n", (int) usage);
    fprintf(file, "hash: stale_usage %d (from info)\n", (int) info->stale_usage);
    fprintf(file, "hash: vla size / 2 = %d\n", (int) jx_vla_size(&I->key_value) >> 1);
    fprintf(file, "hash: mask 0x%x \n", (unsigned int) info->mask);
    switch (info->mode) {
    case JX_HASH_LINEAR:
      {
        int i;
        jx_uint32 *hash_entry = info->table;
        jx_ob *ob = I->key_value;
        char buffer[50];
        for(i = 0; i < usage; i++) {
          sprintf(buffer, "%03d hash %8x kv ", i, *hash_entry);
          jx__pair_dump(file, buffer, ob[0], ob[1]);
          hash_entry++;
          ob += 2;
        }
      }
      break;
    default:
      {
        int i;
        jx_uint32 *hash_entry = info->table;
        jx_ob *ob = I->key_value;
        char buffer[50];
        for(i = 0; i <= info->mask; i++) {
          if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
            sprintf(buffer, "%3x hash %8x -> %8x kv ", i, hash_entry[0],
                    hash_entry[1] >> 1);
          } else if(hash_entry[1] & JX_HASH_ENTRY_DELETED) {
            sprintf(buffer, "%3x hash %8x ->     dele kv ", i, hash_entry[0]);
          } else {
            sprintf(buffer, "%3x hash %8x ->          kv ", i, hash_entry[0]);
          }
          if(i < usage) {
            jx__pair_dump(file, buffer, ob[0], ob[1]);
          } else {
            printf("%s\n", buffer);
          }
          hash_entry += 2;
          ob += 2;
        }
      }
      if(info->mode == JX_HASH_ONE_TO_ONE) {
        int i;
        jx_uint32 *hash_entry = info->table + ((info->mask + 1) << 1);
        jx_ob *ob = I->key_value;
        char buffer[50];
        fprintf(file, "(reverse hash)\n");
        for(i = 0; i <= info->mask; i++) {
          if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
            sprintf(buffer, "%3x hash %8x -> %8x kv ", i, hash_entry[0],
                    hash_entry[1] >> 1);
          } else if(hash_entry[1] & JX_HASH_ENTRY_DELETED) {
            sprintf(buffer, "%3x hash %8x ->     dele kv ", i, hash_entry[0]);
          } else {
            sprintf(buffer, "%3x hash %8x ->          kv ", i, hash_entry[0]);
          }
          if(i < usage) {
            jx__pair_dump(file, buffer, ob[0], ob[1]);
          } else {
            printf("%s\n", buffer);
          }
          hash_entry += 2;
          ob += 2;
        }

      }
      break;
    }
    //    fprintf(file,"hash_dump: mode %d\n",info.mode);

  }
}

static jx_bool Jx__hash_recondition(jx_env * E, jx_hash * I, jx_int mode, jx_bool pack)
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
      I->info =
        Jx_vla_new(E, sizeof(jx_uint32), (2 * min_size) + JX_HASH_INFO_SIZE, JX_TRUE);
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
      I->key_value = Jx_vla_new(E, sizeof(jx_ob), 0, JX_TRUE);
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
      Jx_vla_free(E, &I->info);
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
        Jx_vla_free(E, &I->info);
        break;
      case JX_HASH_ONE_TO_ANY: /* from */
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_hash_info *old_info = (jx_hash_info *) I->info;
          jx_ob *new_key_value = Jx_vla_new(E, sizeof(jx_ob), (usage << 1), JX_TRUE);
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
                  *(new_kv++) = old_key_value[old_kv_offset + 1];
                  /* for ONE_TO_NIL, these will be null */
                }
                old_hash_entry += 2;
              }
            }
            Jx_vla_free(E, &I->key_value);
            I->key_value = new_key_value;
            Jx_vla_free(E, &I->info);
          }
        }
        break;
      }
      break;
    case JX_HASH_LINEAR:       /* to */
      switch (old_mode) {
      case JX_HASH_RAW:        /* from */
        {
          I->info =
            Jx_vla_new(E, sizeof(jx_uint32), usage + JX_HASH_INFO_SIZE, JX_TRUE);
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
          jx_ob *new_key_value = Jx_vla_new(E, sizeof(jx_ob), (usage << 1), JX_TRUE);
          jx_hash_info *new_info = (jx_hash_info *) Jx_vla_new(E, sizeof(jx_uint32),
                                                               usage + JX_HASH_INFO_SIZE,
                                                               JX_TRUE);
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
            Jx_vla_free(E, &I->key_value);
            I->key_value = new_key_value;
            Jx_vla_free(E, &I->info);
            I->info = (jx_uint32 *) new_info;
          } else {
            Jx_vla_free(E, &new_key_value);
            Jx_vla_free(E, &new_info);
          }
        }
        break;
      }
      break;
    case JX_HASH_ONE_TO_ANY:   /* to */
    case JX_HASH_ONE_TO_NIL:
      {
        jx_uint32 new_mask = jx__new_mask_from_min_size(min_size);
        //        printf("recondition new mask %x\n",new_mask);

        {
          /* prepare new info block */
          jx_hash_info *new_info = (jx_hash_info *) Jx_vla_new(E, sizeof(jx_uint32),
                                                               2 * (new_mask + 1) +
                                                               JX_HASH_INFO_SIZE,
                                                               JX_TRUE);
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
                Jx_vla_free(E, &I->info);
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
                Jx_vla_free(E, &I->info);
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
                  jx_ob *new_key_value = Jx_vla_new(E, sizeof(jx_ob),
                                                    (usage << 1), JX_TRUE);
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash_entry = old_info->table;
                    jx_uint32 *hash_table = new_info->table;
                    jx_uint32 i = old_info->mask + 1;
                    jx_uint32 new_kv_offset = 0;
                    while(i--) {
                      if(old_hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                        /* only process actives */
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
                    Jx_vla_free(E, &I->key_value);
                    I->key_value = new_key_value;
                    Jx_vla_free(E, &I->info);
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
                  Jx_vla_free(E, &I->info);
                  I->info = (jx_uint32 *) new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            }
            /* if something went wrong, then we simply free new new info and keep the old */
            Jx_vla_free(E, &new_info);
          }
        }
      }
      break;
    case JX_HASH_ONE_TO_ONE:   /* to */
      {
        jx_uint32 new_mask = jx__new_mask_from_min_size(min_size);
        {
          /* prepare new info block */
          jx_hash_info *new_info = Jx_vla_new(E, sizeof(jx_uint32),
                                              4 * (new_mask + 1) + JX_HASH_INFO_SIZE,
                                              JX_TRUE);
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
                  Jx_vla_free(E, &I->info);
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
                  Jx_vla_free(E, &I->info);
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
                  jx_ob *new_key_value =
                    Jx_vla_new(E, sizeof(jx_ob), (usage << 1), JX_TRUE);
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
                      Jx_vla_free(E, &I->key_value);
                      I->key_value = new_key_value;
                      new_key_value = JX_NULL;
                      Jx_vla_free(E, &I->info);
                      I->info = (jx_uint32 *) new_info;
                      new_info = JX_NULL;
                    }
                    Jx_vla_free(E, &new_key_value);
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
                    Jx_vla_free(E, &I->info);
                    I->info = (jx_uint32 *) new_info;
                    new_info = JX_NULL;
                  }
                }
              }
              break;
            case JX_HASH_ONE_TO_ONE:   /* from */
              {
                jx_hash_info *old_info = (jx_hash_info *) I->info;
                if(pack || (new_mask < old_info->mask)) {       /* smaller than before... */
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value =
                    Jx_vla_new(E, sizeof(jx_ob), (usage << 1), JX_TRUE);
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
                    Jx_vla_free(E, &I->key_value);
                    I->key_value = new_key_value;
                    Jx_vla_free(E, &I->info);
                    I->info = (jx_uint32 *) new_info;
                    new_info = JX_NULL;
                  }
                } else if(new_mask > old_info->mask) {  /* getting bigger! */
                  /* we copy stale entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash_entry = old_info->table;
                  jx_uint32 *hash_table = new_info->table;
                  jx_uint32 *rev_hash_table = new_info->table + ((new_mask + 1) << 1);
                  jx_ob *key_value = I->key_value;
                  new_info->stale_usage = old_info->stale_usage;
                  if(old_info->mask) {
                    jx_uint32 i = old_info->mask + 1;
                    while(i--) {
                      if(old_hash_entry[1]) {   /* only skip never-used slots */

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
                            index = (index + 1) & new_mask;
                          } while(index != sentinel);
                        }
                      }
                      old_hash_entry += 2;
                    }
                  }
                  Jx_vla_free(E, &I->info);
                  I->info = (jx_uint32 *) new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            }
            /* if something went wrong, then we simply free new new info and keep the old */
            Jx_vla_free(E, &new_info);
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

jx_status Jx__hash_set(jx_env * E, jx_hash * I, jx_ob key, jx_ob value)
{
  //  printf("hash_set %p %p\n",E,env ? E->incref.base : 0);
  //jx_jxon_dump(stdout,"value",value);
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else {
      jx_uint32 hash_code = jx_ob_hash_code(key);
      if(!hash_code) {          /* unhashable key */
        status = JX_STATUS_OB_NOT_HASHABLE;
      } else {
        jx_hash_info *info;
        if(!(info = (jx_hash_info *) I->info)) {

          /* "no info" mode -- search & match objects directly  */
          if(!I->key_value) {
            jx_ob *ob;
            /* new table, first entry  */
            I->key_value = Jx_vla_new(E, sizeof(jx_ob), 2, JX_FALSE);
            if((ob = I->key_value)) {
              ob[0] = JX_OWN(E, key); /* takes ownership */
              ob[1] = JX_OWN(E, value);       /* takes ownership */
              status = JX_SUCCESS;
            }
          } else {
            jx_uint32 size = jx_vla_size(&I->key_value);
            jx_bool found = JX_FALSE;
            register jx_uint32 i = (size >> 1);
            register jx_ob *ob = I->key_value;
            while(i--) {
#if 0
              fprintf(stderr, "%08x %04x %04x == %08x %04x %04x %d\n",
                      (unsigned int) ob[0].data.raw.word,
                      (unsigned int) ob[0].meta.fill,
                      (unsigned int) ob[0].meta.bits,
                      (unsigned int) key.data.raw.word,
                      (unsigned int) key.meta.fill,
                      (unsigned int) key.meta.bits, jx_ob_identical(ob[0], key)
                );
#endif
              if(jx_ob_identical(ob[0], key)) {
                found = JX_TRUE;
                Jx_ob_free(E, JX_DISOWN(E, ob[0]));
                Jx_ob_free(E, JX_DISOWN(E, ob[1]));
                ob[0] = JX_OWN(E, key);       /* takes ownership */
                ob[1] = JX_OWN(E, value);     /* takes ownership */
                status = JX_SUCCESS;
                break;
              }
              ob += 2;
            }
            if(!found) {
              if(jx_ok(jx_vla_grow_check(&I->key_value, size + 1))) {
                I->key_value[size] = JX_OWN(E, key);
                I->key_value[size + 1] = JX_OWN(E, value);
                status = JX_SUCCESS;
                if(size > (2 * JX_HASH_RAW_CUTOFF)) {
                  /* switch to linear hash once we have more than N elements */
                  Jx__hash_recondition(E, I, JX_HASH_LINEAR, JX_FALSE);
                }
              }
            }
          }
        } else {                /* we have an info record */
          jx_uint32 index, mask = info->mask;
          jx_uint32 *hash_table = info->table;
          register jx_uint32 *hash_entry = hash_table + ((index = (mask & hash_code)) << 1);    /* prefetch? */
#if 0
          /* the following instruction doesn't help the way it should... */
#ifdef __GNUC__
          __builtin_prefetch(hash_entry + 1, 0, 0);
#endif
#endif

          if(!I->key_value) {
            I->key_value = Jx_vla_new(E, sizeof(jx_ob), 0, JX_TRUE);
          }
          if(I->key_value) {
            switch (info->mode) {
            case JX_HASH_LINEAR:
              {
                register jx_uint32 i = info->usage;
                register jx_ob *ob = I->key_value;
                jx_bool found = JX_FALSE;
                hash_entry = info->table;

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
                    ob[0] = JX_OWN(E, key);
                    ob[1] = JX_OWN(E, value);
                    status = JX_SUCCESS;
                    if(info->usage > JX_HASH_LINEAR_CUTOFF) {
                      Jx__hash_recondition(E, I, JX_HASH_ONE_TO_ANY, JX_FALSE);
                      /* switch to true hash */
                    }
                  }
                } else {
                  Jx_ob_free(E, JX_DISOWN(E, ob[0]));
                  Jx_ob_free(E, JX_DISOWN(E, ob[1]));
                  ob[0] = JX_OWN(E, key);
                  ob[1] = JX_OWN(E, value);
                  status = JX_SUCCESS;
                }
              }
              break;
            case JX_HASH_ONE_TO_ANY:
            case JX_HASH_ONE_TO_NIL:
              {
                if(info->usage > ((info->mask << 1) / 3)) {     /* more than ~2/3rd's full */
                  Jx__hash_recondition(E, I, info->mode, JX_FALSE);
                  info = (jx_hash_info *) I->info;
                  mask = info->mask;
                  index = mask & hash_code;
                  hash_table = info->table;
                  hash_entry = hash_table + (index << 1);
                }
                {
                  jx_uint32 usage = info->usage;
                  jx_ob *key_value = I->key_value;
                  jx_bool found = JX_FALSE;
                  register jx_uint32 sentinel = index;
                  jx_uint32 *dest_ptr = JX_NULL;
                  jx_bool dest_virgin = JX_FALSE;
                  register jx_uint32 hash_entry_1 = hash_entry[1];
                  do {
                    index = (index + 1) & mask;
                    if(!hash_entry_1) {
                      /* virgin slot terminates probe... */
                      if(!dest_ptr) {
                        dest_ptr = hash_entry;
                        dest_virgin = JX_TRUE;
                      }
                      break;
                    } else if(!(hash_entry_1 & JX_HASH_ENTRY_ACTIVE)) {
                      /* deleted slot -- save for future insertion */
                      if(!dest_ptr)
                        dest_ptr = hash_entry;
                    } else if((hash_entry[0] == hash_code) &&
                              jx_ob_identical(key_value
                                              [hash_entry_1 &
                                               JX_HASH_ENTRY_KV_OFFSET_MASK], key)) {
                      /* matched key, so we must replace */
                      dest_ptr = hash_entry;
                      found = JX_TRUE;
                      break;
                    }
                    hash_entry = hash_table + (index << 1);
                    hash_entry_1 = hash_entry[1];
                  } while(index != sentinel);
                  if(dest_ptr) {
                    if(!found) {
                      jx_uint32 kv_offset;
                      if(dest_virgin) {
                        kv_offset = ((info->stale_usage + usage) << 1);
                        /* allocate new key_value */
                      } else {
                        kv_offset = (dest_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                        /* use existing key_value */
                      }
                      if(jx_ok(jx_vla_grow_check(&I->key_value, kv_offset + 1))) {
                        key_value = I->key_value;
                        if(!dest_virgin)
                          info->stale_usage--;
                        dest_ptr[0] = hash_code;
                        dest_ptr[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                        key_value[kv_offset] = JX_OWN(E, key);
                        key_value[kv_offset + 1] = JX_OWN(E, value);
                        status = JX_SUCCESS;
                        info->usage++;
                      } else {
                        status = JX_FAILURE;
                      }
                    } else {
                      jx_uint32 kv_offset = (dest_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                      Jx_ob_free(E, JX_DISOWN(E, key_value[kv_offset]));
                      Jx_ob_free(E, JX_DISOWN(E, key_value[kv_offset + 1]));
                      key_value[kv_offset] = JX_OWN(E, key);
                      key_value[kv_offset + 1] = JX_OWN(E, value);
                      status = JX_SUCCESS;
                    }
                  }
                }
              }
              break;
            case JX_HASH_ONE_TO_ONE:
              {
                jx_uint32 reverse_hash_code = jx_ob_hash_code(value);
                if(!reverse_hash_code) {
                  status = JX_FAILURE;
                } else {
                  if((info->usage + 1) > (3 * info->mask) >> 2) {
                    /* more than ~3/4'rs full */
                    Jx__hash_recondition(E, I, info->mode, JX_FALSE);
                    info = (jx_hash_info *) I->info;
                    mask = info->mask;
                    index = mask & hash_code;
                    hash_table = info->table;
                    hash_entry = hash_table + (index << 1);
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

                        key_value[kv_offset] = JX_OWN(E, key);
                        key_value[kv_offset + 1] = JX_OWN(E, value);

                        status = JX_SUCCESS;
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
    }
    jx_gc_unlock(&I->gc);
  }
  return status;
}

static jx_bool jx__hash_equal(jx_hash * left, jx_hash * right)
{
  /* take pairwise locks in memory order to avoid deadlock */
  if(left > right) {
    jx_hash *tmp = left;
    left = right;
    right = tmp;
  }
  {
    jx_status status = jx_gc_lock(&left->gc);
    if(JX_POS(status)) {
      status = jx_gc_lock(&right->gc);
      if(JX_POS(status)) {

        jx_int left_size = jx__hash_size(left);
        jx_int right_size = jx__hash_size(right);

        if(left_size == right_size) {
          if(!left_size)
            return JX_TRUE;
          else {
            jx_hash_info *info = (jx_hash_info *) left->info;
            if((!info) || (info->mode == JX_HASH_LINEAR)) {
              register jx_int i = (info ? info->usage : left_size);
              register jx_ob *ob = left->key_value;
              while(i--) {
                jx_ob right_value = jx_ob_from_null();
                if(!jx__hash_peek(&right_value, right, ob[0]))
                  return JX_FALSE;
                else if(!jx_ob_equal(ob[1], right_value))
                  return JX_FALSE;
                ob += 2;
              }
              return JX_TRUE;
            } else {
              switch (info->mode) {
              case JX_HASH_ONE_TO_ANY:
              case JX_HASH_ONE_TO_ONE:
              case JX_HASH_ONE_TO_NIL:
                {
                  jx_uint32 mask = info->mask;
                  jx_uint32 *hash_table = info->table;
                  jx_ob *key_value = left->key_value;
                  jx_uint32 index = 0;
                  do {
                    jx_uint32 *hash_entry = hash_table + (index << 1);
                    if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                      jx_ob *kv_ob = key_value + (hash_entry[1] &
                                                  JX_HASH_ENTRY_KV_OFFSET_MASK);
                      jx_ob right_value = jx_ob_from_null();
                      if(!jx__hash_peek(&right_value, right, kv_ob[0]))
                        return JX_FALSE;
                      else if(!jx_ob_equal(kv_ob[1], right_value))
                        return JX_FALSE;
                    }
                    index++;
                  } while(index <= mask);
                  return JX_TRUE;
                }
                break;
              }
            }
          }
        }
        jx_gc_unlock(&right->gc);
      }
      jx_gc_unlock(&left->gc);
    }
  }

  return JX_FALSE;
}

static jx_bool jx__hash_identical(jx_hash * left, jx_hash * right)
{
  /* take pairwise locks in memory order to avoid deadlock */
  if(left > right) {
    jx_hash *tmp = left;
    left = right;
    right = tmp;
  }
  {
    jx_status status = jx_gc_lock(&left->gc);
    if(JX_POS(status)) {
      status = jx_gc_lock(&right->gc);
      if(JX_POS(status)) {

        jx_int left_size = jx__hash_size(left);
        jx_int right_size = jx__hash_size(right);

        if(left_size == right_size) {
          if(!left_size)
            return JX_TRUE;
          else {
            jx_hash_info *info = (jx_hash_info *) left->info;

            if((!info) || (info->mode == JX_HASH_LINEAR)) {
              register jx_int i = (info ? info->usage : left_size);
              register jx_ob *ob = left->key_value;
              while(i--) {
                jx_ob right_value = jx_ob_from_null();
                if(!jx__hash_peek(&right_value, right, ob[0]))
                  return JX_FALSE;
                else if(!jx_ob_identical(ob[1], right_value))
                  return JX_FALSE;
                ob += 2;
              }
              return JX_TRUE;
            } else {
              switch (info->mode) {
              case JX_HASH_ONE_TO_ANY:
              case JX_HASH_ONE_TO_ONE:
              case JX_HASH_ONE_TO_NIL:
                {
                  jx_uint32 mask = info->mask;
                  jx_uint32 *hash_table = info->table;
                  jx_ob *key_value = left->key_value;
                  jx_uint32 index = 0;
                  do {
                    jx_uint32 *hash_entry = hash_table + (index << 1);
                    if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                      jx_ob *kv_ob = key_value + (hash_entry[1] &
                                                  JX_HASH_ENTRY_KV_OFFSET_MASK);
                      jx_ob right_value = jx_ob_from_null();
                      if(!jx__hash_peek(&right_value, right, kv_ob[0]))
                        return JX_FALSE;
                      else if(!jx_ob_identical(kv_ob[1], right_value))
                        return JX_FALSE;
                    }
                    index++;
                  } while(index <= mask);
                  return JX_TRUE;
                }
                break;
              }
            }
          }
        }
        jx_gc_unlock(&right->gc);
      }
      jx_gc_unlock(&left->gc);
    }
  }
  return JX_FALSE;
}

jx_ob jx__hash_copy_members(jx_env * E, jx_hash * I, jx_int flags)
{
  jx_ob result = Jx_list_new(E);
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    if(size) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        register jx_ob *ob = I->key_value;
        while(i--) {
          if(flags & JX__HASH_COPY_KEYS)
            Jx_list_append(E, result, Jx_ob_copy(E, ob[0]));
          if(flags & JX__HASH_COPY_VALUES)
            Jx_list_append(E, result, Jx_ob_copy(E, ob[1]));
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                if(flags & JX__HASH_COPY_KEYS)
                  Jx_list_append(E, result, Jx_ob_copy(E, kv_ob[0]));
                if(flags & JX__HASH_COPY_VALUES)
                  Jx_list_append(E, result, Jx_ob_copy(E, kv_ob[1]));
              }
              index++;
            } while(index <= mask);
          }
          break;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_status jx__hash_set_shared(jx_hash * I, jx_bool shared)
{
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    status = JX_SUCCESS;
    I->gc.shared = shared;
    if(size) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        register jx_ob *kv_ob = I->key_value;
        while(i--) {
          jx_ob_set_shared(kv_ob[0], shared);
          jx_ob_set_shared(kv_ob[1], shared);
          kv_ob += 2;
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                /* active slot with matching hash code */
                jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                jx_ob_set_shared(kv_ob[0], shared);
                jx_ob_set_shared(kv_ob[1], shared);
              }
              index++;
            } while(index <= mask);
          }
          break;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return status;
}

jx_status jx__hash_set_synchronized(jx_hash * I, jx_bool synchronized, jx_bool recursive)
{
  jx_bool synched_on_entry = I->gc.synchronized;
  jx_status status = synched_on_entry ? jx_gc_lock(&I->gc) : JX_YES;
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    status = JX_SUCCESS;
    I->gc.synchronized = synchronized;
    if(size && recursive) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        register jx_ob *kv_ob = I->key_value;
        while(i--) {
          jx_ob_set_synchronized(kv_ob[0], synchronized, recursive);
          jx_ob_set_synchronized(kv_ob[1], synchronized, recursive);
          kv_ob += 2;
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
                /* active slot with matching hash code */
                jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                jx_ob_set_synchronized(kv_ob[0], synchronized, recursive);
                jx_ob_set_synchronized(kv_ob[1], synchronized, recursive);
              }
              index++;
            } while(index <= mask);
          }
          break;
        }
      }
    }
    if(synched_on_entry) {
      jx_gc_unlock(&I->gc);
    }
  }
  return status;
}

jx_bool jx__hash_has_key(jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    if(size) {
      if(I->key_value) {
        if(!I->info) {          /* JX_HASH_RAW */
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
                  if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE)
                     && (hash_entry[0] == hash_code)) {
                    /* active slot with matching hash code */
                    jx_ob *kv_ob =
                      key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                    if(jx_ob_identical(*kv_ob, key)) {
                      found = JX_TRUE;
                      break;
                    }
                  } else if(!hash_entry[1])
                    break;
                  index = (index + 1) & mask;
                } while(index != sentinel);
              }
              break;
            }
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return found;
}

jx_bool jx__hash_remove(jx_env * E, jx_ob * result, jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else {
      jx_uint32 size = jx_vla_size(&I->key_value);
      if(size) {
        if(!I->info) {          /* JX_HASH_RAW */
          register jx_int i = (size >> 1);
          jx_ob *ob = I->key_value;
          while(i--) {
            if(jx_ob_identical(ob[0], key)) {
              found = JX_TRUE;
              Jx_ob_free(E, JX_DISOWN(E, ob[0]));   /* key */
              *result = ob[1];  /* value ownership returned */

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
                      Jx_ob_free(E, JX_DISOWN(E, ob[0]));
                      *result = ob[1];  /* value ownership returned */
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
                  if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE)
                     && (hash_entry[0] == hash_code)) {
                    /* active slot with matching hash code */
                    jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                    if(jx_ob_identical(key_value[kv_offset], key)) {
                      jx_ob *ob = key_value + kv_offset;
                      hash_entry[1] = (kv_offset | JX_HASH_ENTRY_DELETED);
                      Jx_ob_free(E, JX_DISOWN(E, ob[0]));
                      *result = ob[1];
                      memset(ob, 0, sizeof(jx_ob) * 2); /* paranoia? */
                      info->usage--;
                      info->stale_usage++;
                      found = JX_TRUE;
                      break;
                    }
                  } else if(!hash_entry[1])
                    break;
                  index = (index + 1) & mask;
                } while(index != sentinel);
                {
                  jx_uint32 usage = info->usage;
                  if(!usage) {
                    Jx__hash_recondition(JX_NULL, I, JX_HASH_RAW, JX_TRUE);     /* purge empty hash table */
                  } else if(found && (info->stale_usage > usage) &&
                            ((usage + info->stale_usage) > ((info->mask) >> 1))) {
                    Jx__hash_recondition(JX_NULL, I, info->mode, JX_TRUE);      /* pack & (possibly) shrink */
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
                  if((hash_entry[1] & JX_HASH_ENTRY_ACTIVE)
                     && (hash_entry[0] == hash_code)) {
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
                      Jx_ob_free(E, JX_DISOWN(E, ob[0]));
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
                    Jx__hash_recondition(JX_NULL, I, info->mode, JX_TRUE);
                    /* pack & (possibly) shrink */
                  }
                }
              }
              break;
            }
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return found;
}

jx_bool jx__hash_peek_key(jx_ob * result, jx_hash * I, jx_ob value)
{
  jx_bool found = JX_FALSE;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    if(size) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        jx_ob *ob = I->key_value;
        while(i--) {            /* brute-force table scan */
          if(jx_ob_identical(ob[1], value)) {
            found = JX_TRUE;
            *result = JX_BORROW(ob[0]);
            break;
          }
          ob += 2;
        }
      } else {
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
                  *result = JX_BORROW(key_value[kv_offset]);
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
                    *result = JX_BORROW(key_value[kv_offset]);
                    found = JX_TRUE;
                    break;
                  }
                } else if(!hash_entry[1])
                  break;
                index = (index + 1) & mask;
              } while(index != sentinel);
            }
            break;
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return found;
}

jx_bool jx__hash_append_keys(jx_env * E, jx_ob result, jx_hash * I, jx_ob value)
{
  jx_bool found = JX_FALSE;
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    if(size) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        jx_ob *ob = I->key_value;
        while(i--) {            /* brute-force table scan */
          if(jx_ob_identical(ob[1], value)) {
            found = JX_TRUE;
            status = Jx_list_append(E, result, Jx_ob_copy(E, ob[0]));
          }
          ob += 2;
        }
      } else {
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
                  status =
                    Jx_list_append(E, result, Jx_ob_copy(E, key_value[kv_offset]));
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
                    Jx_list_append(E, result, Jx_ob_copy(E, key_value[kv_offset]));
                    found = JX_TRUE;
                    break;      /* we're assuming there aren't any other match keys,
                                   otherwise this hash wouldn't be one-to-one */
                  }
                } else if(!hash_entry[1])
                  break;
                index = (index + 1) & mask;
              } while(index != sentinel);
            }
            break;
          }
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return found;
}

jx_status jx__hash_from_list(jx_env * E, jx_hash * hash, jx_list * list)
{
  jx_status status = jx_gc_lock(&list->gc);
  if(JX_POS(status)) {
    jx_int size = jx__list_size(list);
    status = JX_SUCCESS;
    if(0x1 & size) {
      status = JX_FAILURE;
    } else if(size) {
      jx_int i;
      for(i = 0; i < size; i += 2) {
        jx_ob key = Jx_ob_copy(E, jx__list_borrow_locked(list, i));
        jx_ob value = Jx_ob_copy(E, jx__list_borrow_locked(list, i + 1));
        if(!jx_ok(Jx__hash_set(E, hash, key, value))) {
          Jx_ob_free(E, JX_DISOWN(E, key));
          Jx_ob_free(E, JX_DISOWN(E, value));
          status = JX_FAILURE;
        }
      }
    }
    jx_gc_unlock(&list->gc);
  }
  return status;
}

jx_status jx__list_with_hash(jx_env * E, jx_list * list, jx_hash * hash)
{
  jx_status status = jx_gc_lock(&hash->gc);
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    jx_int size = jx_vla_size(&hash->key_value);
    if(size) {
      jx_hash_info *info = (jx_hash_info *) hash->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        Jx_vla_free(E, &list->data.vla);
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
                key_value[kv_offset].meta.bits = JX_META_NOT_AN_OB;
                /* mark inactive entries */
              }
              index++;
            } while(index <= mask);
            {
              jx_ob *ob = key_value;
              jx_int i, count = 0;
              for(i = 0; i < size; i += 2) {
                if(ob[0].meta.bits != JX_META_NOT_AN_OB) {
                  /* purge the inactives */
                  if(count != i) {
                    jx_os_memcpy(key_value + count, ob, sizeof(jx_ob) * 2);
                  }
                  count += 2;
                }
                ob += 2;
              }
              jx_vla_resize(&hash->key_value, count);
              if(count) {
                Jx_vla_free(E, &list->data.vla);
                list->data.vla = hash->key_value;
                hash->key_value = JX_NULL;
              }
              Jx_vla_free(E, &hash->info);
            }
          }
        }
      }
    }
    jx_gc_unlock(&hash->gc);
  }
  return status;
}

jx_status jx__hash_with_list(jx_env * E, jx_hash * hash, jx_list * list)
{
  jx_status status = jx_gc_lock(&list->gc);
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    jx_int size = jx__list_size(list);
    if(0x1 & size) {
      status = JX_FAILURE;
    } else if(size) {
      status = JX_SUCCESS;
      jx_int i;
      for(i = 0; i < size; i += 2) {
        jx_ob key = jx__list_borrow_locked(list, i);
        jx_ob value = jx__list_borrow_locked(list, i + 1);
        if(!jx_ok(Jx__hash_set(E, hash, key, value))) {
          status = JX_FAILURE;
          break;
        }
      }
      /* must empty contents of one container or the other */
      if(jx_ok(status)) {
        Jx_vla_free(E, &list->data.vla);
      } else {
        Jx_vla_free(E, &hash->key_value);
        Jx_vla_free(E, &hash->info);
      }
    }
    jx_gc_unlock(&list->gc);
  }
  return status;
}

static jx_status jx__builtin_gc_set_synchronized(jx_ob ob, jx_bool synchronized,
                                                 jx_bool recursive)
{
  /* on entry, we know object is GC'd, so only GC'd objects needed here */
  ob.data.io.gc->synchronized = synchronized;
  if(recursive) {
    switch (ob.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
    case JX_META_BIT_BUILTIN_FUNCTION:
    case JX_META_BIT_BUILTIN_MACRO:
      {
        jx_function *fn = ob.data.io.function;
        jx_ob_set_synchronized(fn->name, synchronized, recursive);
        jx_ob_set_synchronized(fn->args, synchronized, recursive);
        jx_ob_set_synchronized(fn->body, synchronized, recursive);
      }
      break;
    }
  }
  return JX_SUCCESS;
}

static jx_status jx__builtin_gc_set_shared(jx_ob ob, jx_bool shared)
{
  /* on entry, we know object is GC'd, so only GC'd objects needed here */
  ob.data.io.gc->shared = shared;
  switch (ob.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
  case JX_META_BIT_BUILTIN_ENTITY:
    ob.data.io.gc->shared = shared;
    break;
  case JX_META_BIT_BUILTIN_FUNCTION:
  case JX_META_BIT_BUILTIN_MACRO:
    {
      jx_function *fn = ob.data.io.function;
      jx_ob_set_shared(fn->name, shared);
      jx_ob_set_shared(fn->args, shared);
      jx_ob_set_shared(fn->body, shared);
    }
    break;
  }
  return JX_SUCCESS;
}


/* shared: read only immortal gc'd objects & containers */

jx_status jx__ob_gc_set_shared(jx_ob ob, jx_bool shared)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    ob.data.io.gc->shared = shared;
    return JX_SUCCESS;
    break;
  case JX_META_BIT_LIST:
    return jx__list_set_shared(ob.data.io.list, shared);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_set_shared(ob.data.io.hash, shared);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__builtin_gc_set_shared(ob, shared);
    break;
  }
  return JX_SUCCESS;
}


/* synchronized: all access subject to spinlocks */

jx_status jx__ob_gc_set_synchronized(jx_ob ob, jx_bool synchronized, jx_bool recursive)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    ob.data.io.gc->synchronized = synchronized;
    break;
  case JX_META_BIT_LIST:
    return jx__list_set_synchronized(ob.data.io.list, synchronized, recursive);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_set_synchronized(ob.data.io.hash, synchronized, recursive);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__builtin_gc_set_synchronized(ob, synchronized, recursive);
    break;
  }
  return JX_SUCCESS;
}

jx_int jx__builtin_compare(jx_ob left, jx_ob right)
{
  jx_fast_bits left_bits = left.meta.bits & JX_META_MASK_BUILTIN_TYPE;
  jx_fast_bits right_bits = right.meta.bits & JX_META_MASK_BUILTIN_TYPE;
  if(left_bits == right_bits) {
    switch (left_bits) {
    case JX_META_BIT_BUILTIN_ENTITY:
      return jx__str__compare(jx_ob_as_ident(&left), jx_ob_as_ident(&right));
      break;
    case JX_META_BIT_BUILTIN_SELECTOR:
      return ((left.data.io.int_ < right.data.io.int_) ? -1 :
              (left.data.io.int_ == right.data.io.int_) ? 0 : 1);
      break;
    case JX_META_BIT_BUILTIN_OPAQUE_OB:
    case JX_META_BIT_BUILTIN_MACRO:
    case JX_META_BIT_BUILTIN_NATIVE_FN:
    case JX_META_BIT_BUILTIN_FUNCTION:
      return ((left.data.io.void_ < right.data.io.void_) ? -1 :
              (left.data.io.void_ == right.data.io.void_) ? 0 : 1);
      break;
    default:
      return 0;
      break;
    }
  } else {
    if((left_bits) && (!right_bits))
      return 1;
    if((!left_bits) && (right_bits))
      return -1;
    return ((left_bits < right_bits) ? -1 : (left_bits == right_bits) ? 0 : 1);
  }
  return 0;
}

jx_ob jx__builtin_copy(jx_env * E, jx_ob ob)
{
  /* on entry, we know object is GC'd, so only GC'd objects needed here */

  switch (ob.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
  case JX_META_BIT_BUILTIN_ENTITY:     /* 0 */
    return jx_builtin_new_entity_with_name(E,
                                           jx__ident_gc_copy_strong(E, ob.data.io.str));
    break;
  case JX_META_BIT_BUILTIN_FUNCTION:
    {
      jx_function *fn = ob.data.io.function;
      //      printf("NOTICE: copying a function!\n");
      return jx_function_new_with_def(Jx_ob_copy(E, fn->name),
                                      Jx_ob_copy(E, fn->args),
                                      Jx_ob_copy(E, fn->body), fn->mode);
    }
    break;
  case JX_META_BIT_BUILTIN_MACRO:
    {
      jx_function *fn = ob.data.io.function;
      return jx_macro_new_with_def(Jx_ob_copy(E, fn->name),
                                   Jx_ob_copy(E, fn->args), Jx_ob_copy(E, fn->body));
    }
    break;
  case JX_META_BIT_BUILTIN_OPAQUE_OB:
    /* TO DO -- do we need a copy function in builtins as well as free? -- I think so.. */
    break;
  }
  return jx_ob_from_null();
}

jx_ob jx__builtin_copy_strong(jx_env * E, jx_ob ob)
{
  /* on entry, we know object is GC'd, so only GC'd objects needed here */

  switch (ob.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
  case JX_META_BIT_BUILTIN_ENTITY:     /* 0 */
    return jx_builtin_new_entity_with_name(E,
                                           jx__ident_gc_copy_strong(E, ob.data.io.str));
    break;
  case JX_META_BIT_BUILTIN_FUNCTION:
    {
      jx_function *fn = ob.data.io.function;
      //      printf("NOTICE: copying a function!\n");
      return jx_function_new_with_def(Jx_ob_copy_strong(E, fn->name),
                                      Jx_ob_copy_strong(E, fn->args),
                                      Jx_ob_copy_strong(E, fn->body), fn->mode);
    }
    break;
  case JX_META_BIT_BUILTIN_MACRO:
    {
      jx_function *fn = ob.data.io.function;
      return jx_macro_new_with_def(Jx_ob_copy_strong(E, fn->name),
                                   Jx_ob_copy_strong(E, fn->args),
                                   Jx_ob_copy_strong(E, fn->body));
    }
    break;
  case JX_META_BIT_BUILTIN_OPAQUE_OB:
    /* TO DO -- do we need a copy function in builtins as well as free? -- I think so.. */
    break;
  }
  return jx_ob_from_null();
}


/* copying */

jx_ob Jx__ob_gc_copy(jx_env * E, jx_ob ob)
{
  /* on entry, we know the object is GC'd and that the reference is
     not weak (otherwise the weak reference itself would have been
     copied)  */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx__str_gc_copy_strong(E, ob.data.io.str);
    break;
  case JX_META_BIT_IDENT:
    return jx__ident_gc_copy_strong(E, ob.data.io.str);
    break;
  case JX_META_BIT_LIST:
    return jx__list_copy(E, ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return Jx__hash_copy(E, ob.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__builtin_copy(E, ob);
    break;
  }
  return jx_ob_from_null();
}

jx_ob Jx__ob_gc_copy_strong(jx_env * E, jx_ob ob)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx__str_gc_copy_strong(E, ob.data.io.str);
    break;
  case JX_META_BIT_IDENT:
    return jx__ident_gc_copy_strong(E, ob.data.io.str);
    break;
  case JX_META_BIT_LIST:
    return jx__list_copy_strong(E, ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return Jx__hash_copy_strong(E, ob.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__builtin_copy_strong(E, ob);
    break;
  }
  return jx_ob_from_null();
}

#if 0
jx_ob jx__ob_make_strong_with_ob(jx_ob ob)
{
  jx_fast_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_WEAK_REF) {
    switch (bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      jx__list_make_strong(E, ob.data.io.list);
      break;
    case JX_META_BIT_HASH:
      jx__hash_make_strong(E, ob.data.io.hash);
      break;
    case JX_META_BIT_STR:
      jx__str_copy_strong(E, ob.data.io.hash);
      break;
    case JX_META_BIT_IDENT:
      jx__ident_copy_strong(E, ob.data.io.hash);
      break;
    }
  } else {
    /* on entry, we know the object is GC'd */
    switch (bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      return jx__str_gc_copy_strong(E, ob.data.io.str);
      break;
    case JX_META_BIT_IDENT:
      return jx__ident_gc_copy_strong(E, ob.data.io.str);
      break;
    case JX_META_BIT_LIST:
      return jx__list_copy(E, ob.data.io.list);
      break;
    case JX_META_BIT_HASH:
      return Jx__hash_copy(E, ob.data.io.hash);
      break;
    case JX_META_BIT_BUILTIN:
      return jx__builtin_copy(ob);
      break;
    }
  }
  return ob;
}

jx_ob jx__ob_only_strong_with_ob(jx_ob ob)
{
  jx_fast_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_WEAK_REF) {
    ob = jx_null_with_ob(ob);
  } else {
    switch (bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      jx__list_only_strong(ob.data.io.list);
      break;
    case JX_META_BIT_HASH:
      jx__hash_only_strong(ob.data.io.hash);    /* TO DO fix resuting corruption of null entry */
      break;
    }
  }
  return ob;
}
#endif


/* thread-local storage for accelerating memory use within a single thread */

void *Jx__vla_new(jx_env * E, jx_int rec_size, jx_int size, jx_bool zero)
{
  if(E && E->vla_chain) {
    jx_mem_chain *chain = E->vla_chain;
    E->vla_chain = chain->next;
    E->n_vla--;
    {
      jx_vla *vla = (jx_vla *) chain;
      vla->ptr = vla;

      if(vla->rec_size == rec_size) {
        if(vla->alloc >= size) {
          /* fast path! */
          vla->size = size;
          if(zero)
            memset(vla + 1, 0, rec_size * size);
          return vla + 1;
        } else {
          vla->size = 0;
        }
      } else if(vla->rec_size < rec_size) {
        vla->alloc = 0;
      }
      vla->size = 0;
      vla->rec_size = rec_size;
      if(jx_ok(jx__vla__resize(&vla, size, JX_TRUE, JX_FALSE))) {
        return vla + 1;
      } else {
        Jx_vla_free(E, &vla);
        return jx_vla_new(rec_size, size);
      }
    }
  }
  return jx_vla_new(rec_size, size);
}

void *Jx__vla_new_with_content(jx_env * E, jx_int rec_size, jx_int size, void *content)
{
  jx_vla *vla = Jx__vla_new(E, rec_size, size, JX_FALSE);
  if(vla) {
    jx_os_memcpy(vla, content, rec_size * size);
    return vla;
  } else
    return NULL;
}

void *Jx__vla_copy(jx_env * E, void **ref)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    return Jx__vla_new_with_content(E, vla->rec_size, vla->size, (*ref));
  } else {
    return NULL;
  }
}

jx_status Jx__vla_free(jx_env * E, void **ref)
{
  if(*ref) {
    if(E && (E->n_vla < JX_TLS_VLA_MAX)) {
      jx_vla *vla = ((jx_vla *) (*ref)) - 1;
      jx_int rec_size = vla->rec_size;
      jx_int alloc = vla->alloc;
      jx_int padding = ((char *) vla) - ((char *) vla->ptr);
      if(padding) {
        switch (rec_size) {
        case 4:
          alloc += (padding >> 2);
          break;
        case 8:
          alloc += (padding >> 3);
          break;
        case 16:
          alloc += (padding >> 4);
          break;
        default:
          alloc += (padding / rec_size);
          break;
        }
      }
      if((alloc * rec_size) < JX_TLS_VLA_ALLOC_MAX) {
        vla = vla->ptr;
        vla->rec_size = rec_size;
        vla->alloc = alloc;
        vla->size = 0;
        (*ref) = NULL;
        {
          jx_mem_chain *chain = (jx_mem_chain *) vla;
          chain->next = E->vla_chain;
          E->vla_chain = chain;
          E->n_vla++;
          return JX_SUCCESS;
        }
      }
    }
    return jx_vla_free(ref);
  }
  return JX_SUCCESS;
}

jx_hash *Jx_hash_calloc(jx_env * E)
{
  if(E && E->hash_chain) {
    jx_mem_chain *chain = E->hash_chain;
    E->hash_chain = chain->next;
    E->n_hash--;
    {
      jx_hash *hash = (jx_hash *) chain;
      memset(hash, 0, sizeof(jx_hash));
      return hash;
    }
  }
  return jx_calloc(1, sizeof(jx_hash));
}

jx_ob Jx_hash_new(jx_env * E)
{
  jx_hash *hash = Jx_hash_calloc(E);
  if(hash) {
    jx_ob result = JX_OB_HASH;
    result.data.io.hash = hash;
    return result;
  }
  return Jx_hash_new(E);
}

jx_ob Jx_hash_new_with_assoc(jx_env * E, jx_ob key, jx_ob value)
{
  jx_hash *hash = Jx_hash_calloc(E);
  if(hash) {
    jx_ob result = JX_OB_HASH;
    result.data.io.hash = hash;
    {
      jx_ob *kv = Jx_vla_new(E, sizeof(jx_ob), 2, JX_TRUE);
      if(kv) {
        kv[0] = JX_OWN(E, key);
        kv[1] = JX_OWN(E, value);
        hash->key_value = kv;
      } else {
        Jx_hash_set(E, result, key, value);
      }
    }
    return result;
  } else {
    jx_ob result = Jx_hash_new(E);
    Jx_hash_set(E, result, key, value);
    return result;
  }
}

jx_list *Jx_list_calloc(jx_env * E)
{
  if(E && E->list_chain) {
    jx_mem_chain *chain = E->list_chain;
    E->list_chain = chain->next;
    E->n_list--;
    {
      jx_list *list = (jx_list *) chain;
      memset(list, 0, sizeof(jx_list));
      return list;
    }
  }
  return jx_calloc(1, sizeof(jx_list));
}

jx_ob Jx_list_new(jx_env * E)
{
  jx_list *list = Jx_list_calloc(E);
  if(list) {
    jx_ob result = JX_OB_LIST;
    result.data.io.list = list;
    return result;
  }
  return jx__list_new();
}

jx_ob Jx_list_new_with_size(jx_env * E, jx_int size)
{
  jx_ob result = Jx_list_new(E);
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->data.vla = Jx_vla_new(E, sizeof(jx_ob), size, JX_TRUE);
  }
  return result;

}

jx_ob Jx__list_copy(jx_env * E, jx_list * I)
{
  jx_ob result = Jx_list_new(E);
  jx_status status = jx_gc_lock(&I->gc);
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_LIST) {
      jx_list *new_I = result.data.io.list;
      (*new_I) = (*I);
      jx__gc_init(&new_I->gc);
      new_I->data.vla = Jx__vla_copy(E, &I->data.vla);
      if(!new_I->packed_meta_bits) {    /* need to recursively copy all GC content */
        jx_int i, size = jx_vla_size(&new_I->data.ob_vla);
        jx_ob *ob = new_I->data.ob_vla;
        for(i = 0; i < size; i++) {
          if(ob->meta.bits & JX_META_BIT_GC) {
            *ob = Jx_ob_copy(E, *ob);
          }
          ob++;
        }
      }
    }
    jx_gc_unlock(&I->gc);
  }
  return result;
}

jx_ob Jx__ob_copy(jx_env * E, jx_ob ob)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx__str_gc_copy_strong(E, ob.data.io.str);
    break;
  case JX_META_BIT_IDENT:
    return jx__ident_gc_copy_strong(E, ob.data.io.str);
    break;
  case JX_META_BIT_LIST:
    return Jx__list_copy(E, ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return Jx__hash_copy(E, ob.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__builtin_copy(E, ob);
    break;
  }
  return jx_ob_from_null();
}

jx_env *jx__env_calloc(void)
{
  jx_env *result = (jx_env *) jx_calloc(1, sizeof(jx_env));
  if(result) {
    result->heap = result;
  }
  return result;
}

void Jx__free(jx_env * E)
{
  jx_mem_chain *chain = E->hash_chain;
  while(chain) {
    jx_mem_chain *next = chain->next;
    jx_free(chain);
    chain = next;
  }
  chain = E->list_chain;
  while(chain) {
    jx_mem_chain *next = chain->next;
    jx_free(chain);
    chain = next;
  }
  chain = E->vla_chain;
  while(chain) {
    jx_mem_chain *next = chain->next;
    jx_free(chain);
    chain = next;
  }
  if(E->heap) {
    jx_free(E->heap);
  }
}


/* comparison */

JX_INLINE jx_bool jx__function_identical(jx_function * left, jx_function * right)
{
  return
    jx_ob_identical(left->name, right->name) &&
    jx_ob_identical(left->args, right->args) &&
    jx_ob_identical(left->body, right->body) && (left->mode == right->mode);
}

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
  case JX_META_BIT_IDENT:
    {
      jx_char *left_st = jx_ob_as_ident(&left);
      jx_char *right_st = jx_ob_as_ident(&right);
      //      printf("[%s]==[%s]  %d\n",left_st,right_st,strcmp(left_st,right_st));
      if(left_st && right_st) {
        return (!strcmp(left_st, right_st));
      }
    }
    break;
  case JX_META_BIT_LIST:
    return jx__list_identical(left.data.io.list, right.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_identical(left.data.io.hash, right.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    switch (left.meta.bits & JX_META_MASK_BUILTIN_TYPE) {
    case JX_META_BIT_BUILTIN_ENTITY:
      {
        jx_char *left_st = jx_ob_as_ident(&left);
        jx_char *right_st = jx_ob_as_ident(&right);
        //      printf("[%s]==[%s]  %d\n",left_st,right_st,strcmp(left_st,right_st));
        if(left_st && right_st) {
          return (!strcmp(left_st, right_st));
        }
      }
      break;
    case JX_META_BIT_BUILTIN_FUNCTION:
      return jx__function_identical(left.data.io.function, right.data.io.function);
      break;
    }
    break;
  }
  return JX_FALSE;
}

jx_bool jx__ob_non_gc_equal(jx_ob left, jx_ob right)
{                               /* we know incoming types don't match */
  jx_fast_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & (JX_META_BIT_STR | JX_META_BIT_IDENT)) {
    return JX_FALSE;
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_as_float(left) == jx_ob_as_float(right);
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_as_int(left) == jx_ob_as_int(right);
  }
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
  case JX_META_BIT_IDENT:
    {
      jx_char *left_st = jx_ob_as_ident(&left);
      jx_char *right_st = jx_ob_as_ident(&right);
      if(left_st && right_st) {
        return (!strcmp(left_st, right_st));
      }
    }
    break;
  case JX_META_BIT_LIST:
    return jx__list_equal(left.data.io.list, right.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_equal(left.data.io.hash, right.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__ob_gc_identical(left, right);
    break;
  }
  return JX_FALSE;
}


/* functions */

jx_ob jx__function_new_with_def(jx_env * E, jx_ob name, jx_ob args,
                                jx_ob body, jx_int mode)
{
  jx_ob result = jx_ob_from_null();
  jx_function *fn = (jx_function *) jx_calloc(1, sizeof(jx_function));
  if(fn) {
    fn->name = name;
    fn->args = args;
    fn->body = body;
    fn->mode = mode;

    //printf("new function at %p\n",fn);
    //jx_jxon_dump(stdout,"new function name",jx_ob_from_null(),name);
    //jx_jxon_dump(stdout,"new function args",args);
    //jx_jxon_dump(stdout,"new function code",body);
    //fprintf(stdout,"new function block: %d\n",block);
    result.data.io.function = fn;
    result.meta.bits =
      JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_FUNCTION | JX_META_BIT_GC;
  } else {
    Jx_ob_free(E, name);
    Jx_ob_free(E, args);
    Jx_ob_free(E, body);
  }
  return result;
}

jx_ob jx_function_new_with_def(jx_ob name, jx_ob args, jx_ob body, jx_int mode)
{
  return jx__function_new_with_def(JX_NULL, name, args, body, mode);
}

jx_ob jx__macro_new_with_def(jx_env * E, jx_ob name, jx_ob args, jx_ob body)
{
  jx_ob result = jx_ob_from_null();
  jx_function *fn = (jx_function *) jx_calloc(1, sizeof(jx_function));
  if(fn) {
    fn->name = name;
    fn->args = args;
    fn->body = body;
    fn->mode = JX_FUNCTION_MODE_EVAL;

    //jx_jxon_dump(stdout,"new function name",name);
    //jx_jxon_dump(stdout,"new function args",args);
    //jx_jxon_dump(stdout,"new function code",body);
    //fprintf(stdout,"new function block: %d\n",block);
    result.data.io.function = fn;
    result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_MACRO | JX_META_BIT_GC;
  } else {
    Jx_ob_free(E, name);
    Jx_ob_free(E, args);
    Jx_ob_free(E, body);
  }
  return result;
}

jx_ob jx_macro_new_with_def(jx_ob name, jx_ob args, jx_ob body)
{
  return jx__macro_new_with_def(JX_NULL, name, args, body);
}

jx_ob jx_function_to_impl(jx_ob ob)
{
  jx_env env;
  jx_ob result = jx_ob_from_null();
  jx_env *E = jx_env_new_in_node(&env, jx_ob_from_null());

  if(jx_function_check(ob)) {
    jx_function *fn = ob.data.io.function;
    result = jx_list_new_with_size(4);
    jx_list_replace(result, 0, Jx_ob_copy(E, fn->name));
    jx_list_replace(result, 1, Jx_ob_copy(E, fn->args));
    jx_list_replace(result, 2, Jx_ob_copy(E, fn->body));
    jx_list_replace(result, 3, jx_ob_from_bool(fn->mode));
  }
  jx_env_free(&env);
  return result;
}

jx_ob jx_macro_to_impl(jx_ob ob)
{
  jx_env env;
  jx_ob result = jx_ob_from_null();
  jx_env *E = jx_env_new_in_node(&env, jx_ob_from_null());
  if(jx_function_check(ob)) {
    jx_function *fn = ob.data.io.function;
    result = jx_list_new_with_size(3);
    jx_list_replace(result, 0, Jx_ob_copy(E, fn->name));
    jx_list_replace(result, 1, Jx_ob_copy(E, fn->args));
    jx_list_replace(result, 2, Jx_ob_copy(E, fn->body));
  }
  jx_env_free(&env);
  return result;
}


/* freeing */

jx_status Jx__ob_gc_free(jx_env * E, jx_ob ob)
{                               /* on entry, we know ob is GC'd */
  if(ob.data.io.gc->shared) {
    return JX_STATUS_FREED_SHARED;
  }
  register jx_fast_bits bits = ob.meta.bits;
  switch (bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    return jx__str_free(ob.data.io.str);
    break;
  case JX_META_BIT_LIST:
    return jx__list_free(E, ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_free(E, ob.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    switch (bits & JX_META_MASK_BUILTIN_TYPE) {
    case JX_META_BIT_BUILTIN_ENTITY:
      return jx__str_free(ob.data.io.str);
      break;
    case JX_META_BIT_BUILTIN_OPAQUE_OB:
      {
        jx_opaque_ob *opaque = ob.data.io.opaque_ob;
        if(opaque) {
          jx_opaque_free_fn fn = opaque->free_fn;
          if(fn) {
            return fn(opaque);
          } else {
            return JX_SUCCESS;
          }
        }
      }
      break;
    case JX_META_BIT_BUILTIN_FUNCTION:
    case JX_META_BIT_BUILTIN_MACRO:
      {
        jx_function *fn = ob.data.io.function;
        Jx_ob_free(E, JX_DISOWN(E, fn->name));
        Jx_ob_free(E, JX_DISOWN(E, fn->args));
        Jx_ob_free(E, JX_DISOWN(E, fn->body));
        jx_free(fn);
      }
      break;
    }
    break;
  }
  return JX_SUCCESS;
}

