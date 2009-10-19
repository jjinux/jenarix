

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

static void dump_hex(jx_char * message, void *ptr, jx_int size)
{
  unsigned char *ch = (unsigned char *) ptr;
  printf("%s: ", message);
  while(size--) {
    printf("%02x", *(ch++));
    if(size)
      printf(" ");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
#if JX_TINY_STR_SIZE == 0
  {
    jx_ob heap = jx_ob_from_str("heap_string_for_sure");

    P1("'heap_string_for_sure' eq '%s'", jx_ob_as_str(&heap));

    dump_hex("# heap", &heap, sizeof(jx_ob));
    P1("0 == %d", jx_ob_free(heap));
  }
#else
  {
#if JX_TINY_STR_SIZE == 6
#define TINY_TEST "12345"
#else
#if JX_TINY_STR_SIZE == 12
#define TINY_TEST "12345678901"
#else
#if JX_TINY_STR_SIZE == 14
#define TINY_TEST "1234567890123"
#endif
#endif
#endif
    jx_ob tiny = jx_ob_from_str(TINY_TEST);
    jx_ob heap = jx_ob_from_str("heap_string_for_sure");

    P2("'%s' eq '%s'", TINY_TEST, jx_ob_as_str(&tiny));
    P1("'heap_string_for_sure' eq '%s'", jx_ob_as_str(&heap));

    dump_hex("# tiny", &tiny, sizeof(jx_ob));
    dump_hex("# heap", &heap, sizeof(jx_ob));

    tiny = jx_ob_from_int(1);
    dump_hex("# one", &tiny, sizeof(jx_ob));

    P1("0 == %d", jx_ob_free(tiny));
    P1("0 == %d", jx_ob_free(heap));
  }
#endif

  {
    jx_ob str = jx_ob_from_str("tny");
    P1("1 == %d", JX_TRUE && jx_null_check(jx_ob_from_null()));
    P1("0 == %d", JX_TRUE && jx_null_check(jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", JX_TRUE && jx_null_check(jx_ob_from_int(0)));
    P1("0 == %d", JX_TRUE && jx_null_check(jx_ob_from_float(1.0)));
    P1("0 == %d", JX_TRUE && jx_null_check(str));

    P1("0 == %d", JX_TRUE && jx_bool_check(jx_ob_from_null()));
    P1("1 == %d", JX_TRUE && jx_bool_check(jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", JX_TRUE && jx_bool_check(jx_ob_from_int(0)));
    P1("0 == %d", JX_TRUE && jx_bool_check(jx_ob_from_float(1.0)));
    P1("0 == %d", JX_TRUE && jx_bool_check(str));

    P1("0 == %d", JX_TRUE && jx_int_check(jx_ob_from_null()));
    P1("0 == %d", JX_TRUE && jx_int_check(jx_ob_from_bool(JX_FALSE)));
    P1("1 == %d", JX_TRUE && jx_int_check(jx_ob_from_int(0)));
    P1("0 == %d", JX_TRUE && jx_int_check(jx_ob_from_float(1.0)));
    P1("0 == %d", JX_TRUE && jx_int_check(str));

    P1("0 == %d", JX_TRUE && jx_float_check(jx_ob_from_null()));
    P1("0 == %d", JX_TRUE && jx_float_check(jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", JX_TRUE && jx_float_check(jx_ob_from_int(0)));
    P1("1 == %d", JX_TRUE && jx_float_check(jx_ob_from_float(1.0)));
    P1("0 == %d", JX_TRUE && jx_float_check(str));

    P1("0 == %d", JX_TRUE && jx_str_check(jx_ob_from_null()));
    P1("0 == %d", JX_TRUE && jx_str_check(jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", JX_TRUE && jx_str_check(jx_ob_from_int(0)));
    P1("0 == %d", JX_TRUE && jx_str_check(jx_ob_from_float(1.0)));
    P1("1 == %d", JX_TRUE && jx_str_check(str));

    P1("0 == %d", JX_TRUE && jx_list_check(jx_ob_from_null()));
    P1("0 == %d", JX_TRUE && jx_list_check(jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", JX_TRUE && jx_list_check(jx_ob_from_int(0)));
    P1("0 == %d", JX_TRUE && jx_list_check(jx_ob_from_float(1.0)));
    P1("0 == %d", JX_TRUE && jx_list_check(str));

    P1("0 == %d", JX_TRUE && jx_hash_check(jx_ob_from_null()));
    P1("0 == %d", JX_TRUE && jx_hash_check(jx_ob_from_bool(JX_FALSE)));
    P1("0 == %d", JX_TRUE && jx_hash_check(jx_ob_from_int(0)));
    P1("0 == %d", JX_TRUE && jx_hash_check(jx_ob_from_float(1.0)));
    P1("0 == %d", JX_TRUE && jx_hash_check(str));

    jx_ob_free(str);
  }

#if JX_TINY_STR_SIZE == 0
  {
    jx_ob heap = jx_ob_from_ident("heap_ident_for_sure__");

    P1("'heap_ident_for_sure__' eq '%s'", jx_ob_as_ident(&heap));

    dump_hex("# heap", &heap, sizeof(jx_ob));

    P1("0 == %d", jx_ob_free(heap));
  }
#else
  {
    jx_ob tiny = jx_ob_from_ident(TINY_TEST);
    jx_ob heap = jx_ob_from_ident("heap_ident_for_sure__");

    P2("'%s' eq '%s'", TINY_TEST, jx_ob_as_ident(&tiny));
    P1("'heap_ident_for_sure__' eq '%s'", jx_ob_as_ident(&heap));

    dump_hex("# tiny", &tiny, sizeof(jx_ob));
    dump_hex("# heap", &heap, sizeof(jx_ob));

    tiny = jx_ob_from_int(1);
    dump_hex("# one", &tiny, sizeof(jx_ob));

    P1("0 == %d", jx_ob_free(tiny));
    P1("0 == %d", jx_ob_free(heap));
  }
#endif

  /* casts */
  {
    {
      jx_ob str = jx_ob_from_str("tiny");

      P1("0 == %d", (int) jx_ob_as_bool(jx_ob_from_null()));
      P1("1 == %d", (int) jx_ob_as_bool(str));
      P1("0 == %d", (int) jx_ob_as_bool(jx_ob_from_bool(JX_FALSE)));
      P1("1 == %d", (int) jx_ob_as_bool(jx_ob_from_bool(JX_TRUE)));
      P1("1 == %d", (int) jx_ob_as_bool(jx_ob_from_int(2)));
      P1("0 == %d", (int) jx_ob_as_bool(jx_ob_from_float(0.0)));
      P1("1 == %d", (int) jx_ob_as_bool(jx_ob_from_float(3.0)));

      P1("0 == %d", (int) jx_ob_as_int(jx_ob_from_null()));
      P1("0 == %d", (int) jx_ob_as_int(str));
      P1("0 == %d", (int) jx_ob_as_int(jx_ob_from_bool(JX_FALSE)));
      P1("1 == %d", (int) jx_ob_as_int(jx_ob_from_bool(JX_TRUE)));
      P1("2 == %d", (int) jx_ob_as_int(jx_ob_from_int(2)));
      P1("3 == %d", (int) jx_ob_as_int(jx_ob_from_float(3.0)));

      P1("0.0 == %1.1f", (float) jx_ob_as_float(jx_ob_from_null()));
      P1("0.0 == %1.1f", (float) jx_ob_as_float(str));
      P1("0.0 == %1.1f", (float) jx_ob_as_float(jx_ob_from_bool(JX_FALSE)));
      P1("1.0 == %1.1f", (float) jx_ob_as_float(jx_ob_from_bool(JX_TRUE)));
      P1("2.0 == %1.1f", (float) jx_ob_as_float(jx_ob_from_float(2.0)));
      P1("3.0 == %1.1f", (float) jx_ob_as_float(jx_ob_from_int(3)));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_to_str(jx_ob_from_null());
      P1("'null' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_to_str(jx_ob_from_bool(JX_FALSE));
      P1("'false' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_to_str(jx_ob_from_bool(JX_TRUE));
      P1("'true' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_to_str(jx_ob_from_int(1));
      P1("'1' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_to_str(jx_ob_from_float(2.0));
      P1("'2.0' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

    {
      jx_ob ident = jx_ob_from_ident("this_is_my_very_long_identifier");
      jx_ob str = jx_ob_to_str(ident);
      P1("'this_is_my_very_long_identifier' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
      jx_ob_free(ident);
    }

    {
      jx_ob str = jx_ob_from_str("");
      P1("0 == %d", jx_ob_as_bool(jx_ob_to_bool(str)));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_from_str("not empty");
      P1("1 == %d", jx_ob_as_bool(jx_ob_to_bool(str)));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_from_str("1");
      P1("1 == %d", (int) jx_ob_as_int(jx_ob_to_int(str)));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_from_str("0.25");
      P1("0.25 == %1.2f", jx_ob_as_float(jx_ob_to_float(str)));
      jx_ob_free(str);
    }

    {
      jx_ob str = jx_ob_from_str("str");
      jx_ob str2 = jx_ob_to_str(str);
      P1("'str' eq '%s'", jx_ob_as_str(&str2));
      jx_ob_free(str);
      jx_ob_free(str2);
    }

    {
      jx_ob ident = jx_ob_from_ident("false");
      P1("0 == %d", jx_ob_as_bool(jx_ob_to_bool(ident)));
      jx_ob_free(ident);
    }

    {
      jx_ob ident = jx_ob_from_ident("true");
      P1("1 == %d", jx_ob_as_bool(jx_ob_to_bool(ident)))
        jx_ob_free(ident);
    }

    {
      jx_ob str = jx_ob_from_str_with_len("1234567890123456789012345", 20);
      P1("'12345678901234567890' eq '%s'", jx_ob_as_str(&str));
      jx_ob_free(str);
    }

  }

}
