
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_private.h"

void *jx_vla_new(jx_int rec_size,jx_int size)
{
  jx_vla *vla = calloc(1, sizeof(jx_vla) + rec_size * size);
  if(vla) {
    vla->size = size;
    vla->rec_size = rec_size;
    printf("allocated %p\n",(void*)vla);
    return (void*)(vla + 1);
  } else
    return NULL;
}

jx_status jx_vla_resize(void **vla, jx_int new_size)
{
  if(vla) {
    jx_vla *rec = ((jx_vla*)(*vla)) - 1;
    if(new_size != rec->size) {
      rec = realloc(rec, sizeof(jx_vla) + rec->rec_size * new_size);
      if(rec) {
        *vla = (void*)(rec + 1);
        if(rec->size < new_size) { /* zero new memory */
          jx_char *ptr = (jx_char*)vla;
          memset( ptr + rec->size * rec->rec_size, 0, (new_size - rec->size) * rec->rec_size);
        }
        rec->size = new_size;
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

jx_status jx_vla_free(void *vla)
{
  printf("freed %p\n",vla);
  if(vla) {
    free( ((jx_vla*)vla) - 1 );
    return JX_SUCCESS;
  } else {
    return JX_FAILURE;
  }
}

static jx_ob jx_ob_from_str_vla(jx_char *vla) /* steals ownership */
{
  jx_ob result = JX_OB_NULL;
  jx_int size = jx_vla_get_size(vla);
  if(size < JX_TINY_STR_SIZE) {
    result.meta = JX_META_BIT_STR | size;
    strcpy(result.data.tiny_str, vla);
    jx_vla_free(vla);
  } else {
    result.meta = JX_META_BIT_STR | JX_META_BIT_GC;
    result.data.str = vla;
  }
  return result;
}

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
    result.data.str = jx_vla_new(1,size);
    if(result.data.str) {
      strcpy(result.data.str, str);
    }
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

jx_ob jx_list_new(void)
{
  jx_ob result = JX_OB_LIST;
  result.data.list = calloc(1,sizeof(jx_list));
  return result;
}

static void jx_list_free(jx_list *list)
{
  jx_vla_free(list->data.vla);
  free(list);
}

jx_int jx_list_get_size(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_LIST) {
    return jx_vla_get_size(ob.data.list->data.vla);
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

static jx_status jx_list_unpack_data(jx_list *list)
{
  /* homogenous list becoming hetergeneous */
  jx_int size = jx_vla_get_size(list->data.vla);
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
    jx_vla_free(list->data.vla);
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
    if(rec->data.vla) { 
      if(rec->packed_meta && (rec->packed_meta != ob.meta)) {
        if(jx_list_unpack_data(rec) != JX_SUCCESS)
          return JX_FAILURE;
      }
      {
        jx_int size = jx_vla_get_size(rec->data.vla);
        if(jx_vla_grow_check( &rec->data.vla, size)) {
          if(rec->packed_meta && (rec->packed_meta == ob.meta)) {
            jx_list_set_packed_data(rec, size, ob);
          } else if(!rec->packed_meta) {
            rec->data.ob_vla[size] = ob;
          }
          return JX_SUCCESS;
        }
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
    if( (index>=0) && (index < jx_vla_get_size(rec->data.vla))) {
      if(rec->packed_meta) {
        result = jx_list_get_packed_data(rec, index);
      } else {
        result = rec->data.ob_vla[index];
      }
    }
  }
  return result;
}

jx_ob jx_ob_to_json_str(jx_ob ob)
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
  case JX_META_BIT_HASH:
    break;
  case JX_META_BIT_LIST:
    break;
  }
  return jx_ob_from_null();
}

jx_status jx__ob_free(jx_ob ob)
{
   if(ob.meta & JX_META_BIT_GC) {
    switch(ob.meta & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      jx_vla_free(ob.data.str);
      break;
    case JX_META_BIT_LIST:
      jx_list_free(ob.data.list);
      break;
    }
    /* handle based on type */
  }
  return JX_SUCCESS;
}
