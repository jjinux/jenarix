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

static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char *ident, jx_int selector)
{
  if(ok)
    ok = jx_ok( jx_hash_set(names, jx_ob_from_ident(ident), 
                            jx_builtin_new_from_selector(selector)));
  return ok;
}

jx_status jx_safe_expose_all_builtins(jx_ob names)
{
  jx_bool ok = JX_TRUE;

  ok = jx_declare(ok,names,"nop", JX_SELECTOR_NOP);
  ok = jx_declare(ok,names,"entity", JX_SELECTOR_ENTITY);
  ok = jx_declare(ok,names,"map_set", JX_SELECTOR_MAP_SET);

  ok = jx_declare(ok,names,"set", JX_SELECTOR_SET);
  ok = jx_declare(ok,names,"get", JX_SELECTOR_GET);
  //  ok = jx_declare(ok,names,"borrow", JX_SELECTOR_BORROW);
  ok = jx_declare(ok,names,"take", JX_SELECTOR_TAKE);
  ok = jx_declare(ok,names,"del", JX_SELECTOR_DEL);

  ok = jx_declare(ok,names,"eq", JX_SELECTOR_EQ);
  ok = jx_declare(ok,names,"gt", JX_SELECTOR_GT);
  ok = jx_declare(ok,names,"lt", JX_SELECTOR_LT);
  ok = jx_declare(ok,names,"ge", JX_SELECTOR_GE);
  ok = jx_declare(ok,names,"le", JX_SELECTOR_LE);
  ok = jx_declare(ok,names,"ne", JX_SELECTOR_NE);
  ok = jx_declare(ok,names,"identical", JX_SELECTOR_IDENTICAL);

  ok = jx_declare(ok,names,"output", JX_SELECTOR_OUTPUT);
  ok = jx_declare(ok,names,"error", JX_SELECTOR_ERROR);

  ok = jx_declare(ok,names,"add", JX_SELECTOR_ADD);
  ok = jx_declare(ok,names,"sub", JX_SELECTOR_SUB);
  ok = jx_declare(ok,names,"mul", JX_SELECTOR_MUL);
  ok = jx_declare(ok,names,"div", JX_SELECTOR_DIV);
  ok = jx_declare(ok,names,"idiv", JX_SELECTOR_IDIV);
  ok = jx_declare(ok,names,"mod", JX_SELECTOR_MOD);

  ok = jx_declare(ok,names,"and", JX_SELECTOR_AND);
  ok = jx_declare(ok,names,"or",  JX_SELECTOR_OR);

  ok = jx_declare(ok,names,"neg", JX_SELECTOR_NEG);
  ok = jx_declare(ok,names,"not", JX_SELECTOR_NOT);

  ok = jx_declare(ok,names,"size", JX_SELECTOR_SIZE);
  ok = jx_declare(ok,names,"append", JX_SELECTOR_APPEND);
  ok = jx_declare(ok,names,"extend", JX_SELECTOR_EXTEND);
  ok = jx_declare(ok,names,"insert", JX_SELECTOR_INSERT);
  ok = jx_declare(ok,names,"resize", JX_SELECTOR_RESIZE);
  ok = jx_declare(ok,names,"push", JX_SELECTOR_PUSH);
  ok = jx_declare(ok,names,"pop", JX_SELECTOR_POP);
  ok = jx_declare(ok,names,"shift", JX_SELECTOR_SHIFT);
  ok = jx_declare(ok,names,"unshift", JX_SELECTOR_UNSHIFT);
  ok = jx_declare(ok,names,"slice", JX_SELECTOR_SLICE);
  ok = jx_declare(ok,names,"cutout", JX_SELECTOR_CUTOUT);
  ok = jx_declare(ok,names,"reverse", JX_SELECTOR_REVERSE);
  ok = jx_declare(ok,names,"sort", JX_SELECTOR_SORT);
  
  ok = jx_declare(ok,names,"incr", JX_SELECTOR_INCR);
  ok = jx_declare(ok,names,"decr", JX_SELECTOR_DECR);
  ok = jx_declare(ok,names,"impl", JX_SELECTOR_IMPL);

  ok = jx_declare(ok,names,"range", JX_SELECTOR_RANGE);
  ok = jx_declare(ok,names,"fill", JX_SELECTOR_FILL);

  ok = jx_declare(ok,names,"symbols", JX_SELECTOR_SYMBOLS);
  ok = jx_declare(ok,names,"has", JX_SELECTOR_HAS);

  ok = jx_declare(ok,names,"same", JX_SELECTOR_SAME);

  ok = jx_declare(ok,names,"str",  JX_SELECTOR_STR);
  ok = jx_declare(ok,names,"int",  JX_SELECTOR_INT);
  ok = jx_declare(ok,names,"float", JX_SELECTOR_FLOAT);
  ok = jx_declare(ok,names,"bool",  JX_SELECTOR_BOOL);

  ok = jx_declare(ok,names,"synchronize", JX_SELECTOR_SYNCHRONIZE);
  ok = jx_declare(ok,names,"synchronized",  JX_SELECTOR_SYNCHRONIZED);

  ok = jx_declare(ok,names,"share", JX_SELECTOR_SHARE);
  ok = jx_declare(ok,names,"shared",  JX_SELECTOR_SHARED);

  ok = jx_declare(ok,names,"pow",  JX_SELECTOR_POW);

  ok = jx_declare(ok,names,"assert",  JX_SELECTOR_ASSERT);
  ok = jx_declare(ok,names,"dump", JX_SELECTOR_DUMP);
  ok = jx_declare(ok,names,"new", JX_SELECTOR_NEW);

  ok = jx_declare(ok,names,"null_op", JX_SELECTOR_NULL_OP);

  return ok ? JX_SUCCESS : JX_FAILURE;
}

