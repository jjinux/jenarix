

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
(set b {});
(while a (set b (decr a) 1) );
(size b);

G5-32 bit: -O3

  2M cycles in VM = 3.4 sec

  20M cycles in C = 12.1 sec

  The C version is less than ~3x faster 

Core 2 Duo-32 bit:

  2M cycles in VM = 2.0 sec (versus 1.7 for Python)

  20M cycles in C = 6.8 sec

  The C version is ONLY 3x faster

  Why is that?  Is our hash table a dog?  

  Profiling with Shark reveals that a full 40% of wall time is
  consumed with the hash table's hashcode-based random-access memory
  fetch operation.  Wow.  A memory-local hashing function might speed
  things up by a factor of 1/0.4 = 2.5!

  For comparison, a simple C program using our hashing function
  requires 17 seconds to perform 200M random-access memory read/write
  combos over a GB of RAM.  Thus, our hash table is only 4X slower
  than the theoretical random-access max, and VM + hash table is slowing
  things down by a factor of 12x.

*/

jx_status run_test(void)
{
  jx_ob hash = jx_hash_new();
  jx_ob counter = jx_ob_from_int(20000000);
  jx_ob one = jx_ob_from_int(1);
  while(jx_ob_as_int(counter)) {
    jx_hash_set(hash, jx_ob_copy(counter), jx_ob_copy(one));
    jx_ob_replace(&counter, jx_ob_sub(counter, one));
  }
  printf("%d\n", jx_hash_size(hash));
  jx_ob_free(hash);
  return JX_SUCCESS;
}

int main(int argc, char **argv)
{
  if(jx_ok(jx_os_process_init(argc, argv))) {

    jx_status status = run_test();

    status = jx_os_process_complete();
    return JX_OK(status) ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  return EXIT_FAILURE;
}
