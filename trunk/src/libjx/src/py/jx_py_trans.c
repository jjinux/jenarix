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

#include "jx_private.h"
#include "jx_heap.h"

jx_ob jx_py_translate_with_tree(jx_ob source)
{
  /* transform the AST to map recognized Python constructs into JXON
     input source */
  switch (JX_META_MASK_TYPE_BITS & source.meta.bits) {
  case JX_META_BIT_LIST:
    if(source.data.io.list->packed_meta_bits) { /* packed lists cannot contain code */
      return source;
    } else {
      jx_ob ident = jx_list_borrow(source, 0);
      jx_ob len_ident = jx_ob_from_ident("len");
      if(jx_ob_identical(ident,len_ident)) {
          jx_list_replace(source,0,jx_ob_from_ident("size"));
      }
      jx_ob_free(len_ident);
      {
        jx_int size = jx_list_size(source);
        while(size--) {
          jx_ob entry = jx_list_swap_with_null(source,size);
          entry = jx_py_translate_with_tree(entry);
          jx_list_replace(source, size, entry);
        }
      }
    }
    return source;
    break;
  case JX_META_BIT_HASH:
    { /* TO DO refactor to avoid rehashing */
      jx_ob result = jx_hash_new();
      jx_ob list = jx_list_new_with_hash(source);       /* destroys source */
      jx_int size = jx_list_size(list);
      while(size) {
        jx_ob key = jx_list_remove(list, 0);
        jx_ob value = jx_list_remove(list, 0);
        size = size - 2;
        jx_hash_set(result, key, jx_py_translate_with_tree(value));
      }
      jx_ob_free(list);
      return result;
    }
    break;
  case JX_META_BIT_IDENT:
    {
      jx_ob null_ident = jx_ob_from_ident("null");
      jx_ob true_ident = jx_ob_from_ident("true");
      jx_ob false_ident = jx_ob_from_ident("false");
      
      if(jx_ob_identical(source,null_ident)) {
        jx_ob_replace(&source,jx_ob_from_null());
      }
      if(jx_ob_identical(source,true_ident)) {
        jx_ob_replace(&source,jx_ob_from_bool(1));
      }
      if(jx_ob_identical(source,false_ident)) {
        jx_ob_replace(&source,jx_ob_from_bool(0));
      }
      jx_ob_free(null_ident);
      jx_ob_free(true_ident);
      jx_ob_free(false_ident);
    }
    return source;
    break;
  case JX_META_BIT_BOOL:
  case JX_META_BIT_FLOAT:
  case JX_META_BIT_INT:
  case JX_META_BIT_STR:
    return source;
    break;
  default:                     
    jx_ob_free(source);
    return jx_ob_from_null();
    break;
  }
}

jx_ob jx_py_print(jx_ob node, jx_ob payload)
{
  jx_int i,size = jx_list_size(payload);
  for(i=0;i<size;i++) {
    jx_ob ob = jx_list_borrow(payload,i);
    switch(ob.meta.bits & JX_META_MASK_TYPE_BITS) {
    case JX_META_BIT_STR:
      fprintf(stdout,"%s",jx_ob_as_str(&ob));
      break;
    default:
      {    
        jx_ob jxon = jx_ob_to_jxon_in_node(node, ob);
        fprintf(stdout,"%s",jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      break;
    }
    if((i+1)<size)
      fprintf(stdout," ");
  }
  fprintf(stdout,"\n");
  jx_ob_free(payload);
  return jx_ob_from_null();
}

static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char * ident, jx_native_fn fn)
{
  if(ok)
    ok = jx_ok(jx_hash_set(names, jx_ob_from_ident(ident),
                           jx_builtin_new_from_native_fn(fn)));
  return ok;
}

jx_status jx_py_expose_python_builtins(jx_ob names)
{

  jx_bool ok = JX_TRUE;

  ok = jx_declare(ok, names, "print", jx_py_print);

  {
    jx_ob has = jx_ob_from_ident("has");
    jx_hash_set(names,jx_ob_from_ident("has_key"),
                jx_hash_get(names,has));
    jx_ob_free(has);
  }
  
  {
    jx_ob range = jx_ob_from_ident("range");
    
    jx_hash_set(names,jx_ob_from_ident("xrange"),
                jx_hash_get(names,range));
    jx_ob_free(range);
  }

  jx_hash_set(names,jx_ob_from_ident("None"),jx_ob_from_null());


  return ok ? JX_SUCCESS : JX_FAILURE;
}
