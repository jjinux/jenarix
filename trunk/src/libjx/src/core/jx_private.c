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

JX_INLINE void jx__gc_init(jx_gc * gc)
{
  memset(gc, 0, sizeof(jx_gc));
}

/* jx_vla routines */

void *jx__vla_new(jx_int rec_size, jx_int size)
{
  jx_vla *vla = (jx_vla *) jx_calloc(1, sizeof(jx_vla) + rec_size * size);
  if(vla) {
    vla->ptr = vla;
    vla->alloc = size;
    vla->rec_size = rec_size;
    vla->size = size;
    return (void *) (vla + 1);
  } else
    return NULL;
}

void *jx__vla_new_with_content(jx_int rec_size, jx_int size, void *content)
{
  jx_vla *vla = jx_malloc(sizeof(jx_vla) + rec_size * size);
  if(vla) {
    jx_char *base = (jx_char *) (vla + 1);
    vla->ptr = vla;
    vla->alloc = size;
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
      jx_char *ptr = (jx_char*)(*ref);
      return jx_vla_new_with_content(vla->rec_size, count,
                                     ptr + vla->rec_size * index);
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
    if((new_size > old_alloc) || force_padding) {
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
          printf("ERROR!\n");
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
    } else {
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
        jx_os_memcpy(base + (old_size ? old_size - 1 : 0), jx_ob_as_str(&ob), str_len + 1);
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

jx_status jx__vla_free(void **ref)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla *) (*ref)) - 1;
    jx_free(vla->ptr);
    (*ref) = NULL;
  }
  return JX_SUCCESS;
}

/* thread-local storage for accelerating memory use within a single thread */

typedef struct jx__tls_chain jx_tls_chain;

struct jx__tls_chain {
  jx_tls_chain *next;
};

#define JX_TLS_VLA_ALLOC_MAX   4000
#define JX_TLS_MAX              100

struct jx__tls {
  jx_int n_hash,n_list,n_vla;
  jx_tls_chain *hash_chain;
  jx_tls_chain *list_chain;
  jx_tls_chain *vla_chain;
};

void *jx__tls_vla_new(jx_tls *tls, jx_int rec_size, jx_int size)
{
  if(tls && tls->vla_chain) {
    jx_tls_chain *chain = tls->vla_chain;
    tls->vla_chain = chain->next;
    tls->n_vla--;
    {
      jx_vla *vla = (jx_vla*)chain;
      vla->ptr = vla;
      if((vla->rec_size == rec_size) && (vla->alloc >= size)) {
        vla->size = size;
        memset(vla + 1, 0, rec_size * size);
        return vla + 1;
      }
      vla->alloc = 0;
      vla->rec_size = rec_size;
      if(jx_ok(jx__vla__resize(&vla, size, JX_TRUE, JX_FALSE)))
        return vla+1;
      else {
        jx_vla_free(&vla);
        return jx_vla_new(rec_size,size);
      }
    }
  }
  return jx_vla_new(rec_size,size);
}

jx_status jx__tls_vla_free(jx_tls *tls,void **ref)
{
  if(*ref) {
    if(tls && (tls->n_vla < JX_TLS_MAX)) {
      jx_vla *vla = ((jx_vla *) (*ref)) - 1;
      jx_int rec_size = vla->rec_size;
      jx_int alloc = vla->alloc;
      if((alloc*rec_size) < JX_TLS_VLA_ALLOC_MAX) {
        vla = vla->ptr;
        vla->rec_size = rec_size;
        vla->alloc = alloc;
        vla->size = 0;
        (*ref) = NULL;
        {
          jx_tls_chain *chain = (jx_tls_chain*)vla;
          chain->next = tls->vla_chain;
          tls->vla_chain = chain;
          tls->n_vla++;
          return JX_SUCCESS;
        }
      }
    }
    return jx_vla_free(ref);
  }
  return JX_SUCCESS;
}

void jx_tls_hash_free(jx_tls *tls,jx_hash *hash)
{
  if(tls && (tls->n_hash < JX_TLS_MAX)) {
    jx_tls_chain *chain = (jx_tls_chain*)hash;
    chain->next = tls->hash_chain;
    tls->hash_chain = chain;
    tls->n_hash++;
  } else {
    jx_free(hash);
  }
}

void jx_tls_list_free(jx_tls *tls,jx_list *I)
{
  if(tls && (tls->n_list < JX_TLS_MAX)) {
    jx_tls_chain *chain = (jx_tls_chain*)I;
    chain->next = tls->list_chain;
    tls->list_chain = chain;
    tls->n_list++;
  } else {
    jx_free(I);
  }
}

jx_hash *jx_tls_hash_calloc(jx_tls *tls)
{
  if(tls && tls->hash_chain) {
    jx_tls_chain *chain = tls->hash_chain;
    tls->hash_chain = chain->next;
    tls->n_hash--;
    {
      jx_hash *hash = (jx_hash*)chain;
      memset(hash,0,sizeof(jx_hash));
      return hash;
    }
  }
  return jx_calloc(1,sizeof(jx_hash));
}

jx_ob jx_tls_hash_new(jx_tls *tls)
{
  jx_hash *hash = jx_tls_hash_calloc(tls);
  if(hash) {
    jx_ob result = JX_OB_HASH;
    result.data.io.hash = hash;
    return result;
  }
  return jx_hash_new();
}

jx_ob jx_tls_hash_new_with_assoc(jx_tls *tls,jx_ob key,jx_ob value)
{
  jx_hash *hash = jx_tls_hash_calloc(tls);
  if(hash) {
    jx_ob result = JX_OB_HASH;
    result.data.io.hash = hash;
#if 1
    {
      jx_ob *kv = jx_tls_vla_new(tls, sizeof(jx_ob), 2);
      if(kv) {
        kv[0] = key;      /* takes ownership */
        kv[1] = value;    /* takes ownership */
        hash->key_value = kv;
      } else {
        jx_hash_set(result,key,value);
      }
    }
#else
    jx_hash_set(result,key,value);
#endif
    return result;
  } else {
    jx_ob result = jx_hash_new();
    jx_hash_set(result,key,value);
    return result;
  }
}

jx_list *jx_tls_list_calloc(jx_tls *tls)
{
  if(tls && tls->list_chain) {
    jx_tls_chain *chain = tls->list_chain;
    tls->list_chain = chain->next;
    tls->n_list--;
    {
      jx_list *list = (jx_list*)chain;
      memset(list,0,sizeof(jx_list));
      return list;
    }
  }
  return jx_calloc(1,sizeof(jx_list));
}

jx_ob jx_tls_list_new(jx_tls *tls)
{
  jx_list *list = jx_tls_list_calloc(tls);
  if(list) {
    jx_ob result = JX_OB_LIST;
    result.data.io.list = list;
    return result;
  }
  return jx_list_new();
}

jx_ob jx_tls_list_new_with_size(jx_tls *tls,jx_int size)
{
  jx_ob result = jx_tls_list_new(tls);
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->data.vla = jx_tls_vla_new(tls, sizeof(jx_ob), size);    /* zero'd memory */
  }
  return result;

}

jx_tls *jx_tls_new(void)
{
  return (jx_tls*)jx_calloc(1,sizeof(jx_tls));
}

void jx_tls_free(jx_tls *tls)
{
  jx_tls_chain *chain = tls->hash_chain;
  while(chain) {
    jx_tls_chain *next = chain->next;
    jx_free(chain);
    chain = next;
  }
  chain = tls->list_chain;
  while(chain) {
    jx_tls_chain *next = chain->next;
    jx_free(chain);
    chain = next;
  }
  chain = tls->vla_chain;
  while(chain) {
    jx_tls_chain *next = chain->next;
    jx_free(chain);
    chain = next;
  }
  jx_free(tls);
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

jx_ob jx__ob_to_str(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case 0:
    return jx_ob_from_str("null");
  case JX_META_BIT_BOOL:
    if(ob.data.io.bool_)
      return jx_ob_from_str("true");
    else
      return jx_ob_from_str("false");
    break;
  default:
    return jx_ob_to_jxon(ob);   /* default encoding */
  }
}

jx_ob jx__ob_to_ident(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx_ob_from_ident(jx_ob_as_str(&ob));
    break;
  }
  return jx_ob_from_null();
}

/* strings */

jx_ob jx_ob_from_str(jx_char * str)
{
  jx_int len = jx_strlen(str);
  jx_ob result = JX_OB_NULL;

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | len;
    jx_os_memcpy(result.data.io.tiny_str, str, len + 1);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = jx_vla_new(1, len + 1 + sizeof(jx_str));
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str + sizeof(jx_str), str, len + 1);
    }
  }
  return result;
}

jx_ob jx_ob_from_str_with_len(jx_char * str, jx_int len)
{
  jx_ob result = JX_OB_NULL;

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_STR | len;
    jx_os_memcpy(result.data.io.tiny_str, str, len + 1);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = jx_vla_new(1, len + 1 + sizeof(jx_str));
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str + sizeof(jx_str), str, len);
      result.data.io.str[sizeof(jx_str) + len] = 0;
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
    jx_os_memcpy(result.data.io.tiny_str, *ref, size + 1);
    jx_vla_free(ref);
  } else {
    /* string not tiny -- use heap */
    jx_int size = jx_vla_size(ref);
    if(jx_ok(jx_vla_resize(ref, size + sizeof(jx_str)))) {
      result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
      /* insert jx_str record in front of chars */
      jx_os_memmove((*ref) + sizeof(jx_str), (*ref), size);
      jx__gc_init((jx_gc *) * ref);
      result.data.io.str = *ref;
    } else {
      jx_vla_free(ref);
    }
  }
  return result;
}

static jx_char jx_ob_as_str_error[] = "jx_ob_as_str-Error: object not a string.";

jx_char *jx_ob_as_str(jx_ob * ob)
{
  jx_bits meta = ob->meta.bits;
  if(meta & JX_META_BIT_STR) {
    if(meta & JX_META_BIT_GC) {
      return ob->data.io.str + sizeof(jx_str);
    } else {
      return ob->data.io.tiny_str;
    }
  }
  return jx_ob_as_str_error;
}

jx_ob jx__str__concat(jx_char * left, jx_int left_len, jx_char * right, jx_int right_len)
{
  jx_ob result = JX_OB_NULL;
  jx_int total_size = left_len + right_len + 1;
  jx_char *vla = jx_vla_new(1, total_size + sizeof(jx_str));
  if(vla) {
    jx_os_memcpy(vla + sizeof(jx_str), left, left_len);
    jx_os_memcpy(vla + sizeof(jx_str) + left_len, right, right_len + 1);
    result.data.io.str = vla;
    result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
  }
  return result;
}

jx_ob jx__str_concat(jx_ob left, jx_ob right)
{                               /* on entry, we know the types aren't matched */
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {    /* we have at least one string */
    jx_ob left_str = jx_ob_to_str(left_str);
    jx_ob right_str = jx_ob_to_str(right_str);
    jx_ob result = jx_str_concat(left_str, right_str);
    jx_ob_free(left_str);
    jx_ob_free(right_str);
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

jx_int jx__str_compare(jx_ob left, jx_ob right)
{                               /* on entry, we know the types aren't matched */
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {    /* we have at least one string */
    jx_ob left_str = jx_ob_to_str(left);
    jx_ob right_str = jx_ob_to_str(right);
    jx_int result = jx_str_compare(left_str, right_str);
    jx_ob_free(left_str);
    jx_ob_free(right_str);
    return result;
  }
  return 0;
}

jx_status jx__str_set_shared(jx_char * str, jx_bool shared)
{
  jx_str *I = (jx_str *) str;
  I->gc.shared = shared;
  return JX_SUCCESS;
}

jx_ob jx__str_gc_copy(jx_char * str)
{
  jx_ob result = JX_OB_NULL;
  result.data.io.str = (jx_char *) jx_vla_copy(&str);
  if(result.data.io.str) {
    jx__gc_init((jx_gc *) result.data.io.str);
    result.meta.bits = result.meta.bits = JX_META_BIT_STR | JX_META_BIT_GC;
  }
  return result;
}

jx_bool jx__str_free(jx_char * str)
{
  jx_str *I = (jx_str *) str;
  if(I->gc.shared) {
    return JX_STATUS_FREED_SHARED;
  } else {
    jx_vla_free(&str);
    return JX_SUCCESS;
  }
}

jx_ob jx__ob_add(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & JX_META_BIT_STR) {
    return jx__str_concat(left, right);
  } else if(merge_bits & JX_META_BIT_FLOAT) {
    return jx_ob_from_float(jx_ob_as_float(left) + jx_ob_as_float(right));
  } else if(merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) {
    return jx_ob_from_int(jx_ob_as_int(left) + jx_ob_as_int(right));
  }
  return jx_ob_from_null();
}

jx_bool jx__ob_lt(jx_ob left, jx_ob right)
{
  /* on entry, we know the types aren't matched */
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
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
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
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
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
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
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
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
  jx_bits merge_bits = left.meta.bits | right.meta.bits; \
  if( merge_bits & JX_META_BIT_FLOAT) { \
    return jx_ob_from_float( jx_ob_as_float(left) OPER jx_ob_as_float(right) ); \
  } else if( merge_bits & (JX_META_BIT_INT | JX_META_BIT_BOOL)) { \
    return jx_ob_from_int( jx_ob_as_int(left) OPER jx_ob_as_int(right)); \
  } \
  return jx_ob_from_null(); \
}

JX_MATH_OP(sub, -)
  JX_MATH_OP(mul, *)
  JX_MATH_OP(div, /)
#undef JX_MATH_OP
#define JX_LOG_OP(SUFFIX,OPER) \
jx_ob jx__ob_ ## SUFFIX(jx_ob left, jx_ob right) \
{  /* on entry, we know the types aren't matched */ \
  jx_bits merge_bits = left.meta.bits | right.meta.bits; \
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
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
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
  jx_ob result = JX_OB_NULL;

  if(len < JX_TINY_STR_SIZE) {
    result.meta.bits = JX_META_BIT_IDENT | len;
    jx_os_memcpy(result.data.io.tiny_str, ident, len + 1);
  } else {
    /* string not tiny -- use heap */
    result.data.io.str = jx_vla_new(1, len + 1 + sizeof(jx_str));
    if(result.data.io.str) {
      result.meta.bits = JX_META_BIT_IDENT | JX_META_BIT_GC;
      jx_os_memcpy(result.data.io.str + sizeof(jx_str), ident, len + 1);
    }
  }
  return result;
}

static jx_char jx_ob_as_ident_error[] = "jx_ob_as_ident-Error: object not an identifier.";

jx_char *jx_ob_as_ident(jx_ob * ob)
{
  jx_bits meta = ob->meta.bits;
  if(meta & JX_META_BIT_IDENT) {
    if(meta & JX_META_BIT_GC) {
      return ob->data.io.str + sizeof(jx_str);
    } else {
      return ob->data.io.tiny_str;
    }
  }
  return jx_ob_as_ident_error;
}

jx_ob jx__ident_gc_copy(jx_char * str)
{
  jx_ob result = JX_OB_NULL;
  result.data.io.str = (jx_char *) jx_vla_copy(&str);
  if(result.data.io.str) {
    jx__gc_init((jx_gc *) result.data.io.str);
    result.meta.bits = result.meta.bits = JX_META_BIT_IDENT | JX_META_BIT_GC;
  }
  return result;
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

jx_ob jx_list_new_with_size(jx_int size)
{
  jx_ob result = jx_list_new();
  if(result.meta.bits & JX_META_BIT_LIST) {
    result.data.io.list->data.vla = jx_vla_new(sizeof(jx_ob), size);    /* zero'd memory */
  }
  return result;
}

jx_ob jx_list_new_with_range(jx_int start, jx_int stop, jx_int step)
{
  jx_ob result = jx_list_new();
  jx_int delta = stop - start;
  if(step && ((delta>0)==(step>0))) {
    jx_int steps = (delta / step);
    if(delta % step) steps++;
    if(steps) {
      jx_int *ptr = jx_list_as_int_vla(result);
      if(jx_ok(jx_vla_resize(&ptr,steps))) {
        jx_list_set_int_vla(result,&ptr);
        while(steps--) {
          *(ptr++) = start;
            start += step;
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
    result.data.io.list->data.vla = 
      jx_vla_new_with_content(sizeof(jx_int), size, array);
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

jx_ob jx__list_copy(jx_list * I)
{
  jx_ob result = jx_list_new();
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_LIST) {
      jx_list *new_I = result.data.io.list;
      (*new_I) = (*I);
      jx__gc_init(&new_I->gc);
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
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

static void jx__list_make_strong(jx_list * I)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
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
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
}

static void jx__list_only_strong(jx_list * I)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
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
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
}

jx_status jx__list_set_shared(jx_list * I, jx_bool shared)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    I->gc.shared = shared;
    if(!I->packed_meta_bits) {    /* no need to scan packed structures */
      jx_int size = jx_vla_size(&I->data.ob_vla);
      jx_ob *ob = I->data.ob_vla;
      while(size--) {
        jx_ob_set_shared(*(ob++), shared);
      }
    }
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_status jx__list_set_synchronized(jx_list * I, jx_bool synchronized, jx_bool recursive)
{
  jx_bool synched_on_entry = I->synchronized;
  jx_status status = synched_on_entry ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    I->synchronized = synchronized;
    if(recursive && !I->packed_meta_bits) { 
      jx_int size = jx_vla_size(&I->data.ob_vla);
      jx_ob *ob = I->data.ob_vla;
      while(size--) {
        jx_ob_set_synchronized(*(ob++), synchronized, recursive);
      }
    }
    if(synched_on_entry) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_bool jx__ob_shared(jx_ob ob)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    return ((jx_str *) ob.data.io.str)->gc.shared;
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

jx_int jx__list_index(jx_list * I,jx_ob ob)
{
  jx_int result = -1;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_int I_size = jx__list_size(I);
    if(I_size) {
      if(I->packed_meta_bits) {
        jx_int i;
        switch (I->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
        case JX_META_BIT_INT:
          {
            jx_int *I_int = I->data.int_vla;
            jx_int target = ob.data.io.int_;
            for(i = 0; i < I_size; i++) {
              if(*(I_int++) == target)
                result = i;
            }
          }
          return JX_TRUE;
          break;
        case JX_META_BIT_FLOAT:
          {
            jx_float *I_float = I->data.float_vla;
            jx_float target = ob.data.io.float_;
            for(i = 0; i < I_size; i++) {
              if(*(I_float++) == target)
                result = i;
            }
          }
          break;
        }
      } else {                    /* standard objects */
        jx_int i;
        jx_ob *I_ob = I->data.ob_vla;
        for(i = 0; i < I_size; i++) {
          if(jx_ob_identical(*(I_ob++), ob))
            result = i;
        }
      }
    }
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

static jx_status jx__list_free(jx_tls *tls, jx_list * I)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_SUCCESS;
    if(I->gc.shared) {
      status = JX_STATUS_FREED_SHARED;
      if(I->synchronized) {
        jx_os_spinlock_release(&I->lock);
      }
    } else {
      if(!I->packed_meta_bits) {
        jx_int i, size = jx_vla_size(&I->data.vla);
        jx_ob *ob = I->data.ob_vla;
        for(i = 0; i < size; i++) {
          jx_ob_free_tls(tls,*(ob++));
        }
      }      
      jx_tls_vla_free(tls,&I->data.vla);
      if(I->synchronized) {
        jx_os_spinlock_release(&I->lock);
      }
      jx_tls_list_free(tls,I);
    }
  }
  return status;
}

jx_ob jx__str_join_with_list(jx_list * I, jx_char * sep)
{
  jx_ob result = JX_OB_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_int sep_len = sep ? jx_strlen(sep) : 0;
    jx_int size = jx__list_size(I);
    jx_int total = size ? sep_len * (size - 1) : 0;
    if(!I->packed_meta_bits) { /* TO DO: handle this */
      jx_int i = size;
      jx_ob *ob = I->data.ob_vla;
      while(i--) {
        total += jx_str_len(*(ob++));
      }
      {
        jx_char *vla = jx_vla_new(1, total + 1);
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
    if(I->synchronized) {
      status = jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
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

jx_bool jx__list_equal(jx_list * left, jx_list * right)
{
  jx_bool result = JX_FALSE;
  if(left>right) {
    jx_list *tmp = left;
    left = right; right = tmp;
  }
  {
    jx_status status = (left->synchronized ? 
                        jx_os_spinlock_acquire(&left->lock,JX_TRUE) : JX_YES);
    if(JX_POS(status)) {
      status = (right->synchronized ? 
                jx_os_spinlock_acquire(&right->lock,JX_TRUE) : JX_YES);
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
                              jx__list_borrow_locked(right, i)))
                goto unlock_both;
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
          } else {                    /* standard objects */
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
        if(right->synchronized) {
          jx_os_spinlock_release(&right->lock);
        }
      }
      if(left->synchronized) {
        jx_os_spinlock_release(&left->lock);
      }
    }
  }
  return result;
}

jx_bool jx__list_identical(jx_list * left, jx_list * right)
{
  jx_bool result = JX_FALSE;
  if(left>right) {
    jx_list *tmp = left;
    left = right; right = tmp;
  }
  {
    jx_status status = (left->synchronized ? 
                        jx_os_spinlock_acquire(&left->lock,JX_TRUE) : JX_YES);
    if(JX_POS(status)) {
      status = (right->synchronized ? 
                jx_os_spinlock_acquire(&right->lock,JX_TRUE) : JX_YES);
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
                                  jx__list_borrow_locked(right, i)))
                goto unlock_both;
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
          } else {                    /* standard objects */
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
        if(right->synchronized) {
          jx_os_spinlock_release(&right->lock);
        }
      }
      if(left->synchronized) {
        jx_os_spinlock_release(&left->lock);
      }
    }
  }
  return result;
}

jx_status jx__list_repack_data_locked(jx_list * I) 
{
  /* re-homogenize (if possible) */
  register jx_int size = jx_vla_size(&I->data.vla);
  if(!size) {
    I->packed_meta_bits = 0;
    if(I->data.vla) {
      jx_vla_free(&I->data.vla);
    }
    return JX_TRUE;
  } else if(I->packed_meta_bits) {
    return JX_SUCCESS;
  } else {
    jx_bits meta_or = 0, meta_and = -1;
    {
      register jx_ob *ob = I->data.ob_vla;
      register jx_bits abort_mask = ( JX_META_BIT_BOOL |
                                      JX_META_BIT_LIST |
                                      JX_META_BIT_HASH |
                                      JX_META_BIT_STR );
      register jx_int i;
      for(i = 0; i < size; i++) { 
        register jx_bits bits = (ob++)->meta.bits;
        meta_or |= bits;
        meta_and &= bits;
        if((!meta_and)||(bits & abort_mask))
          return JX_FAILURE;
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
              jx_ob *src = I->data.ob_vla;
              jx_int *dst = (jx_int *) new_vla;
              jx_int i;
              for(i = 0; i < size; i++) {
                *(dst++) = jx_ob_as_int(*(src++));
              }
            }
            break;
          case JX_META_BIT_FLOAT:
            new_vla = jx_vla_new(sizeof(jx_float), size);
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
            jx_vla_free(&I->data.vla);
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

/* mental / debuggin aids */

#define JX_OWN(ob) ob
#define JX_BORROW(ob) ob
jx_status jx__list_resize(jx_list * I, jx_int size, jx_ob fill)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
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
              status = JX_SUCCESS;
              goto unlock;
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
              status = JX_SUCCESS;
              goto unlock;
            }
          }
        } else {
          /* existing list is not empty, but data sizes are known to match */
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
                status = JX_SUCCESS;
                goto unlock;
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
                status = JX_SUCCESS;
                goto unlock;
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
              status = JX_SUCCESS;
              goto unlock;
            }
          }
        }
      }
    }
  unlock:
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_status jx__list_append(jx_list * I, jx_ob ob)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
      goto unlock;
    }
    if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
      if(!jx_ok(jx__list_unpack_data_locked(I))) {
        goto unlock;
      }
    }
    if(I->data.vla) {
      jx_int size = jx_vla_size(&I->data.vla);
      if(jx_vla_grow_check(&I->data.vla, size)) {
        if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
          jx__list_set_packed_data_locked(I, size, ob);
        } else if(!I->packed_meta_bits) {
          I->data.ob_vla[size] = JX_OWN(ob);
        }
        status = JX_SUCCESS;
        goto unlock;
      }
    } else {
      jx_int packed_size = jx_meta_get_packed_size(ob.meta.bits);
      if(packed_size) {
        I->packed_meta_bits = ob.meta.bits;
        I->data.vla = jx_vla_new(packed_size, 1);
        if(I->data.vla) {
          jx__list_set_packed_data_locked(I, 0, ob);
          status = JX_SUCCESS;
          goto unlock;
        }
      } else {
        I->data.vla = jx_vla_new(sizeof(jx_ob), 1);
        if(I->data.vla) {
          I->data.ob_vla[0] = JX_OWN(ob);
          status = JX_SUCCESS;
          goto unlock;
        }
      }
    }
  unlock:
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_status jx__list_insert(jx_list * I, jx_int index, jx_ob ob)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
      goto unlock;
    }
    if(I->data.vla && I->packed_meta_bits && (I->packed_meta_bits != ob.meta.bits)) {
      if(!jx_ok(jx__list_unpack_data_locked(I)))
        goto unlock;
    }
    if(I->data.vla) {
      if(!jx_ok(jx_vla_insert(&I->data.vla, index, 1)))
        goto unlock;
      else if(I->packed_meta_bits && (I->packed_meta_bits == ob.meta.bits)) {
        jx__list_set_packed_data_locked(I, index, ob);
      } else if(!I->packed_meta_bits) {
        I->data.ob_vla[index] = JX_OWN(ob);
      }
      status = JX_SUCCESS;
      goto unlock;
    } else if(index == 0) {
      jx_int packed_size = jx_meta_get_packed_size(ob.meta.bits);
      if(packed_size) {
        I->packed_meta_bits = ob.meta.bits;
        I->data.vla = jx_vla_new(packed_size, 1);
        if(I->data.vla) {
          jx__list_set_packed_data_locked(I, 0, ob);
          status = JX_SUCCESS;
          goto unlock;
         }
      } else {
        I->data.vla = jx_vla_new(sizeof(jx_ob), 1);
        if(I->data.vla) {
          I->data.ob_vla[0] = JX_OWN(ob);
          status = JX_SUCCESS;
          goto unlock;
        }
      }
    }
  unlock:
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_status jx__list_combine(jx_list * list1, jx_list * list2)
{
  if(list1>list2) {
    jx_list *tmp = list1;
    list1 = list2; list2 = tmp;
  }
  {
    jx_status status = (list1->synchronized ? 
                        jx_os_spinlock_acquire(&list1->lock,JX_TRUE) : JX_YES);
    if(JX_POS(status)) {
      status = (list2->synchronized ? 
                jx_os_spinlock_acquire(&list2->lock,JX_TRUE) : JX_YES);
      if(JX_POS(status)) {
        /* consumes list2 */
        if(list1->gc.shared || list2->gc.shared) {
          status = JX_STATUS_PERMISSION_DENIED;
        } else if(list1 == list2) {
          if(list1->synchronized) 
            jx_os_spinlock_release(&list1->lock);
          if(list2->synchronized) 
            jx_os_spinlock_release(&list2->lock);
          {
            jx_ob ob = jx__list_copy(list2);
            if(jx_ok(jx__list_combine(list1, ob.data.io.list))) {
              return JX_SUCCESS; 
            } else {
              jx_ob_free(ob);
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
                  goto unlock_both;
              }
              if(list2->packed_meta_bits) {
                if(!jx_ok(jx__list_unpack_data_locked(list2)))
                  goto unlock_both;
              }
            }
          }
          if(list1->data.vla && list2->data.vla) {
            if(jx_ok(jx__vla_extend(&list1->data.vla, &list2->data.vla))) {
              jx_vla_free(&list2->data.vla);
              if(list2->synchronized) {
                status = jx_os_spinlock_release(&list2->lock);
              }
              jx__list_free(NULL,list2);
              status = JX_SUCCESS;
              goto unlock1;
            }
          } else if(list2->data.vla) {
            (*list1) = (*list2);
            list2->data.vla = NULL;
            if(list2->synchronized) {
              status = jx_os_spinlock_release(&list2->lock);
            }
            jx__list_free(NULL,list2);
            status = JX_SUCCESS;
            goto unlock1;
          }
        }
      unlock_both:
        if(list2->synchronized) {
          jx_os_spinlock_release(&list2->lock);
        }
      }
    unlock1:
      if(list1->synchronized) {
        jx_os_spinlock_release(&list1->lock);
      }
    }
    return status;
  }
}

jx_ob jx__list_remove(jx_list * I, jx_int index)
{
  jx_ob result = JX_OB_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(!I->gc.shared) {
      if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
        if(I->packed_meta_bits) {
          result = jx__list_get_packed_data_locked(I, index);
          jx_vla_remove(&I->data.vla, index, 1);
        } else {
          result = I->data.ob_vla[index];
          jx_vla_remove(&I->data.vla, index, 1); 
        }
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

jx_ob jx__list_new_from_slice(jx_list * I, jx_int start, jx_int stop)
{
  jx_ob result = JX_OB_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_int size = jx_vla_size(&I->data.vla);
    if(start<0) start = size + start + 1;
    if(stop<0) stop = size + stop + 1;
    if(start<0) 
      start = 0;
    else if(start>size)
      start = size;
    if(stop<0)
      stop = 0;
    if(stop>size)
      stop = size;
    {
      jx_int new_size = stop-start;
      result = jx_list_new();
      {
        jx_list *new_I = result.data.io.list;
        *(new_I) = *I;
        jx__gc_init(&new_I->gc);
        new_I->data.vla = jx_vla_new_from_subset(&I->data.vla,start,new_size);
        if(!I->packed_meta_bits) {
          jx_ob *ob = new_I->data.vla;
          while(new_size--) {
            if(ob->meta.bits & JX_META_BIT_GC) {
              *ob = jx_ob_copy(*ob);
            }
            ob++;
          }
        }
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

jx_ob jx__list_new_with_cutout(jx_list * I, jx_int start, jx_int stop)
{
  jx_ob result = JX_OB_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_int size = jx_vla_size(&I->data.vla);
    if(start<0) start = size + start + 1;
    if(stop<0) stop = size + stop + 1;
    if(start<0) 
      start = 0;
    else if(start>size)
      start = size;
    if(stop<0)
      stop = 0;
    if(stop>size)
      stop = size;
    {
      jx_int new_size = stop-start;
      jx_ob result = jx_list_new();
      {
        jx_list *new_I = result.data.io.list;
        *(new_I) = *I;
        jx__gc_init(&new_I->gc);
        new_I->data.vla = jx_vla_new_from_subset(&I->data.vla,start,new_size);
        jx_vla_remove(&I->data.vla,start,new_size);
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

jx_status jx__list_delete(jx_list * I, jx_int index)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else if((index >= 0) && (index < jx_vla_size(&I->data.vla))) {
      if(!I->packed_meta_bits) {
        jx_ob_free(I->data.ob_vla[index]);
      }
      status = jx_vla_remove(&I->data.vla, index, 1);
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_int *jx_list_as_int_vla(jx_ob ob)
{
  jx_list *I = ob.data.io.list;
  jx_int *result = JX_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(ob.meta.bits & JX_META_BIT_LIST) {
      if(I->packed_meta_bits & JX_META_BIT_INT) {
        result = I->data.int_vla;
      } else if(jx_ok(jx__list_repack_data_locked(I))) {
        if(I->packed_meta_bits & JX_META_BIT_INT) {
          result = I->data.int_vla;
        } else if(!I->data.vla) {
          I->packed_meta_bits = JX_META_BIT_INT;
          I->data.int_vla = (jx_int *) jx_vla_new(sizeof(jx_int), 0);
          result = I->data.int_vla;
        }
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

jx_status jx__list_set_int_vla(jx_list * I, jx_int ** ref)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
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
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_float *jx_list_as_float_vla(jx_ob ob)
{
  jx_list *I = ob.data.io.list;
  jx_float *result = JX_NULL;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(ob.meta.bits & JX_META_BIT_LIST) {
      if(I->packed_meta_bits & JX_META_BIT_FLOAT) {
        result = I->data.float_vla;
      } else if(jx__list_repack_data_locked(I)) {
        if(I->packed_meta_bits & JX_META_BIT_FLOAT) {
          result = I->data.float_vla;
        } else if(!I->data.vla) {
          I->packed_meta_bits = JX_META_BIT_FLOAT;
          I->data.float_vla = (jx_float *) jx_vla_new(sizeof(jx_float), 0);
          result = I->data.float_vla;
        }
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
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
  jx_bits bits = ob.meta.bits;
  if(bits & (JX_META_BIT_STR | JX_META_BIT_IDENT)) {
    if(bits & JX_META_BIT_STR)
      return jx__c_str_hash(jx_ob_as_str(&ob));
    else
      return jx__c_str_hash(jx_ob_as_ident(&ob));
  } else if(bits & JX_META_BIT_BUILTIN) {
    return jx__ob_hash_code(ob);
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

jx_ob jx__hash_copy(jx_hash * hash)
{
  jx_ob result = jx_hash_new();
  jx_status status = hash->synchronized ? 
    jx_os_spinlock_acquire(&hash->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(result.meta.bits & JX_META_BIT_HASH) {
      jx_hash *I = result.data.io.hash;
      *I = *hash;
      jx__gc_init(&I->gc);
      I->info = jx_vla_copy(&I->info);
      I->key_value = jx_vla_copy(&I->key_value);
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
    if(hash->synchronized) {
      status = jx_os_spinlock_release(&hash->lock);
    }
  }
  return result;
}

static void jx__hash_make_strong(jx_hash * I)
{ 
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_int i, size = jx_vla_size(&I->key_value);
    jx_ob *ob = I->key_value;
    for(i = 0; i < size; i++) {
      if(ob->meta.bits & JX_META_BIT_GC) {
        *ob = jx_ob_make_strong_with_ob(*ob);
      }
      ob++;
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
}

static void jx__hash_only_strong(jx_hash * I)
{ 
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {

    jx_int i, size = jx_vla_size(&I->key_value);
    jx_ob *ob = I->key_value;
    for(i = 0; i < size; i++) {
      if(ob->meta.bits & JX_META_BIT_GC) {
        *ob = jx_ob_only_strong_with_ob(*ob);
      }
      ob++;
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
   }
}

static jx_status jx__hash_free(jx_tls *tls, jx_hash * I)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(I->gc.shared) {
      status = JX_STATUS_FREED_SHARED;
      if(I->synchronized) {
        jx_os_spinlock_release(&I->lock);
      } 
    } else {
      jx_int size = jx_vla_size(&I->key_value);
      if(size) {
        jx_ob *ob = I->key_value;
        jx_int i;
        for(i = 0; i < size; i++) {
          jx_ob_free(*(ob++));
        }
      }
      jx_tls_vla_free(tls,&I->key_value);
      jx_tls_vla_free(tls,&I->info);
      if(I->synchronized) {
        jx_os_spinlock_release(&I->lock);
      }
      jx_tls_hash_free(tls,I);
      status = JX_SUCCESS;
    }
  }
  return status;
}

jx_int jx__hash_size(jx_hash * I)
{
  jx_int result = 0;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(I->key_value) {
      if(!I->info) {              /* no info mode -- search & match objects directly  */
        result = jx_vla_size(&I->key_value) >> 1;
      } else {
        jx_hash_info *info = (jx_hash_info *) I->info;
        result = info->usage;
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
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
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_FAILURE;
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else {
      jx_uint32 hash_code = jx_ob_hash_code(key);
      if(!hash_code) {            /* unhashable key */
        status = JX_STATUS_OB_NOT_HASHABLE;
      } else {
        if(!I->info) {            /* "no info" mode -- search & match objects directly  */
          if(!I->key_value) {
            /* new table, first entry  */
            I->key_value = jx_vla_new(sizeof(jx_ob), 2);
            if(I->key_value) {
              I->key_value[0] = JX_OWN(key);      /* takes ownership */
              I->key_value[1] = JX_OWN(value);    /* takes ownership */
              status = JX_SUCCESS;
            }
          } else {
            jx_uint32 size = jx_vla_size(&I->key_value);
            if(1) {
              jx_bool found = JX_FALSE;
              register jx_uint32 i = (size >> 1);
              register jx_ob *ob = I->key_value;
              while(i--) {
#if 0
                fprintf(stderr, "%08x %04x %04x == %08x %04x %04x %d\n",
                        (unsigned int) ob[0].data.raw.word,
                        (unsigned int) ob[0].data.raw.bits,
                        (unsigned int) ob[0].meta.bits,
                        (unsigned int) key.data.raw.word,
                        (unsigned int) key.data.raw.bits,
                        (unsigned int) key.meta.bits, jx_ob_identical(ob[0], key)
                        );
#endif
                if(jx_ob_identical(ob[0], key)) {
                  found = JX_TRUE;
                  jx_ob_free(ob[0]);
                  jx_ob_free(ob[1]);
                  ob[0] = JX_OWN(key);    /* takes ownership */
                  ob[1] = JX_OWN(value);  /* takes ownership */
                  status = JX_SUCCESS;
                  break;
                }
                ob += 2;
              }
              if(!found) {
                if(jx_ok(jx_vla_grow_check(&I->key_value, size + 1))) {
                  I->key_value[size] = key;
                  I->key_value[size + 1] = value;
                  status = JX_SUCCESS;
                  if(size > 8) {  /* switch to linear hash once we have more than four elements */
                    jx__hash_recondition(I, JX_HASH_LINEAR, JX_FALSE);
                  }
                }
              }
            }
          }
        } else {                  /* we have an info record */
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
                    status = JX_SUCCESS;
                    if(info->usage > 8) {
                      jx__hash_recondition(I, JX_HASH_ONE_TO_ANY, JX_FALSE);     
                      /* switch to true hash */
                    }
                  }
                } else {
                  jx_ob_free(ob[0]);
                  jx_ob_free(ob[1]);
                  ob[0] = JX_OWN(key);
                  ob[1] = JX_OWN(value);
                  status = JX_SUCCESS;
                }
              }
              break;
            case JX_HASH_ONE_TO_ANY:
            case JX_HASH_ONE_TO_NIL:
              if((info->usage + 1) > (3 * info->mask) >> 2) {     /* more than ~3/4'rs full */
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
                      kv_offset = ((info->stale_usage + usage) << 1);     /* allocate new key_value */
                    } else {
                      kv_offset = (dest_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);   /* use existing key_value */
                    }
                    if(jx_ok(jx_vla_grow_check(&I->key_value, kv_offset + 1))) {
                      key_value = I->key_value;
                      if(!dest_virgin)
                        info->stale_usage--;
                      dest_ptr[0] = hash_code;
                      dest_ptr[1] = (kv_offset | JX_HASH_ENTRY_ACTIVE);
                      key_value[kv_offset] = JX_OWN(key);
                      key_value[kv_offset + 1] = JX_OWN(value);
                      status = JX_SUCCESS;
                      info->usage++;
                    } else {
                      status = JX_FAILURE;
                    }
                  } else {
                    jx_uint32 kv_offset = (dest_ptr[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                    jx_ob_free(key_value[kv_offset]);
                    jx_ob_free(key_value[kv_offset + 1]);
                    key_value[kv_offset] = JX_OWN(key);
                    key_value[kv_offset + 1] = JX_OWN(value);
                    status = JX_SUCCESS;
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
                  if((info->usage + 1) > (3 * info->mask) >> 2) { /* more than ~3/4'rs full */
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
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

static jx_bool jx__hash_equal(jx_hash * left, jx_hash * right)
{
  /* take pairwise locks in memory order to avoid deadlock */
  if(left>right) {
    jx_hash *tmp = left;
    left = right; right = tmp;
  }
  {
    jx_status status = (left->synchronized ? 
                        jx_os_spinlock_acquire(&left->lock,JX_TRUE) : JX_YES);
    if(JX_POS(status)) {
      status = (right->synchronized ? 
                jx_os_spinlock_acquire(&right->lock,JX_TRUE) : JX_YES);
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
                jx_ob right_value = JX_OB_NULL;
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
                      jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                      jx_ob right_value = JX_OB_NULL;
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
        if(right->synchronized) {
          jx_os_spinlock_release(&right->lock);
        }
      }
      if(left->synchronized) {
        jx_os_spinlock_release(&left->lock);
      }
    }
  }

  return JX_FALSE;
}

static jx_bool jx__hash_identical(jx_hash * left, jx_hash * right)
{
  /* take pairwise locks in memory order to avoid deadlock */
  if(left>right) {
    jx_hash *tmp = left;
    left = right; right = tmp;
  }
  {
    jx_status status = (left->synchronized ? 
                        jx_os_spinlock_acquire(&left->lock,JX_TRUE) : JX_YES);
    if(JX_POS(status)) {
      status = (right->synchronized ? 
                jx_os_spinlock_acquire(&right->lock,JX_TRUE) : JX_YES);
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
                jx_ob right_value = JX_OB_NULL;
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
                      jx_ob *kv_ob = key_value + (hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK);
                      jx_ob right_value = JX_OB_NULL;
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
        if(right->synchronized) {
          jx_os_spinlock_release(&right->lock);
        }
      }
      if(left->synchronized) {
        jx_os_spinlock_release(&left->lock);
      }
    }
  }
  return JX_FALSE;
}

jx_ob jx__hash_copy_members(jx_hash * I, jx_int flags)
{
  jx_ob result = jx_list_new();
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {
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
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return result;
}

jx_status jx__hash_set_shared(jx_hash * I, jx_bool shared)
{
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* active slot with matching hash code */
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
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_status jx__hash_set_synchronized(jx_hash * I, 
                                    jx_bool synchronized, 
                                    jx_bool recursive)
{
  jx_bool synched_on_entry = I->synchronized;
  jx_status status = synched_on_entry ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    status = JX_SUCCESS;
    I->synchronized = synchronized;
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {  /* active slot with matching hash code */
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
      jx_os_spinlock_release(&I->lock);
    }
  }
  return status;
}

jx_bool jx__hash_has_key(jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
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
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return found;
}

jx_bool jx__hash_peek(jx_ob * result, jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    if(size) {
      if(!I->info) {              /* JX_HASH_RAW */
        register jx_int i = (size >> 1);
        register jx_ob *ob = I->key_value;
        while(i--) {
          if(jx_ob_identical(ob[0], key)) {
            found = JX_TRUE;
            *result = JX_BORROW(ob[1];
                                );
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
                    *result = JX_BORROW(ob[1];
                                        );
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
                    *result = JX_BORROW(kv_ob[1];
                                        );
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
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return found;
}

jx_bool jx__hash_remove(jx_ob * result, jx_hash * I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    if(I->gc.shared) {
      status = JX_STATUS_PERMISSION_DENIED;
    } else {
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
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return found;
}


jx_bool jx__hash_peek_key(jx_ob * result, jx_hash * I, jx_ob value)
{
  jx_bool found = JX_FALSE;
  jx_status status = I->synchronized ? 
    jx_os_spinlock_acquire(&I->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_uint32 size = jx_vla_size(&I->key_value);
    if(size) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        jx_ob *ob = I->key_value;
        while(i--) {              /* brute-force table scan */
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
                }
                index = (index + 1) & mask;
              } while(index != sentinel);
            }
            break;
          }
        }
      }
    }
    if(I->synchronized) {
      jx_os_spinlock_release(&I->lock);
    }
  }
  return found;
}

jx_status jx__hash_from_list(jx_hash * hash, jx_list * list)
{
  jx_status status = list->synchronized ? 
    jx_os_spinlock_acquire(&list->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    jx_int size = jx__list_size(list);
    status = JX_SUCCESS;
    if(0x1 & size) {
      status = JX_FAILURE;
    } else if(size) {
      jx_int i;
      for(i = 0; i < size; i += 2) {
        jx_ob key = jx_ob_copy(jx__list_borrow_locked(list, i));
        jx_ob value = jx_ob_copy(jx__list_borrow_locked(list, i + 1));
        if(!jx_ok(jx__hash_set(hash, key, value))) {
          jx_ob_free(key);
          jx_ob_free(value);
          status = JX_FAILURE;
        }
      }
    }
    if(list->synchronized) {
      jx_os_spinlock_release(&list->lock);
    }
  }
  return status;
}

jx_status jx__list_with_hash(jx_list * list, jx_hash * hash)
{
  jx_status status = hash->synchronized ? 
    jx_os_spinlock_acquire(&hash->lock,JX_TRUE) : JX_YES;
  if(JX_POS(status)) {
    status = JX_SUCCESS;
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
                    jx_os_memcpy(key_value + count, ob, sizeof(jx_ob) * 2);
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
    if(hash->synchronized) {
      jx_os_spinlock_release(&hash->lock);
    }
  }
  return status;
}

jx_status jx__hash_with_list(jx_hash * hash, jx_list * list)
{
  jx_status status = list->synchronized ? 
    jx_os_spinlock_acquire(&list->lock,JX_TRUE) : JX_YES;
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
    }
    if(list->synchronized) {
      jx_os_spinlock_release(&list->lock);
    }
  }
  return status;
}

/* shared: read only immortal gc'd objects & containers */

jx_status jx__ob_set_shared(jx_ob ob, jx_bool shared)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    return jx__str_set_shared(ob.data.io.str, shared);
    break;
  case JX_META_BIT_LIST:
    return jx__list_set_shared(ob.data.io.list, shared);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_set_shared(ob.data.io.hash, shared);
    break;
  }
  return JX_SUCCESS;
}

/* synchronized: all access subject to spinlocks */

jx_status jx__ob_set_synchronized(jx_ob ob, jx_bool synchronized, jx_bool recursive)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_LIST:
    return jx__list_set_synchronized(ob.data.io.list, synchronized, recursive);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_set_synchronized(ob.data.io.hash, synchronized, recursive);
    break;
  }
  return JX_SUCCESS;
}

jx_ob jx__builtin_copy(jx_ob ob)
{
  jx_ob result = JX_OB_NULL;
  jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_BUILTIN_VLA) {
    result = ob;
    result.data.io.vla = jx_vla_copy(&ob.data.io.vla);
  } else if(bits & JX_META_BIT_BUILTIN_OPAQUE_OB) {
  } else if(bits & JX_META_BIT_BUILTIN_FUNCTION) {
    jx_function *fn = ob.data.io.function;
    return jx_function_new_with_def(jx_ob_copy(fn->name), 
                                    jx_ob_copy(fn->args), 
                                    jx_ob_copy(fn->body),
                                    fn->block);
  } else if(bits & JX_META_BIT_BUILTIN_MACRO) {
    jx_function *fn = ob.data.io.function;
    return jx_macro_new_with_def(jx_ob_copy(fn->name), 
                                 jx_ob_copy(fn->args), 
                                 jx_ob_copy(fn->body));
  }
  return result;
}

/* copying */

jx_ob jx__ob_copy(jx_ob ob)
{
  /* on entry, we know the object is GC'd */
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    return jx__str_gc_copy(ob.data.io.str);
    break;
  case JX_META_BIT_IDENT:
    return jx__ident_gc_copy(ob.data.io.str);
    break;
  case JX_META_BIT_LIST:
    return jx__list_copy(ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_copy(ob.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    return jx__builtin_copy(ob);
   break;
  }
  return jx_ob_from_null();
}

jx_ob jx__ob_make_strong_with_ob(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_WEAK_REF) {
    /* on entry, we know the object is GC'd */
    switch (bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      return jx__str_gc_copy(ob.data.io.str);
      break;
    case JX_META_BIT_IDENT:
      return jx__ident_gc_copy(ob.data.io.str);
      break;
    case JX_META_BIT_LIST:
      return jx__list_copy(ob.data.io.list);
      break;
    case JX_META_BIT_HASH:
      return jx__hash_copy(ob.data.io.hash);
      break;
    case JX_META_BIT_BUILTIN:
      return jx__builtin_copy(ob);
      break;
    }
  } else {
    switch (bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      jx__list_make_strong(ob.data.io.list);
      break;
    case JX_META_BIT_HASH:
      jx__hash_make_strong(ob.data.io.hash);
      break;
    }
  }
  return ob;
}

jx_ob jx__ob_only_strong_with_ob(jx_ob ob)
{
  jx_bits bits = ob.meta.bits;
  if(bits & JX_META_BIT_WEAK_REF) {
    ob = jx_null_with_ob(ob);
  } else {
    switch (bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_LIST:
      jx__list_only_strong(ob.data.io.list);
      break;
    case JX_META_BIT_HASH:
      jx__hash_only_strong(ob.data.io.hash); /* TO DO fix resuting corruption of null entry */
      break;
    }
  }
  return ob;
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
    return jx__list_identical(left.data.io.list,right.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_identical(left.data.io.hash,right.data.io.hash);
    break;
  }
  return JX_FALSE;
}

jx_bool jx__ob_non_gc_equal(jx_ob left, jx_ob right)
{ /* we know incoming types don't match */
  jx_bits merge_bits = left.meta.bits | right.meta.bits;
  if(merge_bits & (JX_META_BIT_STR|JX_META_BIT_IDENT)) {
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
  }
  return JX_FALSE;
}

/* functions */

jx_ob jx_function_new_with_def(jx_ob name, jx_ob args, jx_ob body, jx_bool block)
{
  jx_ob result = JX_OB_NULL;
  jx_function *fn = (jx_function*) jx_calloc(1, sizeof(jx_function));
  if(fn) {
    fn->name = name;
    fn->args = args;
    fn->body = body;
    fn->block = block;

    //jx_jxon_dump(stdout,"new function name",name);
    //jx_jxon_dump(stdout,"new function args",args);
    //jx_jxon_dump(stdout,"new function code",body);
    //fprintf(stdout,"new function block: %d\n",block);
    result.data.io.function = fn;
    result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_FUNCTION | JX_META_BIT_GC;
  } else {
    jx_ob_free(name);
    jx_ob_free(args);
    jx_ob_free(body);
  }
  return result;
}

jx_ob jx_macro_new_with_def(jx_ob name, jx_ob args, jx_ob body)
{
  jx_ob result = JX_OB_NULL;
  jx_function *fn = (jx_function*) jx_calloc(1, sizeof(jx_function));
  if(fn) {
    fn->name = name;
    fn->args = args;
    fn->body = body;
    fn->block = JX_FALSE;

    //jx_jxon_dump(stdout,"new function name",name);
    //jx_jxon_dump(stdout,"new function args",args);
    //jx_jxon_dump(stdout,"new function code",body);
    //fprintf(stdout,"new function block: %d\n",block);
    result.data.io.function = fn;
    result.meta.bits = JX_META_BIT_BUILTIN | JX_META_BIT_BUILTIN_MACRO | JX_META_BIT_GC;
  } else {
    jx_ob_free(name);
    jx_ob_free(args);
    jx_ob_free(body);
  }
  return result;
}

jx_ob jx_function_to_impl(jx_ob ob)
{
  if(jx_function_check(ob)) {
    jx_function *fn = ob.data.io.function;  
    jx_ob result = jx_list_new_with_size(4);
    jx_list_replace(result, 0, jx_ob_copy(fn->name));
    jx_list_replace(result, 1, jx_ob_copy(fn->args));
    jx_list_replace(result, 2, jx_ob_copy(fn->body));
    jx_list_replace(result, 3, jx_ob_from_bool(fn->block));
    return result;
  }
  return jx_ob_from_null();
}

jx_ob jx_macro_to_impl(jx_ob ob)
{
  if(jx_function_check(ob)) {
    jx_function *fn = ob.data.io.function;  
    jx_ob result = jx_list_new_with_size(3);
    jx_list_replace(result, 0, jx_ob_copy(fn->name));
    jx_list_replace(result, 1, jx_ob_copy(fn->args));
    jx_list_replace(result, 2, jx_ob_copy(fn->body));
    return result;
  }
  return jx_ob_from_null();
}

/* freeing */

jx_status jx__ob_free(jx_tls *tls, jx_ob ob)
{ /* on entry, we know its GC and not weak*/
  register jx_bits bits = ob.meta.bits;
  switch (bits & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT:
    return jx__str_free(ob.data.io.str);
    break;
  case JX_META_BIT_LIST:
    return jx__list_free(tls,ob.data.io.list);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_free(tls,ob.data.io.hash);
    break;
  case JX_META_BIT_BUILTIN:
    if(bits & JX_META_BIT_BUILTIN_VLA) {
      jx_vla_free(&ob.data.io.vla);
    } else if(bits & JX_META_BIT_BUILTIN_OPAQUE_OB) {
      jx_opaque_ob *opaque = ob.data.io.opaque_ob;
      if(opaque) {
        jx_opaque_free_fn fn = opaque->free_fn;
        if(fn) {
          return fn(opaque);
        } else {
          return JX_SUCCESS;
        }
      }
    } else if(bits & (JX_META_BIT_BUILTIN_FUNCTION|JX_META_BIT_BUILTIN_MACRO)) {
      jx_function *fn = ob.data.io.function;
      jx_ob_free_tls(tls,fn->name);
      jx_ob_free_tls(tls,fn->args);
      jx_ob_free_tls(tls,fn->body);
      jx_free(fn);
    }
    break;
  }
  return JX_SUCCESS;
}
