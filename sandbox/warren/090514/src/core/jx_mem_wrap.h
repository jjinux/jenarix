#ifndef _H_jx_mem_wrap
#define _H_jx_mem_wrap

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

#define JX_MEM_LOG_ALL 0
#define JX_MEM_LOG_SUMMARY 1

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#ifndef JX_MEM_WRAP

/* just use system malloc, etc. */

#define jx_malloc(s) malloc(s)
#define jx_calloc(c,s) calloc(c,s)
#define jx_realloc(p,s) realloc(p,s)
#define jx_free(p) free(p)
#define jx_mem_dump() 

#else
 
#ifndef JX_HEAP_TRACKER

/* use our lightweight (threads blind) memory logger, counter, &
   overrun detector */

#define jx_malloc(s) jx__malloc(s, JX__FILE__, JX__LINE__)
#define jx_calloc(c,s) jx__calloc(c, s, JX__FILE__, JX__LINE__)
#define jx_realloc(p,s) jx__realloc(p, s, JX__FILE__, JX__LINE__)
#define jx_free(p) jx__free(p, JX__FILE__, JX__LINE__)

void jx_mem_dump(void);

void *jx__malloc(jx_size size, char *file, int line);
void *jx__malloc(jx_size size, char *file, int line);
void *jx__calloc(jx_size count, jx_size size, char *file, int line);
void *jx__realloc(void *ptr, jx_size size, char *file, int line);
void jx__free(void *ptr, char *file, int line);

#else

/* use our robust, thread-safe, status-returning JX_HEAP system */

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#include "jx_heap.h"

/* enable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#define jx_mem_dump jx_heap_dump(0);

#define jx_malloc JX_HEAP_MALLOC_RAW_VOID(size);
#define jx_calloc(c,s) JX_HEAP_CALLOC_RAW_VOID(((c)*(s)));
#define jx_realloc(p,s) JX_HEAP_REALLOC_RAW_VOID(p,s)
#define jx_free(p) JX_HEAP_VLA_FREE_RAW(p)

#endif
#endif

/* enable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#endif

