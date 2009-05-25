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

#ifndef JX_HEAP_TRACKER

#define JX_TRIP_WIRE_SIZE 16

typedef struct {
  size_t size;
  jx_int active;
  jx_char trip_wire[JX_TRIP_WIRE_SIZE];
} jx_mem_hdr;

typedef struct {
  jx_char trip_wire[JX_TRIP_WIRE_SIZE];
  size_t size;
} jx_mem_ftr;

int jx_mem_count = 0, jx_mem_count_max = 0;
void jx_mem_dump(void)
{
  if(JX_MEM_LOG_ALL || JX_MEM_LOG_SUMMARY || jx_mem_count)
    fprintf(stderr, "jx_mem_count: %d of %d heap blocks remain allocated.\n",
            jx_mem_count, jx_mem_count_max);
}

int jx_mem_count_init = 1;
static void jx_mem_count_inc(void)
{
  if(jx_mem_count_init) {
    jx_mem_count_init = 0;
    atexit(jx_mem_dump);
  }
  jx_mem_count++;               /* not atomic - not thread-accurate */
  if(jx_mem_count_max < jx_mem_count)
    jx_mem_count_max = jx_mem_count;
}

static void jx_mem_count_dec(void)
{
  jx_mem_count--;               /* not atomic - not thread-accurate */
}

static void *jx_trip_wire_set(void *ptr, size_t size)
{
  jx_mem_hdr *hdr = (jx_mem_hdr *) ptr;
  void *result = hdr + 1;
  jx_mem_ftr *ftr = (jx_mem_ftr *) (((jx_char *) result) + size);
  hdr->active = 0x1EE254EC;
  hdr->size = size;
  ftr->size = size;
  {
    jx_char seed = ((((jx_char *) hdr) - (jx_char *) 0) >> 2) + size;
    jx_char *h_tw = hdr->trip_wire, *f_tw = ftr->trip_wire;
    jx_int i;
    for(i = 0; i < JX_TRIP_WIRE_SIZE; i++) {
      *(h_tw++) = *(f_tw++) = seed;
      seed += size;
    }
  }
  return result;
}

static void *jx_trip_wire_check(void *ptr, char *file, int line)
{
  jx_mem_hdr *hdr = ((jx_mem_hdr *) ptr) - 1;
  jx_mem_ftr *ftr = (jx_mem_ftr *) ((jx_char *) ptr + hdr->size);
  if(hdr->active == 0xDEADDEAD) {
    fprintf(stderr, "jx_trip_wire: %p has been freed twice (%s:%d)\n", ptr, file, line);
  } else if(hdr->active != 0x1EE254EC) {
    fprintf(stderr, "jx_trip_wire: %p is not an active block (%s:%d)\n", ptr, file, line);
  } else if(hdr->size != ftr->size) {
    fprintf(stderr, "jx_trip_wire: %p sizes do not match (0x%x != 0x%x) (%s:%d)\n",
            ptr, (jx_uint)hdr->size, (jx_uint)ftr->size, file, line);
  } else {
    size_t size = hdr->size;
    jx_char seed = ((((jx_char *) hdr) - (jx_char *) 0) >> 2) + size;
    jx_char *h_tw = hdr->trip_wire, *f_tw = ftr->trip_wire;
    int i;
    for(i = 0; i < JX_TRIP_WIRE_SIZE; i++) {
      if((*(h_tw++) != seed)) {
        fprintf(stderr, "jx_trip_wire: %p header overrun (byte 0x%x) (%s:%d)\n", ptr, i,
                file, line);
      }
      if((*(f_tw++) != seed)) {
        fprintf(stderr, "jx_trip_wire: %p footer overrun (byte 0x%x) (%s:%d)\n", ptr, i,
                file, line);
      }
      seed += size;
    }
  }
  hdr->active = 0xDEADDEAD;
  return (void *) hdr;
}


void *jx__malloc(size_t size, char *file, int line)
{
  void *result = malloc(size + sizeof(jx_mem_hdr) + sizeof(jx_mem_ftr));
  jx_mem_count_inc();
  if(JX_MEM_LOG_ALL)
    fprintf(stderr, "jx_malloc: %p (%x) (%s:%d)\n", result, (jx_uint)size, file, line);
  return jx_trip_wire_set(result, size);
}


void *jx__calloc(size_t count, size_t size, char *file, int line)
{
  void *ptr = calloc(count, size + sizeof(jx_mem_hdr) + sizeof(jx_mem_ftr));
  void *result = jx_trip_wire_set(ptr, size);
  jx_mem_count_inc();
  if(JX_MEM_LOG_ALL)
    fprintf(stderr, "jx_calloc: %p (%x,%x) (%s:%d)\n", result, (jx_uint)count, 
	    (jx_uint)size, file, line);
  return result;
}

void *jx__realloc(void *ptr, size_t size, char *file, int line)
{
  void *true_ptr = jx_trip_wire_check(ptr, file, line);
  void *new_ptr = realloc(true_ptr, size + sizeof(jx_mem_hdr) + sizeof(jx_mem_ftr));
  if(new_ptr) {
    void *result = jx_trip_wire_set(new_ptr, size);
    if(JX_MEM_LOG_ALL)
      fprintf(stderr, "jx_realloc: %p -> %p (%x) (%s:%d)\n", ptr, result, (jx_uint)size, file,
              line);
    return result;
  } else {
    fprintf(stderr, "jx_realloc: failed %p (%x) (%s:%d)\n", ptr, (jx_uint)size, file, line);
    return NULL;
  }
}

void jx__free(void *ptr, char *file, int line)
{
  void *true_ptr = jx_trip_wire_check(ptr, file, line);
  jx_mem_count_dec();
  if(JX_MEM_LOG_ALL)
    fprintf(stderr, "jx_free: %p (%s:%d)\n", ptr, file, line);
  free(true_ptr);
}

#endif
