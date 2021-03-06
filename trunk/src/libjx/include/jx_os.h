

/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_os
#define _H_jx_os


/* here we basically create a thin layer of "defines" around the OS
   and the compiler */


/* platform-specific hacks */

#ifdef WIN32
#include "stdafx.h"
#endif


/* standard headers we rely upon */

#ifdef WIN32
#include <sys/timeb.h>
#include <time.h>
#include <io.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#ifdef __linux__
#include <malloc.h>
#else
#include <stddef.h>
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef __APPLE__
#include <limits.h>
#else
#include <stdint.h>
#endif


/* memory management */

#define jx_os_malloc malloc
#define jx_os_calloc calloc
#define jx_os_realloc realloc
#define jx_os_free free


/* termination */

#define jx_os_abort abort
#define jx_os_exit exit


/* platform and header-dependent types */

#define jx_os_size_t   size_t
#define jx_os_sizeof(x) ((jx_size)sizeof(x))


/* io */

#define jx_os_fflush fflush
#define jx_os_stdin stdin
#define jx_os_stdout stdout
#define jx_os_stderr stderr

#define jx_os_stdin_fileno STDIN_FILENO
#define jx_os_stdout_fileno STDOUT_FILENO
#define jx_os_stderr_fileno STDERR_FILENO

#define jx_os_random random

#define JX_OS_FILE FILE

#define jx_os_fgets fgets
#define jx_os_feof feof

#define jx_os_printf printf
#define jx_os_fprintf fprintf
#ifdef WIN32
#define jx_os_snprintf _snprintf
#else
#define jx_os_snprintf snprintf
#endif

#define jx_os_sscanf sscanf

#ifndef WIN32
#define jx_os_read read
#define jx_os_close close
#else
#define jx_os_read _read
#define jx_os_close _close
#endif

#define jx_os_exit exit
#define jx_os_abort abort

/* commonly used routines */

#define jx_os_memset memset
#define jx_os_memcpy memcpy
#define jx_os_memmove memmove
#define jx_os_strcat strcat
#define jx_os_strcpy strcpy
#define jx_os_strncpy strncpy
#define jx_os_strcmp strcmp
#define jx_os_strncmp strncmp
#define jx_os_strlen(x) ((jx_size)strlen(x))
#define jx_os_strstr strstr
#define jx_os_qsort qsort
#define jx_os_rand rand

#define jx_os_fopen fopen

/* declare our own C types (to be used exclusively throughout) */

typedef int jx_bool;
typedef char jx_char;
typedef int jx_status;

typedef unsigned short jx_uint16;       /* for small bit masks, etc. */

typedef int jx_int32;
typedef unsigned int jx_uint32; /* for hash codes, etc. */

#ifdef WIN32
typedef __int64 jx_int64;
typedef unsigned __int64 jx_uint64;
#else
typedef long long jx_int64;
typedef unsigned long long jx_uint64;
#endif


/* for pointer arithmetic, memory allocations, and
   machine-word-optimal fields */

typedef size_t jx_size;
typedef ptrdiff_t jx_diff;
typedef ptrdiff_t jx_word;


/* define base types */

#ifdef JX_64_BIT

/* use 64-bit integers and double-precision floating point */
typedef jx_int64 jx_int;
typedef jx_uint64 jx_uint;
typedef double jx_float;
#define JX_TINY_STR_MIN_SIZE 12
#define JX_FLOAT_ZERO 0.0
#define JX_UWORD_MAX 0xFFFFFFFFFFFFFFFF

#else

/* use 32-bit integers and single-precision floating point, EVEN if
   -m64 is true */

typedef jx_int32 jx_int;
typedef jx_uint32 jx_uint;
typedef float jx_float;

#define JX_FLOAT_ZERO 0.0F
#define JX_UINT_MAX 0xFFFFFFFF

#if (JX_TINY_STR_SIZE > 0)

/* if using tiny strings, then make sure tiny_str matches pointer
   width, if we are running in 64 bit mode */

#ifdef __APPLE__

/* the following test only works on macs */

#if ((0LL+SIZE_T_MAX) == 0x00000000FFFFFFFFLL)
#define JX_TINY_STR_MIN_SIZE 0
#else
#define JX_TINY_STR_MIN_SIZE 12
#endif

#else

/* if we're not on mac, do we have stdint.h defines? */

#ifdef UINTPTR_MAX

#if (UINTPTR_MAX) == (4294967295U)
#define JX_TINY_STR_MIN_SIZE 0
#else
#define JX_TINY_STR_MIN_SIZE 12
#endif

#else

/* not on mac, but do we have PTRDIFF_MAX? */

#ifdef PTRDIFF_MAX

#if (PTRDIFF_MAX) == (2147483647)
#define JX_TINY_STR_MIN_SIZE 0
#else
#define JX_TINY_STR_MIN_SIZE 12
#endif

#endif
/* end ifdef PTRDIFF_MAX */

#endif
/* end ifdef UINTPTR_MAX */

#endif
/* end ifdef __APPLE__ */

#endif
/* end if (JX_TINY_STR_SIZE > 0) */

#endif
/* end ifdef JX_64_BIT, else clause */


/* processes, threads, locks, etc. */

#ifdef __linux__
typedef volatile jx_int32 jx_os_atomic32;
#else
#if defined(__MACH__) && defined(__APPLE__)
typedef volatile int32_t jx_os_atomic32;
#else
#ifdef WIN32
typedef volatile LONG jx_os_atomic32;
#endif
#endif
#endif

struct jx__os_spinlock {
  jx_os_atomic32 atomic;
};

typedef struct jx__os_process jx_os_process;
typedef struct jx__os_thread jx_os_thread;
typedef struct jx__os_mutex jx_os_mutex;
typedef struct jx__os_cond jx_os_cond;
typedef struct jx__os_tls jx_os_tls;
typedef struct jx__os_rlock jx_os_rlock;
typedef struct jx__os_spinlock jx_os_spinlock;


/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif
jx_status jx_os_usleep(jx_size usec);

jx_status jx_os_thread_array_new(jx_os_thread ** result, jx_size count);
jx_os_thread *jx_os_thread_array_entry(jx_os_thread * thread, jx_size offset);
jx_status jx_os_thread_array_free(jx_os_thread ** result);

typedef void *(*jx_os_thread_fn) (void *);

jx_status jx_os_thread_start(jx_os_thread * thread, jx_os_thread_fn func, void *arg);
jx_status jx_os_thread_join(jx_os_thread * thread);
jx_status jx_os_thread_get_current(jx_os_thread * thread);

jx_status jx_os_mutex_array_new(jx_os_mutex ** result, jx_size size);
jx_os_mutex *jx_os_mutex_array_entry(jx_os_mutex * mutex, jx_size offset);
jx_status jx_os_mutex_array_free(jx_os_mutex ** result);

jx_status jx_os_mutex_init(jx_os_mutex * mutex);
jx_status jx_os_mutex_lock(jx_os_mutex * mutex);
jx_status jx_os_mutex_unlock(jx_os_mutex * mutex);
jx_status jx_os_mutex_trylock(jx_os_mutex * mutex);
jx_status jx_os_mutex_destroy(jx_os_mutex * mutex);

jx_status jx_os_cond_array_new(jx_os_cond ** result, jx_size size);
jx_os_cond *jx_os_cond_array_entry(jx_os_cond * cond, jx_size offset);
jx_status jx_os_cond_array_free(jx_os_cond ** result);

jx_status jx_os_cond_init(jx_os_cond * cond);
jx_status jx_os_cond_wait(jx_os_cond * cond, jx_os_mutex * mutex);
jx_status jx_os_cond_timedwait(jx_os_cond * cond, jx_os_mutex * mutex, jx_size usec);
jx_status jx_os_cond_signal(jx_os_cond * cond);
jx_status jx_os_cond_broadcast(jx_os_cond * cond);
jx_status jx_os_cond_destroy(jx_os_cond * cond);

jx_status jx_os_tls_set(jx_os_tls * tls, void *value);
jx_status jx_os_tls_get(jx_os_tls * tls, void **value);
jx_status jx_os_tls_init(jx_os_tls * tls);
jx_status jx_os_tls_destroy(jx_os_tls * tls);

jx_status jx_os_rlock_array_new(jx_os_rlock ** result, jx_size size);
jx_status jx_os_rlock_array_free(jx_os_rlock ** result);
jx_status jx_os_rlock_init(jx_os_rlock * rlock);
jx_status jx_os_rlock_acquire(jx_os_rlock * rlock, jx_bool blocking);
jx_status jx_os_rlock_release(jx_os_rlock * rlock);
jx_status jx_os_rlock_destroy(jx_os_rlock * rlock);

jx_status jx_os_spinlock_acquire(jx_os_spinlock * spinlock, jx_bool spin);
jx_status jx_os_spinlock_release(jx_os_spinlock * spinlock);


/* enable C++ mangling */
#ifdef __cplusplus
#if 0
{
#endif
}
#endif


/* platform-specific time stuff */

#ifndef WIN32
typedef struct timeval jx_os_timeval;
#define jx_os_select select
#endif


/* convenience aliases */

#define jx_printf jx_os_printf
#define jx_fprintf  jx_os_fprintf
#define jx_snprintf jx_os_snprintf
#define jx_fflush jx_os_fflush
#define jx_stdin jx_os_stdin
#define jx_stderr jx_os_stderr
#define jx_stdout jx_os_stdout
#define jx_strlen jx_os_strlen
#define jx_strstr jx_os_strstr


/* defines */

#ifdef __cplusplus
#define JX_NULL 0
#else
#define JX_NULL ((void*)0)
#endif


/* for maximum efficiency: 

  JX__FILE__ -> NULL
  JX__LINE__ -> 0   

 * for debugging:

  JX__FILE__ -> __FILE__ 
  JX__LINE__ -> __LINE__ 
*/

#define JX__FILE__ __FILE__
#define JX__LINE__ __LINE__


/* how do we declare inline methods? */

#ifdef WIN32
#define JX_INLINE static
#else
#define JX_INLINE __inline__ static
#endif

/* how do we declare static methods */

#define JX_STATIC static

/* constants we rely upon */

#define JX_EXIT_SUCCESS EXIT_SUCCESS
#define JX_EXIT_FAILURE EXIT_FAILURE

/* workaround for compilers which disallow [0] size arrays */

#ifdef __GNUC__

/* array[] is permitted in the C99 standard, and GNUC supports this */
#define JX_ZERO_ARRAY_SIZE 

#else

#define JX_ZERO_ARRAY_SIZE 1
#endif

#endif
