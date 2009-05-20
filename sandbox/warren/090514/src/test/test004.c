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
    jx_ob ob_null = jx_ob_from_null();
    jx_ob ob_true = jx_ob_from_bool(JX_TRUE);
    jx_ob ob_false = jx_ob_from_bool(JX_FALSE);
    jx_ob ob_7 = jx_ob_from_int(7);
    jx_ob ob_3pt0 = jx_ob_from_float(3.0F);
    jx_ob ob_tiny = jx_ob_from_str("tny");
    jx_ob ob_huge = jx_ob_from_str("this is a huge heap string.");

    P1("1 == %d", jx_null_check(ob_null));
    P1("1 == %d", jx_ob_as_bool(ob_true));
    P1("0 == %d", jx_ob_as_bool(ob_false));
    P1("7 == %d", (int)jx_ob_as_int(ob_7));
    P1("3.0 == %f", jx_ob_as_float(ob_3pt0));
    P1("'tny' eq '%s'", jx_ob_as_str(&ob_tiny));
    P1("'this is a huge heap string.' eq '%s'", jx_ob_as_str(&ob_huge));

    P1("0 == %d", jx_ob_free(ob_null));
    P1("0 == %d", jx_ob_free(ob_true));
    P1("0 == %d", jx_ob_free(ob_false));
    P1("0 == %d", jx_ob_free(ob_7));
    P1("0 == %d", jx_ob_free(ob_3pt0));
    P1("0 == %d", jx_ob_free(ob_tiny));
    P1("0 == %d", jx_ob_free(ob_huge));
  }

  {
    jx_ob ob_null = jx_ob_from_null();
    jx_ob ob_true = jx_ob_from_bool(JX_TRUE);
    jx_ob ob_false = jx_ob_from_bool(JX_FALSE);
    jx_ob ob_7 = jx_ob_from_int(7);
    jx_ob ob_3pt0 = jx_ob_from_float(3.0F);
    jx_ob ob_tiny = jx_ob_from_str("tny");
    jx_ob ob_huge = jx_ob_from_str("this is a huge heap string.");

    jx_ob jxon = jx_ob_to_jxon(ob_null);
    P1("'null' eq '%s'", jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    jxon = jx_ob_to_jxon(ob_true);
    P1("'true' eq '%s'", jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    jxon = jx_ob_to_jxon(ob_false);
    P1("'false' eq '%s'", jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    jxon = jx_ob_to_jxon(ob_7);
    P1("'7' eq '%s'", jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    jxon = jx_ob_to_jxon(ob_3pt0);
    P1("'3.0' eq '%s'", jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    jxon = jx_ob_to_jxon(ob_tiny);
    P2("'\"%s\"' eq '%s'", jx_ob_as_str(&ob_tiny), jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    jxon = jx_ob_to_jxon(ob_huge);
    P2("'\"%s\"' eq '%s'", jx_ob_as_str(&ob_huge), jx_ob_as_str(&jxon));
    jx_ob_free(jxon);

    P1("0 == %d", jx_ob_free(ob_null));
    P1("0 == %d", jx_ob_free(ob_true));
    P1("0 == %d", jx_ob_free(ob_false));
    P1("0 == %d", jx_ob_free(ob_7));
    P1("0 == %d", jx_ob_free(ob_3pt0));
    P1("0 == %d", jx_ob_free(ob_tiny));
    P1("0 == %d", jx_ob_free(ob_huge));
  }

  {
    jx_ob ob_null = jx_ob_from_null();
    jx_ob ob_true = jx_ob_from_bool(JX_TRUE);
    jx_ob ob_false = jx_ob_from_bool(JX_FALSE);
    jx_ob ob_7 = jx_ob_from_int(7);
    jx_ob ob_3pt0 = jx_ob_from_float(3.0F);
    jx_ob ob_tiny = jx_ob_from_str("tny");
    jx_ob ob_huge = jx_ob_from_str("this is a huge heap string.");
    jx_ob list = jx_list_new();

    P1("0 == %d", (int)jx_list_size(list));
    P1("0 == %d", jx_list_append(list, ob_null));
    P1("1 == %d", (int)jx_list_size(list));
    P1("0 == %d", jx_list_append(list, ob_true));
    P1("2 == %d", (int)jx_list_size(list));
    P1("0 == %d", jx_list_append(list, ob_false));
    P1("3 == %d", (int)jx_list_size(list));

    P1("0 == %d", jx_list_append(list, ob_7));
    P1("0 == %d", jx_list_append(list, ob_3pt0));

    P1("0 == %d", jx_list_append(list, ob_tiny));

    P1("0 == %d", jx_list_append(list, ob_huge));

    P1("7 == %d", (int)jx_list_size(list));
    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_ob list = jx_list_new();

    P1("0 == %d", jx_list_append(list, jx_ob_from_null()));
    P1("0 == %d", jx_list_append(list, jx_ob_from_bool(JX_TRUE)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(7)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_float(3.0F)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_str("tny")));
    P1("0 == %d", jx_list_append(list, jx_ob_from_str("this is a huge heap string.")));
    P1("7 == %d", (int)jx_list_size(list));

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      printf("# %s\n", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_ob list = jx_list_new();

    P1("0 == %d", jx_list_insert(list, 0, jx_ob_from_int(1)));
    P1("0 == %d", jx_list_insert(list, 0, jx_ob_from_int(2)));
    P1("0 == %d", jx_list_insert(list, 1, jx_ob_from_int(3)));
    P1("0 == %d", jx_list_insert(list, 3, jx_ob_from_int(4)));
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[2,3,1,4]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_ob list = jx_list_new();

    P1("0 == %d", jx_list_append(list, jx_ob_from_int(1)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(2)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(3)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(4)));
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,2,3,4]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_ob list2 = jx_list_new();
      P1("0 == %d", jx_list_append(list2, jx_ob_from_int(5)));
      P1("0 == %d", jx_list_append(list2, jx_ob_from_int(6)));
      P1("0 == %d", jx_list_append(list2, jx_ob_from_int(7)));
      P1("0 == %d", jx_list_append(list2, jx_ob_from_int(8)));
      jx_list_append(list, list2);

      {
        jx_ob jxon = jx_ob_to_jxon(list);
        P1("'[1,2,3,4,[5,6,7,8]]' eq '%s'", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
    }

    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_ob list = jx_list_new();

    P1("1 && %p", (void *) jx_list_as_int_vla(list));

    P1("1 && %p", (void *) jx_list_as_float_vla(list));

    P1("0 == %d", jx_list_append(list, jx_ob_from_int(1)));
    P1("1 && %p", (void *) jx_list_as_int_vla(list));
    P1("0 == %p", (void *) jx_list_as_float_vla(list));

    P1("0 == %d", jx_list_append(list, jx_ob_from_float(3.0)));
    P1("0 == %p", (void *) jx_list_as_int_vla(list));
    P1("0 == %p", (void *) jx_list_as_float_vla(list));

    P1("0 == %d", jx_list_append(list, jx_ob_from_str("yet another heap string")));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(4)));
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      printf("# %s\n", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    P1("0 == %d", jx_list_delete(list, 1))
      P1("0 == %p", (void *) jx_list_as_float_vla(list));
    P1("0 == %p", (void *) jx_list_as_int_vla(list));

    P1("0 == %d", jx_list_delete(list, 1))
      P1("0 == %p", (void *) jx_list_as_float_vla(list));
    P1("1 && %p", (void *) jx_list_as_int_vla(list));

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,4]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_int *vla = jx_list_as_int_vla(list);
      vla[0] = 10;
      vla[1] = 6;
    }

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[10,6]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_int *vla = jx_list_as_int_vla(list);
      jx_int *new_vla = jx_int_vla_new(3);
      jx_int_vla_free(&vla);
      new_vla[1] = 3;
      jx_list_set_int_vla(list, &new_vla);
    }

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[0,3,0]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_float *vla = jx_float_vla_new(2);
      P1("-1 == %d", jx_list_set_float_vla(list, &vla));
      jx_float_vla_free(&vla);
    }

    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_int array[] = { 3, 2, 1 };
    jx_ob list = jx_list_new_from_int_array(array, 3);

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[3,2,1]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_float array[] = { 1.0F, 2.0F, 3.0F };
    jx_ob list = jx_list_new_from_float_array(array, 3);

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      printf("# %s\n", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_ob list_copy = jx_ob_copy(list);
      P1("3 == %d", (int)jx_list_size(list_copy));
      {
        jx_ob jxon = jx_ob_to_jxon(list_copy);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      P1("0 == %d", jx_ob_free(list_copy));
    }
    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_ob list = jx_list_new();

    P1("0 == %d", jx_list_append(list, jx_ob_from_int(1)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(2)));
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,2]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_ob list2 = jx_list_new();
      P1("0 == %d", jx_list_append(list2, jx_ob_from_str("heap string number one")));
      P1("0 == %d", jx_list_append(list2, jx_ob_from_str("heap string number two")));
      jx_list_append(list, list2);
    }

    {
      jx_ob list_copy = jx_ob_copy(list);
      P1("3 == %d", (int)jx_list_size(list_copy));
      {
        jx_ob jxon = jx_ob_to_jxon(list_copy);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      P1("0 == %d", jx_ob_free(list_copy));
    }

    P1("0 == %d", jx_ob_free(list));
  }

  {
    jx_ob list = jx_list_new();

    P1("0 == %d", jx_list_append(list, jx_ob_from_int(1)));
    P1("0 == %d", jx_list_append(list, jx_ob_from_int(2)));

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,2]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    P1("0 == %d", jx_list_replace(list, 0, jx_ob_from_str("heap string number one")));

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[\"heap string number one\",2]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    P1("0 == %d", jx_list_replace(list, 0, jx_ob_from_str("heap string number two")));

    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[\"heap string number two\",2]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    P1("0 == %d", jx_ob_free(list));
  }

}
