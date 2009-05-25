/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_os_nthreads
#define _H_jx_os_nthreads

#ifdef WIN32
#include <windows.h>
#endif

#include "jx_os_process.h"

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

/* jx_os_nothreads.h placeholder compatibility/stub library for
   single-threaded Jenarix-based applications */

struct jx__os_thread {
  int id;
};

struct jx__os_mutex {
  int locked;
};

struct jx__os_cond {
  int signalled;
};

struct jx__os_tls {
  void *value;
};

struct jx__os_rlock {
  jx_os_mutex mutex;
  jx_boolean owned;
  jx_word count;
};

typedef jx_int32 jx_os_atomic32;

JX_INLINE_STATIC jx_int32 jx_os__cas(volatile jx_int32 *ptr, 
				    jx_int32 old_value,
				    jx_int32 new_value)
{
  if(*ptr == old_value) {
    *ptr = new_value;
    return old_value;
  } else {
    return *ptr;
  }
}

#define JX_OS_CAS(mem, old_value, new_value) \
        jx_os__cas(mem, old_value, new_value)

JX_INLINE_STATIC jx_int32 jx_os_atomic32_read(jx_os_atomic32 *atomic)
{
  return *atomic;
}

JX_INLINE_STATIC jx_int32 jx_os_atomic32_decrement(jx_os_atomic32 *atomic)
{
  return --(*atomic);
}

JX_INLINE_STATIC jx_int32 jx_os_atomic32_increment(jx_os_atomic32 *atomic)
{
  return ++(*atomic);
}

JX_INLINE_STATIC jx_int32 jx_os_atomic32_exchange(jx_os_atomic32 *atomic,
						  jx_os_atomic32 new_value)
{
  int result = *atomic;
  *atomic = new_value;
  return result;
}

JX_INLINE_STATIC jx_boolean jx_os_atomic32_cas(jx_os_atomic32 *atomic, 
                                               jx_os_atomic32 new_value,
                                               jx_os_atomic32 old_value)
{
  return (JX_OS_CAS(atomic,old_value,new_value)==old_value);
}

#ifndef WIN32
JX_INLINE_STATIC jx_status jx__os_usleep(jx_size usec)
{
  jx_os_timeval tv;

  tv.tv_usec = usec % 1000000;
  tv.tv_sec = usec / 1000000;
  
  jx_os_select(0,NULL,NULL,NULL,&tv);
  return JX_SUCCESS;
}
#else
JX_INLINE_STATIC jx_status jx__os_usleep(jx_size usec)
{
  // win32 API only provides millisec resolution
  int msec = ((usec+499)/1000);
  if(usec & !msec) {
    // round up to 1 msec if usec is nonzero
    msec = 1;
  }
  Sleep(msec);
  return JX_SUCCESS;
}
#endif

JX_INLINE_STATIC jx_status jx__os_thread_start(jx_os_thread *thread, void (*func)(void *), void *arg)
{
  return JX_STATUS_OS_THREAD_CREATION_FAILURE;
}

JX_INLINE_STATIC jx_status jx__os_thread_join(jx_os_thread *thread)
{
  return JX_STATUS_OS_THREADING_ERROR;
}

JX_INLINE_STATIC jx_status jx__os_thread_get_current(jx_os_thread *thread)
{
  return JX_STATUS_SUCCESS;
}

JX_INLINE_STATIC jx_status jx__os_mutex_init(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    JX_NULLIFY(*mutex);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_lock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    if(!mutex->locked)
      mutex->locked = JX_TRUE;
    else
      status = JX_STATUS_OS_MUTEX_ERROR;
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_unlock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    if(mutex->locked)
      mutex->locked = JX_FALSE;
    else
      status = JX_STATUS_OS_MUTEX_ERROR;
  }
  return status;

}

JX_INLINE_STATIC jx_status jx__os_mutex_trylock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    if(mutex->locked) {
      status = JX_STATUS_NO;
    } else {
      mutex->locked = JX_TRUE;
      status = JX_STATUS_YES;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_destroy(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_init(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    JX_NULLIFY(*cond);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_wait(jx_os_cond *cond, jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(cond && mutex);
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_timedwait(jx_os_cond *cond, jx_os_mutex *mutex, jx_size usec)
{
  jx_status status = JX_PTR(cond && mutex);
  if(JX_OK(status)) {
    status = JX_STATUS_YES;
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_signal(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_broadcast(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_destroy(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_set(jx_os_tls *tls, void *value)
{
  jx_status status = JX_PTR(tls);
  if(JX_OK(status)) {
    tls->value = value;
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_get(jx_os_tls *tls, void **value)
{
  jx_status status = JX_PTR(tls && value);
  if(JX_OK(status)) {
    *value = tls->value;
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_init(jx_os_tls *tls)
{
  jx_status status = JX_PTR(tls);
  if(JX_OK(status)) {
    JX_NULLIFY(*tls);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_destroy(jx_os_tls *tls)
{
  jx_status status = JX_PTR(tls);
  return status;
}

JX_INLINE_STATIC jx_status jx__os_rlock_init(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    JX_NULLIFY(*rlock);
    status = jx__os_mutex_init(&rlock->mutex);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_rlock_acquire(jx_os_rlock *rlock,jx_boolean blocking)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    if(rlock->owned) {
      status = JX_STATUS_YES;
      rlock->count++;
    } else if(blocking) {
      if(JX_OK( jx__os_mutex_lock(&rlock->mutex) )) {
        status = JX_STATUS_YES;
        rlock->owned = JX_TRUE;
        rlock->count++;
      }
    } else {
      status = jx__os_mutex_trylock(&rlock->mutex);      
      if(status == JX_STATUS_YES) {
        rlock->owned = JX_TRUE;
        rlock->count++;
      }
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_rlock_release(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    if(rlock->owned) {
      if((rlock->count--) == 1) {
        rlock->owned = JX_FALSE;
        jx__os_mutex_unlock(&rlock->mutex);
      } else if(!(rlock->count>0)) {
        status = JX_STATUS_OS_RLOCK_ERROR;
      }
    } else {
      status = JX_STATUS_OS_RLOCK_ERROR;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_rlock_destroy(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    status = jx__os_mutex_destroy(&rlock->mutex);
  }
  return status;
}

/* enable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#endif
