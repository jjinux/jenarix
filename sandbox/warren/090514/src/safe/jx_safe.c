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

/* 
   safe builtin functions (no access to filesystem, network, etc.) 
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jx_safe.h"

static jx_bool jx_declare(jx_bool ok, jx_ob namespace, jx_char *ident, jx_int selector)
{
  if(ok)
    ok = jx_ok( jx_hash_set(namespace, jx_ob_from_ident(ident), 
                            jx_builtin_new_from_selector(selector)));
  return ok;
}

jx_status jx_safe_expose_all_builtins(jx_ob namespace)
{
  jx_bool ok = JX_TRUE;

  ok = jx_declare(ok,namespace,"set", JX_BUILTIN_SET);
  ok = jx_declare(ok,namespace,"get", JX_BUILTIN_GET);
  //  ok = jx_declare(ok,namespace,"borrow", JX_BUILTIN_BORROW);
  ok = jx_declare(ok,namespace,"take", JX_BUILTIN_TAKE);
  ok = jx_declare(ok,namespace,"del", JX_BUILTIN_DEL);

  ok = jx_declare(ok,namespace,"eq", JX_BUILTIN_EQ);
  ok = jx_declare(ok,namespace,"gt", JX_BUILTIN_GT);
  ok = jx_declare(ok,namespace,"lt", JX_BUILTIN_LT);
  ok = jx_declare(ok,namespace,"ge", JX_BUILTIN_GE);
  ok = jx_declare(ok,namespace,"le", JX_BUILTIN_LE);
  ok = jx_declare(ok,namespace,"ne", JX_BUILTIN_NE);
  ok = jx_declare(ok,namespace,"identical", JX_BUILTIN_IDENTICAL);

  ok = jx_declare(ok,namespace,"output", JX_BUILTIN_OUTPUT);
  ok = jx_declare(ok,namespace,"error", JX_BUILTIN_ERROR);

  ok = jx_declare(ok,namespace,"add", JX_BUILTIN_ADD);
  ok = jx_declare(ok,namespace,"sub", JX_BUILTIN_SUB);
  ok = jx_declare(ok,namespace,"mul", JX_BUILTIN_MUL);
  ok = jx_declare(ok,namespace,"div", JX_BUILTIN_DIV);
  ok = jx_declare(ok,namespace,"mod", JX_BUILTIN_MOD);

  ok = jx_declare(ok,namespace,"and", JX_BUILTIN_AND);
  ok = jx_declare(ok,namespace,"or",  JX_BUILTIN_OR);

  ok = jx_declare(ok,namespace,"neg", JX_BUILTIN_NEG);
  ok = jx_declare(ok,namespace,"not", JX_BUILTIN_NOT);

  ok = jx_declare(ok,namespace,"size", JX_BUILTIN_SIZE);

  ok = jx_declare(ok,namespace,"append", JX_BUILTIN_APPEND);
  ok = jx_declare(ok,namespace,"extend", JX_BUILTIN_EXTEND);
  ok = jx_declare(ok,namespace,"insert", JX_BUILTIN_INSERT);
  ok = jx_declare(ok,namespace,"resize", JX_BUILTIN_RESIZE);
  ok = jx_declare(ok,namespace,"pop", JX_BUILTIN_POP);
  ok = jx_declare(ok,namespace,"shift", JX_BUILTIN_SHIFT);
  ok = jx_declare(ok,namespace,"unshift", JX_BUILTIN_UNSHIFT);
  ok = jx_declare(ok,namespace,"slice", JX_BUILTIN_SLICE);
  ok = jx_declare(ok,namespace,"cutout", JX_BUILTIN_CUTOUT);
  
  ok = jx_declare(ok,namespace,"incr", JX_BUILTIN_INCR);
  ok = jx_declare(ok,namespace,"decr", JX_BUILTIN_DECR);
  ok = jx_declare(ok,namespace,"impl", JX_BUILTIN_IMPL);

  ok = jx_declare(ok,namespace,"range", JX_BUILTIN_RANGE);
  ok = jx_declare(ok,namespace,"fill", JX_BUILTIN_FILL);

  ok = jx_declare(ok,namespace,"symbols", JX_BUILTIN_SYMBOLS);
  ok = jx_declare(ok,namespace,"has", JX_BUILTIN_HAS);


  ok = jx_declare(ok,namespace,"same", JX_BUILTIN_SAME);

  return ok ? JX_SUCCESS : JX_FAILURE;
}

