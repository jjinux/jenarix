

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

int main(int argc, char **argv)
{
  {
    jx_ob list = jx_list_new();

    P1("0 == %d", jx_null_check(list));
    P1("0 == %d", jx_bool_check(list));
    P1("0 == %d", jx_int_check(list));
    P1("0 == %d", jx_float_check(list));
    P1("0 == %d", jx_str_check(list));
    P1("1 && %d", jx_list_check(list));
    P1("0 == %d", jx_hash_check(list));

    P1("0 == %d", (int) jx_list_size(list));

    P1("0 == %d", jx_list_append(list, jx_ob_from_int(9)));
    P1("1 == %d", (int) jx_list_size(list));
    P1("9 == %d", (int) jx_ob_as_int(jx_list_borrow(list, 0)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(8)));
    P1("2 == %d", (int) jx_list_size(list));
    P1("8 == %d", (int) jx_ob_as_int(jx_list_borrow(list, 1)));

    P1("0 == %d", jx_list_append(list, jx_ob_from_float(2.0F)));
    P1("3 == %d", (int) jx_list_size(list));
    P1("9 == %d", (int) jx_ob_as_int(jx_list_borrow(list, 0)));
    P1("8 == %d", (int) jx_ob_as_int(jx_list_borrow(list, 1)));
    P1("2.0 == %f", jx_ob_as_float(jx_list_borrow(list, 2)));

    P1("0 == %d", jx_ob_free(list));
  }
  {
    jx_int *vla = jx_vla_new(sizeof(jx_int), 4);
    jx_int i;
    for(i = 0; i < 4; i++) {
      vla[i] = i;
    }
    {
      jx_ob list = jx_list_new_with_int_vla(&vla);
      P1("4 == %d", (int) jx_list_size(list));
      P1("3 == %d", (int) jx_ob_as_int(jx_list_borrow(list, 3)));
      P1("0 == %d", jx_ob_free(list));
    }
  }
  {
    jx_float *vla = jx_vla_new(sizeof(jx_float), 4);
    jx_int i;
    for(i = 0; i < 4; i++) {
      vla[i] = i / 10.0F;
    }
    {
      jx_ob list = jx_list_new_with_float_vla(&vla);
      P1("4 == %d", (int) jx_list_size(list));
      P1("0.3 == %f", jx_ob_as_float(jx_list_borrow(list, 3)));
      P1("0 == %d", jx_ob_free(list));
    }
  }
  {
    jx_int int_array[] = { 1, 2 };
    jx_float float_array[] = { 1.0, 2.0 };

    jx_ob list1 = jx_list_new_from_int_array(int_array, 2);
    jx_ob list2 = jx_list_new_from_int_array(int_array, 2);
    jx_ob list3 = jx_list_new_from_float_array(float_array, 2);

    P1("1 && %p", (void *) jx_list_as_int_vla(list1));

    P1("0 == %d", jx_list_combine(list1, list2));

    P1("1 && %p", (void *) jx_list_as_int_vla(list1));

    P1("0 == %d", jx_list_combine(list1, list3));

    P1("0 == %p", (void *) jx_list_as_int_vla(list1));

    P1("0 == %d", jx_ob_free(list1));
  }
  {
    jx_ob list = jx_list_new();
    jx_list_append(list, jx_ob_from_str("This"));
    jx_list_append(list, jx_ob_from_str("is"));
    jx_list_append(list, jx_ob_from_str("my"));
    jx_list_append(list, jx_ob_from_str("sentence."));
    {
      jx_ob str = jx_str_join_with_list_sep(jx_ob_copy(list), jx_ob_from_str(" "));
      P1("'This is my sentence.' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }
    {
      jx_ob str = jx_str_join_with_list(list);
      P1("'Thisismysentence.' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

  }
}
