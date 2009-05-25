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
#define _H_jx_os_plocess_private

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

struct jx__os_process {
#ifdef JX_HEAP_TRACKER_MUTEX
  jx_os_mutex heap_mutex;
#endif
  jx_os_tls node_tls; /* TLS key for locating the current node (if any) */
};

extern jx_os_process *jx_os_Process;

JX_INLINE jx_status jx__os_process_init(int argc, const char* argv[])
{
  if(!jx_os_Process) {

#ifdef JX_OS_ATOMICITY_MUTEX
    jx_os_mutex_init(&jx_os_AtomicityMutex);
#endif

    jx_os_Process = (jx_os_process*)jx_os_calloc(1,jx_sizeof(jx_os_process));
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
