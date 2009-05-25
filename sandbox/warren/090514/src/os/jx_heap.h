/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_heap
#define _H_jx_heap

#include "jx_public.h"

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

/* Note -- this version of tracker is thread-save (but not
   thread-efficient) due to use of a global mutex */

#define JX_HEAP_DUMP_FILES_TOO      0x1
#define JX_HEAP_DUMP_NO_ADDRESSES   0x2
#define JX_HEAP_DUMP_SUMMARY_ONLY   0x4

typedef struct {
  jx_os_size_t size, alloc, unit_size;
  jx_bool auto_zero;
} Jx_HeapVLA;

#ifdef JX_HEAP_TRACKER

jx_status jx_heap_dump(jx_int32 flags);
jx_size jx_heap_usage(void);
#define _JX_HEAP_TRACKER_CALL , JX__FILE__, JX__LINE__
#define _JX_HEAP_TRACKER_DECL , char *file, int line

#else

JX_INLINE jx_status jx_heap_dump(jx_int32 flags) { return JX_SUCCESS; }
JX_INLINE jx_size jx_heap_usage(void) { return 0; }

#define _JX_HEAP_TRACKER_CALL
#define _JX_HEAP_TRACKER_DECL

#endif

/* macros */

/* NOTE: all normal FREE methods set result to NULL in order to reduce
   the chance of successfully accessing deallocated memory by accident
   and to cause errors to surface ASAP */

/* RAW suffix means that we return the result instead of returning a status
   and passing the result into the memory pointed to by the first
   argument */

/* VOID suffix means that return a void* and that we don't atomically scale
   arrays by the unit array element size */

/* RECOPY suffix means that copy data into new memory and free the
   original pointer.  This is necesssary to actually reclaim memory on
   some operating systems (e.g. versions of Darwin) */

#define JX_HEAP_ALLOC(result, type) \
   jx_heap_Calloc((void**)result, jx_sizeof(type) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_ALLOC_RAW(type) \
  ((type*)jx_heap_CallocRaw(jx_sizeof(type) _JX_HEAP_TRACKER_CALL))

#define JX_HEAP_ALLOC_RAW_VOID(size) \
  jx_heap_CallocRaw(size _JX_HEAP_TRACKER_CALL)


#define JX_HEAP_MALLOC(result, type, size) \
   jx_heap_Malloc((void**)result,jx_sizeof(type)*(size) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_MALLOC_RAW(type, size) \
  ((type*)jx_heap_MallocRaw(jx_sizeof(type)*(size) _JX_HEAP_TRACKER_CALL))

#define JX_HEAP_MALLOC_RAW_VOID(size) \
  jx_heap_MallocRaw(size _JX_HEAP_TRACKER_CALL)


#define JX_HEAP_CALLOC(result, type, size) \
  jx_heap_Calloc((void**)result,jx_sizeof(type)*(size) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_CALLOC_VOID(result, size) \
  jx_heap_Calloc((void**)result,size _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_CALLOC_RAW(type, size) \
  ((type*)jx_heap_CallocRaw(jx_sizeof(type)*(size) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_CALLOC_RAW_VOID(size) \
  jx_heap_CallocRaw(size _JX_HEAP_TRACKER_CALL)



#define JX_HEAP_REALLOC(result, type, size) \
  jx_heap_Realloc((void**)result,jx_sizeof(type)*(size) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_REALLOC_RECOPY(result, type, new_size, cur_size) \
  jx_heap_ReallocRecopy((void**)result,jx_sizeof(type)*(new_size),jx_sizeof(type)*(cur_size) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_REALLOC_RAW(ptr,type, size) \
  ((type*)jx_heap_ReallocRaw((void*)ptr,jx_sizeof(type)*(size) _JX_HEAP_TRACKER_CALL))

#define JX_HEAP_REALLOC_RAW_RECOPY(ptr, type, new_size, cur_size) \
   ((type*)jx_heap_ReallocRawRecopy(ptr,jx_sizeof(type)*(new_size),jx_sizeof(type)*(cur_size) _JX_HEAP_TRACKER_CALL))

#define JX_HEAP_REALLOC_RAW_VOID(ptr, size) \
  jx_heap_ReallocRaw((void*)ptr,(size) _JX_HEAP_TRACKER_CALL)


#define JX_HEAP_RECALLOC(result, type, size, old_size)                  \
  jx_heap_Recalloc((void**)result,jx_sizeof(type)*(size),jx_sizeof(type)*(old_size) _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_RECALLOC_VOID(result, size, old_size)                \
  jx_heap_Recalloc((void**)result,size,old_size _JX_HEAP_TRACKER_CALL)


#define JX_HEAP_FREE(result) jx_heap_Free((void**)result _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_FREE_RAW(ptr) jx_heap_FreeRaw((void*)ptr _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_AUTO_FREE(result) \
   ((*(result)) ? jx_heap_Free((void**)(result) _JX_HEAP_TRACKER_CALL) : JX_SUCCESS)

#define JX_HEAP_VLA_MALLOC(result, type, init_size) \
   jx_heap_VlaAlloc((void**)result, jx_sizeof(type), init_size, JX_FALSE _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_MALLOC_RAW(type, init_size) \
  jx_heap_VlaAllocRaw(jx_sizeof(type), init_size, JX_FALSE _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_CALLOC(result, type, init_size) \
   jx_heap_VlaAlloc((void**)result,jx_sizeof(type),init_size,JX_TRUE _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_CALLOC_RAW(type, init_size) \
   jx_heap_VlaAllocRaw(jx_sizeof(type),init_size,JX_TRUE _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_MALLOC_VOID(result, unit_size, init_size) \
   jx_heap_VlaAlloc((void**)(result), unit_size, init_size, JX_FALSE _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_CALLOC_VOID(result, unit_size, init_size) \
   jx_heap_VlaAlloc((void**)(result), unit_size, init_size, JX_TRUE _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_CLONE(result, ptr) \
  jx_heap_VlaClone((void**)result,ptr _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_CLONE_RAW(ptr) \
  jx_heap_VlaCloneRaw(ptr _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_FREE(result) \
   jx_heap_VlaFree((void**)result _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_FREE_RAW(array) \
   jx_heap_VlaFreeRaw((void*)array _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_AUTO_FREE(result) \
   ((*(result)) ? jx_heap_VlaFree((void**)(result) _JX_HEAP_TRACKER_CALL) : JX_SUCCESS)

#define JX_HEAP_VLA_ADD_INDEX(result,idx) \
   jx_heap_VlaAddIndex((void**)(result),idx _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_ADD_INDEX_RAW(ptr,idx) \
   jx_heap_VlaAddIndexRaw((void*)(ptr),idx _JX_HEAP_TRACKER_CALL)

#if 0
/* consider... */
#define JX_HEAP_VLA_CHECK(result,idx) \
  (((*result) && (idx >= (*result)->size)) ? \
   JX_HEAP_VLA_ADD_INDEX((void**)result,idx) : JX_SUCCESS)
#endif

#define JX_HEAP_VLA_SET_SIZE(result,size) \
   jx_heap_VlaSetSize((void**)(result),size _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_SET_SIZE_RECOPY(result,size) \
   jx_heap_VlaSetSizeRecopy((void**)(result),size _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_SET_SIZE_RAW(ptr,size) \
   jx_heap_VlaSetSizeRaw((void*)(ptr),size _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_SET_SIZE_RAW_RECOPY(ptr,size) \
   jx_heap_VlaSetSizeRawRecopy((void*)(ptr),size _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_GET_SIZE(result,array) \
   jx_heap_VlaGetSize(result,(void*)array _JX_HEAP_TRACKER_CALL)

#define JX_HEAP_VLA_GET_SIZE_RAW(array) \
   jx_heap_VlaGetSizeRaw((void*)array _JX_HEAP_TRACKER_CALL)


/* basic memory allocation */

jx_status jx_heap_Malloc(void **result, jx_size size _JX_HEAP_TRACKER_DECL);

void *jx_heap_MallocRaw(jx_size size _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_Calloc(void **result, jx_size size _JX_HEAP_TRACKER_DECL);

void *jx_heap_CallocRaw(jx_size size _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_Realloc(void **result, jx_size size _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_ReallocRecopy(void **result, jx_size new_size,
                                 jx_size cur_size _JX_HEAP_TRACKER_DECL);

void *jx_heap_ReallocRaw(void *ptr, jx_size size _JX_HEAP_TRACKER_DECL);

void *jx_heap_ReallocRawRecopy(void *ptr, jx_size new_size,
                                jx_size cur_size _JX_HEAP_TRACKER_DECL); 

jx_status jx_heap_Recalloc(void **result, jx_size size, jx_size old_size 
                           _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_Free(void **result _JX_HEAP_TRACKER_DECL );

jx_status jx_heap_FreeRaw(void *ptr _JX_HEAP_TRACKER_DECL);

/* variable-length arrays */

jx_status jx_heap_VlaAlloc(void **result, 
                           jx_size unit_size, jx_size init_size,
                           jx_bool auto_zero _JX_HEAP_TRACKER_DECL);

void *jx_heap_VlaAllocRaw(jx_size unit_size, jx_size init_size,
                          jx_bool auto_zero _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaClone(void **result, void *ptr _JX_HEAP_TRACKER_DECL);

void *jx_heap_VlaCloneRaw(void *ptr _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaFree(void **result _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaFreeRaw(void *ptr _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaAddIndex(void **result, jx_size index _JX_HEAP_TRACKER_DECL);

void *jx_heap_VlaAddIndexRaw(void *ptr, jx_size index _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaSetSize(void **result, jx_size size _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaSetSizeRecopy(void **result, jx_size size _JX_HEAP_TRACKER_DECL);

void *jx_heap_VlaSetSizeRaw(void *ptr, jx_size size _JX_HEAP_TRACKER_DECL);

void *jx_heap_VlaSetSizeRawRecopy(void *ptr, jx_size size _JX_HEAP_TRACKER_DECL);

jx_status jx_heap_VlaGetSize(jx_size *result, void *array _JX_HEAP_TRACKER_DECL);

jx_size jx_heap_VlaGetSizeRaw(void *array _JX_HEAP_TRACKER_DECL);

/* enable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#endif
