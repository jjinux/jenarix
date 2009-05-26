/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_os_pthreads
#define _H_jx_os_pthreads

#include <pthread.h>
#include <sys/errno.h>
#include <sys/time.h>

#include "jx_os_process.h"

/* disable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#define noJX_OS_TLS_TRACE_DEBUG

struct jx__os_thread {
  pthread_t thread;
};

struct jx__os_mutex {
  pthread_mutex_t mutex;
};

struct jx__os_cond {
  pthread_cond_t cond;
};

struct jx__os_tls {
  pthread_key_t key;
};

struct jx__os_rlock {
  jx_os_mutex mutex;
  jx_bool owned;
  pthread_t owner;
  jx_int count;
};


#ifdef __linux__

typedef volatile jx_int32 jx_os_atomic32;

#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define JX_GCC_ATOMIC_BUILTINS
#else
#if defined(__GNUC__) && (defined(_X86_) || defined(__i386__) || defined(__i586__) || defined(__i686__)) || defined(__x86_64__)
#define JX_GCC_X86_ASM
#endif
#endif

#ifndef JX_GCC_ATOMIC_BUILTINS
#ifndef  JX_GCC_X86_ASM
/* falling back to a mutex for atomic operations (SLOW!) */
#define JX_OS_ATOMICITY_MUTEX
extern jx_os_mutex jx_os_AtomicityMutex;
#else
JX_INLINE jx_int32 jx_os__cas(volatile jx_int32 *ptr, 
				    jx_int32 old_value,
				    jx_int32 new_value)
{
  jx_int32 prev;
  __asm__ __volatile__("lock; cmpxchgl %k1,%2"
		       : "=a"(prev)
		       : "r"(new_value), "m"(*ptr), "0"(old_value)
		       : "memory");
  return prev;
}

#define JX_OS_CAS(mem, old_value, new_value) \
        jx_os__cas(mem, old_value, new_value)
#endif
#endif


JX_INLINE jx_int32 jx_os_atomic32_read(jx_os_atomic32 *atomic)
{
  return *atomic;
}

JX_INLINE jx_int32 jx_os_atomic32_decrement(jx_os_atomic32 *atomic)
{
#ifdef JX_OS_CAS
  jx_os_atomic32 old_value, new_value;
  while(1) {
    old_value = *atomic;
    new_value = old_value - 1;
    if(JX_OS_CAS(atomic,old_value,new_value) == old_value)
      break;
  }
  return new_value;
#else
#ifdef JX_GCC_ATOMIC_BUILTINS
  return __sync_sub_and_fetch(atomic,1);
#else
  int result;
  jx_os_mutex_lock(&jx_os_AtomicityMutex);
  result = --(*atomic);
  jx_os_mutex_unlock(&jx_os_AtomicityMutex);
  return result;
#endif
#endif
}

JX_INLINE jx_int32 jx_os_atomic32_increment(jx_os_atomic32 *atomic)
{
#ifdef JX_OS_CAS
  jx_os_atomic32 old_value, new_value;
  while(1) {
    old_value = *atomic;
    new_value = old_value + 1;
    if(JX_OS_CAS(atomic,old_value,new_value) == old_value)
      break;
  }
  return new_value;
#else
#ifdef JX_GCC_ATOMIC_BUILTINS
  return __sync_add_and_fetch(atomic,1);
  return new_value;
#else
  int result;
  jx_os_mutex_lock(&jx_os_AtomicityMutex);
  result = ++(*atomic);
  jx_os_mutex_unlock(&jx_os_AtomicityMutex);
  return result;
#endif
#endif
}

JX_INLINE jx_int32 jx_os_atomic32_exchange(jx_os_atomic32 *atomic,
                                           jx_int32 new_value)
{
#ifdef JX_OS_CAS
  jx_os_atomic32 old_value;
  while(1) {
    old_value = *atomic;
    if(JX_OS_CAS(atomic,old_value,new_value)==old_value)
      break;
  }
  return old_value;
#else
#ifdef JX_GCC_ATOMIC_BUILTINS
  jx_os_atomic32 old_value;
  while(1) {
    old_value = *atomic;
    if(__sync_bool_compare_and_swap(atomic,old_value,new_value))
      break;
  }
  return old_value;
#else
  int result;
  jx_os_mutex_lock(&jx_os_AtomicityMutex);
  result = *atomic;
  *atomic = new_value;
  jx_os_mutex_unlock(&jx_os_AtomicityMutex);
  return result;
#endif
#endif
}

JX_INLINE jx_bool jx_os_atomic32_cas(jx_os_atomic32 *atomic, 
                                     jx_int32 old_value,
                                     jx_int32 new_value)
{
#ifdef JX_OS_CAS
  return (JX_OS_CAS(atomic,old_value,new_value)==old_value);
#else
#ifdef JX_GCC_ATOMIC_BUILTINS
  return __sync_bool_compare_and_swap(atomic,old_value,new_value);
#else
  jx_bool result;
  jx_os_mutex_lock(&jx_os_AtomicityMutex);
  if( *atomic == old_value) {
    *atomic = new_value;
    result = JX_TRUE;
  } else {
    result = JX_FALSE;
  }
  jx_os_mutex_unlock(&jx_os_AtomicityMutex);
  return result;
#endif
#endif
}

#endif

#if defined(__MACH__) && defined(__APPLE__)

#include <libkern/OSAtomic.h>

typedef volatile int32_t jx_os_atomic32;

JX_INLINE int32_t jx_os_atomic32_read(jx_os_atomic32 *atomic)
{
  return *((int32_t*)atomic);
}

JX_INLINE int32_t jx_os_atomic32_decrement(jx_os_atomic32 *atomic)
{
  return OSAtomicDecrement32Barrier((int32_t*)atomic);
}
  
JX_INLINE int32_t jx_os_atomic32_increment(jx_os_atomic32 *atomic)
{
  return OSAtomicIncrement32Barrier((int32_t*)atomic);
}

JX_INLINE int32_t jx_os_atomic32_exchange(jx_os_atomic32 *atomic,
                                          jx_int32 new_value)
{
  jx_os_atomic32 old_value;
  while(1) {
    old_value = *atomic;
    if(OSAtomicCompareAndSwap32Barrier(old_value, new_value, (int32_t*)atomic))
      break;
  }
  return old_value;
}

JX_INLINE jx_bool jx_os_atomic32_cas(jx_os_atomic32 *atomic, 
                                     jx_int32 old_value,
                                     jx_int32 new_value)

{
  return OSAtomicCompareAndSwap32Barrier(old_value, new_value, (int32_t*)atomic);
}
#endif


struct jx__os_spinlock {
  jx_os_atomic32 atomic;
  jx_bool owned;
  pthread_t owner;
  jx_int count;
};

JX_INLINE jx_status jx__os_usleep(jx_size usec)
{
  jx_os_timeval tv;

  tv.tv_sec = usec / 1000000;
  tv.tv_usec = usec % 1000000;
 
  jx_os_select(0,NULL,NULL,NULL,&tv);
  return JX_SUCCESS;
}

JX_INLINE jx_status jx__os_thread_start(jx_os_thread *thread, jx_os_thread_fn func, void *arg)
{
  jx_status status = JX_PTR(thread);
  if(JX_OK(status)) {
    int pt_status = pthread_create(&thread->thread, 
                                   (pthread_attr_t*)NULL,
                                   (void* (*)(void *))func,
                                   (void *)arg );
    if(pt_status) {
      status = JX_STATUS_OS_THREAD_CREATION_FAILURE;
    } else {
#if 0
      pt_status = pthread_detach(thread->thread); /* is this a good idea? */
      if(pt_status) {
        status = JX_STATUS_OS_THREADING_ERROR;
      }
#endif
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_thread_join(jx_os_thread *thread)
{
  jx_status status = JX_PTR(thread);
  if(JX_OK(status)) {
    /* int pt_status = */
    pthread_join(thread->thread,NULL);
  }
  return status;
}

JX_INLINE jx_status jx__os_thread_get_current(jx_os_thread *thread)
{
  jx_status status = JX_PTR(thread);
  if(JX_OK(status)) {
    thread->thread = pthread_self();
  }
  return status;
}

JX_INLINE jx_status jx__os_mutex_init(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    int pt_status = pthread_mutex_init(&mutex->mutex,
                                       (pthread_mutexattr_t*)NULL);
    if(pt_status) {
      status = JX_STATUS_OS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_mutex_lock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    int pt_status = pthread_mutex_lock(&mutex->mutex);
    if(pt_status) {
      status = JX_STATUS_OS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_mutex_unlock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    int pt_status = pthread_mutex_unlock(&mutex->mutex);
    if(pt_status) {
      status = JX_STATUS_OS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_mutex_trylock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    int pt_status = pthread_mutex_trylock(&mutex->mutex);
    if(pt_status == EBUSY ) {
      status = JX_STATUS_NO;
    } else if(pt_status) {
      status = JX_STATUS_OS_MUTEX_ERROR;
    } else {
      status = JX_STATUS_YES;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_mutex_destroy(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    int pt_status = pthread_mutex_destroy(&mutex->mutex);
    if(pt_status) {
      status = JX_STATUS_OS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_cond_init(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    int pt_status = pthread_cond_init(&cond->cond,
                                       (pthread_condattr_t*)NULL);
    if(pt_status) {
      status = JX_STATUS_OS_COND_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_cond_wait(jx_os_cond *cond, jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(cond && mutex);
  if(JX_OK(status)) {
    int pt_status = pthread_cond_wait(&cond->cond,&mutex->mutex);
    if(pt_status) {
      status = JX_STATUS_OS_COND_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_cond_timedwait(jx_os_cond *cond, jx_os_mutex *mutex, jx_size usec)
{
  jx_status status = JX_PTR(cond && mutex);
  if(JX_OK(status)) {
    struct timeval now;
    struct timespec timeout;

    gettimeofday(&now,NULL);
    usec = usec + now.tv_usec;
    timeout.tv_sec = now.tv_sec + (usec / 1000000);
    timeout.tv_nsec = (usec % 1000000) * 1000;
    
    {
      int pt_status = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &timeout);
      if(pt_status == ETIMEDOUT ) {
        status = JX_STATUS_NO;
      } else if(pt_status) {
        status = JX_STATUS_OS_COND_ERROR;
      } else {
      status = JX_STATUS_YES;
      }
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_cond_signal(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    int pt_status = pthread_cond_signal(&cond->cond);
    if(pt_status) {
      status = JX_STATUS_OS_COND_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_cond_broadcast(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    int pt_status = pthread_cond_broadcast(&cond->cond);
    if(pt_status) {
      status = JX_STATUS_OS_COND_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_cond_destroy(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    int pt_status = pthread_cond_destroy(&cond->cond);
    if(pt_status) {
      status = JX_STATUS_OS_COND_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_tls_set(jx_os_tls *tls, void *value)
{
  jx_status status = JX_PTR(tls);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_set(%p, %p)",tls,value);
#endif
  if(JX_OK(status)) {
    int pt_status = pthread_setspecific(tls->key,value);
    if(pt_status) {
      status = JX_STATUS_OS_TLS_ERROR;
    }
  }
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr," return %d\n",status);
#endif
  return status;
}

JX_INLINE jx_status jx__os_tls_get(jx_os_tls *tls, void **value)
{
  jx_status status = JX_PTR(tls && value);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_get(%p, %p)",tls,value);
#endif
  if(JX_OK(status)) {
    *value = pthread_getspecific(tls->key);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr," -> *value = %p",*value);
#endif
  }
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr," return %d\n",status);
#endif
  return status;
}

JX_INLINE jx_status jx__os_tls_init(jx_os_tls *tls)
{
  jx_status status = JX_PTR(tls);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_init(%p)\n",tls);
#endif
  if(JX_OK(status)) {
    int pt_status = pthread_key_create(&tls->key,NULL);
    if(pt_status) {
      status = JX_STATUS_OS_TLS_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_tls_destroy(jx_os_tls *tls)
{
  jx_status status = JX_PTR(tls);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_destroy(%p)\n",tls);
#endif
  if(JX_OK(status)) {
    int pt_status = pthread_key_delete(tls->key);
    if(pt_status) {
      status = JX_STATUS_OS_TLS_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_rlock_init(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    jx_os_memset((void*)rlock,0,sizeof(jx_os_rlock));
    status = jx__os_mutex_init(&rlock->mutex);
  }
  return status;
}

JX_INLINE jx_status jx__os_rlock_acquire(jx_os_rlock *rlock,jx_bool blocking)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    if(rlock->owned && (rlock->owner == pthread_self())) {
      /* will this non-barriered read actually work on multi-core systems? */
      status = JX_STATUS_YES;
      rlock->count++;
    } else if(blocking) {
      if(JX_OK( jx__os_mutex_lock(&rlock->mutex) )) {
        status = JX_STATUS_YES;
        rlock->owned = JX_TRUE;
        rlock->owner = pthread_self();
        rlock->count++;
      }
    } else {
      status = jx__os_mutex_trylock(&rlock->mutex);      
      if(status == JX_STATUS_YES) {
        rlock->owned = JX_TRUE;
        rlock->owner = pthread_self();
        rlock->count++;
      }
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_rlock_release(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    if(rlock->owned && (rlock->owner == pthread_self())) {
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

JX_INLINE jx_status jx__os_rlock_destroy(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    status = jx__os_mutex_destroy(&rlock->mutex);
  }
  return status;
}

JX_INLINE jx_status jx__os_spinlock_init(jx_os_spinlock *spinlock)
{
  jx_status status = JX_PTR(spinlock);
  if(JX_OK(status)) {
    jx_os_memset((void*)spinlock,0,sizeof(jx_os_spinlock));
  }
  return status;
}

JX_INLINE jx_status jx__os_spinlock_acquire(jx_os_spinlock *spinlock, jx_bool spin)
{
  jx_status status = JX_PTR(spinlock);
  if(JX_OK(status)) {
    if(spinlock->owned && (spinlock->owner == pthread_self())) {
      status = JX_STATUS_YES;
      spinlock->count++;
    } else {
      while(1) {
        if(jx_os_atomic32_cas(&spinlock->atomic,0,1)) {
          spinlock->owned = JX_TRUE;
          spinlock->owner = pthread_self();
          spinlock->count++;
          status = JX_STATUS_YES;
          break;
        } else if(!spin) {
          status = JX_STATUS_NO;
          break;
        }
      }
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_spinlock_release(jx_os_spinlock *spinlock)
{
  jx_status status = JX_PTR(spinlock);
  if(JX_OK(status)) {
    if(spinlock->owned && (spinlock->owner == pthread_self())) {
      if((spinlock->count--) == 1) {
        spinlock->owned = JX_FALSE;
	while(!jx_os_atomic32_cas(&spinlock->atomic,1,0));
      } else if(spinlock->count < 0) {
        status = JX_STATUS_OS_SPINLOCK_ERROR;
        printf("error A\n");
      }
    } else {
      printf("error B\n");
      status = JX_STATUS_OS_SPINLOCK_ERROR;
    }
  }
  return status;
}

JX_INLINE jx_status jx__os_spinlock_destroy(jx_os_spinlock *spinlock)
{
  return JX_PTR(spinlock);
}

/* enable C++ mangling */
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#endif
