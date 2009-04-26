
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
  jx_vla *vla = (jx_vla*)jx_calloc(1, sizeof(jx_vla) + rec_size * size);
  if(vla) {
    vla->size = size;
    vla->rec_size = rec_size;
    return (void*)(vla + 1);
  } else
    return NULL;
}

void *jx_vla_new_with_content(jx_int rec_size, jx_int size, void *content)
{
  jx_vla *vla = jx_malloc(sizeof(jx_vla) + rec_size * size);
  if(vla) {
    jx_char *base = (jx_char*)(vla + 1);
    vla->size = size;
    vla->rec_size = rec_size;
    memcpy(base, content, rec_size * size);
    return (void*)base;
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
        (*ref) = base;
        memmove(base + (index+count) * vla->rec_size, 
                base + index * vla->rec_size,
                (vla->size - index) * vla->rec_size);
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
  }
  return JX_SUCCESS;
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

/* comparison */

jx_bool jx__ob_gc_identical(jx_ob left, jx_ob right)
{
  /* on entry, we know left.meta == right.meta and that
     both objects are GC'd */
  switch(left.meta & JX_META_MASK_TYPE_BITS) {
  case JX_META_BIT_STR:
    {
      jx_char *left_st = jx_ob_as_str(&left);
      jx_char *right_st = jx_ob_as_str(&right);
      if(left_st && right_st) {
        return (!strcmp(left_st,right_st));
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

/* lists */

jx_ob jx_list_new(void)
{
  jx_ob result = JX_OB_LIST;
  result.data.list = (jx_list*)jx_calloc(1,sizeof(jx_list));
  return result;
}

jx_ob jx_list_new_from_int_array(jx_int *array, jx_int size)
{
  jx_ob result = jx_list_new();
  if(result.meta & JX_META_BIT_LIST) {
    result.data.list->packed_meta = JX_META_BIT_INT;
    result.data.list->data.vla = jx_vla_new_with_content(sizeof(jx_int),size,array);
  }
  return result;
}

jx_ob jx_list_new_from_float_array(jx_float *array, jx_float size)
{
  jx_ob result = jx_list_new();
  if(result.meta & JX_META_BIT_LIST) {
    result.data.list->packed_meta = JX_META_BIT_FLOAT;
    result.data.list->data.vla = jx_vla_new_with_content(sizeof(jx_float),size,array);
  }
  return result;
}

static jx_status jx_list_free(jx_list *list)
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
  return JX_SUCCESS;
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

jx_status jx_list_insert(jx_ob list, jx_int index, jx_ob ob)
{
  if(list.meta & JX_META_BIT_LIST) {
    jx_list *rec = list.data.list;
    if(rec->data.vla && rec->packed_meta && (rec->packed_meta != ob.meta)) {
      if(!jx_ok(jx_list_unpack_data(rec)))
        return JX_FAILURE;
    }
    if(rec->data.vla) {
      if(!jx_ok(jx_vla_insert(&rec->data.vla,index,1)))
        return JX_FAILURE;
      else if(rec->packed_meta && (rec->packed_meta == ob.meta)) {
        jx_list_set_packed_data(rec, index, ob);
      } else if(!rec->packed_meta) {
        rec->data.ob_vla[index] = ob;
      }
      return JX_SUCCESS;
    } else if(index==0) { 
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

jx_status jx_list_set_int_vla(jx_ob ob,jx_int *vla)
{
  if(ob.meta & JX_META_BIT_LIST) {
    jx_list *list = ob.data.list;
    if(list->packed_meta & JX_META_BIT_INT) {
      list->data.int_vla = vla;
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
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
        list->data.float_vla = (jx_float*)jx_vla_new(sizeof(float),0);
        return list->data.float_vla;
      }
    }
  }
  return NULL;
}

jx_status jx_list_set_float_vla(jx_ob ob,jx_float *vla)
{
  if(ob.meta & JX_META_BIT_LIST) {
    jx_list *list = ob.data.list;
    if(list->packed_meta & JX_META_BIT_FLOAT) {
      list->data.float_vla = vla;
      return JX_SUCCESS;
    }
  }
  return JX_FAILURE;
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
          jx_ob ob = JX_OB_FLOAT;
          jx_float *float_ = list->data.float_vla;
          for(i=0;i<size;i++) {
            ob.data.float_ = *(float_++);
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

/* hashing */

static jx_uint32 jx__c_str_hash(jx_char *str)
{
  register unsigned char *p = (unsigned char*)str;
  register jx_uint32 x, len = 0;
  register unsigned char c;
  
  x = *p << 7;
  while ( (c=*(p++)) ) {
#if 0
    /* PYTHON (time: G5 = 3.2, P3 = 19.3, P4=11.1)*/
    x = (1000003*x) + c;    
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
  return x ? x : 1; /* zero is reserved as the hash code of an
                       unhashable object */
}

jx_uint32 jx__ob_gc_hash_code(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_STR) { /* right now, we only hash GC strings */
    return jx__c_str_hash(jx_ob_as_str(&ob));
  } else {
    return 0; /* unhashable */
  }
}

/* hash objects */

jx_ob jx_hash_new(void)
{
  jx_ob result = JX_OB_HASH;
  result.data.hash = (jx_hash*)jx_calloc(1,sizeof(jx_hash));
  return result;
}

static jx_status jx_hash_free(jx_hash *I)
{
  jx_int size = jx_vla_size( &I->key_value );
  if(size) {
    jx_ob *ob = I->key_value;
    jx_int i;
    for(i=0;i<size;i++) {
      jx_ob_free( *(ob++) );
    }
  }
  jx_vla_free( &I->key_value );
  jx_vla_free( &I->info );
  jx_free(I);
  return JX_SUCCESS;
}

jx_int jx__hash_size(jx_hash *I)
{
  jx_int result = 0;
  if(I->key_value) {
    if(!I->info) { /* no info mode -- search & match objects directly  */
      result = jx_vla_size(&I->key_value) >> 1;
    } else {
      jx_hash_info *info = (jx_hash_info*)I->info;
      result = info->usage;
    }
  }
  return result;
}

JX_INLINE jx_uint32 jx__new_mask_from_min_size(jx_uint32 min_size)
{
  jx_uint32 new_mask = 0;
  jx_uint32 tmp_usage = ((3*min_size) >> 1);
  while(tmp_usage) {
    new_mask = (new_mask << 1) + 1;
    tmp_usage = (tmp_usage >> 1);
  }
  while(new_mask<min_size) {
    new_mask = (new_mask << 1);
  }
  return new_mask;
}

jx_bool jx__hash_recondition(jx_hash *I, jx_int mode, jx_bool pack)
{
  /* note that on recondition we assume that there are no two
     identical keys present in the hash */

  jx_bool result = JX_TRUE;
  jx_uint32 usage = 0;
  jx_uint32 old_mode = JX_HASH_RAW;
  jx_uint32 min_size = 0;
#if 0
  putchar('$');
#endif
  if(!I->info) {
    usage = jx_vla_size(&I->key_value) >> 1;
    if(min_size < usage)
      min_size = usage;
    if(mode > JX_HASH_LINEAR) { 
      I->info = jx_vla_new( sizeof(jx_uint32), (2*min_size) + JX_HASH_INFO_SIZE);
      if(I->info) {
        jx_hash_info *info = (jx_hash_info*)I->info;
        info->mode = mode;
      }
    }
  } else {
    jx_hash_info *info = (jx_hash_info*)I->info;
    old_mode = info->mode;
    usage = info->usage;
    if(min_size<usage)
      min_size = usage;
  }

  switch(mode) {
  case JX_HASH_ONE_TO_ANY:
  case JX_HASH_ONE_TO_ONE:
  case JX_HASH_ONE_TO_NIL:
    min_size = usage+1; /* always make sure there is space for a new entry */
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
    switch(mode) {
    case JX_HASH_ONE_TO_ONE:
    case JX_HASH_ONE_TO_NIL:
      break;
    default:
      jx_vla_free( &I->info );
      break;
    }
  } else {
    switch(mode) {
    case JX_HASH_RAW: /* to */
      switch(old_mode) {
      case JX_HASH_LINEAR: /* from */
        if(I->key_value) {
          jx_vla_resize( &I->key_value, (usage<<1) );
        }
        jx_vla_free( &I->info );
        break;
      case JX_HASH_ONE_TO_ANY: /* from */
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_hash_info *old_info = (jx_hash_info*)I->info;
          jx_ob *new_key_value = jx_vla_new(sizeof(jx_ob), (usage<<1));
          if(new_key_value) {
            jx_ob *old_key_value = I->key_value, *new_ptr = new_key_value;
            jx_uint32 *old_hash = old_info->start;
            if(old_info->mask) {
              jx_uint32 c = old_info->mask + 1;
              while(c--) { 
                if(old_hash[1] & JX_HASH_ACTIVE) { /* only process actives */ 
                  jx_uint32 old_i = old_hash[1] & JX_HASH_OFFSET_MASK;
                  /* copy object into new key_value table */
                  *(new_ptr++) = old_key_value[old_i];
                  *(new_ptr++) = old_key_value[old_i+1];  /* for ONE_TO_NIL, these will be null */
                }
                old_hash += 2;
              }
            }
            jx_vla_free( &I->key_value );
            I->key_value = new_key_value;
            jx_vla_free( &I->info );
          }
        }
        break;
      }
      break;
    case JX_HASH_LINEAR: /* to */
      switch(old_mode) {
      case JX_HASH_RAW: /* from */
        {
          I->info = jx_vla_new( sizeof(jx_uint32), usage + JX_HASH_INFO_SIZE);
          if(I->info) {
            jx_hash_info *info = (jx_hash_info*)I->info;
            jx_uint32 i = usage;
            jx_ob *ob = I->key_value;
            jx_uint32 *hash_entry_ptr = info->start;
            while(i--) {
              *(hash_entry_ptr) = jx_ob_hash_code( *ob );
              hash_entry_ptr++;
              ob += 2;
            }
            info->mode = mode;
            info->usage = usage;
          }
        }
        break;
      case JX_HASH_LINEAR: /* from */
        /* do nuttin' */
        break;
      case JX_HASH_ONE_TO_ANY: /* from */
      case JX_HASH_ONE_TO_ONE:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_hash_info *old_info = (jx_hash_info*)I->info;
          jx_ob *new_key_value = jx_vla_new( sizeof(jx_ob), (usage<<1));
          jx_hash_info *new_info = (jx_hash_info*)jx_vla_new( sizeof(jx_uint32), usage + JX_HASH_INFO_SIZE);
          if( new_key_value && new_info ) {
            jx_ob *old_key_value = I->key_value, *new_ptr = new_key_value;
            jx_uint32 *old_hash = old_info->start;
            jx_uint32 *new_hash = new_info->start;
            if(old_info->mask) {
              jx_uint32 c = old_info->mask + 1;
              jx_uint32 new_i = 0;
              new_info->mode = mode;
              new_info->usage = usage;
              while(c--) { 
                if(old_hash[1] & JX_HASH_ACTIVE) { /* only process actives */ 
                  jx_uint32 old_i = old_hash[1] & JX_HASH_OFFSET_MASK;
                  /* copy object into new key_value table */
                  *(new_ptr++) = old_key_value[old_i];
                  *(new_ptr++) = old_key_value[old_i+1];
                  *(new_hash++) = old_hash[0];
                  new_i += 2;
                }
                old_hash += 2;
              }
            }
            jx_vla_free( &I->key_value );
            I->key_value = new_key_value;
            jx_vla_free( &I->info );
            I->info = (jx_uint32*)new_info;
          } else {
            jx_vla_free( &new_key_value );
            jx_vla_free( &new_info );
          }
        }
        break;
      }
      break;
    case JX_HASH_ONE_TO_ANY: /* to */
    case JX_HASH_ONE_TO_NIL:
      {
        jx_uint32 new_mask = jx__new_mask_from_min_size(min_size);
        {
          /* prepare new info block */
          jx_hash_info *new_info = (jx_hash_info*)jx_vla_new( sizeof(jx_uint32),2*(new_mask + 1) + JX_HASH_INFO_SIZE);
          if(new_info) {
            new_info->mode = mode;
            new_info->usage = usage;
            new_info->mask = new_mask;
            switch(old_mode) {
            case JX_HASH_RAW: /* from */
              {
                jx_ob *ob = I->key_value;
                jx_uint32 *hash = new_info->start;
                jx_uint32 i = 0;
                jx_uint32 size = (usage<<1);
                while(i<size) {
                  jx_uint32 hash_code = jx_ob_hash_code( *ob );
                  jx_uint32 index = new_mask & hash_code;
                  jx_uint32 *hash_entry_ptr = hash + (index<<1);
                  if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                    hash_entry_ptr[0] = hash_code;
                    hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                  } else {
                    jx_uint32 give_up = index;
                    index = (index+1) & new_mask;
                    while(index != give_up) {
                      hash_entry_ptr = hash + (index<<1);
                      if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                        hash_entry_ptr[0] = hash_code;
                        hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                        break;
                      }
                      index = (index+1) & new_mask;
                    }
                  }
                  i+=2;
                  ob+=2;
                }
                jx_vla_free( &I->info );
                I->info = (jx_uint32*)new_info;
                new_info = NULL;
              }
              break;
            case JX_HASH_LINEAR: /* from */
              {
                jx_hash_info *old_info = (jx_hash_info*)I->info;
                jx_uint32 *old_hash_code = old_info->start;
                jx_uint32 *hash = new_info->start;
                jx_uint32 i = 0;
                jx_uint32 size = (usage<<1);
                while(i<size) {
                  jx_uint32 hash_code = *(old_hash_code++);
                  jx_uint32 index = new_mask & hash_code;
                  jx_uint32 *hash_entry_ptr = hash + (index<<1);
                  if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE) ) {
                    hash_entry_ptr[0] = hash_code;
                    hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                  } else {
                    jx_uint32 give_up = index;
                    index = (index+1) & new_mask;
                    while(index != give_up) {
                      hash_entry_ptr = hash + (index<<1);
                      if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE) ) {
                        hash_entry_ptr[0] = hash_code;
                        hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                        break;
                      }
                      index = (index+1) & new_mask;
                    }
                  }
                  i+=2;
                }
                jx_vla_free( &I->info );
                I->info = (jx_uint32*)new_info;
                new_info = NULL;
              }
              break;
            case JX_HASH_ONE_TO_ANY: /* from */
            case JX_HASH_ONE_TO_ONE:
            case JX_HASH_ONE_TO_NIL:
              {
                jx_hash_info *old_info = (jx_hash_info*)I->info;
#if 0                
                printf("\n(%p:%x->%x:%d)",(void*)I,(int)old_info->mask,(int)new_mask,(int)pack);
#endif
                if(pack || (new_mask < old_info->mask)) {
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value = jx_vla_new( sizeof(jx_ob),  (usage<<1));
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash = old_info->start;
                    jx_uint32 *hash = new_info->start;
                    jx_uint32 c = old_info->mask+1;
                    jx_uint32 new_i = 0;
                    while(c--) { 
                      if(old_hash[1] & JX_HASH_ACTIVE) { /* only process actives */ 
                        jx_uint32 hash_code = old_hash[0];
                        jx_uint32 index = new_mask & hash_code;
                        jx_uint32 *hash_entry_ptr = hash + (index<<1);
                        jx_uint32 old_i = old_hash[1] & JX_HASH_OFFSET_MASK;
                        /* copy object into new key_value table */
                        new_key_value[new_i] = old_key_value[old_i];
                        new_key_value[new_i+1] = old_key_value[old_i+1];
                        if( !hash_entry_ptr[1]) {
                          hash_entry_ptr[0] = hash_code;
                          hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                        } else {
                          jx_uint32 give_up = index;
                          index = (index+1) & new_mask;
                          while(index != give_up) {
                            hash_entry_ptr = hash + (index<<1);
                            if( !hash_entry_ptr[1] ) {
                              hash_entry_ptr[0] = hash_code;
                              hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                              break;
                            }
                            index = (index+1) & new_mask;
                          }
                        }
                        new_i += 2;
                      }
                      old_hash += 2;
                    }
                    jx_vla_free( &I->key_value );
                    I->key_value = new_key_value;
                    jx_vla_free( &I->info );
                    I->info = (jx_uint32*)new_info;
                    new_info = NULL;
                  }
                } else if(new_mask > old_info->mask) {
                  /* we copy deleted entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash = old_info->start;
                  jx_uint32 *hash = new_info->start;
                  new_info->stale = old_info->stale;
                  if(old_info->mask) {
                    jx_uint32 c = old_info->mask + 1;
                    while(c--) { 
                      if(old_hash[1]) { /* only skip virgin entries */
                        jx_uint32 hash_code = old_hash[0];
                        jx_uint32 index = new_mask & hash_code;
                        jx_uint32 *hash_entry_ptr = hash + (index<<1);
                        if( !hash_entry_ptr[1]) {
                          hash_entry_ptr[0] = hash_code;
                          hash_entry_ptr[1] = old_hash[1];
                        } else {
                          jx_uint32 give_up = index;
                          index = (index+1) & new_mask;
                          while(index != give_up) {
                            hash_entry_ptr = hash + (index<<1);
                            if( !hash_entry_ptr[1] ) {
                              hash_entry_ptr[0] = hash_code;
                              hash_entry_ptr[1] = old_hash[1];
                              break;
                            }
                            index = (index+1) & new_mask;
                          }
                        }
                      }
                      old_hash+=2;
                    }
                  }
                  jx_vla_free( &I->info );
                  I->info = (jx_uint32*)new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            }
            /* if something went wrong, then we simply free new new info and keep the old */
            jx_vla_free( &new_info );
          }
        }
      }
      break;
    case JX_HASH_ONE_TO_ONE: /* to */
      {
        jx_uint32 new_mask = jx__new_mask_from_min_size(min_size);
        {
          /* prepare new info block */
          jx_hash_info *new_info = jx_vla_new( sizeof(jx_uint32),  4*(new_mask + 1) + JX_HASH_INFO_SIZE);
          if(new_info) {
            new_info->mode = mode;
            new_info->usage = usage;
            new_info->mask = new_mask;
            switch(old_mode) {
            case JX_HASH_RAW: /* from */
              {
                jx_ob *ob = I->key_value;
                jx_uint32 *hash = new_info->start;
                jx_uint32 *rev_hash = new_info->start + ((new_mask+1)<<1);
                jx_uint32 hash_code;
                jx_uint32 i = 0;
                jx_uint32 size = (usage<<1);

                while(i<size) {

                  /* forward */
                  hash_code = jx_ob_hash_code( ob[0] );
                  {
                    jx_uint32 index = new_mask & hash_code;
                    jx_uint32 *hash_entry_ptr = hash + (index<<1);
                    if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                      hash_entry_ptr[0] = hash_code;
                      hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                    } else {
                      jx_uint32 give_up = index;
                      index = (index+1) & new_mask;
                      while(index != give_up) {
                        hash_entry_ptr = hash + (index<<1);
                        if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                          hash_entry_ptr[0] = hash_code;
                          hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                          break;
                        }
                        index = (index+1) & new_mask;
                      }
                    }
                  }
                  /* reverse */
                  hash_code = jx_ob_hash_code( ob[1] );
                  if( !hash_code ) { /* value not hashable */
                    result = JX_FALSE;
                    break;
                  } else { 
                    jx_uint32 index = new_mask & hash_code;
                    jx_uint32 *hash_entry_ptr = rev_hash + (index<<1);
                    jx_ob *key_value = I->key_value;
                    if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                      hash_entry_ptr[0] = hash_code;
                      hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                    } else if((hash_entry_ptr[0] == hash_code) && 
                              jx_ob_identical(key_value[1+(hash_entry_ptr[1] & JX_HASH_OFFSET_MASK)], ob[1])) {
                      result = JX_FALSE;
                      break;
                    } else {
                      jx_uint32 give_up = index;
                      index = (index+1) & new_mask;
                      while(index != give_up) {
                        hash_entry_ptr = rev_hash + (index<<1);
                        if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                          hash_entry_ptr[0] = hash_code;
                          hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                          break;
                        } else if((hash_entry_ptr[0] == hash_code) && 
                                  jx_ob_identical(key_value[1+(hash_entry_ptr[1] & JX_HASH_OFFSET_MASK)], ob[1])) {
                          result = JX_FALSE;
                          break;
                        }
                        index = (index+1) & new_mask;
                      }
                    }
                  }
                  i+=2;
                  ob+=2;
                }
                if(result) {
                  jx_vla_free( &I->info );
                  I->info = (jx_uint32*)new_info;
                  new_info = JX_NULL;
                } 
              }
              break;
            case JX_HASH_LINEAR: /* from */
              { 
                jx_hash_info *old_info = (jx_hash_info*)I->info;
                jx_uint32 *old_hash_code = old_info->start;
                jx_ob *ob = I->key_value;
                jx_uint32 *hash = new_info->start;
                jx_uint32 *rev_hash = new_info->start + ((new_mask+1)<<1);
                jx_uint32 hash_code;
                jx_uint32 i = 0;
                jx_uint32 size = (usage<<1);
                while(i<size) {
                  /* forward */
                  hash_code = *(old_hash_code++);
                  { 
                    jx_uint32 index = new_mask & hash_code;
                    jx_uint32 *hash_entry_ptr = hash + (index<<1);
                    if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE) ) {
                      hash_entry_ptr[0] = hash_code;
                      hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                    } else {
                      jx_uint32 give_up = index;
                      index = (index+1) & new_mask;
                      while(index != give_up) {
                        hash_entry_ptr = hash + (index<<1);
                        if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE) ) {
                          hash_entry_ptr[0] = hash_code;
                          hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                          break;
                        }
                        index = (index+1) & new_mask;
                      }
                    }
                  }
                  /* reverse */
                  hash_code = jx_ob_hash_code( ob[1] );
                  if( !hash_code ) {
                    result = JX_FALSE;
                    break;
                  } else { 
                    jx_uint32 index = new_mask & hash_code;
                    jx_uint32 *hash_entry_ptr = rev_hash + (index<<1);
                    jx_ob *key_value = I->key_value;
                    if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                      hash_entry_ptr[0] = hash_code;
                      hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                    } else if((hash_entry_ptr[0] == hash_code) && 
                              jx_ob_identical(key_value[1+(hash_entry_ptr[1] & JX_HASH_OFFSET_MASK)], ob[1])) {
                      result = JX_FALSE;
                      break;
                    } else {
                      jx_uint32 give_up = index;
                      index = (index+1) & new_mask;
                      while(index != give_up) {
                        hash_entry_ptr = rev_hash + (index<<1);
                        if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                          hash_entry_ptr[0] = hash_code;
                          hash_entry_ptr[1] = (i | JX_HASH_ACTIVE);
                          break;
                        } else if((hash_entry_ptr[0] == hash_code) && 
                                  jx_ob_identical(key_value[1+(hash_entry_ptr[1] & JX_HASH_OFFSET_MASK)], ob[1])) {
                          result = JX_FALSE;
                          break;
                        }
                        index = (index+1) & new_mask;
                      }
                    }
                  }
                  i+=2;
                  ob+=2;
                }
                if(result) {
                  jx_vla_free( &I->info );
                  I->info = (jx_uint32*)new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            case JX_HASH_ONE_TO_ANY: /* from */
            case JX_HASH_ONE_TO_NIL:
              {
                jx_hash_info *old_info = (jx_hash_info*)I->info;

                if(pack || (new_mask < old_info->mask)) {
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value = jx_vla_new( sizeof(jx_ob), (usage<<1));
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash = old_info->start;
                    jx_uint32 *hash = new_info->start;
                    jx_uint32 *rev_hash = new_info->start + ((new_mask+1)<<1);
                    jx_uint32 c = old_info->mask+1;
                    jx_uint32 new_i = 0;
                    while(c--) { 
                      if(old_hash[1] & JX_HASH_ACTIVE) { /* only process actives */ 
                        jx_uint32 old_i = old_hash[1] & JX_HASH_OFFSET_MASK;
                        
                        { /* forward */
                          jx_uint32 hash_code = old_hash[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = hash + (index<<1);
                          /* copy object into new key_value table */
                          new_key_value[new_i] = old_key_value[old_i];
                          new_key_value[new_i+1] = old_key_value[old_i+1];
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }

                        { /* reverse */
                          jx_uint32 hash_code = jx_ob_hash_code( old_key_value[1+(old_hash[1]&JX_HASH_OFFSET_MASK)] );
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = rev_hash + (index<<1);
                          
                          if(!hash_code) {
                            result = JX_FALSE;
                            break;
                          } else if(!hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                          } else if((hash_entry_ptr[0] == hash_code) && 
                                    jx_ob_identical(new_key_value[1+(hash_entry_ptr[1]&&JX_HASH_OFFSET_MASK)],
                                                    new_key_value[new_i+1])) {
                            result = JX_FALSE;
                            break;
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = rev_hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                                break;
                              } else if((hash_entry_ptr[0] == hash_code) && 
                                        jx_ob_identical(new_key_value[1+(hash_entry_ptr[1]&&JX_HASH_OFFSET_MASK)],
                                                        new_key_value[new_i+1])) {
                                result = JX_FALSE;
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }
                        new_i += 2;
                      }
                      old_hash += 2;
                    }
                    if(result) {
                      jx_vla_free( &I->key_value );
                      I->key_value = new_key_value;
                      new_key_value = JX_NULL;
                      jx_vla_free( &I->info );
                      I->info = (jx_uint32*)new_info;
                      new_info = JX_NULL;
                    }
                    jx_vla_free( &new_key_value );
                  }
                } else if(new_mask > old_info->mask) {
                  /* we copy stale entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash = old_info->start;
                  jx_uint32 *hash = new_info->start;
                  jx_uint32 *rev_hash = new_info->start + ((new_mask+1)<<1);
                  jx_ob *key_value = I->key_value;
                  new_info->stale = old_info->stale;
                  if(old_info->mask) {
                    jx_uint32 c = old_info->mask+1;
                    while(c--) { 
                      if(old_hash[1]) { /* only skip virgin entries */
                        { /* forward */
                          jx_uint32 hash_code = old_hash[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = hash + (index<<1);
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = old_hash[1];
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = old_hash[1];
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }
                        if( old_hash[1] & JX_HASH_ACTIVE ) { /* reverse for actives only */
                          jx_uint32 hash_code = jx_ob_hash_code( key_value[1+(old_hash[1]&JX_HASH_OFFSET_MASK)] );  
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = rev_hash + (index<<1);
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = old_hash[1];
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = rev_hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = old_hash[1];
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }
                      }
                      old_hash+=2;
                    }
                  }
                  if(result) {
                    jx_vla_free( &I->info );
                    I->info = (jx_uint32*)new_info;
                    new_info = JX_NULL;
                  }
                }
              }
              break;
            case JX_HASH_ONE_TO_ONE: /* from */
              {
                jx_hash_info *old_info = (jx_hash_info*)I->info;
                if(pack || (new_mask < old_info->mask)) {
                  /* we're going to pack key_value and eliminate inactive blocks */
                  jx_ob *new_key_value = jx_vla_new( sizeof(jx_ob),  (usage<<1));
                  if(new_key_value) {
                    jx_ob *old_key_value = I->key_value;
                    jx_uint32 *old_hash = old_info->start;
                    jx_uint32 *hash = new_info->start;
                    jx_uint32 *rev_hash = new_info->start + ((new_mask+1)<<1);
                    jx_uint32 c = old_info->mask+1;
                    jx_uint32 new_i = 0;
                    while(c--) { 
                      if(old_hash[1] & JX_HASH_ACTIVE) { /* only process actives */ 
                        jx_uint32 old_i = old_hash[1] & JX_HASH_OFFSET_MASK;
                        /* forward */
                        { 
                          jx_uint32 hash_code = old_hash[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = hash + (index<<1);
                          /* copy object into new key_value table */
                          new_key_value[new_i] = old_key_value[old_i];
                          new_key_value[new_i+1] = old_key_value[old_i+1];
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }
                        /* reverse */
                        { 
                          jx_uint32 hash_code = jx_ob_hash_code(old_key_value[1+(old_hash[1]&JX_HASH_OFFSET_MASK)]);
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = rev_hash + (index<<1);
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = rev_hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = (new_i | JX_HASH_ACTIVE);
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }
                        new_i += 2;
                      }
                      old_hash += 2;
                    }
                    jx_vla_free( &I->key_value );
                    I->key_value = new_key_value;
                    jx_vla_free( &I->info );
                    I->info = (jx_uint32*)new_info;
                    new_info = JX_NULL;
                  }
                } else if(new_mask > old_info->mask) {
                  /* we copy stale entries too since they point at vacant key_value slots */
                  jx_uint32 *old_hash = old_info->start;
                  jx_uint32 *hash = new_info->start;
                  jx_uint32 *rev_hash = new_info->start + ((new_mask+1)<<1);
                  jx_ob *key_value = I->key_value;
                  new_info->stale = old_info->stale;
                  if(old_info->mask) {
                    jx_uint32 c = old_info->mask+1;
                    while(c--) { 
                      if(old_hash[1]) { /* only skip virgin entries */

                        { /* forward */
                          jx_uint32 hash_code = old_hash[0];
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = hash + (index<<1);
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = old_hash[1];
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = old_hash[1];
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }

                        if( old_hash[1] & JX_HASH_ACTIVE ) { /* reverse for actives only */
                          jx_uint32 hash_code = jx_ob_hash_code( key_value[1+(old_hash[1]&JX_HASH_OFFSET_MASK)] );  
                          jx_uint32 index = new_mask & hash_code;
                          jx_uint32 *hash_entry_ptr = rev_hash + (index<<1);
                          if( !hash_entry_ptr[1]) {
                            hash_entry_ptr[0] = hash_code;
                            hash_entry_ptr[1] = old_hash[1];
                          } else {
                            jx_uint32 give_up = index;
                            index = (index+1) & new_mask;
                            while(index != give_up) {
                              hash_entry_ptr = rev_hash + (index<<1);
                              if( !hash_entry_ptr[1] ) {
                                hash_entry_ptr[0] = hash_code;
                                hash_entry_ptr[1] = old_hash[1];
                                break;
                              }
                              index = (index+1) & new_mask;
                            }
                          }
                        }
                      }
                      old_hash+=2;
                    }
                  }
                  jx_vla_free( &I->info );
                  I->info = (jx_uint32*)new_info;
                  new_info = JX_NULL;
                }
              }
              break;
            }
            /* if something went wrong, then we simply free new new info and keep the old */
            jx_vla_free( &new_info );
          }
        }
      }
      break;
    }
    if(I->info && I->key_value) {
      jx_hash_info *info = (jx_hash_info*)I->info;
      jx_uint32 n_ent = jx_vla_size(&I->key_value);
#if 0
      putchar('0'+old_mode);
      putchar('>');
      putchar('0'+info->mode);
      putchar(' ');
#endif
      if(n_ent/2 != (info->usage + info->stale)) {
        printf("mismatch %d != %d+%d = %d\n",
               (int)n_ent/2, (int)info->usage, (int)info->stale,
               (int)info->usage + (int)info->stale);
      }
    }
  }
  return result;
}

jx_status jx__hash_set(jx_hash *I, jx_ob key, jx_ob value)
{
  jx_status result = JX_FAILURE;
  jx_uint32 hash_code = jx_ob_hash_code(key);
  if(!hash_code) { /* unhashable key */
    return JX_FAILURE;
  } else {
    if(!I->info) { /* "no info" mode -- search & match objects directly  */
      if(!I->key_value) {
        /* new table, first entry  */
        I->key_value = jx_vla_new(sizeof(jx_ob), 2);
        if(I->key_value) {
          I->key_value[0] = key; /* takes ownership */
          I->key_value[1] = value; /* takes ownership */
          result = JX_SUCCESS;
        }
      } else {
        jx_uint32 size = jx_vla_size( &I->key_value );
        if(1) {
          jx_bool found = JX_FALSE;
          register jx_uint32 i = (size >> 1);
          register jx_ob *ob = I->key_value;
          while(i--) {
            if(jx_ob_identical(*ob, key)) {            
              found = JX_TRUE;
              jx_ob_free(I->key_value[0]);
              jx_ob_free(I->key_value[1]);
              I->key_value[0] = key;
              I->key_value[1] = value;
              result = JX_SUCCESS;
              break;
            }
            ob+=2;
          }
          if(!found) {
            if(jx_ok( jx_vla_grow_check( &I->key_value, size+1))) {
              I->key_value[size] = key;
              I->key_value[size+1] = value;
              result = JX_SUCCESS;
#if 1
              jx__hash_recondition(I, JX_HASH_LINEAR, JX_FALSE); /* switch to linear at N=2 */
#endif
            }
          }
        }
      }
    } else { /* we have an info record */
      if(!I->key_value) {
        I->key_value = jx_vla_new( sizeof(jx_ob), 0);
      }
      if(I->key_value && I->info ) {
        jx_hash_info *info = (jx_hash_info*)I->info;
        switch(info->mode) {
        case JX_HASH_LINEAR:
          {
            register jx_uint32 i = info->usage;
            register jx_uint32 *hash_entry_ptr = info->start;
            register jx_ob *ob = I->key_value;
            
            jx_bool found = JX_FALSE;
            while(i--) {
              if( *hash_entry_ptr == hash_code ) {
                if(jx_ob_identical(*ob,key)) {            
                  found = JX_TRUE;
                  break;
                }
              }
              hash_entry_ptr++;
              ob+=2;
            }
            if(!found) {
              jx_uint32 usage = info->usage;
              if( jx_ok( jx_vla_grow_check( &I->info, usage + JX_HASH_INFO_SIZE)) &&
                  jx_ok( jx_vla_grow_check( &I->key_value, (usage<<1)+1 ))) {
                info = (jx_hash_info*)I->info;
                hash_entry_ptr = info->start + usage;
                ob = I->key_value + (usage << 1);
                info->usage++;
                *hash_entry_ptr = hash_code;
                ob[0] = key;
                ob[1] = value;
                result = JX_SUCCESS;
                if(info->usage > 15) {
                  jx__hash_recondition(I, JX_HASH_ONE_TO_ANY, JX_FALSE); /* switch to true hash */
                }
              }
            } else {
              jx_ob_free(ob[0]);
              jx_ob_free(ob[1]);
              ob[0] = key;
              ob[1] = value;
              result = JX_SUCCESS;
            }
          }
          break;
        case JX_HASH_ONE_TO_ANY:           
        case JX_HASH_ONE_TO_NIL:           
          if( (info->usage+1) > (3*info->mask)>>2) { /* more than ~3/4'rs full */
            jx__hash_recondition(I, info->mode, JX_FALSE);
            info = (jx_hash_info*)I->info;
          }
          {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash = info->start;
            jx_uint32 usage = info->usage;
            jx_ob *key_value = I->key_value;
            jx_uint32 index = mask & hash_code;
            jx_uint32 *hash_entry_ptr = hash + (index<<1);
            jx_uint32 *dest_ptr = JX_NULL;
            jx_bool found = JX_FALSE;
            jx_bool virgin = JX_FALSE;
            if(!hash_entry_ptr[1]) {
              dest_ptr = hash_entry_ptr;
              virgin = JX_TRUE;
              /* available */
            } else if( (hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == hash_code) &&
                       jx_ob_identical(key_value[hash_entry_ptr[1] & JX_HASH_OFFSET_MASK], key)) {
              dest_ptr = hash_entry_ptr;
              found = JX_TRUE;
            } else {
              jx_uint32 give_up = index;
              index = (index+1) & mask;
              while(index != give_up) {
                hash_entry_ptr = hash + (index<<1);
                if(!hash_entry_ptr[1]) {
                  /* virgin slot terminates probe... */
                  if(!dest_ptr) {
                    dest_ptr = hash_entry_ptr;
                    virgin = JX_TRUE;
                  }
                  break;
                } else if(!(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                  /* deleted slot -- save for future insertion */
                  if(!dest_ptr) dest_ptr = hash_entry_ptr;
                } else if((hash_entry_ptr[0] == hash_code) &&
                          jx_ob_identical(key_value[hash_entry_ptr[1] & JX_HASH_OFFSET_MASK], key)) { 
                  /* matched key, so we must replace */
                  dest_ptr = hash_entry_ptr;
                  found = JX_TRUE;
                  break;
                }
                index = (index+1) & mask;
              }
            }
            
            if(dest_ptr) {
              if(!found) {
                jx_uint32 i;
                if(virgin) {
                  i = ((info->stale+usage)<<1) | JX_HASH_ACTIVE; /* allocate new key_value */
                } else {
                  i = (dest_ptr[1]&JX_HASH_OFFSET_MASK) | JX_HASH_ACTIVE; /* use existing key_value */
                }
                
                if(jx_ok( jx_vla_grow_check( &I->key_value, i ) )) {
                  key_value = I->key_value;
                  
                  if(!virgin) info->stale--;
                  dest_ptr[0] = hash_code;
                  dest_ptr[1] = i;
                  key_value[i-1] = key;
                  key_value[i] = value;
                  result = JX_SUCCESS;
                  info->usage++;
                } else {
                  result = JX_FAILURE;
                }
              } else {
                jx_uint32 i = (dest_ptr[1]&JX_HASH_OFFSET_MASK) | JX_HASH_ACTIVE;
                jx_ob_free(key_value[i-1]);
                jx_ob_free(key_value[i]);
                key_value[i-1] = key;
                key_value[i] = value;
                result = JX_SUCCESS;
              }
            }
          }
          break;
        case JX_HASH_ONE_TO_ONE:
          {
            jx_uint32 rev_hash_code = jx_ob_hash_code( value );
            if(!rev_hash_code) {
              result = JX_FAILURE;
            } else {
              if( (info->usage+1) > (3*info->mask)>>2) { /* more than ~3/4'rs full */
                jx__hash_recondition(I, info->mode, JX_FALSE);
                info = (jx_hash_info*)I->info;
              }
              {
                jx_uint32 mask = info->mask;
                jx_uint32 *hash = info->start;
                jx_uint32 usage = info->usage;
                jx_ob *key_value = I->key_value;
                jx_uint32 index = mask & hash_code;
                jx_uint32 *hash_entry_ptr = hash + (index<<1);
                jx_uint32 *dest_forward_ptr = JX_NULL;
                jx_uint32 *dest_reverse_ptr = JX_NULL;
                jx_bool forward_found = JX_FALSE;
                jx_bool reverse_found = JX_FALSE;
                jx_bool virgin_forward = JX_FALSE;
                jx_bool virgin_reverse = JX_FALSE;
                if(!hash_entry_ptr[1]) {
                  dest_forward_ptr = hash_entry_ptr;
                  virgin_forward = JX_TRUE;
                  /* available */
                } else if( (hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == hash_code) &&
                           jx_ob_identical(key_value[hash_entry_ptr[1] & JX_HASH_OFFSET_MASK], key)) {
                  forward_found = JX_TRUE;
                } else {
                  jx_uint32 give_up = index;
                  index = (index+1) & mask;
                  while(index != give_up) {
                    hash_entry_ptr = hash + (index<<1);
                    if(!hash_entry_ptr[1]) {
                      if(!dest_forward_ptr) {
                        dest_forward_ptr = hash_entry_ptr;
                        virgin_forward = JX_TRUE;
                      }
                      break;
                    } else if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                      if(!dest_forward_ptr) dest_forward_ptr = hash_entry_ptr;
                    } else if((hash_entry_ptr[0] == hash_code) &&
                              jx_ob_identical(key_value[hash_entry_ptr[1] & JX_HASH_OFFSET_MASK], key)) {
                      forward_found = JX_TRUE;
                      break;
                    }
                    index = (index+1) & mask;
                  }
                }
                if(1) {
                  hash = info->start + ((mask+1)<<1);
                  index = mask & rev_hash_code;
                  hash_entry_ptr = hash + (index<<1);
                
                  if(!hash_entry_ptr[1]) {
                    dest_reverse_ptr = hash_entry_ptr;
                    virgin_reverse = JX_TRUE;
                  } else if( (hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == rev_hash_code) &&
                             jx_ob_identical(key_value[1+(hash_entry_ptr[1] & JX_HASH_OFFSET_MASK)], value)) {
                    reverse_found = JX_TRUE;
                  } else {
                    jx_uint32 give_up = index;
                    index = (index+1) & mask;
                    while(index != give_up) {
                      hash_entry_ptr = hash + (index<<1);
                  
                      if(!hash_entry_ptr[1]) {
                        if(!dest_reverse_ptr) {
                          dest_reverse_ptr = hash_entry_ptr;
                          virgin_reverse = JX_TRUE;
                        }
                        break;
                      } else if( !(hash_entry_ptr[1] & JX_HASH_ACTIVE)) {
                        if(!dest_reverse_ptr) dest_reverse_ptr = hash_entry_ptr;
                      } else if((hash_entry_ptr[0] == rev_hash_code) &&
                                jx_ob_identical(key_value[1+(hash_entry_ptr[1] & JX_HASH_OFFSET_MASK)], value)) {
                        reverse_found = JX_TRUE;
                        break;
                      }
                      index = (index+1) & mask;
                    }
                  }
                }

                if(!(forward_found||reverse_found)) {
                  jx_uint32 i;
                  if(virgin_forward&&virgin_reverse) {
                    i = ((info->stale+usage)<<1) + 1;
                  } else if(!virgin_forward) {
                    i = (dest_forward_ptr[1]&JX_HASH_OFFSET_MASK) | JX_HASH_ACTIVE;
                  } else {
                    i = (dest_reverse_ptr[1]&JX_HASH_OFFSET_MASK) | JX_HASH_ACTIVE;
                  }
                  if(jx_ok( jx_vla_grow_check( &I->key_value, i ) )) {
                    key_value = I->key_value;

                    if(!(virgin_forward||virgin_reverse)) info->stale--;

                    dest_forward_ptr[0] = hash_code;
                    dest_forward_ptr[1] = i;

                    dest_reverse_ptr[0] = rev_hash_code;
                    dest_reverse_ptr[1] = i;

                    key_value[i-1] = key;
                    key_value[i] = value;

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
#if 0
  {
    if(result) {
      putchar('.');
    } else {
      jx_hash_info *info = (jx_hash_info*)I->info;
      putchar('x');
      printf("(%d %p %d)",(int)jx__hash_size(I),(void*)I->info,(int)info->mode);
    }
  }
#endif
  return result;
}

jx_bool jx__hash_has_key(jx_hash *I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_uint32 size = jx_vla_size( &I->key_value );
  if(size) {
    register jx_uint32 hash_code = jx_ob_hash_code(key);
    if(hash_code) {
      if(I->key_value) {
        if(!I->info) { /* JX_HASH_RAW */
          register jx_uint32 i = (size>>1);
          jx_ob *ob = I->key_value;
          while(i--) {
            if(jx_ob_identical(*ob, key)) {            
              found = JX_TRUE;
              break;
            }
            ob += 2;
          }
        } else {
          jx_hash_info *info = (jx_hash_info*)I->info;
          switch(info->mode) {
          case JX_HASH_LINEAR:
            {
              register jx_uint32 i = info->usage;
              register jx_uint32 *hash_ptr = info->start;
              register jx_ob *ob = I->key_value;
              while(i--) {
                if( *hash_ptr == hash_code ) {
                  if(jx_ob_identical(*ob, key)) {            
                    found = JX_TRUE;
                    break;
                  }
                }
                hash_ptr++;
                ob += 2;
              }
            }
            break;
          case JX_HASH_ONE_TO_ANY:
          case JX_HASH_ONE_TO_ONE:
          case JX_HASH_ONE_TO_NIL:
            {
              jx_uint32 mask = info->mask;
              jx_uint32 *hash = info->start;
              jx_ob *key_value = I->key_value;
              jx_uint32 index = mask & hash_code;
              jx_uint32 *hash_ptr = hash + (index<<1);
              if( (hash_ptr[1] & JX_HASH_ACTIVE) && (hash_ptr[0] == hash_code)) { /* active slot with matching hash code */
                jx_uint32 i = hash_ptr[1] & JX_HASH_OFFSET_MASK;
                if(jx_ob_identical(key_value[i], key)) {
                  found = JX_TRUE;
                }
              }
              if(!found) { 
                /* otherwise, probe... */
                jx_uint32 give_up = index;
                index = (index+1) & mask;
                while(index != give_up) {
                  hash_ptr = hash + (index<<1);
                  if( !hash_ptr[1] ) 
                    break; /* virgin slot terminates probe */
                  else if((hash_ptr[1] & JX_HASH_ACTIVE) && (hash_ptr[0] == hash_code)) {
                    jx_uint32 i = hash_ptr[1] & JX_HASH_OFFSET_MASK;
                    if(jx_ob_identical(key_value[i], key)) {
                      found = JX_TRUE;
                      break;
                    }
                  }
                  index = (index+1) & mask;
                }
              }
            }
            break;
          }
        }
      }
    }
  }
  return found;
}

jx_bool jx__hash_get(jx_ob *result, jx_hash *I, jx_ob key)
{
  jx_bool found = JX_FALSE;
  jx_uint32 size = jx_vla_size( &I->key_value );
  if(size) {
    register jx_uint32 hash_code = jx_ob_hash_code(key);
    if(hash_code) {
      if(!I->info) { /* JX_HASH_RAW */
        register jx_int i = (size>>1);
        register jx_ob *ob = I->key_value;
        while(i--) {
          if(jx_ob_identical(*ob, key)) {            
            found = JX_TRUE;
            *result = ob[1]; /* borrowing */
            break;
          }
          ob += 2;
        }
      } else {
        jx_hash_info *info = (jx_hash_info*)I->info;
        switch(info->mode) {
        case JX_HASH_LINEAR:
          {
            register jx_uint32 i = info->usage;
            register jx_uint32 *hash_ptr = info->start;
            register jx_ob *ob = I->key_value;
            while(i--) {
              if( *hash_ptr == hash_code ) {
                if(jx_ob_identical(*ob, key)) {            
                  found = JX_TRUE;
                  *result = ob[1]; /* borrowing */
                  break;
                }
              }
              hash_ptr++;
              ob += 2;
            }
          }
          break;
        case JX_HASH_ONE_TO_ANY:
        case JX_HASH_ONE_TO_ONE:
        case JX_HASH_ONE_TO_NIL:
          {
            jx_uint32 mask = info->mask;
            jx_uint32 *hash = info->start;
            jx_ob *key_value = I->key_value;
            jx_uint32 index = mask & hash_code;
            jx_uint32 *hash_ptr = hash + (index<<1);
            if( (hash_ptr[1] & JX_HASH_ACTIVE) && (hash_ptr[0] == hash_code)) { /* active slot with matching hash code */
              jx_uint32 i = hash_ptr[1] & JX_HASH_OFFSET_MASK;
              if(jx_ob_identical(key_value[i], key)) {
                *result = key_value[i+1]; /* borrowing */
                found = JX_TRUE;
                break;
              }
            }
            if(!found) { 
              /* otherwise, probe... */
              jx_uint32 give_up = index;
              index = (index+1) & mask;
              while(index != give_up) {
                hash_ptr = hash + (index<<1);
                if( !hash_ptr[1] ) 
                  break; /* virgin slot terminates probe */
                else if((hash_ptr[1] & JX_HASH_ACTIVE) && (hash_ptr[0] == hash_code)) {
                  jx_uint32 i = hash_ptr[1] & JX_HASH_OFFSET_MASK;
                  if(jx_ob_identical(key_value[i], key)) {
                    *result = key_value[i+1]; /* borrowing */
                    found = JX_TRUE;
                    break;
                  }
                }
                index = (index+1) & mask;
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

#if 0

JX_INLINE jx_bool jx__hash_mark(jx_hash *I, jx_ob key)
{
#if 0
    jx_hash_info *info = (jx_hash_info*)I->info;
    putchar('~');
    if(info) {
      putchar('0'+info->mode);
    }
  jx_bool result = jx__hash_set(I,key,jx_none);
#if 0
  if(key.data.uword == 0x407738) {
    putchar('s');putchar('\n');
  }
#endif
  if(!result) {
    printf("set failed for %p in %p %d\n",(void*)key.data.gc,(void*)I,
           (int)jx__hash_size(I));
  } else {
    jx_hash_info *info = (jx_hash_info*)I->info;
    putchar('+');
    if(info) {
      putchar('0'+info->mode);
    }
  }
  return result;
#if 0
      {
        jx_ob tst = {0x40, {0x407738}};
        putchar(jx__hash_check(I,tst) ? 'y' : 'n');
      }
#endif
#else
  return jx__hash_set(I,key,jx_none);
#endif
}

JX_INLINE jx_bool jx__hash_clear(jx_hash *I, jx_ob key)
{
#if 0
  jx_bool result = jx__hash_delete(I,key);
#if 0
      {
        jx_ob tst = {0x40, {0x407738}};
        putchar(jx__hash_check(I,tst) ? '+' : '-');
      }
#endif

  if(!result) {
    printf("clear failed for %p %p %d\n",(void*)key.data.gc,(void*)I,
           (int)jx__hash_size(I));
  } else {
    jx_hash_info *info = (jx_hash_info*)I->info;
    putchar('-');
    if(info) {
      putchar('0'+info->mode);
    }
  }
#if 0
      {
        jx_ob tst = {0x40, {0x407738}};
        putchar(jx__hash_check(I,tst) ? '+' : '-');
      }
#endif

  return result;
#else
  return jx__hash_delete(I,key);
#endif
}


JX_INLINE jx_bool jx__hash_get_key(jx_ob *result, 
                                         jx_hash *I, jx_ob value)
{
  jx_bool found = JX_FALSE;
  if(I->key_value && !(value.meta & JX_BIT_MUTABLE)) {
    if(!I->info) { /* JX_HASH_RAW */
      jx_size size = 0;
      if(jx_ok( JX_HEAP_VLA_GET_SIZE( &size, I->key_value ))) {
        register jx_int i = (size>>1);
        jx_ob *ptr = I->key_value;
        while(i--) {
          if(jx_ob_identical(ptr[1], value)) {            
            found = JX_TRUE;
            *result = jx_retain(ptr[0]);
          }
          ptr+=2;
        }
      }
    } else { /* not JX_HASH_RAW */
      jx_hash_info *info = (jx_hash_info*)I->info;
      register jx_uint32 hash_code;
      
      if((value.meta & JX_META_STR_CONST) == JX_META_STR_CONST) {
        hash_code = jx__get_c_str_hash((const jx_uchar8*)value.data.str_const);
      } else {
        hash_code = jx_ob_hash_code(value);
      }

      switch(info->mode) { /* brute-force table scan */
      case JX_HASH_LINEAR:
      case JX_HASH_ONE_TO_ANY:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_size size = 0;
          if(jx_ok( JX_HEAP_VLA_GET_SIZE( &size, I->key_value ))) {
            register jx_int i = (size>>1);
            jx_ob *ptr = I->key_value;
            while(i--) {
              if(jx_ob_identical(ptr[1], value)) {            
                found = JX_TRUE;
                *result = jx_retain(ptr[0]);
              }
              ptr+=2;
            }
          }
        }
        break;
      case JX_HASH_ONE_TO_ONE:
        {
          jx_uint32 mask = info->mask;
          jx_uint32 *hash = info->start + ((mask+1)<<1);
          jx_ob *key_value = I->key_value;
          jx_uint32 index = mask & hash_code;
          jx_uint32 *hash_ptr = hash + (index<<1);
          if( (hash_ptr[1] & JX_HASH_ACTIVE) && (hash_ptr[0] == hash_code)) { /* active slot with matching hash code */
            jx_uint32 i = hash_ptr[1] & JX_HASH_OFFSET_MASK;
            if(jx_ob_identical(key_value[i+1], value)) {
              *result = jx_retain(key_value[i]);
              found = JX_TRUE;
              break;
            }
          }
          if(!found) { 
            /* otherwise, probe... */
            jx_uint32 give_up = index;
            index = (index+1) & mask;
            while(index != give_up) {
              hash_ptr = hash + (index<<1);
              if( !hash_ptr[1] ) 
                break; /* virgin slot terminates probe */
              else if((hash_ptr[1] & JX_HASH_ACTIVE) && (hash_ptr[0] == hash_code)) {
                jx_uint32 i = hash_ptr[1] & JX_HASH_OFFSET_MASK;
                if(jx_ob_identical(key_value[i+1], value)) {
                  *result = jx_retain(key_value[i]);
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index+1) & mask;
            }
          }
        }
        break;
      }
    }
  }
  return found;
}




jx_bool jx__hash_delete(jx_hash *I, jx_ob key)

{  
  jx_bool found = JX_FALSE;
  if((key.meta & JX_BIT_HANDLE) || (!(key.meta & JX_BIT_MUTABLE))) {

#if 0
      {
        jx_ob tst = {0x40, {0x407738}};
        putchar(jx__hash_check(I,tst) ? 'M' : '_');
        if((key.meta == tst.meta)&&(key.data.uword == tst.data.uword)) { 
          putchar('D');
        } else {
          putchar('N');
        }
      }
#endif

    if(!I->info) { /* JX_HASH_RAW */
      jx_size size = 0;
      if(jx_ok( JX_HEAP_VLA_GET_SIZE( &size, I->key_value ))) {
        register jx_int i = (size>>1);
        jx_ob *ob = I->key_value;
        while(i--) {
          if(jx_ob_identical(*ob, key)) {            
            found = JX_TRUE;
            jx_release(ob[0]);
            jx_release(ob[1]);
            if(size>2) {
              ob[0] = I->key_value[size-2];
              ob[1] = I->key_value[size-1];
            }
            if(!jx_ok( jx_vla_resize( &I->key_value, size-2) )) {
              ob[0] = jx_ob_from_null();             
              ob[1] = jx_ob_from_null();
            }
            break;
          }
          ob+=2;
        }
      }
    } else {
      jx_hash_info *info = (jx_hash_info*)I->info;

      switch(info->mode) {
      case JX_HASH_LINEAR:
        {
          register jx_uint32 i = info->usage;
          register jx_uint32 hash_code = jx_ob_hash_code( key );
          register jx_uint32 *hash_entry_ptr = info->start;
          register jx_ob *ob = I->key_value;
          while(i--) {
            if( *hash_entry_ptr == hash_code ) {
              if(jx_ob_identical(*ob, key)) {                          
                jx_size size = ((info->usage-1) << 1);
                found = JX_TRUE;
                jx_release(ob[0]);
                jx_release(ob[1]);
                if(size) {
                  ob[0] = I->key_value[size];
                  ob[1] = I->key_value[size+1];
                  *hash_entry_ptr = info->start[info->usage-1];
                }
                if(!jx_ok( jx_vla_resize( &I->key_value, size ))) {
                  ob[0] = jx_ob_from_null();             
                  ob[1] = jx_ob_from_null();
                }
                info->usage--;
                break;
              }
            }
            hash_entry_ptr++;
            ob+=2;
          }
        }
        break;
      case JX_HASH_ONE_TO_ANY:
      case JX_HASH_ONE_TO_NIL:
        {
          jx_uint32 hash_code = jx_ob_hash_code( key );
          jx_uint32 mask = info->mask;
          jx_uint32 *hash = info->start;
          jx_ob *key_value = I->key_value;
          jx_uint32 index = mask & hash_code;
          jx_uint32 *hash_entry_ptr = hash + (index<<1);

          if( (hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == hash_code)) { /* active slot with matching hash code */
            jx_uint32 i = hash_entry_ptr[1] & JX_HASH_OFFSET_MASK;
            if(jx_ob_identical(key_value[i], key)) {
              jx_ob *ob = key_value + i;
              hash_entry_ptr[1] = (i | JX_HASH_DELETED);
              jx_release(ob[0]);
              jx_release(ob[1]);
              jx_os_memset( ob, 0, sizeof(jx_ob)*2);
              info->usage--;
              info->stale++;
              found = JX_TRUE;
            }
          }
          if(!found) { 
            /* otherwise, probe... */
            jx_uint32 give_up = index;
            index = (index+1) & mask;
            while(index != give_up) {
              hash_entry_ptr = hash + (index<<1);
              if( !hash_entry_ptr[1] ) {
                break; /* virgin slot terminates probe */
              } else if((hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == hash_code)) {
                jx_uint32 i = hash_entry_ptr[1] & JX_HASH_OFFSET_MASK;
                if(jx_ob_identical(key_value[i], key)) {
                  jx_ob *ob = key_value + i;
                  hash_entry_ptr[1] = (i | JX_HASH_DELETED);
                  jx_release(ob[0]);
                  jx_release(ob[1]);
                  jx_os_memset( ob, 0, sizeof(jx_ob)*2);
                  info->usage--;
                  info->stale++;
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index+1) & mask;
            }
          }

          {
            jx_size usage = info->usage;
            if(!usage) {
              jx__hash_recondition(I,JX_HASH_RAW,JX_TRUE); /* purge empty hash table */
            } else if(found && (info->stale > usage) && 
                      (usage+info->stale) > (info->mask)>>1) {
              jx__hash_recondition(I,info->mode,JX_TRUE); /* pack & (possibly) shrink */
            }
          }
        }
        break;
      case JX_HASH_ONE_TO_ONE:
        {
          jx_uint32 hash_code = jx_ob_hash_code( key );
          jx_uint32 mask = info->mask;
          jx_uint32 *hash = info->start;
          jx_ob *key_value = I->key_value;
          jx_uint32 index = mask & hash_code;
          jx_uint32 *hash_entry_ptr = hash + (index<<1);
          if( (hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == hash_code)) { /* active slot with matching hash code */
            jx_uint32 i = hash_entry_ptr[1] & JX_HASH_OFFSET_MASK;
            if(jx_ob_identical(key_value[i], key)) {
              jx_ob *ob = key_value + i;
              hash_entry_ptr[1] = (i | JX_HASH_DELETED);

              { /* reverse */
                jx_uint32 rev_hash_code = jx_ob_hash_code( ob[1] );
                jx_uint32 *rev_hash = info->start + ((mask+1)<<1);
                jx_uint32 rev_index = mask & rev_hash_code;
                jx_uint32 *rev_hash_entry_ptr = rev_hash + (rev_index<<1);
                
                if( (rev_hash_entry_ptr[1] & JX_HASH_ACTIVE) && (rev_hash_entry_ptr[0] == hash_code) && 
                    jx_ob_identical(key_value[rev_hash_entry_ptr[1]], ob[1])) {
                  rev_hash_entry_ptr[1] = (i | JX_HASH_DELETED);
                } else {
                  jx_uint32 give_up = rev_index;
                  rev_index = (rev_index+1) & mask;
                  while(rev_index != give_up) {
                    rev_hash_entry_ptr = hash + (rev_index<<1);
                    if( (rev_hash_entry_ptr[1] & JX_HASH_ACTIVE) && (rev_hash_entry_ptr[0] == hash_code) && 
                        jx_ob_identical(key_value[rev_hash_entry_ptr[1]], ob[1])) {
                      rev_hash_entry_ptr[1] = (i | JX_HASH_DELETED);
                      break;
                    }
                    rev_index = (rev_index+1) & mask;
                  }
                }
              }
                
              jx_release(ob[0]);
              jx_release(ob[1]);
              jx_os_memset( ob, 0, sizeof(jx_ob)*2);
              info->usage--;
              info->stale++;
              found = JX_TRUE;
            }
          }
          if(!found) { 
            /* otherwise, probe... */
            jx_uint32 give_up = index;
            index = (index+1) & mask;
            while(index != give_up) {
              hash_entry_ptr = hash + (index<<1);
              if( !hash_entry_ptr[1] ) {
                break; /* virgin slot terminates probe */
              } else if((hash_entry_ptr[1] & JX_HASH_ACTIVE) && (hash_entry_ptr[0] == hash_code)) {
                jx_uint32 i = hash_entry_ptr[1] & JX_HASH_OFFSET_MASK;
                if(jx_ob_identical(key_value[i], key)) {
                  jx_ob *ob = key_value + i;
                  hash_entry_ptr[1] = (i | JX_HASH_DELETED);

                  { /* reverse */
                    jx_uint32 rev_hash_code = jx_ob_hash_code( ob[1] );
                    jx_uint32 *rev_hash = info->start + ((mask+1)<<1);
                    jx_uint32 rev_index = mask & rev_hash_code;
                    jx_uint32 *rev_hash_entry_ptr = rev_hash + (rev_index<<1);
                    
                    if( (rev_hash_entry_ptr[1] & JX_HASH_ACTIVE) && (rev_hash_entry_ptr[0] == hash_code) && 
                        jx_ob_identical(key_value[rev_hash_entry_ptr[1]], ob[1])) {
                      rev_hash_entry_ptr[1] = (i | JX_HASH_DELETED);
                    } else {
                      jx_uint32 give_up = rev_index;
                      rev_index = (rev_index+1) & mask;
                      while(rev_index != give_up) {
                        rev_hash_entry_ptr = hash + (rev_index<<1);
                        if( (rev_hash_entry_ptr[1] & JX_HASH_ACTIVE) && (rev_hash_entry_ptr[0] == hash_code) && 
                            jx_ob_identical(key_value[rev_hash_entry_ptr[1]], ob[1])) {
                          rev_hash_entry_ptr[1] = (i | JX_HASH_DELETED);
                          break;
                        }
                        rev_index = (rev_index+1) & mask;
                      }
                    }
                  }
 
                  jx_release(ob[0]);
                  jx_release(ob[1]);
                  jx_os_memset( ob, 0, sizeof(jx_ob)*2);
                  info->usage--;
                  info->stale++;
                  found = JX_TRUE;
                  break;
                }
              }
              index = (index+1) & mask;
            }
          }

          {
            jx_size usage = info->usage;
            if(found && (info->stale > usage) && 
               ((usage+info->stale) > ((info->mask)>>1))) {
              jx__hash_recondition(I,info->mode,JX_TRUE); /* pack & (possibly) shrink */
            }
          }
        }
        break;
      }
    }
  }
#if 0
      {
        jx_ob tst = {0x40, {0x407738}};
        putchar(jx__hash_check(I,tst) ? 'm' : '~');
        if((key.meta == tst.meta)&&(key.data.uword == tst.data.uword)) { 
          putchar('D');
        }
      }
#endif
#if 0
  {
    if(found) {
      putchar('_');
    } else {
      putchar('x');
    }
  }
#endif
  return found;
}


#endif

/* general purpose free method */

jx_status jx__ob_free(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_GC) {
    switch(ob.meta & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      return jx_vla_free(&ob.data.str);
      break;
    case JX_META_BIT_LIST:
      return jx_list_free(ob.data.list);
      break;
    case JX_META_BIT_HASH:
      return jx_hash_free(ob.data.hash);
      break;
    }
    /* handle based on type */
  }
  return JX_SUCCESS;
}

