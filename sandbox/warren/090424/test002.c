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

#define P1(ex,s1) printf(ex " || die('fail: %s line %d.');\n",s1,__FILE__,__LINE__);
#define P2(ex,s1,s2) printf(ex " || die('fail: %s line %d.');\n",s1,s2,__FILE__,__LINE__);

static void dump_hex(jx_char *message, void *ptr, jx_int size)
{
  unsigned char *ch = (unsigned char*)ptr;
  printf("%s: ",message);
  while(size--) {
    printf("%02x",*(ch++));
    if(size) printf(" ");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
#if JX_TINY_STR_SIZE == 4
#define TINY_TEST "123"
#else
#if JX_TINY_STR_SIZE == 8
#define TINY_TEST "1234567"
#else
#if JX_TINY_STR_SIZE == 16
#define TINY_TEST "123456789012345"
#endif
#endif
#endif
  jx_ob tiny = jx_ob_from_str(TINY_TEST);
  jx_ob heap = jx_ob_from_str("heap_string_for_sure");

  P2("'%s' eq '%s'",TINY_TEST,jx_ob_as_str(&tiny));
  P1("'heap_string_for_sure' eq '%s'",jx_ob_as_str(&heap));

  dump_hex("# tiny", &tiny, sizeof(jx_ob));
  dump_hex("# heap", &heap, sizeof(jx_ob));

  tiny = jx_ob_from_int(1);
  dump_hex("# one", &tiny,sizeof(jx_ob));  

  P1("0 == %d", jx_ob_free(tiny));
  P1("0 == %d", jx_ob_free(heap));
}

