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

int main(int argc, char **argv)
{

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100000);
    {
      jx_int size;
      while( (size = jx_vla_size(&vla)) ) {
        jx_vla_remove(&vla,size-1,1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100000);
    {
      jx_int size;
      while( (size = jx_vla_size(&vla)) ) {
        jx_vla_remove(&vla,0,1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100000);
    {
      jx_int size;
      while( (size = jx_vla_size(&vla))>1 ) {
        jx_vla_remove(&vla,0,2);
        jx_vla_append(&vla,1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100);
    int i;
    for(i=0;i<10000;i++) {
      if(jx_vla_grow_check(&vla,i)) {
        vla[i] = i;
        jx_vla_remove(&vla,0,1);
      }
    }
    P1("5000 == %d", (int)vla[0]);
    P1("0 == %d", (int)vla[1]);
    P1("5001 == %d", (int)vla[2]);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);
    int i;
    for(i=0;i<1000000;i++) {
      if(jx_vla_grow_check(&vla,i)) 
        vla[i] = i;
    }
    P1("999999 == %d", (int)vla[999999]);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 10000000);
    int i;
    P1("10000000 == %d", (int)jx_vla_size(&vla));
    for(i=0;i<10000000;i++) {
      vla[i] = i;
    }
    for(i=0;i<10000;i++) {
      jx_vla_remove(&vla, 0, 100);
    }
    P1("9000000 == %d", (int)jx_vla_size(&vla));
    P1("1000000 == %d", (int)vla[0]);
     P1("0 == %d", jx_vla_free(&vla));
  }


  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);

    P1("1 && %p", (void *) vla);
    P1("0 == %d", jx_vla_insert(&vla, 0, 1));
    P1("1 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_insert(&vla, 1, 1));
    P1("0 == %d", jx_vla_insert(&vla, 0, 1));
    P1("3 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 10);
    P1("1 && %p", (void *) vla);
    P1("10 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_insert(&vla, 0, 10));
    P1("20 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_remove(&vla, 5, 10));
    P1("10 == %d", (int)jx_vla_size(&vla));
    P1("-1 == %d", jx_vla_remove(&vla, 5, 10));
    P1("10 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_append(&vla, 5));
    P1("15 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_remove(&vla, 5, 10));
    P1("5 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_remove(&vla, 0, 5));
    P1("0 == %d", (int)jx_vla_size(&vla));
    P1("0 == %d", jx_vla_free(&vla));
  }

}
