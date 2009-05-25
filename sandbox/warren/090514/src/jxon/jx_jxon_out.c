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

/* JXON output */

JX_INLINE jx_int get_width(jx_int flags)
{
  return ((flags & JX_JXON_FLAG_WIDTH_MASK)>>8);
}

JX_INLINE jx_int get_indent(jx_int flags)
{
  return flags & JX_JXON_FLAG_INDENT_MASK;
}

JX_INLINE jx_int get_tab(jx_int flags)
{
  return 2;
  //  return (flags & JX_JXON_FLAG_TAB_MASK)>>22;
}

JX_INLINE jx_int prefix(jx_char **ref, jx_int flags, jx_int *space_left)
{
  if(ref && space_left && (flags & JX_JXON_FLAG_PRETTY )) {

    if (!(JX_JXON_FLAG_NOT_NEWLINE & flags)) {
      int indent = get_indent(flags);
      char spaces[JX_JXON_FLAG_INDENT_MASK+1];
      int start = jx_vla_size(ref);
      int pos = start;
    
      if(JX_JXON_FLAG_COMMENT & flags) {
        pos = jx_vla_append_c_str(ref, "#");
      }
      if(indent) {
        char *c = spaces;
        while(indent--) {
          *(c++)=' ';
        }
        *c = 0;

        pos = jx_vla_append_c_str(ref,spaces);
      }
      *space_left = get_width(flags) - (pos - start);
      
      return flags | JX_JXON_FLAG_NOT_NEWLINE;
    }
  }
  return flags;
}

JX_INLINE jx_int newline(jx_char **ref, jx_int flags, jx_int *space_left)
{
  if(ref && space_left && (flags & JX_JXON_FLAG_PRETTY )) {
    if(JX_JXON_FLAG_NOT_NEWLINE & flags) {
      jx_vla_append_c_str(ref,"\n");
      *space_left = get_width(flags);
      return flags & ~(JX_JXON_FLAG_NOT_NEWLINE);
    }
  }
  return flags;
}

JX_INLINE jx_int autowrap(jx_char **ref,  jx_int flags, jx_int *space_left, jx_int needed)
{

  if(ref && space_left && (flags & JX_JXON_FLAG_PRETTY )) {
    if(get_width(flags)) {
      if(needed > *space_left) {
        flags = newline(ref,flags,space_left);
        flags = prefix(ref,flags,space_left);
      } else {
        *space_left -= needed;
      }
    }
  }
  return flags;
}

JX_INLINE jx_int nest(jx_int flags,jx_int amount)
{
  jx_int column = get_indent(flags) + get_tab(flags) * amount;
  if(column > JX_JXON_FLAG_INDENT_MASK)
    column = JX_JXON_FLAG_INDENT_MASK;
  else if(column<0)
    column = 0;
  return  (flags & ~(JX_JXON_FLAG_INDENT_MASK)) | 
    (column & JX_JXON_FLAG_INDENT_MASK);
}

jx_ob jx__ob_to_jxon_with_flags(jx_ob ob, jx_char **ref, jx_int flags, jx_int *space_left);

static jx_status jx__list_to_jxon(jx_list * list, jx_char **ref, jx_int flags, jx_int *space_left)
{
  jx_int i, size = jx_vla_size(&list->data.vla);
  char square[] = "]", rounded[] = ")";
  char space[] = " ", comma[] = ",";
  char *delimit = square;
  char *sep = comma;

  flags = prefix(ref,flags,space_left);
  flags = autowrap(ref,flags,space_left,2);
  {
    int ob_start = jx_vla_size(ref);

    jx_vla_append_c_str(ref, " ["); 

    flags = nest(flags,1);

    if(list->packed_meta_bits) {
      switch (list->packed_meta_bits & JX_META_MASK_TYPE_BITS) {
      case JX_META_BIT_INT:
        {
          jx_ob ob = JX_OB_INT;
          jx_int *int_ = list->data.int_vla;
          for(i = 0; i < size; i++) {
            ob.data.io.int_ = *(int_++);
            jx_ob_free( jx__ob_to_jxon_with_flags(ob, ref, flags, space_left));
            if(i<(size-1)) {
              flags = autowrap(ref,flags,space_left,1);
              jx_vla_append_c_str(ref, sep);
              if(JX_JXON_FLAG_PRETTY && sep[0]!=' ') {
                flags = autowrap(ref,flags,space_left,1);
                jx_vla_append_c_str(ref, " ");
              }
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
            jx_ob_free( jx__ob_to_jxon_with_flags(ob, ref, flags, space_left));
            if(i<(size-1)) {
              flags = autowrap(ref,flags,space_left,1);
              jx_vla_append_c_str(ref, sep);
              if(JX_JXON_FLAG_PRETTY && sep[0]!=' ') {
                flags = autowrap(ref,flags,space_left,1);
                jx_vla_append_c_str(ref, " ");
              }
            }
          }
        }
        break;
      }
    } else {
      jx_ob *ob = list->data.ob_vla;
      if(!(flags&(JX_JXON_FLAG_JSON_ENCODE |
                  JX_JXON_FLAG_NO_ROUNDED))) {
        if(ob && (jx_ident_check(*ob)||(jx_builtin_callable_check(*ob)))) {
          (*ref)[ob_start] = '(';
          delimit = rounded;
          sep = space;
        }
      }
      for(i = 0; i < size; i++) {
        jx_ob_free( jx__ob_to_jxon_with_flags(*(ob++), ref, flags, space_left) );
        if(i<(size-1)) {
          flags = autowrap(ref,flags,space_left,1);
          jx_vla_append_c_str(ref, sep);
          if(JX_JXON_FLAG_PRETTY && jx_list_check(*ob)) {
            flags = newline(ref,flags,space_left);
            flags = prefix(ref,flags,space_left);
          } else if(JX_JXON_FLAG_PRETTY && sep[0]!=' ') {
            flags = autowrap(ref,flags,space_left,1);
            jx_vla_append_c_str(ref, " ");
          }
        }
      }
    }
    flags = autowrap(ref,flags,space_left,1);
    jx_vla_append_c_str(ref, delimit);
    flags = nest(flags,-1);
  }
  return JX_SUCCESS;
}

static jx_status jx__hash_to_jxon(jx_hash * I, jx_char **ref, jx_int flags, jx_int *space_left)
{
  jx_int size = jx_vla_size(&I->key_value);
  jx_bool comma = JX_FALSE;
  flags = autowrap(ref,flags,space_left,1);
  jx_vla_append_c_str(ref, "{"); 
  if(size) {
    jx_hash_info *info = (jx_hash_info *) I->info;
    if((!info) || (info->mode == JX_HASH_LINEAR)) {
      register jx_int i = (info ? info->usage : (size >> 1));
      register jx_ob *ob = I->key_value;
      while(i--) {
        if(comma) {
          flags = autowrap(ref,flags,space_left,1);
          jx_vla_append_c_str(ref, ",");
        } else {
          comma = JX_TRUE;
        }
#if 0
        if((flags & JX_JXON_FLAG_JSON_LOSSY) && 
           !(ob[0].meta.bits &
             (JX_META_BIT_STR|JX_META_BIT_IDENT|JX_META_BIT_BUILTIN))) {
          jx_vla_append_ob_str(ref, jx_ob_from_str("\""));
          jx_vla_append_ob_str(ref, key);                 
          jx_vla_append_ob_str(ref, jx_ob_from_str("\""));
        } else {
          jx_vla_append_ob_str(ref, key);
        }
#endif

        jx_ob_free( jx__ob_to_jxon_with_flags(ob[0], ref, flags, space_left) );
              flags = autowrap(ref,flags,space_left,1);
        jx_vla_append_c_str(ref, ":");
        jx_ob_free( jx__ob_to_jxon_with_flags(ob[1], ref, flags, space_left) );
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
              if(comma) {
              flags = autowrap(ref,flags,space_left,1);
                jx_vla_append_c_str(ref, ",");
              } else {
                comma = JX_TRUE;
              }
              jx_ob_free( jx__ob_to_jxon_with_flags(key_value[kv_offset], ref, flags, space_left) );
              flags = autowrap(ref,flags,space_left,1);
              jx_vla_append_c_str(ref, ":");
              jx_ob_free( jx__ob_to_jxon_with_flags(key_value[kv_offset + 1], ref, flags, space_left) );
            }
            index++;
          } while(index <= mask);
        }
        break;
      }
    }
  }
              flags = autowrap(ref,flags,space_left,1);
  jx_vla_append_c_str(ref, "}");
  return JX_SUCCESS;
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

jx_ob jx__ob_to_jxon_with_flags(jx_ob ob, jx_char **ref, jx_int flags, jx_int *space_left)
{
  jx_bits bits = ob.meta.bits;
  if(ref && flags & JX_JXON_FLAG_SHOW_WEAK) {
    if(ob.meta.bits & JX_META_BIT_WEAK_REF) {
      jx_vla_append_c_str(ref,">-WEAK->");
    }
  }
  switch (bits & JX_META_MASK_TYPE_BITS) {
  case 0:
    {
      jx_ob st = jx_ob_from_str("null");
      if(!ref) {
        return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
      }
    }
    break;
  case JX_META_BIT_BOOL:
    {
      jx_ob st;
      if(ob.data.io.bool_)
        st = jx_ob_from_str("true");
      else
        st = jx_ob_from_str("false");
      if(!ref) {
        return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
      }

    }
    break;
  case JX_META_BIT_INT:
    {
      char buffer[50];
#ifdef JX_64_BIT
      snprintf(buffer, sizeof(buffer), "%lld", ob.data.io.int_);
#else
      snprintf(buffer, sizeof(buffer), "%d", ob.data.io.int_);
#endif
      {
        jx_ob st = jx_ob_from_str(buffer);
        if(!ref) {
          return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
        }
      }
    }
    break;
  case JX_META_BIT_FLOAT:
    {
      char buffer[50];
#ifdef JX_64_BIT
      if(flags & JX_JXON_FLAG_APPROX_FLOAT) {
        snprintf(buffer, sizeof(buffer), 
                 "%.15g", 
                 ob.data.io.float_);
      } else {
        snprintf(buffer, sizeof(buffer), 
                 "%.17g", 
                 ob.data.io.float_);
      }
#else
      if(flags & JX_JXON_FLAG_APPROX_FLOAT) {
        snprintf(buffer, sizeof(buffer), 
                 "%.7g", 
                 ob.data.io.float_);
      } else {
        snprintf(buffer, sizeof(buffer), 
                 "%.8g", 
                 ob.data.io.float_);
      }

#endif
      force_float(buffer);
      {
        jx_ob st = jx_ob_from_str(buffer);
        if(!ref) {
          return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
        }
      }
    }
    break;
  case JX_META_BIT_STR:
    if(bits & JX_META_BIT_GC) {
      jx_int size = jx_vla_size(&ob.data.io.str) - sizeof(jx_str);
      jx_char *buffer = jx_malloc(size + 2);
      if(buffer) {
        buffer[0] = '"';
        memcpy(buffer + 1, ob.data.io.str + sizeof(jx_str), size);
        buffer[size] = '"';
        buffer[size + 1] = 0;
        {
          jx_ob st = jx_ob_from_str(buffer);
          jx_free(buffer);
          if(!ref) {
            return st;
       } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
         }
          return st;
        }
      }
    } else {
      char buffer[JX_TINY_STR_SIZE + 2];
      jx_int size = bits & JX_META_MASK_TINY_STR_SIZE;
      buffer[0] = '"';
      memcpy(buffer + 1, ob.data.io.tiny_str, size);
      buffer[size + 1] = '"';
      buffer[size + 2] = 0;
      {
        jx_ob st = jx_ob_from_str(buffer);
        if(!ref) {
          return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
        }
      }        
    }
    break;
  case JX_META_BIT_IDENT: /* Jenarix identifiers (extended JXON) */
    if(flags & JX_JXON_FLAG_JSON_LOSSY) {
      /* encode as an ordinary string */
      if(bits & JX_META_BIT_GC) {
        jx_int size = jx_vla_size(&ob.data.io.str) - sizeof(jx_str);
        jx_char *buffer = jx_malloc(size + 2);
        if(buffer) {
          buffer[0] = '"';
          memcpy(buffer + 1, ob.data.io.str + sizeof(jx_str), size);
          buffer[size] = '"';
          buffer[size + 1] = 0;
          {
            jx_ob st = jx_ob_from_str(buffer);
            jx_free(buffer);
            if(!ref) {
              return st;
            } else {
              flags = autowrap(ref,flags,space_left,jx_str_len(st));
              jx_vla_append_ob_str(ref, st);
              return jx_null_with_ob(st);
            }
          }
        }
      } else {
        char buffer[JX_TINY_STR_SIZE + 2];
        jx_int size = bits & JX_META_MASK_TINY_STR_SIZE;
        buffer[0] = '"';
        memcpy(buffer + 1, ob.data.io.tiny_str, size);
        buffer[size + 1] = '"';
        buffer[size + 2] = 0;
        {
          jx_ob st = jx_ob_from_str(buffer);
          if(!ref) {
            return st;
          } else {
            flags = autowrap(ref,flags,space_left,jx_str_len(st));
            jx_vla_append_ob_str(ref, st);
            return jx_null_with_ob(st);
          }
        }        
      }
    } else { /* not lossy */
      jx_ob st;
      if(bits & JX_META_BIT_GC) {
        st = jx_ob_from_str(ob.data.io.str + sizeof(jx_str));
      } else {
        st = jx_ob_from_str(ob.data.io.tiny_str);
      }
      if(!ref) {
        return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
      }
    }
    break;
  case JX_META_BIT_BUILTIN: 
    if(flags & JX_JXON_FLAG_JSON_LOSSY) {
      jx_ob st = jx_ob_from_str("\"builtin\"");
      if(!ref) {
        return st;
      } else {
        flags = autowrap(ref,flags,space_left,jx_str_len(st));
        jx_vla_append_ob_str(ref, st);
        return jx_null_with_ob(st);
      }
    } else { /* for debugging and troublshooting only */
      char buffer[50];
      buffer[0] = 0;
      switch(bits & JX_META_MASK_BUILTIN_TYPE) {
      case JX_META_BIT_BUILTIN_VLA:
        sprintf(buffer,"`vla:%p",(void*)ob.data.io.vla);
        break;
      case JX_META_BIT_BUILTIN_SELECTOR:
        {
          jx_ob builtins = jx_hash_new_with_flags(JX_HASH_FLAG_BIDIRECTIONAL);
        if(jx_ok(jx_code_expose_secure_builtins(builtins))) {
          jx_ob name = jx_hash_get_key(builtins,ob);
          if(jx_ident_check(name)) {
            sprintf(buffer,"`%s",jx_ob_as_ident(&name)); /* TO DO replace -- not range checked */
          }
          jx_ob_free(name);
        }
        if(!buffer[0]) {
          sprintf(buffer,"`op:%d",(int)ob.data.io.int_);
        }
        jx_ob_free(builtins);
        }
        break;
      case JX_META_BIT_BUILTIN_OPAQUE_OB:
        sprintf(buffer,"`opaque_ob:%p",(void*)ob.data.io.vla); /* deliberate misread */
        break;
      case JX_META_BIT_BUILTIN_NATIVE_FN:
        sprintf(buffer,"`native_fn:%p",(void*)ob.data.io.vla); /* deliberate misread */
        break;
      case JX_META_BIT_BUILTIN_FUNCTION:
        {
          jx_function *fn = ob.data.io.function;
          if(jx_ident_check(fn->name)) {
            sprintf(buffer,"`function:%s",jx_ob_as_ident(&fn->name)); /* deliberate misread */
          } else {
          sprintf(buffer,"`function:%p",(void*)ob.data.io.vla); /* deliberate misread */
          }
        }
        break;
      case JX_META_BIT_BUILTIN_MACRO:
        {
          jx_function *fn = ob.data.io.function;
          if(jx_ident_check(fn->name)) {
            sprintf(buffer,"`macro:%s",jx_ob_as_ident(&fn->name)); /* deliberate misread */
          } else {
            sprintf(buffer,"`macro:%p",(void*)ob.data.io.vla); /* deliberate misread */
          }
        }
        break;
      }
      {      
        jx_ob st = jx_ob_from_str(buffer);
        if(!ref) {
          return st;
        } else {
          flags = autowrap(ref,flags,space_left,jx_str_len(st));
          jx_vla_append_ob_str(ref, st);
          return jx_null_with_ob(st);
        }
      }
    }
    break;
  case JX_META_BIT_LIST:
    jx__list_to_jxon(ob.data.io.list, ref, flags, space_left);
    break;
  case JX_META_BIT_HASH:
    jx__hash_to_jxon(ob.data.io.hash, ref, flags, space_left);
    break;
  }
  return jx_ob_from_null();
}

jx_ob jx_ob_to_jxon_with_flags(jx_ob ob, jx_int flags, 
                               jx_int indent, jx_int width, jx_int space_left)
{
  switch (ob.meta.bits & JX_META_MASK_TYPE_BITS) {
  case 0:
  case JX_META_BIT_BOOL:
  case JX_META_BIT_INT:
  case JX_META_BIT_FLOAT:
  case JX_META_BIT_STR:
  case JX_META_BIT_IDENT: 
  case JX_META_BIT_BUILTIN:
    return jx__ob_to_jxon_with_flags(ob,NULL,flags,NULL); /* unformatted */
    break;
  default: /* prepare for a large response */
    {
      jx_char *vla = jx_vla_new(1,1);
      if(!width) {
        width = get_width(flags);
      }
      flags |= ((width<<8) & JX_JXON_FLAG_WIDTH_MASK);
      if(!indent) {
        indent = get_indent(flags);
      }
      flags |= (indent & JX_JXON_FLAG_INDENT_MASK);
      if(vla) {
        jx_ob_free(jx__ob_to_jxon_with_flags(ob,&vla,flags,&space_left));
        return jx_ob_with_str_vla(&vla);
      }
  }
    break;
  }
  return jx_ob_from_null();
}

void jx_jxon_dump(FILE *f, char *prefix, jx_ob ob)
{
  jx_int width = 0;
  jx_int left = width;
  if(width && prefix) {
    left -= strlen(prefix) + 2;
  }
  {
    jx_ob jxon = jx_ob_to_jxon_with_flags(ob,
                                          JX_JXON_FLAG_COMMENT | 
                                          JX_JXON_FLAG_PRETTY |  
                                          //JX_JXON_FLAG_NO_ROUNDED |
                                          JX_JXON_FLAG_APPROX_FLOAT |
                                          JX_JXON_FLAG_SHOW_WEAK | 
                                          JX_JXON_FLAG_NOT_NEWLINE,
                                          0,width,left);
    
    fprintf(f,"%s: %s\n",prefix, jx_ob_as_str(&jxon));
    jx_ob_free(jxon);
  }
}

/*
    if(ob.meta.bits & JX_META_BIT_WEAK_REF) {
      info = weak;
    }
  jx_char weak[] = "(weak ref) ",nothing[] ="";
  jx_char *info = nothing;
%s

*/

