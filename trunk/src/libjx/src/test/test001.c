

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

#include "jx_public.h"

#define P1(ex,s1) printf(ex " || die('fail: %s line %d.');\n",s1,__FILE__,__LINE__);
#define P2(ex,s1,s2) printf(ex " || die('fail: %s line %d.');\n",s1,s2,__FILE__,__LINE__);

unsigned int random_int(unsigned int range)
{                               /* [0-range) */
  return (int) (((((double) random()) * range) - 1) / 4294967296.0);
}

int main(int argc, char **argv)
{

  /* various tests of the vla - variable length array container */

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);

    P1("1 && %p", (void *) vla);
    P1("0 == %d", jx_vla_insert(&vla, 0, 1));
    P1("1 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_insert(&vla, 1, 1));
    P1("0 == %d", jx_vla_insert(&vla, 0, 1));
    P1("3 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 10);
    P1("1 && %p", (void *) vla);
    P1("10 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_insert(&vla, 0, 10));
    P1("20 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_remove(&vla, 5, 10));
    P1("10 == %d", (int) jx_vla_size(&vla));
    P1("-1 == %d", jx_vla_remove(&vla, 5, 10));
    P1("10 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_append(&vla, 5));
    P1("15 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_remove(&vla, 5, 10));
    P1("5 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_remove(&vla, 0, 5));
    P1("0 == %d", (int) jx_vla_size(&vla));
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100000);
    {
      jx_int size;
      while((size = jx_vla_size(&vla))) {
        jx_vla_remove(&vla, size - 1, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);
    int i;
    for(i = 0; i < 1000000; i++) {
      if(jx_vla_grow_check(&vla, i))
        vla[i] = i;
    }
    P1("999999 == %d", (int) vla[999999]);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100000);
    {
      jx_int size;
      while((size = jx_vla_size(&vla))) {
        jx_vla_remove(&vla, 0, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100000);
    {
      jx_int size;
      while((size = jx_vla_size(&vla)) > 1) {
        jx_vla_remove(&vla, 0, 2);
        jx_vla_append(&vla, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 10000);
    {
      jx_int *start = vla;
      P1("0 == %d", (int) (vla - start));
      jx_vla_remove(&vla, 0, 5000);
      P1("5000 == %d", (int) (vla - start));
      jx_vla_insert(&vla, 0, 2500);
      P1("2500 == %d", (int) (vla - start));
      jx_vla_insert(&vla, 0, 2500);
      P1("0 == %d", (int) (vla - start));
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 100);
    int i;
    for(i = 0; i < 10000; i++) {
      if(jx_vla_grow_check(&vla, i)) {
        vla[i] = i;
        jx_vla_remove(&vla, 0, 1);
      }
    }
    P1("5000 == %d", (int) vla[0]);
    P1("0 == %d", (int) vla[1]);
    P1("5001 == %d", (int) vla[2]);
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 10000000);
    int i;
    P1("10000000 == %d", (int) jx_vla_size(&vla));
    for(i = 0; i < 10000000; i++) {
      vla[i] = i;
    }
    for(i = 0; i < 10000; i++) {
      jx_vla_remove(&vla, 0, 100);
    }
    P1("9000000 == %d", (int) jx_vla_size(&vla));
    P1("1000000 == %d", (int) vla[0]);
    P1("0 == %d", jx_vla_free(&vla));
  }

  /* make sure these actions are fast */

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);
    {
      int i;
      for(i = 0; i < 1000000; i++) {
        jx_vla_insert(&vla, 0, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 1);
    {
      int i;
      for(i = 0; i < 1000000; i++) {
        jx_vla_insert(&vla, 1, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 0);
    {
      int i;
      for(i = 0; i < 1000000; i++) {
        jx_vla_insert(&vla, i, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 1);
    {
      int i;
      for(i = 0; i < 1000000; i++) {
        jx_vla_insert(&vla, i, 1);
      }
    }
    P1("0 == %d", jx_vla_free(&vla));
  }

  /* prove that it really works */

  {
    int i;
    for(i = 2; i < 3000; i += (i / 2)) {
      int ii;
      for(ii = 0; ii < i; ii++) {
        jx_int size = random_int(i);
        jx_int *vla = jx_vla_new(sizeof(jx_int), size);
        int iii;

        for(iii = 0; iii < size; iii++) {
          {
            int x;
            for(x = 0; x < size; x++) { /* renumber array */
              vla[x] = x;
            }
          }
          {
            int index = random_int(size);
            int count = random_int(size - index);
            int x;
            /* delete a chunk of the array */
            jx_vla_remove(&vla, index, count);
            size -= count;
            /* verify array integrity */
            for(x = 0; x < index; x++) {
              if(vla[x] != x) {
                printf("die('fail1: %s line %d.');\n", __FILE__, __LINE__);
              }
            }
            for(x = index; x < (size - count); x++) {
              if(vla[x] != x + count) {
                printf("die('fail2: %s line %d.');\n", __FILE__, __LINE__);
              }
            }
          }
        }
        jx_vla_free(&vla);
      }
    }
    printf("# passed %s line %d.\n", __FILE__, __LINE__);
  }

  {
    int i;
    for(i = 2; i < 1000; i += (i / 2)) {
      int ii;
      for(ii = 0; ii < i; ii++) {
        jx_int size = random_int(i);
        jx_int *vla = jx_vla_new(sizeof(jx_int), size);
        int iii;

        for(iii = 0; iii < size; iii++) {
          {
            int x;
            for(x = 0; x < size; x++) { /* renumber array */
              vla[x] = x;
            }
          }
          {
            int index = random_int(size);
            int count = random_int(size - index);
            int x;
            /* delete a chunk of the array */
            jx_vla_remove(&vla, index, count);
            size -= count;
            /* verify array integrity */
            for(x = 0; x < index; x++) {
              if(vla[x] != x) {
                printf("die('fail1: %s line %d.');\n", __FILE__, __LINE__);
              }
            }
            for(x = index; x < (size - count); x++) {
              if(vla[x] != x + count) {
                printf("die('fail2: %s line %d.');\n", __FILE__, __LINE__);
              }
            }
          }
          {
            int x;
            for(x = 0; x < size; x++) { /* renumber array */
              vla[x] = x;
            }
          }
          {
            int index = random_int(size);
            int count = random_int(size - index);
            int x;
            /* add a chunk to the array */
            jx_vla_insert(&vla, index, count);
            size += count;
            /* verify array integrity */
            for(x = 0; x < index; x++) {
              if(vla[x] != x) {
                printf("die('fail3: %s line %d.');\n", __FILE__, __LINE__);
              }
            }
            for(x = index + count; x < size; x++) {
              if(vla[x] != x - count) {
                printf("die('fail4: %s line %d.');\n", __FILE__, __LINE__);
              }
            }
          }
        }
        jx_vla_free(&vla);
      }
    }
    printf("# passed %s line %d.\n", __FILE__, __LINE__);
  }

}
