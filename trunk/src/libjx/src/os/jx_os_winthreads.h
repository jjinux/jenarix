/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/
#ifndef _H_jx_os_winthreads
#define _H_jx_os_winthreads

#include "jx_os.h"
#include "jx_status.h"

/* sleep function */

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile LONG jx_os_atomic32;

JX_INLINE_STATIC jx_int32 jx_os_atomic32_read(jx_os_atomic32 *atomic)
{
  return *atomic;
}

JX_INLINE_STATIC jx_os_atomic32 jx_os_atomic32_decrement(jx_os_atomic32 *atomic)
{
  return InterlockedDecrement(atomic);
}
  
JX_INLINE_STATIC jx_os_atomic32 jx_os_atomic32_increment(jx_os_atomic32 *atomic)
{
  return InterlockedIncrement(atomic);
}

JX_INLINE_STATIC jx_os_atomic32 jx_os_atomic32_exchange(jx_os_atomic32 *atomic, jx_os_atomic32 new_value)
{
  return InterlockedExchange(atomic,new_value);
}

JX_INLINE_STATIC jx_boolean jx_os_atomic32_cas(jx_os_atomic32 *atomic, 
                                               jx_os_atomic32 old_value,
                                               jx_os_atomic32 new_value)

{
  if(InterlockedCompareExchange(atomic, new_value, old_value) == old_value) 
    return JX_TRUE;
  else
    return JX_FALSE;
}

/* BEGIN Python Snippet 

 * Non-recursive mutex implementation adapted from Python source: */

/* This code implemented by Dag.Gruneau@elsa.preseco.comm.se */
/* Fast NonRecursiveMutex support by Yakov Markovitch, markovitch@iso.ru */
/* Eliminated some memory leaks, gsw@agere.com */

typedef struct NRMUTEX {
	LONG   owned ;
	DWORD  thread_id ;
	HANDLE hevent ;
} NRMUTEX, *PNRMUTEX ;

JX_INLINE_STATIC BOOL InitializeNonRecursiveMutex(PNRMUTEX mutex)
{
	mutex->owned = -1 ;  /* No threads have entered NonRecursiveMutex */
	mutex->thread_id = 0 ;
	mutex->hevent = CreateEvent(NULL, FALSE, FALSE, NULL) ;
	return mutex->hevent != NULL ;	/* TRUE if the mutex is created */
}

JX_INLINE_STATIC VOID DeleteNonRecursiveMutex(PNRMUTEX mutex)
{
	/* No in-use check */
	CloseHandle(mutex->hevent) ;
	mutex->hevent = NULL ; /* Just in case */
}

JX_INLINE_STATIC DWORD EnterNonRecursiveMutex(PNRMUTEX mutex, BOOL wait)
{
	/* Assume that the thread waits successfully */
	DWORD ret ;

	/* InterlockedIncrement(&mutex->owned) == 0 means that no thread currently owns the mutex */
	if (!wait){
		if (InterlockedCompareExchange(&mutex->owned, 0, -1) != -1)
			return WAIT_TIMEOUT ;
		ret = WAIT_OBJECT_0 ;
	} else {
		ret = InterlockedIncrement(&mutex->owned) ?
			/* Some thread owns the mutex, let's wait... */
			WaitForSingleObject(mutex->hevent, INFINITE) : WAIT_OBJECT_0 ;
   }
	mutex->thread_id = GetCurrentThreadId() ; /* We own it */
	return ret ;
}

JX_INLINE_STATIC BOOL LeaveNonRecursiveMutex(PNRMUTEX mutex)
{
	/* We don't own the mutex */
	mutex->thread_id = 0 ;
   {
     int owned = InterlockedDecrement(&mutex->owned);
	return ( owned < 0) ||
		SetEvent(mutex->hevent) ; /* Other threads are waiting, wake one on them up */
   }
}

JX_INLINE_STATIC PNRMUTEX AllocNonRecursiveMutex(void)
{
	PNRMUTEX mutex = (PNRMUTEX)malloc(sizeof(NRMUTEX)) ;
	if (mutex && !InitializeNonRecursiveMutex(mutex))
	{
		free(mutex) ;
		mutex = NULL ;
	}
	return mutex ;
}

JX_INLINE_STATIC void FreeNonRecursiveMutex(PNRMUTEX mutex)
{
	if (mutex)
	{
		DeleteNonRecursiveMutex(mutex) ;
		free(mutex) ;
	}
}

/* END Python Snippet */


#define noJX_OS_TLS_TRACE_DEBUG

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

struct jx__os_thread {
  HANDLE thread;
  DWORD thread_id;
};

struct jx__os_mutex {
  NRMUTEX mutex;
};

struct jx__os_cond {
  CRITICAL_SECTION critical_section;
  int waiter_count;
  HANDLE semaphore; // for queued threads to wait on 
  LONG broadcasting; 
  HANDLE broadcast_complete; // event for when broadcast is complete
};

struct jx__os_rlock {
  jx_os_mutex mutex;
  jx_boolean owned;
  DWORD owner;
  jx_word count;
};

struct jx__os_tls {
  DWORD key;
};

JX_INLINE_STATIC jx_status jx__os_thread_start(jx_os_thread *thread, void (*func)(void *), void *arg)
{
  jx_status status = JX_PTR(thread);
  if(JX_OK(status)) {
	  thread->thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)func,arg,0,&thread->thread_id);
     if(!thread->thread) {
        status = JX_STATUS_THREAD_CREATION_FAILURE;
	  }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_thread_join(jx_os_thread *thread)
{
  jx_status status = JX_PTR(thread);
  if(JX_OK(status)) {
    WaitForSingleObject(thread->thread,INFINITE);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_thread_get_current(jx_os_thread *thread)
{
  jx_status status = JX_PTR(thread);
  if(JX_OK(status)) {
    thread->thread_id = GetCurrentThreadId();
	thread->thread = GetCurrentThread();
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_init(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    if( ! InitializeNonRecursiveMutex(&mutex->mutex) ) {
      status = JX_STATUS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_lock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    if( EnterNonRecursiveMutex(&mutex->mutex, TRUE) != WAIT_OBJECT_0 ) {
      status = JX_STATUS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_unlock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    if( LeaveNonRecursiveMutex(&mutex->mutex) == FALSE ) {
      status = JX_STATUS_MUTEX_ERROR;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_trylock(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    DWORD dw_status = EnterNonRecursiveMutex(&mutex->mutex,FALSE);
    if(dw_status == WAIT_TIMEOUT ) {
      status = JX_STATUS_NO;
    } else if(dw_status != WAIT_OBJECT_0) {
      status = JX_STATUS_MUTEX_ERROR;
    } else {
      status = JX_STATUS_YES;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_mutex_destroy(jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(mutex);
  if(JX_OK(status)) {
    DeleteNonRecursiveMutex(&mutex->mutex);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_init(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    
    InitializeCriticalSectionAndSpinCount(&cond->critical_section,
                                          0x80000400);

    cond->waiter_count = 0;
    cond->broadcasting = 0;
    cond->semaphore = CreateSemaphore (NULL,       // no security
                                   0,          // initially 0
                                   0x7fffffff, // max count
                                   NULL);      // unnamed 

    cond->broadcast_complete = CreateEvent (NULL, FALSE, FALSE, NULL);
  }
  return status;
}

JX_INLINE_STATIC CriticalIncrement(LONG *ptr, CRITICAL_SECTION *cs) 
{
  LONG result;
  EnterCriticalSection(cs);
  result = (*ptr)++;
  LeaveCriticalSection(cs);
  return result;
}

JX_INLINE_STATIC CriticalDecrement(LONG *ptr, CRITICAL_SECTION *cs) 
{
  LONG result;
  EnterCriticalSection(cs);
  result = (*ptr)--;
  LeaveCriticalSection(cs);
  return result;
}

JX_INLINE_STATIC CriticalExchange(LONG *ptr, LONG value, CRITICAL_SECTION *cs) 
{
  LONG result;
  EnterCriticalSection(cs);
  result = (*ptr);
  (*ptr) = value;
  LeaveCriticalSection(cs);
  return result;
}

JX_INLINE_STATIC jx_status jx__os_cond_wait(jx_os_cond *cond, jx_os_mutex *mutex)
{
  jx_status status = JX_PTR(cond && mutex);
  if(JX_OK(status)) {
    int broadcasting;
    int last_waiter;
    // indicate that we are waiting
    CriticalIncrement(&cond->waiter_count,&cond->critical_section);

    // release & signal the mutex while waiting on the semaphore...
    mutex->mutex.thread_id = 0 ;

    SignalObjectAndWait(mutex->mutex.hevent, cond->semaphore, INFINITE, FALSE);

    EnterCriticalSection(&cond->critical_section);
    broadcasting = cond->broadcasting;
    last_waiter = ! --(cond->waiter_count);
    LeaveCriticalSection(&cond->critical_section);

    if (last_waiter && broadcasting ) {
      // we were the last waiter of a broadcast
      InterlockedDecrement(&cond->broadcasting);
      
      // so signal broadcast complete and retake the mutex
      SignalObjectAndWait(cond->broadcast_complete, mutex->mutex.hevent, INFINITE, FALSE);
      mutex->mutex.thread_id = GetCurrentThreadId() ;    
    } else {
      // otherwise, just retake the mutex
      WaitForSingleObject (mutex->mutex.hevent, INFINITE);
      mutex->mutex.thread_id = GetCurrentThreadId() ;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_timedwait(jx_os_cond *cond, jx_os_mutex *mutex, jx_size usec)
{
  jx_status status = JX_PTR(cond && mutex);
  if(JX_OK(status)) {
    int broadcasting;
    int last_waiter;
    DWORD dw_status;

    // win32 API only provides millisec resolution
    int msec = ((usec+499)/1000);
    if(usec & !msec) {
      // round up to 1 msec if usec is nonzero
      msec = 1;
    }
    // indicate that we are waiting
    CriticalIncrement(&cond->waiter_count,&cond->critical_section);

    // release & signal the mutex while waiting on the semaphore...
    mutex->mutex.thread_id = 0 ;

    dw_status = SignalObjectAndWait(mutex->mutex.hevent, cond->semaphore, msec, FALSE);
    
    if(dw_status == WAIT_TIMEOUT) {
      status = JX_STATUS_NO;
    } else if(dw_status == WAIT_OBJECT_0) {
      status = JX_STATUS_YES;
    } else {
      status = JX_STATUS_COND_ERROR;
    }
    
    EnterCriticalSection(&cond->critical_section);
    broadcasting = cond->broadcasting;
    last_waiter = ! --(cond->waiter_count);
    LeaveCriticalSection(&cond->critical_section);
    
    if (last_waiter && broadcasting ) {
      // we were the last waiter of a broadcast
      InterlockedDecrement(&cond->broadcasting);
      
      // so signal broadcast complete and retake the mutex
      SignalObjectAndWait(cond->broadcast_complete, mutex->mutex.hevent, INFINITE, FALSE);
      mutex->mutex.thread_id = GetCurrentThreadId() ;    
    } else {
      // otherwise, just retake the mutex
      WaitForSingleObject (mutex->mutex.hevent, INFINITE);
      mutex->mutex.thread_id = GetCurrentThreadId() ;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_signal(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    if (cond->waiter_count)
      // note that harmless (?) race is that the semaphore could be
      // signaled more times than there are actual waiters presently 
      // waiting, thus causing future threads not to wait
      ReleaseSemaphore (cond->semaphore, 1, 0);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_broadcast(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    EnterCriticalSection(&cond->critical_section);
    if( cond->waiter_count > 0 ) {
      InterlockedIncrement(&cond->broadcasting);
      // Wake up all the waiters atomically.
      ReleaseSemaphore (cond->semaphore, cond->waiter_count, 0);
      LeaveCriticalSection(&cond->critical_section);
      // Wait for all the awakened threads to acquire the counting
      // semaphore. 
      WaitForSingleObject(cond->broadcast_complete, INFINITE);
    } else {
      LeaveCriticalSection(&cond->critical_section);    
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_cond_destroy(jx_os_cond *cond)
{
  jx_status status = JX_PTR(cond);
  if(JX_OK(status)) {
    DeleteCriticalSection(&cond->critical_section);
    CloseHandle(cond->semaphore);
    CloseHandle(cond->broadcast_complete);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_set(jx_os_tls *tls, void *value)
{
  jx_status status = JX_PTR(tls);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_set(%p, %p)",tls,value);
#endif
  if(JX_OK(status)) {
	  if(!TlsSetValue(tls->key,value)) {
      status = JX_STATUS_TLS_ERROR;
    }
  }
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr," return %d\n",status);
#endif
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_get(jx_os_tls *tls, void **value)
{
  jx_status status = JX_PTR(tls && value);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_get(%p, %p)",tls,value);
#endif
  if(JX_OK(status)) {
    *value = TlsGetValue(tls->key);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr," -> *value = %p",*value);
#endif
  }
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr," return %d\n",status);
#endif
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_init(jx_os_tls *tls)
{
  jx_status status = JX_PTR(tls);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_init(%p)\n",tls);
#endif
  if(JX_OK(status)) {
	tls->key = TlsAlloc();
    if(tls->key == TLS_OUT_OF_INDEXES) {
      status = JX_STATUS_TLS_ERROR;
    }
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_tls_destroy(jx_os_tls *tls)
{
  jx_status status = JX_PTR(tls);
#ifdef JX_OS_TLS_TRACE_DEBUG
  fprintf(jx_os_stderr,"jx_os_tls_destroy(%p)\n",tls);
#endif
  if(JX_OK(status)) {
	if(!TlsFree(tls->key)) {
       status = JX_STATUS_TLS_ERROR;
    }
  }
  return status;
}


JX_INLINE_STATIC jx_status jx__os_rlock_init(jx_os_rlock *rlock)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    jx_os_memset((void*)rlock,0,sizeof(jx_os_rlock));
    status = jx__os_mutex_init(&rlock->mutex);
  }
  return status;
}

JX_INLINE_STATIC jx_status jx__os_rlock_acquire(jx_os_rlock *rlock,jx_boolean blocking)
{
  jx_status status = JX_PTR(rlock);
  if(JX_OK(status)) {
    if(rlock->owned && (rlock->owner == GetCurrentThreadId())) {
      status = JX_STATUS_YES;
      rlock->count++;
    } else if(blocking) {
      status = jx__os_mutex_lock(&rlock->mutex);
      rlock->owned = JX_TRUE;
      rlock->owner = GetCurrentThreadId();
      rlock->count++;
    } else {
      status = jx__os_mutex_trylock(&rlock->mutex);      
      if(status == JX_STATUS_YES) {
        rlock->owned = JX_TRUE;
        rlock->owner = GetCurrentThreadId();
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
    if(rlock->owned && (rlock->owner == GetCurrentThreadId())) {
      if((rlock->count--) == 1) {
        rlock->owned = JX_FALSE;
        jx__os_mutex_unlock(&rlock->mutex);
      } else if(!(rlock->count>0)) {
        status = JX_STATUS_RLOCK_ERROR;
      }
    } else {
      status = JX_STATUS_RLOCK_ERROR;
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

#ifdef __cplusplus
}
#endif
#endif
