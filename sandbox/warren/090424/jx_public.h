#ifndef _H_jx_public
#define _H_jx_public

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

/* owned C types (to be use exclusively throughout) */

typedef  int          jx_bool;
typedef  int          jx_int;
typedef  float        jx_float;
typedef  char         jx_char;
typedef  int          jx_status;

typedef  int          jx_int32;
typedef  long long    jx_int64;

/* our ubuitous object jenarix object type */

typedef struct jx__ob jx_ob;

/* status codes constants */

#define JX_FAILURE  -1
#define JX_SUCCESS   0

#define JX_FALSE     0  
#define JX_TRUE      1

/* inline functions and structs and required by the compiler to be
 public, but should be treated by the developer as private */

#include "jx_inline.h"

/* boxing (all of these return owned objects) */

jx_ob jx_ob_from_null(void);
jx_ob jx_ob_from_bool(jx_bool bool_);
jx_ob jx_ob_from_int(jx_int int_);
jx_ob jx_ob_from_float(jx_float float_);
jx_ob jx_ob_from_str(jx_char *str); /* copies string into new storage */

/* unboxing */

jx_int    jx_ob_as_int(jx_ob ob);
jx_float  jx_ob_as_float(jx_ob ob);

jx_char  *jx_ob_as_str(jx_ob *ob); /* returns borrowed (volatile)
                                   jx_char pointer -- note possing
                                   object in as a pointer because we
                                   need to return a pointer to
                                   characters within the object
                                   itself when using tiny_str's */

/* determining object type */

jx_status jx_ob_is_null(jx_ob ob);
jx_status jx_ob_is_int(jx_ob ob);
jx_status jx_ob_is_float(jx_ob ob);
jx_status jx_ob_is_str(jx_ob ob);
jx_status jx_ob_is_list(jx_ob ob);
jx_status jx_ob_is_hash(jx_ob ob);

/* copying */

jx_ob jx_ob_copy(jx_ob ob); /* deep (recursive) copy */

/* variable length array (vla) functions provide untyped, auto-zeroed,
   size and record-length aware, variable length arrays */

void *jx_vla_new(jx_int rec_size, jx_int size);
jx_status jx_vla_resize(void **vla, jx_int new_size);
jx_status jx_vla_free(void *vla);

/* lists */

jx_ob jx_list_new(void);
jx_ob jx_list_new_from_float_vla(jx_float *array); /* takes ownership of vla */
jx_ob jx_list_new_from_int_vla(jx_int *array); /* takes ownership of vla */

jx_int jx_list_get_size(jx_ob list);
jx_int jx_list_set_size(jx_ob list, jx_ob fill); /* borrows and copies fill object (as necessary) */

jx_status jx_list_append(jx_ob list, jx_ob ob); /* takes ownership of ob */
jx_status jx_list_insert(jx_ob list, jx_int index, jx_ob ob); /* takes ownership of ob */

jx_ob     jx_list_borrow(jx_ob list, jx_int index); /* borrows ownership of list member */
jx_ob     jx_list_remove(jx_ob list, jx_int index); /* returns ownership of result */
jx_status jx_list_eliminate(jx_ob list, jx_int index); /* free entry at index */

jx_float *jx_list_as_float_array(jx_ob list); /* if list is homogenous float, else NULL */
jx_int   *jx_list_as_int_array(jx_ob list); /* if list is homogenous int, else NULL */

/* hashes */

jx_ob jx_hash_new(void);

jx_ob jx_hash_set(jx_ob hash, jx_ob key, jx_ob value); /* assumes ownership of both key & value */

jx_ob jx_hash_keys(jx_ob hash); /* returns owned list of hash keys */

jx_status jx_hash_has_key(jx_ob hash, jx_ob key); /* borrows key */

jx_ob     jx_hash_borrow(jx_ob hash, jx_ob key); /* borrows key and returns borrowed value */
jx_ob     jx_hash_remove(jx_ob hash, jx_ob key); /* borrows key and returns owned value */
jx_status jx_hash_eliminate(jx_ob hash, jx_ob key); /* borrows key and eliminates value */

/* destroying owned objects */

jx_status jx_ob_free(jx_ob ob);

#endif