/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/

#ifndef _H_jx_os_process_private
#define _H_jx_os_process_private

#include "jx_os_process.h"

#ifdef JX_OS_FAKE_THREADS
#include "jx_os_nothreads.h"
#else
#ifdef WIN32
#include "jx_os_winthreads.h"
#else
#include "jx_os_pthreads.h"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef JX_HEAP_TRACKER
#ifndef JX_OS_FAKE_THREADS
/* either JX_HEAP_TRACKER_MUTEX or JX_HEAP_TRACKER_SPINLOCK
   ...they both suck, but spinlocks perform better... */
#define JX_HEAP_TRACKER_SPINLOCK
#endif
#endif

struct jx__os_process {
#ifdef JX_HEAP_TRACKER_MUTEX
  jx_os_mutex heap_mutex;
#endif
#ifdef JX_HEAP_TRACKER_SPINLOCK
  jx_os_spinlock heap_spinlock;
#endif
  jx_os_tls node_tls; /* TLS key for locating the current node (if any) */
};

extern jx_os_process *jx_os_Process;

JX_INLINE jx_status jx__os__process_init(int argc, char* argv[], 
					 int num_size, int tiny_size,
					 int ptr_size, int ob_size)
{
  if((num_size!=sizeof(jx_int)) ||
     (tiny_size!=JX_TINY_STR_SIZE) ||
     (ptr_size!=sizeof(void*)) ||
     (ob_size!=sizeof(jx_ob))) {
    fprintf(stderr,"FatalError: size mismatch, program vs. library:\n");
    fprintf(stderr,"FatalError: numbers:         %2d    vs.   %2d\n",
	   num_size,(int)sizeof(jx_int));
    fprintf(stderr,"FatalError: tiny strings:    %2d    vs.   %2d\n",
	   tiny_size, (int)JX_TINY_STR_SIZE);
    fprintf(stderr,"FatalError: pointers:        %2d    vs.   %2d\n",
	   ptr_size, (int)sizeof(void*));
    fprintf(stderr,"FatalError: objects:         %2d    vs.   %2d\n",
	   ob_size, (int)sizeof(jx_ob));
    exit(EXIT_FAILURE);
  } else if(!jx_os_Process) {

#ifdef JX_OS_ATOMICITY_MUTEX
    jx_os_mutex_init(&jx_os_AtomicityMutex);
#endif

    jx_os_Process = (jx_os_process*)jx_os_calloc(1,sizeof(jx_os_process));
    { /* msvc6 workaround */
      jx_status status = JX_PTR(jx_os_Process);  
#ifdef JX_HEAP_TRACKER_MUTEX
      if(JX_OK(status)) status = jx_os_mutex_init(&jx_os_Process->heap_mutex);
      /*      if(JX_OK(status)) status = jx_os_mutex_unlock(&jx_os_Process->heap_mutex);*/
#endif
      if(JX_OK(status)) status = jx_os_tls_init(&jx_os_Process->node_tls);
      return status;
	}
  } else {
    return JX_SUCCESS;
  }
}

JX_INLINE jx_status jx__os_process_complete(void)
{
  jx_status status = JX_PTR(jx_os_Process);  
#ifdef JX_HEAP_TRACKER_MUTEX
  if(JX_OK(status)) status = jx_os_mutex_destroy(&jx_os_Process->heap_mutex);
#endif
  if(JX_OK(status)) status = jx_os_tls_destroy(&jx_os_Process->node_tls);
  if(jx_os_Process) jx_os_free(jx_os_Process);
  return status;
}


#ifdef __cplusplus
}
#endif

#endif
