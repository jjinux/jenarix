#ifndef _H_jx_private
#define _H_jx_private

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

struct jx__list {
  jx_meta packed_meta;
  union {
    jx_ob    *ob_vla; /* heterogeneous */
    jx_float *float_vla;
    jx_int   *int_vla;
    void     *vla;
  } data;
};

#define JX_ZERO_ARRAY_SIZE 1

typedef struct {
  jx_int    mode; 
  jx_uint32 mask; /* 2^n - 1 */
  jx_uint32 usage;
  jx_uint32 stale;
  jx_uint32 start[JX_ZERO_ARRAY_SIZE];
} jx_hash_info;

/* hash table modes */

#define JX_HASH_RAW         0
#define JX_HASH_LINEAR      1
#define JX_HASH_ONE_TO_ANY  2
#define JX_HASH_ONE_TO_ONE  3
#define JX_HASH_ONE_TO_NIL  4

/* 
   If info is null, then RAW more is implied, meaning that client must
   simply probe the key_value vla for a match (without local packed
   copies of the hash codes).

   LINEAR:
   
   table simply contains object hash codes in same order as the key_value vla.

   ONE_TO_MANY:

   table contains [hash_code, offset]

   ONE_TO_ONE:

   table contains forward: [hash_code, offset] and reverse: [hash_code,offset] 
   
   ONE_TO_NIL:

   simple set behavior -- keys but no values.  This behavior is not
   yet optimized for storage or performance: right now it is simply
   behaves as ONE_TO_ANY with null for all values.

*/

struct jx__hash {
  jx_ob *key_value; /* variable length array of key/value objects owned by the table */
  jx_uint32 *info; /* variable length array of the table itself */
};

#endif
