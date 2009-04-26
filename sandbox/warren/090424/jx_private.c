
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_private.h"

/* optional memory logger, counter, & overrun detection wrapper */
#if 0
#define jx_malloc(s) malloc(s)
#define jx_calloc(c,s) calloc(c,s)
#define jx_realloc(p,s) realloc(p,s)
#define jx_free(p) free(p)
#else
#define JX_MEM_LOG_ALL 0
#define jx_malloc(s) jx__malloc(s,__FILE__,__LINE__)
#define jx_calloc(c,s) jx__calloc(c,s,__FILE__,__LINE__)
#define jx_realloc(p,s) jx__realloc(p,s,__FILE__,__LINE__)
#define jx_free(p) jx__free(p,__FILE__,__LINE__)
#define JX_TRIPWIRE_SIZE 128
typedef struct {
  size_t size;
  jx_int active;
  jx_char tripwire[JX_TRIPWIRE_SIZE];
} jx_mem_hdr;
typedef struct {
  jx_char tripwire[JX_TRIPWIRE_SIZE];
  size_t size;
} jx_mem_ftr;
static jx_int jx_mem_count = 0; 
static void jx_mem_dump(void)
{
  if(JX_MEM_LOG_ALL || jx_mem_count)
    fprintf(stderr,"jx_mem_count: %d blocks remain allocated.\n",jx_mem_count);
}
static jx_int jx_mem_count_init = 1;
static void jx_mem_count_inc(void)
{
  if(jx_mem_count_init) { jx_mem_count_init=0; atexit(jx_mem_dump); }
  jx_mem_count++; /* not atomic - not thread-accurate */
}
static void jx_mem_count_dec(void)
{
  jx_mem_count--;  /* not atomic - not thread-accurate */
}
static void *jx_tripwire_set(void *ptr, size_t size)
{
  jx_mem_hdr *hdr = (jx_mem_hdr*)ptr;
  void *result = hdr + 1;
  jx_mem_ftr *ftr = (jx_mem_ftr*)(((jx_char*)result) + size);
  hdr->active = 0x1EE254EC;
  hdr->size = size;
  ftr->size = size;
  {
    jx_char seed = ((((jx_char*)hdr) - (jx_char*)0) >> 2) + size;
    jx_char *h_tw = hdr->tripwire, *f_tw = ftr->tripwire;
    jx_int i;
    for(i=0;i<JX_TRIPWIRE_SIZE;i++) {
      *(h_tw++) = *(f_tw++) = seed;
      seed += size;
    }
  }
  return result;
}
static void *jx_tripwire_check(void *ptr,char *file,int line)
{
  jx_mem_hdr *hdr = ((jx_mem_hdr*)ptr) - 1;
  jx_mem_ftr *ftr = (jx_mem_ftr*) ((jx_char*)ptr + hdr->size);
  if(hdr->active == 0xDEADDEAD) {
    fprintf(stderr,"jx_tripwire: %p has been freed twice (%s:%d)\n",
            ptr,file,line);
  } else if(hdr->active != 0x1EE254EC) {
    fprintf(stderr,"jx_tripwire: %p is not an active block (%s:%d)\n",
            ptr,file,line);
  } else if(hdr->size != ftr->size) {
    fprintf(stderr,"jx_tripwire: %p sizes do not match (0x%lx != 0x%lx) (%s:%d)\n",
            ptr,hdr->size,ftr->size,file,line);
  } else {
    size_t size = hdr->size;
    jx_char seed = ((((jx_char*)hdr) - (jx_char*)0) >> 2) + size;
    jx_char *h_tw = hdr->tripwire, *f_tw = ftr->tripwire;
    jx_int i;
    for(i=0;i<JX_TRIPWIRE_SIZE;i++) {
      if( (*(h_tw++) != seed )) {
        fprintf(stderr, "jx_tripwire: %p header overrun (byte 0x%x) (%s:%d)\n",ptr,i,file,line);
      }
      if( (*(f_tw++) != seed )) {
        fprintf(stderr, "jx_tripwire: %p footer overrun (byte 0x%x) (%s:%d)\n",ptr,i,file,line);
      }
      seed += size;
    } 
  }
  hdr->active = 0xDEADDEAD;
  return (void*)hdr;
}
static void *jx__malloc(size_t size, char *file, int line)
{
  void *result = malloc(size + sizeof(jx_mem_hdr) + sizeof(jx_mem_ftr));
  jx_mem_count_inc();
  if(JX_MEM_LOG_ALL)
    fprintf(stderr,"jx_malloc: %p (%lx) (%s:%d)\n",result,size,file,line);
  return jx_tripwire_set(result,size);
}
static void *jx__calloc(size_t count, size_t size, char *file, int line)
{
  void *ptr = calloc(count, size + sizeof(jx_mem_hdr) + sizeof(jx_mem_ftr));
  void *result = jx_tripwire_set(ptr,size);
  jx_mem_count_inc();
  if(JX_MEM_LOG_ALL)
    fprintf(stderr,"jx_calloc: %p (%lx,%lx) (%s:%d)\n",result,count,size,file,line);
  return result;
}
static void *jx__realloc(void *ptr, size_t size, char *file, int line)
{
  void *true_ptr = jx_tripwire_check(ptr,file,line);
  void *new_ptr = realloc(true_ptr, size + sizeof(jx_mem_hdr) + sizeof(jx_mem_ftr));
  if(new_ptr) {
    void *result = jx_tripwire_set(new_ptr, size);
    if(JX_MEM_LOG_ALL)
      fprintf(stderr,"jx_realloc: %p -> %p (%lx) (%s:%d)\n",ptr,result,size,file,line);
    return result;
  } else {
    printf("here\n");
    fprintf(stderr,"jx_realloc: failed %p (%lx) (%s:%d)\n",ptr,size,file,line);
    return NULL;
  }
}
void jx__free(void *ptr, char *file, int line)
{
  void *true_ptr = jx_tripwire_check(ptr,file,line);
  jx_mem_count_dec();
  if(JX_MEM_LOG_ALL)
    fprintf(stderr,"jx_free: %p (%s:%d)\n",ptr,file,line);
  free(true_ptr);
}
#endif

/* jx_vla routines */

void *jx_vla_new(jx_int rec_size,jx_int size)
{
  jx_vla *vla = jx_calloc(1, sizeof(jx_vla) + rec_size * size);
  if(vla) {
    vla->size = size;
    vla->rec_size = rec_size;
    return (void*)(vla + 1);
  } else
    return NULL;
}

jx_status jx__vla_resize(void **ref, jx_int new_size)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla*)(*ref)) - 1;
    if(new_size != vla->size) {
      vla = jx_realloc(vla, sizeof(jx_vla) + vla->rec_size * new_size);
      if(vla) {
        *ref = (void*)(vla + 1);
        if(vla->size < new_size) { /* zero new memory */
          jx_char *base = (jx_char*)(*ref);
          memset( base + vla->size * vla->rec_size, 0, 
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
    jx_vla *vla = ((jx_vla*)(*ref)) - 1;
    if(count >= 0) {
      jx_int new_size = vla->size + count;
      vla = jx_realloc(vla, sizeof(jx_vla) + vla->rec_size * new_size);
      if(vla) {
        jx_char *base = (jx_char*)(vla+1);
        (*ref) = base;
        memset(base + vla->rec_size * vla->size, 0, vla->rec_size * count);
        vla->size = new_size;
        return JX_SUCCESS;
      }
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_append_c_str(void **ref, jx_char *str)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla*)(*ref)) - 1;
    if(vla->rec_size == 1) {
      jx_int str_len = strlen(str);
      jx_int size = vla->size;
      jx_status status = jx_vla_append(ref, (size ? str_len : str_len+1));
      if(jx_ok(status)) {
        jx_char *base = (jx_char*)(*ref);
        memcpy(base + (size ? size-1 : 0), str, str_len+1);
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
    jx_vla *vla = ((jx_vla*)(*ref)) - 1;
    if(vla->rec_size == 1) {
      jx_int str_len = jx_str_len(ob);
      jx_int size = vla->size;
      jx_status status = jx_vla_append(ref, (size ? str_len : str_len+1));
      if(jx_ok(status)) {
        jx_char *base = (jx_char*)(*ref);
        memcpy(base + (size ? size-1 : 0), jx_ob_as_str(&ob), str_len+1);
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
    jx_vla *vla = ((jx_vla*)(*ref)) - 1;
    if( (index >= 0) && (count >= 0) && (index <= vla->size) ) {
      jx_int new_size = vla->size + count;
      vla = jx_realloc(vla, sizeof(jx_vla) + vla->rec_size * new_size);
      if(vla) {
        jx_char *base = (jx_char*)(vla+1);
        vla->size = new_size;
        (*ref) = base;
        memmove(base + index * vla->rec_size, 
                base + (index+count) * vla->rec_size,
                vla->rec_size * count );
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
    jx_vla *vla1 = ((jx_vla*)(*ref1)) - 1;    
    jx_vla *vla2 = ((jx_vla*)(*ref2)) - 1;    
    if(vla1->rec_size == vla2->rec_size) {
      jx_int new_size = vla1->size + vla2->size;
      vla1 = jx_realloc(vla1, sizeof(jx_vla) + vla1->rec_size * new_size);
      if(vla1) {
        jx_char *base1 = (jx_char*)(vla1+1);
        jx_char *base2 = (jx_char*)(vla2+1);
        (*ref1) = base1;
        memcpy(base1 + vla1->rec_size * vla1->size,
               base2, vla2->rec_size * vla2->size);
        vla1->size = new_size;
        return JX_SUCCESS;
      }
    } 
  }
  return JX_FAILURE;
}

jx_status jx__vla_remove(void **ref, jx_int index, jx_int count)
{
  if(*ref) {
    jx_vla *vla = ((jx_vla*)(*ref)) - 1;
    if( (index >= 0) && (count >= 0) && 
        (index < vla->size) && ((count+index) <= vla->size) ) {
      jx_int new_size = vla->size - count;
      jx_char *base = (jx_char*)(*ref);
      memmove(base + index * vla->rec_size,
              base + (count+index) * vla->rec_size,
              ((vla->size - index) - count) * vla->rec_size);
      return jx_vla_resize(ref, new_size);
    }
  }
  return JX_FAILURE;
}

jx_status jx__vla_free(void **ref)
{
  if(*ref) {
    jx_free( ((jx_vla*)(*ref)) - 1 );
    (*ref) = NULL;
    return JX_SUCCESS;
  } else {
    return JX_FAILURE;
  }
}

/* strings */

jx_ob jx_ob_from_str(jx_char *str)
{
  jx_int size = strlen(str);
  jx_ob result = JX_OB_NULL;

  if(size < JX_TINY_STR_SIZE) {
    result.meta = JX_META_BIT_STR | size;
    strcpy(result.data.tiny_str, str);
  } else {
    /* string not tiny -- use heap */
    result.meta = JX_META_BIT_STR | JX_META_BIT_GC;
    result.data.str = jx_vla_new(1,size+1);
    if(result.data.str) {
      strcpy(result.data.str, str);
    }
  }
  return result;
}

static jx_ob jx_ob_with_str_vla(jx_char *vla)
{
  jx_int size = jx_vla_size(&vla);
  jx_ob result = JX_OB_NULL;
  if(size) size--; 
  if(size < JX_TINY_STR_SIZE) {
    result.meta = JX_META_BIT_STR | size;
    strcpy(result.data.tiny_str, vla);
    jx_vla_free(&vla);
  } else {
    result.meta = JX_META_BIT_STR | JX_META_BIT_GC;
    result.data.str = vla;
  }
  return result;
}

jx_char *jx_ob_as_str(jx_ob *ob) {
  jx_meta meta = ob->meta;
  if(meta & JX_META_BIT_STR) {
    if(meta & JX_META_BIT_GC) {
      return ob->data.str;
    } else {
      return ob->data.tiny_str;
    }
  }
  return NULL;
}

/* lists */

jx_ob jx_list_new(void)
{
  jx_ob result = JX_OB_LIST;
  result.data.list = jx_calloc(1,sizeof(jx_list));
  return result;
}

static void jx_list_free(jx_list *list)
{
  if(!list->packed_meta) {
    jx_int i, size = jx_vla_size(&list->data.vla);
    jx_ob *ob = list->data.ob_vla;
    for(i=0;i<size;i++) {
      jx_ob_free(*(ob++));
    }
  }
  jx_vla_free(&list->data.vla);
  jx_free(list);
}

jx_int jx_list_size(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_LIST) {
    return jx_vla_size(&ob.data.list->data.vla);
  } else
    return 0;
}

static jx_status jx_meta_get_packed_size(jx_meta meta)
{
  switch(meta & JX_META_MASK_TYPE_BITS) {
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

static jx_status jx_list_repack_data(jx_list *list)
{
  /* re-homogenize (if possible) */
  jx_int i, size = jx_vla_size(&list->data.vla);    
  if(!size) {
    list->packed_meta = 0;
    if(list->data.vla) {
      jx_vla_free(&list->data.vla);
    }
    return JX_TRUE;
  }
  jx_meta meta_or = 0, meta_and = -1;
  {
    jx_ob *ob = list->data.ob_vla;
    for(i=0;i<size;i++) { 
      meta_or |= ob->meta;
      meta_and &= (ob++)->meta;
    }
  }
  if(meta_and == meta_or) { /* all meta bits identical in all objects */
    switch(meta_or) {
    case JX_META_BIT_INT:
    case JX_META_BIT_FLOAT:
      {
        void *new_vla = NULL;
        switch(meta_or) {
        case JX_META_BIT_INT:
          new_vla = jx_vla_new(sizeof(jx_int),size);      
          if(new_vla) {
            jx_ob *src = list->data.ob_vla;
            jx_int *dst = (jx_int*)new_vla;
            for(i=0;i<size;i++) {
              *(dst++) = jx_ob_as_int( *(src++) );
            }
          }
          break;
        case JX_META_BIT_FLOAT:
          new_vla = jx_vla_new(sizeof(jx_float),size);      
          if(new_vla) {
            jx_ob *src = list->data.ob_vla;
            jx_float *dst = (jx_float*)new_vla;
            for(i=0;i<size;i++) {
              *(dst++) = jx_ob_as_int( *(src++) );
            }
          }
          break;
        }
        if(new_vla) {
          jx_vla_free(&list->data.vla);
          list->data.vla = new_vla;
          list->packed_meta = meta_or;
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

static jx_status jx_list_unpack_data(jx_list *list)
{
  /* homogenous list becoming hetergeneous */
  jx_int size = jx_vla_size(&list->data.vla);
  if(!size) {
    list->packed_meta = 0;
    if(list->data.vla) {
      jx_vla_free(&list->data.vla);
    }
    return JX_SUCCESS;
  }
  jx_ob *new_vla = jx_vla_new(sizeof(jx_ob),size);
  if(new_vla) {
    switch(list->packed_meta & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_INT:
      {
        jx_int i;
        jx_int *src = list->data.int_vla;
        jx_ob *dst = new_vla;
        for(i=0;i<size;i++) {
          *(dst++) = jx_ob_from_int( *(src++) );
        }
      }
      break;
    case JX_META_BIT_FLOAT:
      {
        jx_int i;
        jx_float *src = list->data.float_vla;
        jx_ob *dst = new_vla;
        for(i=0;i<size;i++) {
          *(dst++) = jx_ob_from_float( *(src++) );
        }
      }
      break;
    }
    jx_vla_free(&list->data.vla);
    list->data.vla = new_vla;
    list->packed_meta = 0;
    return JX_SUCCESS;
  } else 
    return JX_FAILURE;
}

JX_INLINE void jx_list_set_packed_data(jx_list *list, jx_int index, jx_ob ob)
{
  switch(list->packed_meta & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_INT:
    list->data.int_vla[index] = ob.data.int_;
    break;
  case JX_META_BIT_FLOAT:
    list->data.float_vla[index] = ob.data.float_;
    break;
  }
}

JX_INLINE jx_ob jx_list_get_packed_data(jx_list *list, jx_int index)
{
  switch(list->packed_meta & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_INT:
    return jx_ob_from_int(list->data.int_vla[index]);
    break;
  case JX_META_BIT_FLOAT:
    return jx_ob_from_float(list->data.float_vla[index]);
    break;
  }
  return jx_ob_from_null();
}

jx_status jx_list_append(jx_ob list, jx_ob ob)
{
  if(list.meta & JX_META_BIT_LIST) {
    jx_list *rec = list.data.list;
    if(rec->data.vla && rec->packed_meta && (rec->packed_meta != ob.meta)) {
      if(!jx_ok(jx_list_unpack_data(rec)))
        return JX_FAILURE;
    }
    if(rec->data.vla) {
      jx_int size = jx_vla_size(&rec->data.vla);
      if(jx_vla_grow_check( &rec->data.vla, size)) {
        if(rec->packed_meta && (rec->packed_meta == ob.meta)) {
          jx_list_set_packed_data(rec, size, ob);
        } else if(!rec->packed_meta) {
          rec->data.ob_vla[size] = ob;
        }
        return JX_SUCCESS;
      }
    } else { 
      jx_int packed_size = jx_meta_get_packed_size(ob.meta);
      if(packed_size) {
        rec->packed_meta = ob.meta;
        rec->data.vla = jx_vla_new(packed_size,1);
        if(rec->data.vla) {
          jx_list_set_packed_data(rec, 0, ob);
          return JX_SUCCESS;
        }
      } else {
        rec->data.vla = jx_vla_new(sizeof(jx_ob),1);
        if(rec->data.vla) {
          rec->data.ob_vla[0] = ob;
          return JX_SUCCESS;
        }
      }
    }
  }
  return JX_FAILURE;
}

jx_ob jx_list_borrow(jx_ob list, jx_int index)
{
  jx_ob result = JX_OB_NULL;
  if(list.meta & JX_META_BIT_LIST) {
    jx_list *rec = list.data.list;
    if( (index>=0) && (index < jx_vla_size(&rec->data.vla))) {
      if(rec->packed_meta) {
        result = jx_list_get_packed_data(rec, index);
      } else {
        result = rec->data.ob_vla[index];
      }
    }
  }
  return result;
}

jx_status jx_list_eliminate(jx_ob list, jx_int index)
{
  if(list.meta & JX_META_BIT_LIST) {
    jx_list *rec = list.data.list;
    if( (index>=0) && (index < jx_vla_size(&rec->data.vla) )) {
      if(!rec->packed_meta) {
        jx_ob_free(rec->data.ob_vla[index]);
      }
      return jx_vla_remove(&rec->data.vla, index, 1);
    }
  }
  return JX_FAILURE;
}

jx_int *jx_list_as_int_vla(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_LIST) {
    jx_list *list = ob.data.list;
    if(list->packed_meta & JX_META_BIT_INT) {
      return list->data.int_vla;
    } else if(jx_list_repack_data(list)) {
      if(list->packed_meta & JX_META_BIT_INT) {
        return list->data.int_vla;
      } else if(!list->data.vla) { 
        list->packed_meta = JX_META_BIT_INT;
        list->data.int_vla = (jx_int*)jx_vla_new(sizeof(int),0);
        return list->data.int_vla;
      }
    }
  }
  return NULL;
}

jx_float *jx_list_as_float_vla(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_LIST) {
    jx_list *list = ob.data.list;
    if(list->packed_meta & JX_META_BIT_FLOAT) {
      return list->data.float_vla;
    } else if(jx_list_repack_data(list)) {
      if(list->packed_meta & JX_META_BIT_FLOAT) {
        return list->data.float_vla;
      } else if(!list->data.vla) { 
        list->packed_meta = JX_META_BIT_FLOAT;
        list->data.int_vla = (jx_int*)jx_vla_new(sizeof(float),0);
        return list->data.int_vla;
      }
    }
  }
  return NULL;
}


/* JSON output */

static jx_ob jx_list_to_json(jx_list *list)
{
  jx_int i, size = jx_vla_size(&list->data.vla);
  jx_char *vla = jx_vla_new(1,2);
  if(vla) {
    jx_bool comma = JX_FALSE;
    vla[0] = '[';
    if(list->packed_meta) {
      switch(list->packed_meta & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_INT:
        {
          jx_ob ob = JX_OB_INT;
          jx_int *int_ = list->data.int_vla;
          for(i=0;i<size;i++) {
            ob.data.int_ = *(int_++);
            {
              jx_ob str = jx_ob_to_json(ob);
              if(comma) {
                jx_vla_append_c_str(&vla,",");          
              } else 
                comma = JX_TRUE;
              jx_vla_append_ob_str(&vla,str);
              jx_ob_free(str);
            }
          }
        }
        break;
      case JX_META_BIT_FLOAT:
        {
          jx_ob ob = JX_OB_INT;
          jx_float *float_ = list->data.float_vla;
          for(i=0;i<size;i++) {
            ob.data.int_ = *(float_++);
            {
              jx_ob str = jx_ob_to_json(ob);
              if(comma) {
                jx_vla_append_c_str(&vla,",");          
              } else 
                comma = JX_TRUE;
              jx_vla_append_ob_str(&vla,str);
              jx_ob_free(str);
            }
          }
        }
        break;
      } 
    } else {
      jx_ob *ob = list->data.ob_vla;
      
      for(i=0;i<size;i++) {
        jx_ob str = jx_ob_to_json(*(ob++));
        if(comma) {
          jx_vla_append_c_str(&vla,",");          
        } else 
          comma = JX_TRUE;
        jx_vla_append_ob_str(&vla,str);
        jx_ob_free(str);
      }
    }
    jx_vla_append_c_str(&vla,"]");
    return jx_ob_with_str_vla(vla);
  }
  return jx_ob_from_null();
}

jx_ob jx_ob_to_json(jx_ob ob)
{
  switch(ob.meta & JX_META_MASK_TYPE_BITS) {
  case 0:
    return jx_ob_from_str("null");
    break;
  case JX_META_BIT_BOOL:
    if(ob.data.bool_) 
      return jx_ob_from_str("true");
    else
      return jx_ob_from_str("false");
    break;
  case JX_META_BIT_INT:
    {
      char buffer[50];
      snprintf(buffer, sizeof(buffer),"%d",ob.data.int_);
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_FLOAT:
    {
      char buffer[50];
      snprintf(buffer, sizeof(buffer),"%f",ob.data.float_);
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_STR:
    if(ob.meta & JX_META_BIT_GC) {
      jx_int size = jx_vla_size(&ob.data.str);
      jx_char *buffer = jx_malloc(size+2);
      if(buffer) {
        buffer[0] = '"';
        memcpy(buffer+1, ob.data.str, size);
        buffer[size] = '"';
        buffer[size+1] = 0;
        {
          jx_ob result = jx_ob_from_str(buffer);      
          jx_free(buffer);
          return result;
        }
      }
    } else {
      char buffer[JX_TINY_STR_SIZE+2];
      jx_int size = ob.meta & JX_META_MASK_TINY_SIZE;
      buffer[0] = '"';
      memcpy(buffer+1, ob.data.tiny_str, size);
      buffer[size+1] = '"';
      buffer[size+2] = 0;
      return jx_ob_from_str(buffer);        
    }
    break;
  case JX_META_BIT_LIST:
    return jx_list_to_json(ob.data.list);
    break;
  }
  return jx_ob_from_null();
}

jx_status jx__ob_free(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_GC) {
    switch(ob.meta & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      jx_vla_free(&ob.data.str);
      break;
    case JX_META_BIT_LIST:
      jx_list_free(ob.data.list);
      break;
    }
    /* handle based on type */
  }
  return JX_SUCCESS;
}

