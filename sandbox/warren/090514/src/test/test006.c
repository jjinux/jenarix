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

#include "jx_public.h"

#define C1(ex,s1) printf("# %s (%s line %d,);\n",s1,__FILE__,__LINE__);
#define P1(ex,s1) printf(ex " || die('fail: %s line %d.');\n",s1,__FILE__,__LINE__);
#define P2(ex,s1,s2) printf(ex " || die('fail: %s line %d.');\n",s1,s2,__FILE__,__LINE__);

#define N_CYC 100
#define N_KEY 10000

int main(int argc, char **argv)
{
  jx_ob key[N_KEY];
  {
    int i;
    for(i = 0; i < N_KEY; i++) {
      jx_char buffer[50];
      sprintf(buffer, "%07d", i);       /* too large to fit in tiny string of size 6 */
      key[i] = jx_ob_from_str(buffer);
    }
  }
  {
    jx_int c;
    for(c = 0; c < N_CYC; c++) {
      jx_ob hash = jx_hash_new();
      int i;
      for(i = 0; i < N_KEY; i++) {
        jx_hash_set(hash, jx_ob_copy(key[i]), jx_ob_from_int(i));
      }
      for(i = 0; i < N_KEY; i++) {
        int test = jx_ob_as_int(jx_hash_borrow(hash, key[i]));
        if(test != i) {
          P2("%d == %d", test, i);
        }
      }
      jx_ob_free(hash);
    }
  }
  {
    jx_int i;
    for(i = 0; i < N_KEY; i++) {
      jx_ob_free(key[i]);
    }
  }
}
