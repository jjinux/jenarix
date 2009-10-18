

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
#include "jx_safe.h"

#define N_THREAD 5


/* list container performance comparison versus test/loop01.jx

How much overhead do symbols, the VM, and namespace impose?

JXON version:

(set a 2000000);
(set b []);
(while a (append b (decr a)) );
(output (get b 0));
(output (size b));

G5-32 bit: -O3

  2M cycles in VM = 2.3 sec

  20M cycles in C = 1.8 sec

  The C version is 13x faster 

Core 2 Duo-32 bit:

  2M cycles in VM = 1.5 sec

  20M cycles in C = 1.1 sec

  The C version is 13x faster

*/

jx_status run_test(void)
{
  jx_ob list = jx_list_new();
  jx_ob counter = jx_ob_from_int(20000000);
  jx_ob one = jx_ob_from_int(1);
  while(jx_ob_as_int(counter)) {
    jx_list_append(list, jx_ob_copy(counter));
    jx_ob_replace(&counter, jx_ob_sub(counter, one));
  }
  {
    jx_ob tmp = jx_ob_to_str(jx_list_get(list, 0));
    printf("%s\n", jx_ob_as_str(&tmp));
    jx_ob_free(tmp);
  }
  printf("%d\n", jx_list_size(list));
  jx_ob_free(list);
  return JX_SUCCESS;
}

int main(int argc, char **argv)
{
  if(jx_ok(jx_os_process_init(argc, argv))) {

    jx_status status = run_test();

    status = jx_os_process_complete();
    return JX_OK(status) ? JX_EXIT_SUCCESS : JX_EXIT_FAILURE;
  }
  return JX_EXIT_FAILURE;
}
