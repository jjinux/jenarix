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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_private.h"

/* JSON output */

static jx_ob jx__list_to_json(jx_list * list, jx_int flags)
{
  jx_int i, size = jx_vla_size(&list->data.vla);
  jx_char *vla = jx_vla_new(1, 2);
  if(vla) {
    jx_bool comma = JX_FALSE;
    vla[0] = '[';
    if(list->packed_meta_bits) {
      switch (list->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_INT:
        {
          jx_ob ob = JX_OB_INT;
          jx_int *int_ = list->data.int_vla;
          for(i = 0; i < size; i++) {
            ob.data.io.int_ = *(int_++);
            {
              jx_ob str = jx_ob_to_json_with_flags(ob, flags);
              if(comma) {
                jx_vla_append_c_str(&vla, ",");
              } else
                comma = JX_TRUE;
              jx_vla_append_ob_str(&vla, str);
              jx_ob_free(str);
            }
          }
        }
        break;
      case JX_META_BIT_FLOAT:
        {
          jx_ob ob = JX_OB_FLOAT;
          jx_float *float_ = list->data.float_vla;
          for(i = 0; i < size; i++) {
            ob.data.io.float_ = *(float_++);
            {
              jx_ob str = jx_ob_to_json_with_flags(ob, flags);
              if(comma) {
                jx_vla_append_c_str(&vla, ",");
              } else
                comma = JX_TRUE;
              jx_vla_append_ob_str(&vla, str);
              jx_ob_free(str);
            }
          }
        }
        break;
      }
    } else {
      jx_ob *ob = list->data.ob_vla;

      for(i = 0; i < size; i++) {
        jx_ob str = jx_ob_to_json_with_flags(*(ob++), flags);
        if(comma) {
          jx_vla_append_c_str(&vla, ",");
        } else
          comma = JX_TRUE;
        jx_vla_append_ob_str(&vla, str);
        jx_ob_free(str);
      }
    }
    jx_vla_append_c_str(&vla, "]");
    return jx_ob_with_str_vla(&vla);
  }
  return jx_ob_from_null();
}

static jx_ob jx__hash_to_json(jx_hash * I, jx_int flags)
{
  jx_int size = jx_vla_size(&I->key_value);
  jx_char *vla = jx_vla_new(1, 2);
  if(vla) {
    jx_bool comma = JX_FALSE;
    vla[0] = '{';
    if(size) {
      jx_hash_info *info = (jx_hash_info *) I->info;
      if((!info) || (info->mode == JX_HASH_LINEAR)) {
        register jx_int i = (info ? info->usage : (size >> 1));
        register jx_ob *ob = I->key_value;
        while(i--) {
          jx_ob key = jx_ob_to_json_with_flags(ob[0], flags);
          jx_ob value = jx_ob_to_json_with_flags(ob[1], flags);
          if(comma) {
            jx_vla_append_c_str(&vla, ",");
          } else {
            comma = JX_TRUE;
          }
          jx_vla_append_ob_str(&vla, key);
          jx_vla_append_c_str(&vla, ":");
          jx_vla_append_ob_str(&vla, value);
          jx_ob_free(key);
          jx_ob_free(value);
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
              if(hash_entry[1] & JX_HASH_ENTRY_ACTIVE) {        /* active slot with matching hash code */
                jx_uint32 kv_offset = hash_entry[1] & JX_HASH_ENTRY_KV_OFFSET_MASK;
                jx_ob key = jx_ob_to_json_with_flags(key_value[kv_offset], flags);
                jx_ob value = jx_ob_to_json_with_flags(key_value[kv_offset + 1], flags);
                if(comma) {
                  jx_vla_append_c_str(&vla, ",");
                } else {
                  comma = JX_TRUE;
                }
                jx_vla_append_ob_str(&vla, key);
                jx_vla_append_c_str(&vla, ":");
                jx_vla_append_ob_str(&vla, value);
                jx_ob_free(key);
                jx_ob_free(value);
              }
              index++;
            } while(index <= mask);
          }
          break;
        }
      }
    }
    jx_vla_append_c_str(&vla, "}");
    return jx_ob_with_str_vla(&vla);
  }
  return jx_ob_from_null();
}

static void force_float(char *str) 
{
  char ch,*p;
  p = str;
  while( (ch=*(p++)) ) {
    switch(ch) {
    case '.':
    case 'e':
    case 'E':
      return; /* float implied */
      break;
    }
  }
  p[-1] = '.';
  p[0] = '0';
  p[1] = 0;
}

jx_ob jx_ob_to_json_with_flags(jx_ob ob, jx_int flags)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case 0:
    return jx_ob_from_str("null");
    break;
  case JX_META_BIT_BOOL:
    if(ob.data.io.bool_)
      return jx_ob_from_str("true");
    else
      return jx_ob_from_str("false");
    break;
  case JX_META_BIT_INT:
    {
      char buffer[50];
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
      char buffer[50];
#ifdef JX_64_BIT
      snprintf(buffer, sizeof(buffer), "%.17g", ob.data.io.float_);
#else
      snprintf(buffer, sizeof(buffer), "%.8g", ob.data.io.float_);
#endif
      force_float(buffer);
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_STR:
    if(ob.meta.bits & JX_META_BIT_GC) {
      jx_int size = jx_vla_size(&ob.data.io.str);
      jx_char *buffer = jx_malloc(size + 2);
      if(buffer) {
        buffer[0] = '"';
        memcpy(buffer + 1, ob.data.io.str, size);
        buffer[size] = '"';
        buffer[size + 1] = 0;
        {
          jx_ob result = jx_ob_from_str(buffer);
          jx_free(buffer);
          return result;
        }
      }
    } else {
      char buffer[JX_TINY_STR_SIZE + 2];
      jx_int size = ob.meta.bits & JX_META_MASK_TINY_STR_SIZE;
      buffer[0] = '"';
      memcpy(buffer + 1, ob.data.io.tiny_str, size);
      buffer[size + 1] = '"';
      buffer[size + 2] = 0;
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_IDENT: /* Jenarix JSON extension */
    if(ob.meta.bits & JX_META_BIT_GC) {
      return jx_ob_from_str(ob.data.io.str);
    } else {
      return jx_ob_from_str(ob.data.io.tiny_str);
    }
    break;
  case JX_META_BIT_BUILTIN: /* for debuggin only */
    {
      char buffer[50];
      if(ob.meta.bits & JX_META_BIT_GC) {
        sprintf(buffer,"*builtin_%p*",(void*)ob.data.io.builtin);
      } else {
        sprintf(buffer,"*builtin_%03d*",ob.data.io.int_);
      }
      return jx_ob_from_str(buffer);
    }
    break;
  case JX_META_BIT_LIST:
    return jx__list_to_json(ob.data.io.list, flags);
    break;
  case JX_META_BIT_HASH:
    return jx__hash_to_json(ob.data.io.hash, flags);
    break;
  }
  return jx_ob_from_null();
}

