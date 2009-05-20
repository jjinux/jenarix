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

int main(int argc, char **argv)
{
  {
    /* check hashing */

    jx_ob ob_null = jx_ob_from_null();
    jx_ob ob_true = jx_ob_from_bool(JX_TRUE);
    jx_ob ob_false = jx_ob_from_bool(JX_FALSE);
    jx_ob ob_0 = jx_ob_from_int(0);
    jx_ob ob_1 = jx_ob_from_int(1);
    jx_ob ob_2 = jx_ob_from_int(2);
    jx_ob ob_3pt0 = jx_ob_from_float(3.0F);
    jx_ob ob_tiny = jx_ob_from_str("tny");
    jx_ob ob_huge1 = jx_ob_from_str("this is a huge heap string.");
    jx_ob ob_huge2 = jx_ob_from_str("this is a huge heap string.");
    jx_ob ob_huge3 = jx_ob_from_str("this is b huge heap string.");

    P1("0x4636b9c9 == 0x%08x", jx_ob_hash_code(ob_null));
    P1("0xe63ec46d == 0x%08x", jx_ob_hash_code(ob_true));
    P1("0xc877f3b2 == 0x%08x", jx_ob_hash_code(ob_false));

    P1("0x87179af5 == 0x%08x", jx_ob_hash_code(ob_0));
    P1("0xa13c598b == 0x%08x", jx_ob_hash_code(ob_1));
    P1("0x4eea7ab3 == 0x%08x", jx_ob_hash_code(ob_2));

    P2("(0x387778c7 == 0x%08x) || (0x9e069f3e == 0x%08x)",
       jx_ob_hash_code(ob_3pt0), jx_ob_hash_code(ob_3pt0));

    P2("(0xced8ea80 == 0x%08x) || (0xf70711c3 == 0x%08x)",
       jx_ob_hash_code(ob_tiny),jx_ob_hash_code(ob_tiny));

    P1("0x09774a5a == 0x%08x", jx_ob_hash_code(ob_huge1));
    P1("0x09774a5a == 0x%08x", jx_ob_hash_code(ob_huge2));
    P1("0xba51b099 == 0x%08x", jx_ob_hash_code(ob_huge3));

    P1("0 == %d", jx_ob_free(ob_huge1));
    P1("0 == %d", jx_ob_free(ob_huge2));
    P1("0 == %d", jx_ob_free(ob_huge3));
  }
  {
    /* confirm that we don't hash lists hashes */
    jx_ob list = jx_list_new();
    P1("0x00000000 == 0x%08x", jx_ob_hash_code(list));
    P1("0 == %d", jx_ob_free(list));
  }
  {
    /* confirm that we don't hash hashes */
    jx_ob hash = jx_hash_new();
    P1("0x00000000 == 0x%08x", jx_ob_hash_code(hash));
    P1("0 == %d", jx_ob_free(hash));
  }
  {
    jx_ob hash = jx_hash_new();

    P1("0 == %d", jx_null_check(hash));
    P1("0 == %d", jx_bool_check(hash));
    P1("0 == %d", jx_int_check(hash));
    P1("0 == %d", jx_float_check(hash));
    P1("0 == %d", jx_str_check(hash));
    P1("0 == %d", jx_list_check(hash));
    P1("1 == %d", jx_hash_check(hash));

    P1("0 == %d", (int)jx_hash_size(hash));

    P1("0 == %d", jx_hash_has_key(hash, jx_ob_from_int(1)));
    P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(1), jx_ob_from_int(2)));
    P1("1 == %d", (int)jx_hash_size(hash));
    P1("1 == %d", jx_hash_has_key(hash, jx_ob_from_int(1)));

    P1("0 == %d", jx_hash_set(hash, jx_ob_from_str("ky1"), jx_ob_from_str("vl1")));
    P1("2 == %d", (int)jx_hash_size(hash));
    P1("1 == %d", jx_hash_has_key(hash, jx_ob_from_str("ky1")));

    {
      jx_ob borrowed = jx_hash_borrow(hash, jx_ob_from_str("ky1"));
      P1("'vl1' eq '%s'", jx_ob_as_str(&borrowed));
    }

    {
      jx_ob key1 = jx_ob_from_str("heap string as a hash key");
      jx_ob key2 = jx_ob_from_str("heap string as a hash key");
      jx_ob value1 = jx_ob_from_str("heap string as a hash value");

      P1("0 == %d", jx_hash_set(hash, key1, value1));
      P1("1 == %d", jx_hash_has_key(hash, key1));
      P1("1 == %d", jx_hash_has_key(hash, key2));
      P1("3 == %d", (int)jx_hash_size(
hash));

      {
        jx_ob borrowed = jx_hash_borrow(hash, key2);
        P1("'heap string as a hash value' eq '%s'", jx_ob_as_str(&borrowed));
      }

      jx_ob_free(key2);
    }

    P1("0 == %d", jx_ob_free(hash));
  }

  {
    jx_ob hash = jx_hash_new();
    jx_ob key1 = jx_ob_from_str("heap string as hash key");

    P1("1 == %d", jx_null_check(jx_hash_remove(hash, jx_ob_from_null())));
    P1("1 == %d", jx_null_check(jx_hash_remove(hash, key1)));

    {
      jx_ob key2 = jx_ob_from_str("heap string as hash key");
      jx_ob value = jx_ob_from_str("heap string as a hash value");
      P1("0 == %d", jx_hash_set(hash, key2, value));
    }

    {
      jx_ob copied = jx_hash_get(hash, key1);
      P1("'heap string as a hash value' eq '%s'", jx_ob_as_str(&copied));
      jx_ob_free(copied);
    }

    P1("1 == %d", jx_hash_has_key(hash, key1));
    P1("0 == %d", jx_hash_delete(hash, key1));
    P1("0 == %d", jx_hash_has_key(hash, key1));

    P1("0 == %d", jx_ob_free(key1));
    P1("0 == %d", jx_ob_free(hash));
  }
  {
    jx_ob hash = jx_hash_new();
    jx_int i;
    for(i = 0; i < 48; i++) {
      P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(i), jx_ob_from_int(-i)));
      {
        jx_ob list = jx_hash_keys(hash);
        jx_ob jxon = jx_ob_to_jxon(list);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
        jx_ob_free(list);
      }
    }

    for(i = 0; i < 48; i++) {
      P1("0 == %d", jx_hash_delete(hash, jx_ob_from_int(i)));
      {
        jx_ob list = jx_hash_keys(hash);
        jx_ob jxon = jx_ob_to_jxon(list);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
        jx_ob_free(list);
      }
    }

    for(i = 0; i < 8; i++) {
      P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(i), jx_ob_from_int(-i)));
    }

    {
      jx_ob list = jx_hash_keys(hash);
      jx_ob jxon = jx_ob_to_jxon(list);
      printf("# %s\n", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
      jx_ob_free(list);
    }

    {
      jx_ob list = jx_hash_values(hash);
      jx_ob jxon = jx_ob_to_jxon(list);
      printf("# %s\n", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
      jx_ob_free(list);
    }

    {
      jx_ob list = jx_list_new_from_hash(hash);
      {
        jx_ob jxon = jx_ob_to_jxon(list);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      {
        jx_ob hash2 = jx_hash_new_from_list(list);
        jx_ob jxon1 = jx_ob_to_jxon(hash);
        jx_ob jxon2 = jx_ob_to_jxon(hash2);
        P2("'%s' eq '%s'", jx_ob_as_str(&jxon1), jx_ob_as_str(&jxon2));
        P1("0 == %d", jx_ob_free(jxon1));
        P1("0 == %d", jx_ob_free(jxon2));
        P1("0 == %d", jx_ob_free(hash2));
      }
      jx_ob_free(list);
    }

    P1("0 == %d", jx_ob_free(hash));
  }
  {
    jx_ob hash = jx_hash_new();
    jx_int i;
    for(i = 0; i < 18; i++) {
      jx_hash_set(hash, jx_ob_from_int(i), jx_ob_from_int(-i));
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
    }
    P1("0 == %d", jx_ob_free(hash));
  }

  {
    jx_ob hash = jx_hash_new();
    jx_int i;
    for(i = 0; i < 16; i++) {
      P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(i), jx_ob_from_int(-i)));
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
    }
    for(i = 0; i < 16; i++) {   /* confirm ability to do reverse mapping */
      jx_ob borrowed = jx_hash_borrow_key(hash, jx_ob_from_int(-i));
      P2("%d == %d", (int) i, (int) jx_ob_as_int(borrowed));
    }

    for(i = 0; i < 16; i++) {   /* confirm ability to set duplicate values */
      P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(-i), jx_ob_from_int(-i)));
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
    }
    for(i = 0; i < 16; i++) {   /* confirm randomness of reverse mapping */
      P2("%d == abs(%d)", (int) i,
         (int) jx_ob_as_int(jx_hash_borrow_key(hash, jx_ob_from_int(-i))));
    }
    P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(13), jx_ob_from_int(-13)));
    P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(-1), jx_ob_from_int(1)));
    P1("0 == %d", jx_ob_free(hash));
  }

  {
    jx_ob hash = jx_hash_new_with_flags(JX_HASH_FLAG_BIDIRECTIONAL);
    jx_int i;
    for(i = 0; i < 12; i++) {
      P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(i), jx_ob_from_int(-i)));
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        printf("# %s\n", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
    }
    for(i = 0; i < 12; i++) {   /* confirm inability to set duplicate values */
      P1("-1 == %d", jx_hash_set(hash, jx_ob_from_int(-i), jx_ob_from_int(-i)));
    }
    for(i = 0; i < 12; i++) {   /* confirm ability to do reverse mapping */
      P2("%d == %d", (int) i,
         (int) jx_ob_as_int(jx_hash_borrow_key(hash, jx_ob_from_int(-i))));
    }
    P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(13), jx_ob_from_int(-13)));
    P1("0 == %d", jx_hash_set(hash, jx_ob_from_int(-1), jx_ob_from_int(1)));
    P1("0 == %d", jx_ob_free(hash));
  }
  {
    jx_int array[] = { 1, -1, 3, -3 };
    jx_ob list = jx_list_new_from_int_array(array, 4);
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,-1,3,-3]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    {
      jx_ob hash = jx_hash_new_with_list(list);
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        P1("'{1:-1,3:-3}' eq '%s'", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      {
        jx_ob list2 = jx_list_new_with_hash(hash);
        {
          jx_ob jxon = jx_ob_to_jxon(list2);
          P1("'[1,-1,3,-3]' eq '%s'", jx_ob_as_str(&jxon));
          jx_ob_free(jxon);
        }
        P1("0 == %d", jx_ob_free(list2));
      }
    }
  }
  {
    jx_int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    jx_ob list = jx_list_new_from_int_array(array, 10);
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,2,3,4,5,6,7,8,9,10]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    {
      jx_ob hash = jx_hash_new_with_list(list);
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        P1("'{1:2,3:4,5:6,7:8,9:10}' eq '%s'", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      {
        jx_ob list2 = jx_list_new_with_hash(hash);
        {
          jx_ob jxon = jx_ob_to_jxon(list2);
          P1("'[1,2,3,4,5,6,7,8,9,10]' eq '%s'", jx_ob_as_str(&jxon));
          jx_ob_free(jxon);
        }
        P1("0 == %d", jx_ob_free(list2));
      }
    }
  }
  {
    jx_int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
    jx_ob list = jx_list_new_from_int_array(array, 18);
    {
      jx_ob jxon = jx_ob_to_jxon(list);
      P1("'[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18]' eq '%s'", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    {
      jx_ob hash = jx_hash_new_with_list(list);
      {
        jx_ob jxon = jx_ob_to_jxon(hash);
        C1("%s", jx_ob_as_str(&jxon));
        jx_ob_free(jxon);
      }
      {
        jx_ob list2 = jx_list_new_with_hash(hash);
        {
          jx_ob jxon = jx_ob_to_jxon(list2);
          P1("'[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18]' eq '%s'",
             jx_ob_as_str(&jxon));
          jx_ob_free(jxon);
        }
        P1("0 == %d", jx_ob_free(list2));
      }
    }
  }

  {
    jx_ob hash = jx_hash_new();
    
    jx_hash_set(hash,jx_ob_from_null(),jx_ob_from_int(0));
    jx_hash_set(hash,jx_ob_from_bool(1),jx_ob_from_int(1));
    jx_hash_set(hash,jx_ob_from_bool(0),jx_ob_from_int(2));
    jx_hash_set(hash,jx_ob_from_int(1),jx_ob_from_int(3));
    jx_hash_set(hash,jx_ob_from_float(2.0),jx_ob_from_int(4));
    jx_hash_set(hash,jx_ob_from_str("str"),jx_ob_from_int(5));
    jx_hash_set(hash,jx_ob_from_ident("ident1"),jx_ob_from_ident("ident2"));
    jx_hash_set(hash,jx_builtin_new(NULL),jx_builtin_new_from_selector(1));


    {
      jx_ob jxon = jx_ob_to_jxon(hash);
      C1("%s", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    {
      jx_ob jxon = jx_ob_to_jxon_with_flags(hash,JX_JXON_FLAG_LOSSY_JSON);
      C1("%s", jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    jx_ob_free(hash);
  }
}
