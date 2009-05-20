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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jx_public.h"


int main(int argc, char **argv)
{
  jx_ob namespace = jx_hash_new();
  jx_ob scanner = jx_jxon_scanner_new_with_file(stdin);
  jx_ob node = jx_hash_new();
  
  jx_code_expose_special_forms(namespace);
  jx_code_expose_builtins(namespace);
  
  {
    jx_ob source = JX_OB_NULL;
    while(jx_ok(jx_jxon_scanner_next_ob(&source, scanner))) {
      jx_jxon_dump(stdout, "source", source);
      {
        jx_ob code = jx_code_bind_with_source(namespace, source);
        source = jx_ob_from_null();

        jx_jxon_dump(stdout, "code", code);
        
        {
          jx_ob result = jx_code_exec(node,code);
          
          jx_jxon_dump(stdout, "result", result);
          
          jx_ob_free(result);
        }
        jx_ob_free(code);
      }
    }
  }
  jx_ob_free(node);
  jx_ob_free(scanner);
  jx_ob_free(namespace);
}

