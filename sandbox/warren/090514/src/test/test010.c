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

#define C1(ex,s1) printf("# %s (%s line %d,);\n",s1,__FILE__,__LINE__);
#define P1(ex,s1) printf(ex " || die('fail: %s line %d.');\n",s1,__FILE__,__LINE__);
#define P2(ex,s1,s2) printf(ex " || die('fail: %s line %d.');\n",s1,s2,__FILE__,__LINE__);

jx_ob my_fn(jx_ob node, jx_ob payload)
{
  printf("# my_fn called with value %d\n",(int)jx_ob_as_int(jx_list_borrow(payload,1)));
  jx_ob_free(payload);
  return jx_ob_from_str("this is the result of the call");
}

int main(int argc, char **argv)
{

  /* can we call our native function? */

  {
    jx_ob builtin = jx_builtin_new_from_native_fn(my_fn);
    jx_ob builtins = jx_hash_new();

    P1("0 == %d",jx_hash_set(builtins, jx_ob_from_ident("my_fn"),builtin));
    {
      jx_ob jxon = jx_ob_to_jxon(builtins);
      printf("# %s\n",jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }
    {
      jx_ob source = jx_ob_from_jxon_str("[ my_fn ]");
      jx_ob code = jx_code_bind_with_source(builtins, source);
      {
        jx_ob jxon = jx_ob_to_jxon(code);
        printf("# eval: %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob node = JX_OB_NULL;
        jx_ob result = jx_code_exec(node,code);
        {
          jx_ob jxon = jx_ob_to_jxon(result);
          P1("'this is the result of the call' eq %s",jx_ob_as_str(&jxon));
          P1("0 == %d",jx_ob_free(jxon));
        }
        P1("0 == %d",jx_ob_free(result));
      }
      P1("0 == %d",jx_ob_free(code));
    }
    P1("0 == %d",jx_ob_free(builtins));
  }

  /* can we get / set? */

  {
    jx_ob node = jx_hash_new();
    jx_ob builtins = jx_hash_new();

    P1("0 == %d",jx_code_expose_secure_builtins(builtins));


    {
      jx_ob jxon = jx_ob_to_jxon(builtins);
      printf("# %s\n",jx_ob_as_str(&jxon));
      jx_ob_free(jxon);
    }

    {
      jx_ob source = jx_ob_from_jxon_str("[ [ set, \"a\", \"b\" ], [get, \"a\"] ]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        {
          jx_ob jxon = jx_ob_to_jxon(code);
          printf("# %s\n",jx_ob_as_str(&jxon));
          P1("0 == %d",jx_ob_free(jxon));
        }
        {
          jx_ob result = jx_code_exec(node,code);
          {
            jx_ob jxon = jx_ob_to_jxon(result);
            P1("'b' eq %s",jx_ob_as_str(&jxon));
            P1("0 == %d",jx_ob_free(jxon));
          }
          P1("0 == %d",jx_ob_free(result));
        }
        P1("0 == %d",jx_ob_free(code));
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
    P1("0 == %d",jx_ob_free(node));
  }

  /* can we add two ints? */

  {
    jx_ob builtins = jx_hash_new();
    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str("[ [ add, 1, 2 ] ]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        jx_ob node = JX_OB_NULL;
        jx_ob result = jx_code_exec(node,code);
        P1("3 == %d",(int)jx_ob_as_int(result));
        P1("0 == %d",jx_ob_free(result));
        P1("0 == %d",jx_ob_free(code));
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
  }

  /* can we add two floats? */

  {
    jx_ob builtins = jx_hash_new();
    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str("[ [ add, 2.0, 3.0 ] ]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        jx_ob node = JX_OB_NULL;
        jx_ob result = jx_code_exec(node,code);
        P1("5.0 == %f",jx_ob_as_float(result));
        P1("0 == %d",jx_ob_free(result));
        P1("0 == %d",jx_ob_free(code));
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
  }

  /* can we set an evaluated result */
  {
    jx_ob node = jx_hash_new();
    jx_ob builtins = jx_hash_new();
    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str("[ [ set, 0, [ add, -1, 7 ] ], [get, 0] ]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        jx_ob result = jx_code_exec(node,code);
        P1("6 == %d",(int)jx_ob_as_int(result));
        P1("0 == %d",jx_ob_free(result));
        P1("0 == %d",jx_ob_free(code));
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
    P1("0 == %d",jx_ob_free(node));
  }
 
  /* can we add a gotten result */
  {
    jx_ob node = jx_hash_new();
    jx_ob builtins = jx_hash_new();
    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str("[ [set, a, 5], [ add, -1, [get, a] ] ]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        jx_ob result = jx_code_exec(node,code);
        P1("4 == %d",(int)jx_ob_as_int(result));
        P1("0 == %d",jx_ob_free(result));
        P1("0 == %d",jx_ob_free(code));
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
    P1("0 == %d",jx_ob_free(node));
  }


  /* can we combine all of the above? */
  {
    jx_ob node = jx_hash_new();
    jx_ob builtins = jx_hash_new();
    jx_ob builtin = jx_builtin_new_from_native_fn(my_fn);

    P1("0 == %d",jx_hash_set(builtins, jx_ob_from_ident("my_fn"),builtin));

    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str(
 "[[set,a,10],[while,[get,a],[[my_fn, [get,a]],[set,a,[add,-1,[get,a]]]]]]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        jx_ob result = jx_code_exec(node,code);
        P1("0 == %d",(int)jx_ob_as_int(result));
        P1("0 == %d",jx_ob_free(result));
        P1("0 == %d",jx_ob_free(code));
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
    P1("0 == %d",jx_ob_free(node));
   }

  /* can we perform operations inside of nested data structures and store result? */

  {
    jx_ob node = jx_hash_new();
    jx_ob builtins = jx_hash_new();
    jx_ob builtin = jx_builtin_new_from_native_fn(my_fn);

    P1("0 == %d",jx_hash_set(builtins, jx_ob_from_ident("my_fn"),builtin));

    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str("[[set,a,{1:2,3:[4,5,[sub,9,3]]}],[get,a]]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        {
          jx_ob jxon = jx_ob_to_jxon(code);
          printf("# exec: %s\n",jx_ob_as_str(&jxon));
          P1("0 == %d",jx_ob_free(jxon));
        }
        {
          jx_ob result = jx_code_exec(node,code);
          {
            jx_ob jxon = jx_ob_to_jxon(result);
            printf("# %s\n",jx_ob_as_str(&jxon));
            P1("0 == %d",jx_ob_free(jxon));
          }
          P1("0 == %d",jx_ob_free(result));
          P1("0 == %d",jx_ob_free(code));
        }
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
    P1("0 == %d",jx_ob_free(node));
   }

  /* can we borrow and modify containers owned by the node? */

  {
    jx_ob node = jx_hash_new();
    jx_ob builtins = jx_hash_new();
    jx_ob builtin = jx_builtin_new_from_native_fn(my_fn);

    P1("0 == %d",jx_hash_set(builtins, jx_ob_from_ident("my_fn"),builtin));

    P1("0 == %d",jx_code_expose_secure_builtins(builtins));

    {
      jx_ob source = jx_ob_from_jxon_str(
"[[set,x,[]],[[append,x,1]],[[append,x,{2:3}]],[get,x]]");
      {
        jx_ob jxon = jx_ob_to_jxon(source);
        printf("# %s\n",jx_ob_as_str(&jxon));
        P1("0 == %d",jx_ob_free(jxon));
      }
      {
        jx_ob code = jx_code_bind_with_source(builtins, source);
        {
          jx_ob jxon = jx_ob_to_jxon(code);
          printf("# %s\n",jx_ob_as_str(&jxon));
          P1("0 == %d",jx_ob_free(jxon));
        }
        {
          jx_ob result = jx_code_exec(node,code);
          {
            jx_ob jxon = jx_ob_to_jxon(result);
            printf("# %s\n",jx_ob_as_str(&jxon));
            P1("0 == %d",jx_ob_free(jxon));
          }
          P1("0 == %d",jx_ob_free(result));
          P1("0 == %d",jx_ob_free(code));
        }
      }
    }
    P1("0 == %d",jx_ob_free(builtins));
    P1("0 == %d",jx_ob_free(node));
   }
}

