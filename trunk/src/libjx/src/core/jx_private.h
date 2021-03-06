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
#include "jx_mem_wrap.h"
#include "jx_os_process_private.h"


/* macros for our private status-handling idiom */

#define JX_IS_OK(x) (JX_OK(status=(x)))

#define JX_IS_YES(x) ((status=(x))==JX_YES)

#define JX_OK_DO(x) { if(JX_OK(status)) {status = (x);}}

#define JX_ERR_DO(x) { if(JX_ERR(status)) {x;}}

#define JX_DO(x) {status = (x);}


/* 
   HASH TABLE:

   If info is null, then RAW more is implied, meaning that client must
   simply probe the key_value vla for a match (without local packed
   copies of the hash codes).

   LINEAR:
   
   table simply contains object hash codes in same order as the key_value vla.

   ONE_TO_MANY:

   hash table entries contain [hash_code, offset] as circular hash entry table

   ONE_TO_ONE: table is actually two adjacent hash tables

   entries in first hash table contain forward: [hash_code, offset] 
   entries in second hash table contain reverse: [hash_code, offset] 
   
   ONE_TO_NIL:

   simple set behavior -- keys but no values.  This behavior is not
   yet optimized for storage or performance: right now it is simply
   behaves as ONE_TO_ANY with null for all values.
   
*/

#define JX_HASH_INFO_SIZE (sizeof(jx_hash_info)/sizeof(jx_uint32))


/* methods we're not currently exposing */

jx_ob jx_ob_with_str_vla(jx_char ** ref);

#define JX_TLS_VLA_ALLOC_MAX   4000-sizeof(jx_vla)
#define JX_TLS_VLA_MAX          100
#define JX_TLS_MAX              100

struct jx__mem_chain {
  jx_mem_chain *next;
};

jx_ob jx_ident_split_from_dotted(jx_env * E, jx_ob ident);
jx_ob jx_ident_split_with_dotted(jx_env * E, jx_ob ident);

#endif
