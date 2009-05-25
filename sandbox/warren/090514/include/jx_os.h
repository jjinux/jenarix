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

#define JX_OS_FILE FILE

#define jx_os_fgets fgets
#define jx_os_feof feof

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
#define jx_os_qsort qsort
#define jx_os_rand rand

/* processes and threading */

typedef struct jx__os_process jx_os_process;
typedef struct jx__os_thread jx_os_thread;
typedef struct jx__os_mutex jx_os_mutex;
typedef struct jx__os_cond jx_os_cond;
typedef struct jx__os_tls jx_os_tls;
typedef struct jx__os_rlock jx_os_rlock;

/* platform-specific time stuff */

#ifndef WIN32
typedef struct timeval jx_os_timeval;
#define jx_os_select select
#endif

/* convenience aliases */

#define jx_fprintf  jx_os_fprintf
#define jx_snprintf jx_os_snprintf
#define jx_fflush jx_os_fflush
#define jx_stdin jx_os_stdin
#define jx_stderr jx_os_stderr
#define jx_stdout jx_os_stdout
#define jx_strlen jx_os_strlen

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


/* workaround for compilers which disallow [0] size arrays */

#define JX_ZERO_ARRAY_SIZE 1

#endif


