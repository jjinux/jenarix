#ifndef _H_jx_safe
#define _H_jx_safe

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

#include "jx_private.h"

/* the first 512 selectors are reserved for "safe" functions which
   involve only internal actions within the interpreter and container
   space (no access to filesystem, network, memory, etc.) */

#define JX_BUILTIN_NOP      0
#define JX_BUILTIN_SET      1
#define JX_BUILTIN_GET      2
#define JX_BUILTIN_ADD      3
#define JX_BUILTIN_SUB      4
#define JX_BUILTIN_BORROW   5
#define JX_BUILTIN_APPEND   6
#define JX_BUILTIN_SIZE     7
#define JX_BUILTIN_HASH_SET 8
#define JX_BUILTIN_HASH_GET 9

jx_status jx_safe_expose_all_builtins(jx_ob namespace);

JX_INLINE jx_ob jx_safe_set(jx_ob node, jx_ob payload)
{
  jx_ob value = jx_list_remove(payload,2);
  jx_ob key = jx_list_remove(payload,1);
  return jx_ob_from_status( jx_hash_set(node, key, value ) );
}

JX_INLINE jx_ob jx_safe_get(jx_ob node, jx_ob payload)
{
  return jx_hash_get(node, jx_list_borrow(payload,1));
}

JX_INLINE jx_ob jx_safe_borrow(jx_ob node, jx_ob payload)
{
  return jx_ob_take_weak_ref( jx_hash_borrow(node, jx_list_borrow(payload,1)));
}

JX_INLINE jx_ob jx_safe_add(jx_ob node, jx_ob payload)
{
  return jx_add( jx_list_borrow(payload,1), jx_list_borrow(payload,2) );
}

JX_INLINE jx_ob jx_safe_sub(jx_ob node, jx_ob payload)
{
  return jx_sub( jx_list_borrow(payload,1), jx_list_borrow(payload,2) );
}

JX_INLINE jx_ob jx_safe_append(jx_ob node, jx_ob payload)
{
  jx_ob item = jx_list_remove(payload,2);
  jx_ob list = jx_list_borrow(payload,1);
  return jx_ob_from_status( jx_list_append(list, item) );
}

JX_INLINE jx_ob jx_safe_size(jx_ob node, jx_ob payload)
{
  return jx_ob_size( jx_list_borrow(payload,1) );
}

JX_INLINE jx_ob jx_safe_hash_set(jx_ob node, jx_ob payload)
{
  return jx_ob_from_status(jx_hash_set(jx_list_borrow(payload,1),
                                       jx_list_borrow(payload,2),
                                       jx_list_borrow(payload,3)));
}

JX_INLINE jx_ob jx_safe_hash_get(jx_ob node, jx_ob payload)
{
  return jx_hash_get(jx_list_borrow(payload,1),
                     jx_list_borrow(payload,2));
}

#endif
