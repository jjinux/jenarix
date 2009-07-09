
/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/

#include "jx_os_process_private.h"
#include "jx_heap.h"

jx_os_process *jx_os_Process = JX_NULL;

#ifdef JX_OS_ATOMICITY_MUTEX
jx_os_mutex jx_os_AtomicityMutex;
#endif

jx_status jx__os_process_init(int argc, char *argv[],
                              int num_size, int tiny_size, int ptr_size, int ob_size)
{
  return jx__os__process_init(argc, argv, num_size, tiny_size, ptr_size, ob_size);
}

jx_status jx_os_process_complete(void)
{
  return jx__os_process_complete();
}

jx_status jx_os_usleep(jx_size usec)
{
  return jx__os_usleep(usec);
}

jx_status jx_os_thread_array_new(jx_os_thread ** result, jx_size size)
{
  return JX_HEAP_CALLOC(result, jx_os_thread, size);
}

jx_os_thread *jx_os_thread_array_entry(jx_os_thread * thread, jx_size offset)
{
  return thread + offset;
}

jx_status jx_os_thread_array_free(jx_os_thread ** result)
{
  return JX_HEAP_FREE(result);
}

jx_status jx_os_thread_start(jx_os_thread * thread, jx_os_thread_fn func, void *arg)
{
  return jx__os_thread_start(thread, func, arg);
}

jx_status jx_os_thread_join(jx_os_thread * thread)
{
  return jx__os_thread_join(thread);
}

jx_status jx_os_thread_get_current(jx_os_thread * thread)
{
  return jx__os_thread_get_current(thread);
}

jx_status jx_os_mutex_array_new(jx_os_mutex ** result, jx_size size)
{
  return JX_HEAP_CALLOC(result, jx_os_mutex, size);
}

jx_os_mutex *jx_os_mutex_array_entry(jx_os_mutex * mutex, jx_size offset)
{
  return mutex + offset;
}

jx_status jx_os_mutex_array_free(jx_os_mutex ** result)
{
  return JX_HEAP_FREE(result);
}

jx_status jx_os_mutex_init(jx_os_mutex * mutex)
{
  return jx__os_mutex_init(mutex);
}

jx_status jx_os_mutex_lock(jx_os_mutex * mutex)
{
  return jx__os_mutex_lock(mutex);
}

jx_status jx_os_mutex_unlock(jx_os_mutex * mutex)
{
  return jx__os_mutex_unlock(mutex);
}

jx_status jx_os_mutex_trylock(jx_os_mutex * mutex)
{
  return jx__os_mutex_trylock(mutex);
}

jx_status jx_os_mutex_destroy(jx_os_mutex * mutex)
{
  return jx__os_mutex_destroy(mutex);
}

jx_status jx_os_cond_array_new(jx_os_cond ** result, jx_size size)
{
  return JX_HEAP_CALLOC(result, jx_os_cond, size);
}

jx_os_cond *jx_os_cond_array_entry(jx_os_cond * cond, jx_size offset)
{
  return cond + offset;
}

jx_status jx_os_cond_array_free(jx_os_cond ** result)
{
  return JX_HEAP_FREE(result);
}

jx_status jx_os_cond_init(jx_os_cond * cond)
{
  return jx__os_cond_init(cond);
}

jx_status jx_os_cond_wait(jx_os_cond * cond, jx_os_mutex * mutex)
{
  return jx__os_cond_wait(cond, mutex);
}

jx_status jx_os_cond_timedwait(jx_os_cond * cond, jx_os_mutex * mutex, jx_size usec)
{
  return jx__os_cond_timedwait(cond, mutex, usec);
}

jx_status jx_os_cond_signal(jx_os_cond * cond)
{
  return jx__os_cond_signal(cond);
}

jx_status jx_os_cond_broadcast(jx_os_cond * cond)
{
  return jx__os_cond_broadcast(cond);
}

jx_status jx_os_cond_destroy(jx_os_cond * cond)
{
  return jx__os_cond_destroy(cond);
}

jx_status jx_os_tls_set(jx_os_tls * tls, void *value)
{
  return jx__os_tls_set(tls, value);
}

jx_status jx_os_tls_get(jx_os_tls * tls, void **value)
{
  return jx__os_tls_get(tls, value);
}

jx_status jx_os_tls_init(jx_os_tls * tls)
{
  return jx__os_tls_init(tls);
}

jx_status jx_os_tls_destroy(jx_os_tls * tls)
{
  return jx__os_tls_destroy(tls);
}

jx_status jx_os_rlock_array_new(jx_os_rlock ** result, jx_size size)
{
  return JX_HEAP_CALLOC(result, jx_os_rlock, size);
}

jx_os_rlock *jx_os_rlock_array_entry(jx_os_rlock * rlock, jx_size offset)
{
  return rlock + offset;
}

jx_status jx_os_rlock_array_free(jx_os_rlock ** result)
{
  return JX_HEAP_FREE(result);
}

jx_status jx_os_rlock_init(jx_os_rlock * rlock)
{
  return jx__os_rlock_init(rlock);
}

jx_status jx_os_rlock_acquire(jx_os_rlock * rlock, jx_bool blocking)
{
  return jx__os_rlock_acquire(rlock, blocking);
}

jx_status jx_os_rlock_release(jx_os_rlock * rlock)
{
  return jx__os_rlock_release(rlock);
}

jx_status jx_os_rlock_destroy(jx_os_rlock * rlock)
{
  return jx__os_rlock_destroy(rlock);
}

jx_status jx_os_spinlock_acquire(jx_os_spinlock * spinlock, jx_bool spin)
{
  return jx__os_spinlock_acquire(spinlock, spin);
}

jx_status jx_os_spinlock_release(jx_os_spinlock * spinlock)
{
  return jx__os_spinlock_release(spinlock);
}
