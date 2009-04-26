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

    P1("0x4636b9c9 == 0x%08x",jx_ob_hash_code(ob_null));
    P1("0xa89b411a == 0x%08x",jx_ob_hash_code(ob_true));
    P1("0x198f94f8 == 0x%08x",jx_ob_hash_code(ob_false));

    P1("0x60787288 == 0x%08x",jx_ob_hash_code(ob_0));
    P1("0x8a9830b0 == 0x%08x",jx_ob_hash_code(ob_1));
    P1("0x231eb015 == 0x%08x",jx_ob_hash_code(ob_2));

    P1("0x4a38819a == 0x%08x",jx_ob_hash_code(ob_3pt0));

    P1("0xb555fa5d == 0x%08x",jx_ob_hash_code(ob_tiny));

    P1("0x09774a5a == 0x%08x",jx_ob_hash_code(ob_huge1));
    P1("0x09774a5a == 0x%08x",jx_ob_hash_code(ob_huge2));
    P1("0xba51b099 == 0x%08x",jx_ob_hash_code(ob_huge3));

    P1("0 == %d",jx_ob_free(ob_huge1));
    P1("0 == %d",jx_ob_free(ob_huge2));
    P1("0 == %d",jx_ob_free(ob_huge3));
  }
  {
    /* confirm that we don't hash lists */
    jx_ob list = jx_list_new();
    P1("0x00000000 == 0x%08x",jx_ob_hash_code(list));
    P1("0 == %d",jx_ob_free(list));
  }
  {
    jx_ob hash = jx_hash_new();

    P1("0 == %d",jx_hash_size(hash));

    P1("0 == %d",jx_hash_has_key(hash, jx_ob_from_int(1)));
    P1("0 == %d",jx_hash_set(hash, jx_ob_from_int(1), jx_ob_from_int(2)));
    P1("1 == %d",jx_hash_size(hash));
    P1("1 == %d",jx_hash_has_key(hash, jx_ob_from_int(1)));

    P1("0 == %d",jx_hash_set(hash, jx_ob_from_str("ky1"), jx_ob_from_str("vl1")));
    P1("2 == %d",jx_hash_size(hash));
    P1("1 == %d",jx_hash_has_key(hash, jx_ob_from_str("ky1")));

    {
      jx_ob borrowed = jx_hash_get(hash, jx_ob_from_str("ky1"));
      P1("'vl1' eq '%s'",jx_ob_as_str(&borrowed));
    }

    {
      jx_ob key1 = jx_ob_from_str("heap string as a hash key");
      jx_ob key2 = jx_ob_from_str("heap string as a hash key");
      jx_ob value1 = jx_ob_from_str("heap string as a hash value");

      P1("0 == %d",jx_hash_set(hash, key1, value1));
      P1("1 == %d",jx_hash_has_key(hash, key1));
      P1("1 == %d",jx_hash_has_key(hash, key2));
      P1("3 == %d",jx_hash_size(hash));

      {
        jx_ob borrowed = jx_hash_get(hash,key2);
        P1("'heap string as a hash value' eq '%s'",jx_ob_as_str(&borrowed));
      }

      //   P2("'%s' == '%s'",jx_ob_as_str(&value1), jx_hash_size(hash));      
      jx_ob_free(key2);
    }

    P1("0 == %d",jx_ob_free(hash));
  }
                             
  
}
