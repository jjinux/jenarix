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

jx_status jx_safe_expose_all_builtins(jx_ob namespace)
{
  jx_bool ok = JX_TRUE;

  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("set"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_SET)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("get"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_GET)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("add"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_ADD)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("sub"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_SUB)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("borrow"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_BORROW)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("append"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_APPEND)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("size"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_SIZE)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("hash_set"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_HASH_SET)));
  ok = ok && 
    jx_ok( jx_hash_set(namespace, jx_ob_from_ident("hash_get"), 
                       jx_builtin_new_from_selector(JX_BUILTIN_HASH_GET)));

  return ok ? JX_SUCCESS : JX_FAILURE;
}

