/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_os_process
#define _H_jx_os_plocess

#include "jx_public.h"

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

  extern jx_os_process *jx_os_Process;

  jx_status jx_os_process_init(int argc, const char* argv[]);
  
  jx_status jx_os_process_complete(void);
  jx_status jx_os_usleep(jx_size usec);
  
  jx_status jx_os_thread_array_new(jx_os_thread **result, jx_size count);
  jx_os_thread *jx_os_thread_array_entry(jx_os_thread *thread, jx_size offset);
  jx_status jx_os_thread_array_free(jx_os_thread **result);
  
  jx_status jx_os_thread_start(jx_os_thread *thread, void (*func)(void *), void *arg);
  jx_status jx_os_thread_join(jx_os_thread *thread);
  jx_status jx_os_thread_get_current(jx_os_thread *thread);
  
  jx_status jx_os_mutex_array_new(jx_os_mutex **result, jx_size size);
  jx_os_mutex *jx_os_mutex_array_entry(jx_os_mutex *mutex, jx_size offset);
  jx_status jx_os_mutex_array_free(jx_os_mutex **result);

  jx_status jx_os_mutex_init(jx_os_mutex *mutex);
  jx_status jx_os_mutex_lock(jx_os_mutex *mutex);
  jx_status jx_os_mutex_unlock(jx_os_mutex *mutex);
  jx_status jx_os_mutex_trylock(jx_os_mutex *mutex);
  jx_status jx_os_mutex_destroy(jx_os_mutex *mutex);
  
  jx_status jx_os_cond_array_new(jx_os_cond **result, jx_size size);
  jx_os_cond *jx_os_cond_array_entry(jx_os_cond *cond, jx_size offset);
  jx_status jx_os_cond_array_free(jx_os_cond **result);

  jx_status jx_os_cond_init(jx_os_cond *cond);
  jx_status jx_os_cond_wait(jx_os_cond *cond, jx_os_mutex *mutex);
  jx_status jx_os_cond_timedwait(jx_os_cond *cond, jx_os_mutex *mutex, jx_size usec);
  jx_status jx_os_cond_signal(jx_os_cond *cond);
  jx_status jx_os_cond_broadcast(jx_os_cond *cond);
  jx_status jx_os_cond_destroy(jx_os_cond *cond);
  
  jx_status jx_os_tls_set(jx_os_tls *tls, void *value);
  jx_status jx_os_tls_get(jx_os_tls *tls, void **value);
  jx_status jx_os_tls_init(jx_os_tls *tls);
  jx_status jx_os_tls_destroy(jx_os_tls *tls);

  jx_status jx_os_rlock_array_new(jx_os_rlock **result, jx_size size);
  jx_status jx_os_rlock_array_free(jx_os_rlock **result);
  jx_status jx_os_rlock_init(jx_os_rlock *rlock);
  jx_status jx_os_rlock_acquire(jx_os_rlock *rlock, jx_bool blocking);
  jx_status jx_os_rlock_release(jx_os_rlock *rlock);
  jx_status jx_os_rlock_destroy(jx_os_rlock *rlock);

/* enable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#endif
