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

#define N_CYC 200
#define N_KEY 10000

int main(int argc, char **argv)
{
  {
    jx_ob list0 = jx_list_new();
    jx_ob list1 = jx_list_new();
    
    P1("0 == %d", jx_list_resize(list0,5,jx_ob_from_int(0)));
    P1("0 == %d", jx_list_resize(list1,5,jx_ob_from_int(1)));
    
    P1("0 == %d", jx_list_combine(list0,list1));

    {
      jx_ob json = jx_ob_to_json(list0);
      P1("'[0,0,0,0,0,1,1,1,1,1]' eq '%s'", jx_ob_as_str(&json));
      jx_ob_free(json);
    }

    P1("0 == %d", jx_ob_free(list0));
  }

  {
    jx_ob list0 = jx_list_new();
    jx_ob list1 = jx_list_new();
    
    jx_ob str0 = jx_ob_from_str("01234567890");
    jx_ob str1 = jx_ob_from_str("abcdefghijk");
      
    P1("0 == %d", jx_list_resize(list0,2,str0));
    P1("0 == %d", jx_list_resize(list1,2,str1));
    
    P1("0 == %d", jx_list_combine(list0,list1));

    {
      jx_ob json = jx_ob_to_json(list0);
      P1("'[\"01234567890\",\"01234567890\",\"abcdefghijk\",\"abcdefghijk\"]' eq '%s'",
         jx_ob_as_str(&json));
      jx_ob_free(json);
    }

    P1("0 == %d", jx_ob_free(list0));
  }
  
  {
    jx_ob list0 = jx_list_new();
    jx_ob list1 = jx_list_new();

    jx_list_append(list0,jx_ob_from_int(1));
    jx_list_append(list0,jx_ob_from_int(2));


    P1("0 == %d", jx_ob_read_only(list0));    
    list0 = jx_ob_set_read_only(list0,JX_TRUE);
    P1("1 == %d", jx_ob_read_only(list0));    

    P1("0 == %d", jx_list_resize(list1,2,list0));

    P1("0 == %d", jx_list_combine(list1,list0));

    {
      jx_ob json = jx_ob_to_json(list1);
      P1("'[[1,2],[1,2],1,2]' eq '%s'", jx_ob_as_str(&json));
      jx_ob_free(json);
    }

    P1("0 == %d", jx_ob_free(list1));

    P1("-1 == %d", jx_ob_free(list0));    

    list0 = jx_ob_set_read_only(list0,JX_FALSE);
    P1("0 == %d", jx_ob_free(list0));    
  }

  {
    jx_ob list = jx_list_new_with_fill(1000000,jx_ob_from_float(3.0));

    P1("1000000 == %d", jx_list_size(list));

    P1("0 == %d", jx_list_combine(list,list));

    P1("2000000 == %d", jx_list_size(list));

    P1("3.0 == %f", jx_ob_as_float(jx_list_borrow(list,1234567)));
    P1("0 == %d", jx_ob_free(list));    
  }

  {
    jx_ob list1 = jx_list_new_with_fill(2,jx_ob_from_int(1));
    jx_ob list2 = jx_list_new_with_fill(2,jx_ob_from_int(1));
    jx_ob list3 = jx_list_new_with_fill(2,jx_ob_from_int(2));
    jx_ob list4 = jx_list_new_with_fill(3,jx_ob_from_str("a really really long string"));
    jx_ob list5 = jx_list_new_with_fill(3,jx_ob_from_str("a really really long string"));
    jx_ob list6 = jx_list_new_with_fill(3,jx_ob_from_str("a really really really long string"));
    
    P1("0 == %d", jx_ob_identical(list1,list2));    
    P1("1 == %d", jx_ob_equal(list1,list2));    
    P1("0 == %d", jx_ob_equal(list1,list3));    
    P1("0 == %d", jx_ob_equal(list2,list3));    

    P1("0 == %d", jx_ob_identical(list4,list5));    
    P1("1 == %d", jx_ob_equal(list4,list5));    
    P1("0 == %d", jx_ob_equal(list4,list6));    
    P1("0 == %d", jx_ob_equal(list2,list6));    

    P1("0 == %d", jx_ob_free(list1));    
    P1("0 == %d", jx_ob_free(list2));    
    P1("0 == %d", jx_ob_free(list3));    

    P1("0 == %d", jx_ob_free(list4));    
    P1("0 == %d", jx_ob_free(list5));    
    P1("0 == %d", jx_ob_free(list6));    

  }

  {
    jx_ob hash1 = jx_hash_new();
    jx_ob hash2 = jx_hash_new();
    jx_ob hash3 = jx_hash_new();

    P1("0 == %d", jx_ob_identical(hash1,hash2));    
    P1("1 == %d", jx_ob_equal(hash1,hash2));    
    P1("1 == %d", jx_ob_equal(hash2,hash3));    

    jx_hash_set(hash1, jx_ob_from_int(1), jx_ob_from_int(2));
    jx_hash_set(hash2, jx_ob_from_int(1), jx_ob_from_int(2));
    jx_hash_set(hash3, jx_ob_from_int(3), jx_ob_from_int(4));

    P1("1 == %d", jx_ob_equal(hash1,hash2));    
    P1("0 == %d", jx_ob_equal(hash2,hash3));    

    jx_hash_set(hash1, jx_ob_from_int(3), jx_ob_from_int(4));
    jx_hash_set(hash2, jx_ob_from_int(3), jx_ob_from_int(4));
    jx_hash_set(hash3, jx_ob_from_int(1), jx_ob_from_int(2));

    P1("1 == %d", jx_ob_equal(hash1,hash2));    
    P1("1 == %d", jx_ob_equal(hash2,hash3));    

    jx_hash_set(hash1, jx_ob_from_int(5), jx_list_new_with_fill(1,jx_ob_from_str("hi")));
    jx_hash_set(hash2, jx_ob_from_int(5), jx_list_new_with_fill(1,jx_ob_from_str("hi")));
    jx_hash_set(hash3, jx_ob_from_int(5), jx_list_new_with_fill(1,jx_ob_from_str("lo")));

    P1("1 == %d", jx_ob_equal(hash1,hash2));    
    P1("0 == %d", jx_ob_equal(hash2,hash3));    

    jx_hash_set(hash3, jx_ob_from_int(5), jx_list_new_with_fill(1,jx_ob_from_str("hi")));

    P1("1 == %d", jx_ob_equal(hash1,hash2));    
    P1("1 == %d", jx_ob_equal(hash2,hash3));    

    P1("0 == %d", jx_ob_free(hash1));    
    P1("0 == %d", jx_ob_free(hash2));    
    P1("0 == %d", jx_ob_free(hash3));    

  }

}
