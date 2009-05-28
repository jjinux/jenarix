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

jx_ob jx_py_translate_with_tree(jx_ob tree)
{
  return tree;
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
        jx_ob jxon = jx_ob_to_jxon(ob);
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

static jx_bool jx_declare(jx_bool ok, jx_ob namespace, jx_char * ident, jx_native_fn fn)
{
  if(ok)
    ok = jx_ok(jx_hash_set(namespace, jx_ob_from_ident(ident),
                           jx_builtin_new_from_native_fn(fn)));
  return ok;
}

jx_status jx_py_expose_python_builtins(jx_ob namespace)
{

  jx_bool ok = JX_TRUE;

  ok = jx_declare(ok, namespace, "print", jx_py_print);

  return ok ? JX_SUCCESS : JX_FAILURE;
}
