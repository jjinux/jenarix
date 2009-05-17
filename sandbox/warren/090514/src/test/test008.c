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

#define JS(st) \
{ \
  { \
    jx_ob ob = jx_ob_from_json_str(st); \
    jx_ob json = jx_ob_to_json(ob); \
    P2("'%s' == '%s'", st,jx_ob_as_str(&json)); \
    P1("0 == %d", jx_ob_free(json)); \
    P1("0 == %d", jx_ob_free(ob)); \
  } \
}

#define JF(st) \
{ \
  { \
    jx_ob ob = jx_ob_from_json_str(st); \
    jx_ob json = jx_ob_to_json(ob); \
    P1("'null' == '%s'", jx_ob_as_str(&json)); \
    P1("0 == %d", jx_ob_free(json)); \
    P1("0 == %d", jx_ob_free(ob)); \
  } \
}

int main(int argc, char **argv)
{
  JS("null");
  JS("true");
  JS("false");
  JS("0");
  JS("1");
  JS("-1");
  JS("1.0");
  JS("-1.0");
#ifdef JX_64_BIT
  JS("1.0000100000000001");
  JS("-1.0000100000000001");
  JS("9223372036854775807");
  JS("-9223372036854775808");
#else
  JS("1.00001");
  JS("-1.00001");
  JS("2147483647");
  JS("-2147483648");
#endif
  JS("[]");
  JS("{}");
  JS("\"hi\"");
  JS("[null]");
  JS("[true]");
  JS("[false]");
  JS("[1]");
  JS("[-1]");
  JS("[\"one\"]");
  JS("{1:2}");
  JS("{-1:-2}");
  JS("{\"one\":\"two\"}");
  JS("[1,2]");
  JS("[-1,-2]");
  JS("[1,[2]]");
  JS("[1,[2,3]]");
  JS("{1:{2:3}}");
  JS("[1,[2,3,4,5]]");
  JS("{1:{2:3,4:5}}");
  JS("[1,[2,\"three\",4,5]]");

  /* below are all invalid and should give null */
  JF("");
  JF("-");
  JF("{[]:1}");
  JF("{{}:1}");
  JF("{[1,2]:1}");
  JF("{[1,2]:[3,4]}");

}
