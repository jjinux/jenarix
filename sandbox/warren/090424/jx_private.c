
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_private.h"

typedef struct {
  jx_int size;
  jx_int rec_size;
} jx_vla; 

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
    jx_vla *rec = ((jx_vla*)vla) - 1;
    if(new_size != rec->size) {
      rec = realloc(vla, sizeof(jx_vla) + rec->rec_size * new_size);
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

jx_ob jx_ob_from_str(jx_char *str)
{
  jx_int size = strlen(str);
  jx_ob result = JX_OB_NULL;

  if(size < JX_TINY_STR_SIZE) {
    result.meta = JX_META_BIT_STR | size;
    strcpy(result.data.tiny_str, str);
  } else {
    /* string not tiny -- use external storage */
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

jx_status jx__ob_free(jx_ob ob)
{
  if(ob.meta & JX_META_BIT_GC) {
    switch(ob.meta & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      jx_vla_free(ob.data.str);
      break;
    }
    /* handle based on type */
  }
  return JX_SUCCESS;
}
