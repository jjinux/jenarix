

/* 
 * COPYRIGHT NOTICE: This file contains original source code from the
 * Jenarix (TM) Library, Copyright (C) 2007-8 by Warren L. Delano of
 * DeLano Scientific LLC, Palo Alto, California, United States.
 * Please see the accompanying LICENSE file for further information.
 * All rights not explicitly granted in that LICENSE file are
 * reserved.  It is unlawful to modify or remove this notice.
 * TRADEMARK NOTICE: Jenarix is a Trademark of DeLano Scientific LLC.
*/

#include "jx_private.h"
#include "jx_heap.h"

#ifdef JX_HEAP_TRACKER

#define Jx__FILE_MAX_SIZE 64

typedef struct Jx__heapTrackerEntry Jx_heapTrackerEntry;

struct Jx__heapTrackerEntry {
  jx_uint32 magic;
  Jx_heapTrackerEntry *next, *prev;
  char file[Jx__FILE_MAX_SIZE];
  int line;
  jx_os_size_t size;
  jx_int32 type;
};

#define JX__HEAP_HASH_SIZE 0x40000


/* global variables */

static Jx_heapTrackerEntry *jx_heap_Hash[JX__HEAP_HASH_SIZE];
static jx_word jx_heap_Count = 0, jx_heap_MaxCount = 0, jx_heap_TotalCount = 0;
static jx_word jx_atExitFlag = JX_FALSE;

#define JX__HEAP_TYPE_BLOCK 0
#define JX__HEAP_TYPE_VLA 1

#define JX__HEAP_MAGIC 0x89273612

#define JX__HEAP_PTR_TO_HASH_CODE(x) ((((jx_int32)((jx_int)x))>>11)&(JX__HEAP_HASH_SIZE-1))

static void Jx__heapInitialize(void);
static void (*Jx_heapInitialize) (void) = Jx__heapInitialize;

static jx_bool Jx__heapInitialized = JX_FALSE;

static void Jx__heapBlockOthersUntilReady(void)
{
  while(!Jx__heapInitialized);
}

static void Jx__atExit(void)
{
  jx_atExitFlag = JX_TRUE;
  jx_heap_dump(JX_HEAP_DUMP_CONTENT_TOO);
}

extern jx_os_process *jx_os_Process;

#ifdef JX_HEAP_TRACKER_SPINLOCK
#define JX_HEAP_LOCK_INIT jx_os_process_init(0,NULL)
#define JX_HEAP_LOCK     if(jx_atExitFlag || JX_OK(status = jx_os_spinlock_acquire(&jx_os_Process->heap_spinlock,JX_TRUE)))
#define JX_HEAP_UNLOCK   {if(!jx_atExitFlag) jx_os_spinlock_release(&jx_os_Process->heap_spinlock);}
#else
#ifdef JX_HEAP_TRACKER_MUTEX
#define JX_HEAP_LOCK_INIT jx_os_process_init(0,NULL)
#define JX_HEAP_LOCK    if(jx_atExitFlag || JX_OK(status = jx_os_mutex_lock(&jx_os_Process->heap_mutex)))
#define JX_HEAP_UNLOCK  if(!jx_atExitFlag) jx_os_mutex_unlock(&jx_os_Process->heap_mutex);}
#else
#define JX_HEAP_LOCK_INIT
#define JX_HEAP_LOCK
#define JX_HEAP_UNLOCK
#endif
#endif

static void Jx__heapInitialize(void)
{
  /* bogus attempt to prevent race on multi-threaded startup scenario */

  Jx_heapInitialize = Jx__heapBlockOthersUntilReady;

  jx_os_memset(jx_heap_Hash, 0, sizeof(Jx_heapTrackerEntry *) * JX__HEAP_HASH_SIZE);

  JX_HEAP_LOCK_INIT;

  if(Jx__heapInitialized)       /* uh oh, race on startup...so kill process */
    exit(JX_EXIT_FAILURE);
  else {                        /* no race detected, so let 'er rip! */
    atexit(Jx__atExit);
    Jx__heapInitialized = JX_TRUE;
  }
}

JX_INLINE jx_status Jx_heapTrack(Jx_heapTrackerEntry * entry,
                                 char *file, int line, int type, jx_os_size_t size)
{
  register jx_status status = JX_STATUS_NULL_PTR;
  if(Jx_heapInitialize)
    Jx_heapInitialize();
  if(entry) {

    JX_HEAP_LOCK {
      register jx_int32 hash_code = JX__HEAP_PTR_TO_HASH_CODE(entry);
      register Jx_heapTrackerEntry *old_head = jx_heap_Hash[hash_code];
      jx_os_strncpy(entry->file, file, Jx__FILE_MAX_SIZE);
      jx_heap_Hash[hash_code] = entry;
      entry->magic = JX__HEAP_MAGIC;
      entry->line = line;
      entry->type = type;
      entry->next = old_head;
      entry->prev = NULL;
      entry->size = size;
      if(old_head)
        old_head->prev = entry;
      if((++jx_heap_Count) > jx_heap_MaxCount) {
        jx_heap_MaxCount = jx_heap_Count;
      }
      jx_heap_TotalCount++;
      JX_HEAP_UNLOCK;
      status = JX_SUCCESS;
    }
  }
  return status;
}

JX_INLINE jx_status Jx_heapEntryFromPtr(Jx_heapTrackerEntry ** result,
                                        void *ptr, int expected_type)
{
  register jx_status status = JX_SUCCESS;
  if(Jx_heapInitialize)
    Jx_heapInitialize();
  JX_HEAP_LOCK {
    register Jx_heapTrackerEntry *cur, *entry = ((Jx_heapTrackerEntry *) ptr) - 1;
    register jx_int32 hash_code = JX__HEAP_PTR_TO_HASH_CODE(entry);
    /* make sure we're initialized */
    /* see if we can find this pointer in the hash table */
    cur = jx_heap_Hash[hash_code];
    while(cur) {
      if(cur == entry) {
        /* found it -- does it match expectations? */
        if((entry->magic != JX__HEAP_MAGIC) || (entry->type != expected_type)) {
          status = JX_STATUS_BAD_PTR_OR_CORRUPT_HEAP;
        } else
          *result = entry;
        break;
      }
      cur = cur->next;
    }
    if(!cur) {
      status = JX_STATUS_BAD_PTR_OR_CORRUPT_HEAP;
    }
    JX_HEAP_UNLOCK;
    if(!JX_OK(status)) {
      fprintf(stderr, "HeapTracker-Error: bad free or corrupted heap at %p\n", ptr);
      abort();
    }
  }
  return status;
}

JX_INLINE jx_status Jx_heapUntrack(Jx_heapTrackerEntry * entry)
{
  register jx_status status = JX_STATUS_NULL_PTR;
  if(entry) {
    JX_HEAP_LOCK {
      register Jx_heapTrackerEntry *prev = entry->prev;
      register Jx_heapTrackerEntry *next = entry->next;
      entry->magic = 0;         /* destroy magic */
      if(prev)
        prev->next = next;
      else {                    /* this must be the first entry in hash... */
        register int hash_code = JX__HEAP_PTR_TO_HASH_CODE(entry);
        jx_heap_Hash[hash_code] = next;
      }
      jx_heap_Count--;
      if(next)
        next->prev = prev;
      status = JX_SUCCESS;
      JX_HEAP_UNLOCK;
    }
  }
  return status;
}

typedef struct {
  void *p;
  char text[1280];
} HeapDumpOutput;

JX_INLINE int Jx_heapOutputInOrder(HeapDumpOutput * output, int line1, int line2)
{
  return (output[line1].p <= output[line2].p);
}

static void Jx_heapSortOutput(jx_size n, HeapDumpOutput * array, jx_uint * x)
{
  jx_size l, a, r, t, i;

  if(n < 1)
    return;
  if(n == 1) {
    x[0] = 0;
    return;
  }
  x--;
  for(a = 1; a <= n; a++)
    x[a] = a;
  l = (n >> 1) + 1;
  r = n;
  while(1) {
    if(l > 1)
      t = x[--l];
    else {
      t = x[r];
      x[r] = x[1];
      if(--r == 1) {
        x[1] = t;
        break;
      }
    }
    i = l;
    a = l << 1;
    while(a <= r) {
      if(a < r && (!Jx_heapOutputInOrder(array, x[a + 1] - 1, x[a] - 1)))
        a++;
      if(!Jx_heapOutputInOrder(array, x[a] - 1, t - 1)) {
        x[i] = x[a];
        a += (i = a);
      } else
        a = r + 1;
    }
    x[i] = t;
  }
  x++;
  for(a = 0; a < n; a++)
    x[a]--;
}

jx_size jx_heap_usage(void)
{
  jx_size tot = 0;
  Jx_heapTrackerEntry *rec;
  {
    jx_size a;
    for(a = 0; a < JX__HEAP_HASH_SIZE; a++) {
      rec = jx_heap_Hash[a];
      while(rec) {
        tot += rec->size;
        rec = rec->next;
      }
    }
  }
  return (tot);
}

jx_status jx_heap_dump(jx_int32 flags)
{
  jx_status status = JX_SUCCESS;
  JX_HEAP_LOCK {
    jx_word a;
    jx_uint tot = 0;
    Jx_heapTrackerEntry *rec;
    jx_char type[] = "FV";
    jx_word cnt = 0;
    HeapDumpOutput *output = NULL;
    jx_uint *index = NULL;
    jx_os_fflush(jx_os_stdout);
    jx_os_fflush(jx_os_stderr);
#if 0
    if(jx_atExitFlag)
      flags = flags & (~JX_HEAP_DUMP_SORT);     /* don't touch heap if we're in an atExit call */
    else
#endif
      flags = flags | JX_HEAP_DUMP_SORT;        /* normally we use the heap to sort */
    if(flags & JX_HEAP_DUMP_SORT) {
      for(a = 0; a < JX__HEAP_HASH_SIZE; a++) {
        rec = jx_heap_Hash[a];
        while(rec) {
          rec = rec->next;
          cnt++;
        }
      }
      output = (HeapDumpOutput *) jx_os_malloc(cnt * sizeof(HeapDumpOutput));
      index = (jx_uint *) jx_os_malloc(cnt * sizeof(jx_uint));
      cnt = 0;
    }

    jx_os_fprintf(jx_os_stderr,
                  "Heap: ==============================================================\n");
    jx_os_fflush(jx_os_stderr);
    for(a = 0; a < JX__HEAP_HASH_SIZE; a++) {
      rec = jx_heap_Hash[a];
      while(rec) {
        tot += rec->size;
        if(!(flags & JX_HEAP_DUMP_SUMMARY_ONLY)) {
          if(flags & JX_HEAP_DUMP_NO_ADDRESSES) {
            if(flags & JX_HEAP_DUMP_SORT) {
              output[cnt].p = rec;
              sprintf(output[cnt].text, "Heap: (%7x) %c %s", (unsigned int)
                      rec->size, type[rec->type], rec->file);
            } else {
              jx_os_fprintf(jx_os_stderr, "Heap: (%7x) %c %s", (unsigned int)
                            rec->size, type[rec->type], rec->file);
            }
          } else {
            if(flags & JX_HEAP_DUMP_SORT) {
              output[cnt].p = rec;
              sprintf(output[cnt].text,
                      "Heap: %12p-%12p (%7x) %c %s:%-4d",
                      (void *) (rec + 1),
                      (void *) ((char *) (rec + 1) + rec->size), (unsigned int)
                      rec->size, type[rec->type], rec->file, rec->line);
            } else {
              jx_os_fprintf(jx_os_stderr,
                            "Heap: %12p-%12p (%7x) %c %s:%-4d",
                            (void *) (rec + 1),
                            (void *) ((char *) (rec + 1) + rec->size), (unsigned int)
                            rec->size, type[rec->type], rec->file, rec->line);
            }
          }
          if(flags & JX_HEAP_DUMP_CONTENT_TOO) {
            jx_int i, size = 80;
            jx_char buffer[size + 2];
            jx_char *q, *p = (jx_char *) (rec + 1);
            jx_bool last_was_zero = JX_FALSE;
            if(size > rec->size)
              size = rec->size;
            buffer[0] = ' ';
            q = buffer + 1;
            for(i = 0; i < size; i++) {
              jx_bool advance = JX_TRUE;
              jx_char ch = *(p++);
              if(ch < 32) {
                if(ch == 0) {
                  ch = '.';
                  if(last_was_zero)
                    advance = JX_FALSE;
                  last_was_zero = JX_TRUE;
                } else {
                  last_was_zero = JX_FALSE;
                  ch = '?';
                }
              } else
                last_was_zero = JX_FALSE;
              if(advance)
                *(q++) = ch;
            }
            *q = 0;
            if(flags & JX_HEAP_DUMP_SORT) {
              jx_os_strcat(output[cnt].text, buffer);
            } else {
              jx_os_fprintf(jx_os_stderr, "%s", buffer);
            }
          }
          if(flags & JX_HEAP_DUMP_FILES_TOO) {
            JX_OS_FILE *f;
            int line = rec->line;
            jx_char buffer[1024], *c;
            f = fopen(rec->file, "r");
            while(line--)
              fgets(buffer, 2048, f);
            c = buffer;
            while(*c && *c < 33)
              c++;
            if(flags & JX_HEAP_DUMP_SORT) {
              jx_os_strcat(output[cnt].text, c);
            } else {
              jx_os_fprintf(jx_os_stderr, "%s", c);
            }
            fclose(f);
          } else {
            if(flags & JX_HEAP_DUMP_SORT) {
              jx_os_strcat(output[cnt].text, "\n");
            } else {
              jx_os_fprintf(jx_os_stderr, "\n");
            }
          }
        }
        rec = rec->next;
        cnt++;
      }
    }
    if(flags & JX_HEAP_DUMP_SORT) {
      Jx_heapSortOutput(cnt, output, index);
      for(a = 0; a < cnt; a++) {
        jx_os_fprintf(jx_os_stderr, "%s", output[index[a]].text);
      }
      jx_os_free(output);
      jx_os_free(index);
    }
    jx_os_fprintf(jx_os_stderr,
                  "Heap: Blocks: tracked %d, expected %d, found %d, peak at %d.\n",
                  (jx_uint32) jx_heap_TotalCount,
                  (jx_int32) jx_heap_Count,
                  (jx_uint32) cnt, (jx_uint32) jx_heap_MaxCount);
    jx_os_fprintf(jx_os_stderr,
                  "Heap: Summary: remaining memory allocated 0x%x (%0.3f MB).\n",
                  (unsigned int) tot, tot / (1024.0 * 1024));
    jx_os_fprintf(jx_os_stderr,
                  "Heap: ==============================================================\n");
    jx_os_fflush(jx_os_stderr);
    jx_os_fflush(jx_os_stdout);

    JX_HEAP_UNLOCK;
  }
  return status;
}

#endif /* #ifdef JX_HEAP_TRACKER */


/* basic memory allocation */


/* internal inlined routines for module */

JX_INLINE jx_status jx_heap_malloc(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                                   , char *file, int line, int type
#endif
  )
{
  register jx_status status = JX_SUCCESS;
#ifndef JX_HEAP_TRACKER
  if(!(*result = jx_os_malloc(size))) {
    status = JX_STATUS_OUT_OF_MEMORY;
  }
#else /* #ifndef JX_HEAP_TRACKER */
  {
    register Jx_heapTrackerEntry *entry;

    if(!(entry = (Jx_heapTrackerEntry *)
         jx_os_malloc(sizeof(Jx_heapTrackerEntry) + size))) {
      status = JX_STATUS_OUT_OF_MEMORY;
    } else {
      Jx_heapTrack(entry, file, line, type, size);
      *result = (entry + 1);
    }
  }
#endif /* #ifndef JX_HEAP_TRACKER */
  return status;
}

JX_INLINE jx_status jx_heap_calloc(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                                   , char *file, int line, int type
#endif
  )
{
  register jx_status status = JX_SUCCESS;
#ifndef JX_HEAP_TRACKER

  if(!(*result = jx_os_calloc(size, 1))) {
    status = JX_STATUS_OUT_OF_MEMORY;
  }
#else /* #ifndef JX_HEAP_TRACKER */

  register Jx_heapTrackerEntry *entry;

  if(!(entry = (Jx_heapTrackerEntry *)
       jx_os_calloc(sizeof(Jx_heapTrackerEntry) + size, 1))) {
    status = JX_STATUS_OUT_OF_MEMORY;
  } else {
    Jx_heapTrack(entry, file, line, type, size);
    *result = (entry + 1);
  }
#endif /* #ifndef JX_HEAP_TRACKER */
  return status;
}

JX_INLINE jx_status jx_heap_realloc(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                                    , char *file, int line, int type
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(!*result) {                /* we don't allow null pointers as input to internal realloc */
    status = JX_STATUS_NULL_PTR;
  } else {
    if(!size)
      size++;                   /* nor do we allow our realloc to free blocks */
#ifndef JX_HEAP_TRACKER
    {
      void *tmp = jx_os_realloc(*result, size);
      if(!tmp) {
        status = JX_STATUS_OUT_OF_MEMORY;
      } else {
        *result = tmp;
      }
    }
#else /* #ifndef JX_HEAP_TRACKER */
    {
      Jx_heapTrackerEntry *entry = NULL;
      if(JX_IS_OK(Jx_heapEntryFromPtr(&entry, *result, type))) {
        jx_size old_size = entry->size;
        if(JX_IS_OK(Jx_heapUntrack(entry))) {
          Jx_heapTrackerEntry *new_entry = (Jx_heapTrackerEntry *)
            jx_os_realloc(entry, size + sizeof(Jx_heapTrackerEntry));
          if(!new_entry) {
            status = JX_STATUS_OUT_OF_MEMORY;
            Jx_heapTrack(entry, file, line, type, old_size);
          } else {
            status = Jx_heapTrack(new_entry, file, line, type, size);
            *result = (new_entry + 1);
          }
        }
      }
    }
#endif /* #ifndef JX_HEAP_TRACKER */
  }

  return status;
}

JX_INLINE jx_status jx_heap_realloc_recopy(void **result, jx_size new_size,
                                           jx_size cur_size
#ifdef JX_HEAP_TRACKER
                                           , char *file, int line, int type
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(!*result) {                /* we don't allow null pointers as input to our internal realloc */
    status = JX_STATUS_NULL_PTR;
  } else {
    if(!new_size)
      new_size++;               /* nor do we allow our realloc to free blocks */
#ifndef JX_HEAP_TRACKER
    {
      void *tmp = jx_os_malloc(new_size);
      if(!tmp) {
        status = JX_STATUS_OUT_OF_MEMORY;
      } else {
        jx_size min_size = (new_size < cur_size) ? new_size : cur_size;
        if(min_size)
          jx_os_memcpy(tmp, *result, min_size);
        jx_os_free(*result);
        *result = tmp;
      }
    }
#else /* #ifndef JX_HEAP_TRACKER */
    {
      Jx_heapTrackerEntry *entry = NULL;
      if(JX_IS_OK(Jx_heapEntryFromPtr(&entry, *result, type))) {
        jx_size old_size = entry->size;
        if(JX_IS_OK(Jx_heapUntrack(entry))) {
          jx_size new_tot_size = new_size + sizeof(Jx_heapTrackerEntry);
          Jx_heapTrackerEntry *new_entry = (Jx_heapTrackerEntry *)
            jx_os_malloc(new_tot_size);
          if(!new_entry) {
            status = JX_STATUS_OUT_OF_MEMORY;
            Jx_heapTrack(entry, file, line, type, old_size);
          } else {
            jx_size cur_tot_size = cur_size + sizeof(Jx_heapTrackerEntry);
            jx_size min_tot_size =
              (new_tot_size < cur_tot_size) ? new_tot_size : cur_tot_size;
            if(min_tot_size)
              jx_os_memcpy(new_entry, entry, min_tot_size);
            jx_os_free(entry);
            status = Jx_heapTrack(new_entry, file, line, type, new_size);
            *result = (new_entry + 1);
          }
        }
      }
    }
#endif /* #ifndef JX_HEAP_TRACKER */
  }
  return status;
}

JX_INLINE jx_status jx_heap_free(void **result
#ifdef JX_HEAP_TRACKER
                                 , char *file, int line, int type
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(!*result) {
    status = JX_STATUS_NULL_PTR;
  } else {
#ifndef JX_HEAP_TRACKER
#ifndef JX_HEAP_SINGLE_USE
    jx_os_free(*result);
#endif
    *result = NULL;
#else /* #ifndef JX_HEAP_TRACKER */
    {
      Jx_heapTrackerEntry *entry = NULL;
      if(JX_IS_OK(Jx_heapEntryFromPtr(&entry, *result, type))) {
        if(JX_IS_OK(Jx_heapUntrack(entry))) {
#ifdef JX_HEAP_GRINDER
          jx_os_memset((void *) entry, -1, sizeof(Jx_heapTrackerEntry) + entry->size);
#endif
#ifndef JX_HEAP_SINGLE_USE
          jx_os_free(entry);
#endif
          *result = NULL;
        }
      }
    }
#endif /* #ifndef JX_HEAP_TRACKER */
  }
  return status;
}

jx_status jx_heap_Malloc(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                         , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  return jx_heap_malloc(result, size);
#else
  return jx_heap_malloc(result, size, file, line, JX__HEAP_TYPE_BLOCK);
#endif
}

void *jx_heap_MallocRaw(jx_size size
#ifdef JX_HEAP_TRACKER
                        , char *file, int line
#endif
  )
{
  void *result = JX_NULL;
#ifndef JX_HEAP_TRACKER
  result = jx_os_malloc(size);
#else
  {
    register Jx_heapTrackerEntry *entry;

    if((entry = (Jx_heapTrackerEntry *)
        jx_os_malloc(sizeof(Jx_heapTrackerEntry) + size))) {
      Jx_heapTrack(entry, file, line, JX__HEAP_TYPE_BLOCK, size);
      result = (entry + 1);
    }
  }
#endif /* #ifndef JX_HEAP_TRACKER */
  return result;
}

jx_status jx_heap_Calloc(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                         , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  return jx_heap_calloc(result, size);
#else
  return jx_heap_calloc(result, size, file, line, JX__HEAP_TYPE_BLOCK);
#endif
}

void *jx_heap_CallocRaw(jx_size size
#ifdef JX_HEAP_TRACKER
                        , char *file, int line
#endif
  )
{
  void *result = JX_NULL;
#ifndef JX_HEAP_TRACKER
  result = jx_os_calloc(size, 1);
#else
  jx_heap_calloc(&result, size, file, line, JX__HEAP_TYPE_BLOCK);
#endif /* #ifndef JX_HEAP_TRACKER */
  return result;
}

jx_status jx_heap_Realloc(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                          , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  if(!*result) {                /* a route to malloc */
    return jx_heap_malloc(result, size);
  } else if(!size) {            /* a route to free */
    return jx_heap_free(result);
  } else {
    return jx_heap_realloc(result, size);
  }
#else
  if(!*result) {                /* a route to malloc */
    return jx_heap_malloc(result, size, file, line, JX__HEAP_TYPE_BLOCK);
  }
  if(!size) {                   /* a route to free */
    return jx_heap_free(result, file, line, JX__HEAP_TYPE_BLOCK);
  } else {
    return jx_heap_realloc(result, size, file, line, JX__HEAP_TYPE_BLOCK);
  }
#endif
}

jx_status jx_heap_ReallocRecopy(void **result, jx_size new_size, jx_size old_size
#ifdef JX_HEAP_TRACKER
                                , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  if(!*result) {                /* a route to malloc */
    return jx_heap_malloc(result, new_size);
  } else if(!new_size) {        /* a route to free */
    return jx_heap_free(result);
  } else {
    return jx_heap_realloc_recopy(result, new_size, old_size);
  }
#else
  if(!*result) {                /* a route to malloc */
    return jx_heap_malloc(result, new_size, file, line, JX__HEAP_TYPE_BLOCK);
  }
  if(!new_size) {               /* a route to free */
    return jx_heap_free(result, file, line, JX__HEAP_TYPE_BLOCK);
  } else {
    return jx_heap_realloc_recopy(result, new_size, old_size, file, line,
                                  JX__HEAP_TYPE_BLOCK);
  }
#endif
}

void *jx_heap_ReallocRaw(void *ptr, jx_size size
#ifdef JX_HEAP_TRACKER
                         , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  if(!ptr) {                    /* a route to malloc */
    if(!JX_OK(jx_heap_malloc(&ptr, size)))
      ptr = NULL;
  } else if(!size) {            /* a route to free */
    if(!JX_OK(jx_heap_free(&ptr)))
      ptr = NULL;
  } else {
    if(!JX_OK(jx_heap_realloc(&ptr, size)))
      ptr = NULL;
  }
#else
  if(!ptr) {                    /* a route to malloc */
    if(!JX_OK(jx_heap_malloc(&ptr, size, file, line, JX__HEAP_TYPE_BLOCK)))
      ptr = NULL;
  }
  if(!size) {                   /* a route to free */
    if(!JX_OK(jx_heap_free(&ptr, file, line, JX__HEAP_TYPE_BLOCK)))
      ptr = NULL;
  } else {
    if(!JX_OK(jx_heap_realloc(&ptr, size, file, line, JX__HEAP_TYPE_BLOCK)))
      ptr = NULL;
  }
#endif
  return ptr;
}

void *jx_heap_ReallocRawRecopy(void *ptr, jx_size new_size, jx_size old_size
#ifdef JX_HEAP_TRACKER
                               , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  if(!ptr) {                    /* a route to malloc */
    if(!JX_OK(jx_heap_malloc(&ptr, new_size)))
      ptr = NULL;
  } else if(!new_size) {        /* a route to free */
    if(!JX_OK(jx_heap_free(&ptr)))
      ptr = NULL;
  } else {
    if(!JX_OK(jx_heap_realloc_recopy(&ptr, new_size, old_size)))
      ptr = NULL;
  }
#else
  if(!ptr) {                    /* a route to malloc */
    if(!JX_OK(jx_heap_malloc(&ptr, new_size, file, line, JX__HEAP_TYPE_BLOCK)))
      ptr = NULL;
  }
  if(!new_size) {               /* a route to free */
    if(!JX_OK(jx_heap_free(&ptr, file, line, JX__HEAP_TYPE_BLOCK)))
      ptr = NULL;
  } else {
    if(!JX_OK
       (jx_heap_realloc_recopy
        (&ptr, new_size, old_size, file, line, JX__HEAP_TYPE_BLOCK)))
      ptr = NULL;
  }
#endif
  return ptr;
}

jx_status jx_heap_Recalloc(void **result, jx_size size, jx_size old_size
#ifdef JX_HEAP_TRACKER
                           , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  if(!*result) {                /* a route to malloc */
    return jx_heap_malloc(result, size);
  } else if(!size) {            /* a route to free */
    return jx_heap_free(result);
  } else if(size <= old_size) {
    return jx_heap_realloc(result, size);
  } else {                      /* need to zero expanded memory */
    jx_status status;
    if(JX_OK(status = jx_heap_realloc(result, size))) {
      jx_char *ptr = (jx_char *) * result;
      jx_os_memset(ptr + old_size, 0, size - old_size);
    }
    return status;
  }
#else
  if(!*result) {                /* a route to malloc */
    return jx_heap_malloc(result, size, file, line, JX__HEAP_TYPE_BLOCK);
  }
  if(!size) {                   /* a route to free */
    return jx_heap_free(result, file, line, JX__HEAP_TYPE_BLOCK);
  } else if(size <= old_size) {
    return jx_heap_realloc(result, size, file, line, JX__HEAP_TYPE_BLOCK);
  } else {
    jx_status status;
    if(JX_OK(status = jx_heap_realloc(result, size, file, line, JX__HEAP_TYPE_BLOCK))) {
      jx_char *ptr = (jx_char *) * result;
      jx_os_memset(ptr + old_size, 0, size - old_size);
    }
    return status;
  }
#endif
}

jx_status jx_heap_Free(void **result
#ifdef JX_HEAP_TRACKER
                       , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  return jx_heap_free(result);
#else
  return jx_heap_free(result, file, line, JX__HEAP_TYPE_BLOCK);
#endif
}

jx_status jx_heap_FreeRaw(void *ptr
#ifdef JX_HEAP_TRACKER
                          , char *file, int line
#endif
  )
{
#ifndef JX_HEAP_TRACKER
  return jx_heap_free(&ptr);
#else
  return jx_heap_free(&ptr, file, line, JX__HEAP_TYPE_BLOCK);
#endif
}


/* variable-length arrays */

#ifndef JX_HEAP_TRACKER

#define JX__HEAP_VLA_MALLOC(result,size) jx_heap_malloc((void**)result,size)
#define JX__HEAP_VLA_CALLOC(result,size) jx_heap_calloc((void**)result,size)
#define JX__HEAP_VLA_REALLOC(result,size) jx_heap_realloc((void**)result,size)
#define JX__HEAP_VLA_REALLOC_RECOPY(result,new_size,cur_size) \
   jx_heap_realloc_recopy((void**)result,new_size,cur_size)
#define JX__HEAP_VLA_FREE(result) jx_heap_free((void**)result)

#else

#define JX__HEAP_VLA_MALLOC(result,size) \
  jx_heap_malloc((void**)result,size,file,line,JX__HEAP_TYPE_VLA)
#define JX__HEAP_VLA_CALLOC(result,size) \
   jx_heap_calloc((void**)result,size,file,line,JX__HEAP_TYPE_VLA)
#define JX__HEAP_VLA_REALLOC(result,size) \
   jx_heap_realloc((void**)result,size,file,line,JX__HEAP_TYPE_VLA)
#define JX__HEAP_VLA_REALLOC_RECOPY(result,new_size,cur_size) \
   jx_heap_realloc_recopy((void**)result,new_size,cur_size,file,line,JX__HEAP_TYPE_VLA)
#define JX__HEAP_VLA_FREE(result) \
   jx_heap_free((void**)result,file,line,JX__HEAP_TYPE_VLA)

#endif

jx_status jx_heap_VlaAlloc(void **result,
                           jx_size unit_size, jx_size init_size, jx_bool auto_zero
#ifdef JX_HEAP_TRACKER
                           , char *file, int line
#endif
  )
{
  void *void_vla = NULL;
  register jx_status status = JX_SUCCESS;
  if(auto_zero) {
    status =
      JX__HEAP_VLA_CALLOC(&void_vla, sizeof(jx_heap_vla) + (unit_size * init_size));
  } else {
    status =
      JX__HEAP_VLA_MALLOC(&void_vla, sizeof(jx_heap_vla) + (unit_size * init_size));
  }
  if(JX_OK(status)) {
    jx_heap_vla *vla = (jx_heap_vla *) void_vla;
    vla->alloc = vla->size = init_size;
    vla->unit_size = unit_size;
    vla->auto_zero = auto_zero;
    *result = (void *) (vla + 1);
  } else {
    if(void_vla) {
      JX__HEAP_VLA_FREE(&void_vla);
    }
  }
  return status;
}

void *jx_heap_VlaAllocRaw(jx_size unit_size, jx_size init_size, jx_bool auto_zero
#ifdef JX_HEAP_TRACKER
                          , char *file, int line
#endif
  )
{
  void *result = NULL;
  if(jx_heap_VlaAlloc(&result, unit_size, init_size, auto_zero
#ifdef JX_HEAP_TRACKER
                      , file, line
#endif
     ) < 0) {
    result = NULL;
  }
  return result;
}

jx_status jx_heap_VlaFree(void **result
#ifdef JX_HEAP_TRACKER
                          , char *file, int line
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(*result) {
    void *vla = (void *) (((jx_heap_vla *) * result) - 1);
    status = JX__HEAP_VLA_FREE(&vla);
    *result = NULL;
  } else {
    status = JX_STATUS_NULL_PTR;
  }
  return status;
}

jx_status jx_heap_VlaFreeRaw(void *ptr
#ifdef JX_HEAP_TRACKER
                             , char *file, int line
#endif
  )
{
  return jx_heap_VlaFree(&ptr
#ifdef JX_HEAP_TRACKER
                         , file, line
#endif
    );
}

jx_status jx_heap_VlaAddIndex(void **result, jx_size index
#ifdef JX_HEAP_TRACKER
                              , char *file, int line
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(*result) {
    jx_heap_vla *vla = (((jx_heap_vla *) * result) - 1);
    if(index >= vla->alloc) {
      jx_os_size_t new_alloc = (index + (index >> 1) + 1);
      while(new_alloc > vla->alloc) {
        void *void_vla = (void *) vla;
        /* NOTE: hardcoded 50% growth factor for arrays */
        if(!JX_IS_OK(JX__HEAP_VLA_REALLOC(&void_vla,
                                          sizeof(jx_heap_vla) +
                                          (vla->unit_size * new_alloc)))) {
          vla = (jx_heap_vla *) void_vla;
          /* no luck? keep trying for less and less memory */
          new_alloc = ((new_alloc + vla->alloc) >> 1);
        } else {
          vla = (jx_heap_vla *) void_vla;
          if(vla->auto_zero) {

            jx_char *start = ((char *) vla) +
              sizeof(jx_heap_vla) + (vla->unit_size * vla->size);
            jx_os_memset(start, 0, vla->unit_size * (new_alloc - vla->size));
          }
          vla->alloc = new_alloc;
          vla->size = index + 1;
          *result = (void *) (vla + 1);
          break;
        }
      }
    } else if(index >= vla->size) {
      vla->size = index + 1;
    }
  } else {
    status = JX_STATUS_NULL_PTR;
  }
  return status;
}

void *jx_heap_VlaAddIndexRaw(void *ptr, jx_size index
#ifdef JX_HEAP_TRACKER
                             , char *file, int line
#endif
  )
{
  if(jx_heap_VlaAddIndex(&ptr, index
#ifdef JX_HEAP_TRACKER
                         , file, line
#endif
     ) < 0) {
    ptr = NULL;
  }
  return ptr;
}

jx_status jx_heap_VlaSetSize(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                             , char *file, int line
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(*result) {
    jx_heap_vla *vla = (((jx_heap_vla *) * result) - 1);
    if((size < vla->size) && (vla->alloc < (size << 1))) {      /* modest size reduction */
      if(vla->auto_zero) {
        /* zero out the records we are no longer using */
        jx_char *start = ((char *) vla) + sizeof(jx_heap_vla) + (vla->unit_size * size);
        jx_os_memset(start, 0, vla->unit_size * (vla->size - size));
      }
      vla->size = size;
      *result = (void *) (vla + 1);
    } else {
      void *void_vla = (void *) vla;
      if(JX_IS_OK(JX__HEAP_VLA_REALLOC(&void_vla,
                                       sizeof(jx_heap_vla) + (vla->unit_size * size)))) {
        vla = (jx_heap_vla *) void_vla;
        if((size > vla->size) && vla->auto_zero) {
          jx_char *start = ((char *) vla) +
            sizeof(jx_heap_vla) + (vla->unit_size * vla->size);
          jx_os_memset(start, 0, vla->unit_size * (size - vla->size));
        }
        vla->size = size;
        vla->alloc = size;
        *result = (void *) (vla + 1);
      }
    }
  } else {
    status = JX_STATUS_NULL_PTR;
  }
  return status;
}

jx_status jx_heap_VlaSetSizeRecopy(void **result, jx_size size
#ifdef JX_HEAP_TRACKER
                                   , char *file, int line
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(*result) {
    void *void_vla = (void *) (((jx_heap_vla *) * result) - 1);
    jx_heap_vla *vla = (jx_heap_vla *) void_vla;
    if(JX_IS_OK(JX__HEAP_VLA_REALLOC_RECOPY(&void_vla,
                                            sizeof(jx_heap_vla) +
                                            (vla->unit_size * size),
                                            sizeof(jx_heap_vla) +
                                            (vla->unit_size * vla->size))
       )) {
      vla = (jx_heap_vla *) void_vla;
      if((size > vla->size) && vla->auto_zero) {
        jx_char *start = ((char *) vla) +
          sizeof(jx_heap_vla) + (vla->unit_size * vla->size);
        jx_os_memset(start, 0, vla->unit_size * (size - vla->size));
      }
      vla->size = size;
      vla->alloc = size;
      *result = (void *) (vla + 1);
    }
  } else {
    status = JX_STATUS_NULL_PTR;
  }
  return status;
}

void *jx_heap_VlaSetSizeRaw(void *ptr, jx_size size
#ifdef JX_HEAP_TRACKER
                            , char *file, int line
#endif
  )
{
  if(jx_heap_VlaSetSize(&ptr, size
#ifdef JX_HEAP_TRACKER
                        , file, line
#endif
     ) < 0) {
    ptr = NULL;
  }
  return ptr;
}

void *jx_heap_VlaSetSizeRawRecopy(void *ptr, jx_size size
#ifdef JX_HEAP_TRACKER
                                  , char *file, int line
#endif
  )
{
  if(jx_heap_VlaSetSizeRecopy(&ptr, size
#ifdef JX_HEAP_TRACKER
                              , file, line
#endif
     ) < 0) {
    ptr = NULL;
  }
  return ptr;
}

jx_status jx_heap_VlaGetSize(jx_size * result, void *ptr
#ifdef JX_HEAP_TRACKER
                             , char *file, int line
#endif
  )
{
  register jx_status status = JX_SUCCESS;
  if(ptr) {
    jx_heap_vla *vla = (((jx_heap_vla *) ptr) - 1);
    *result = (jx_size) vla->size;
  } else {
    status = JX_STATUS_NULL_PTR;
  }
  return status;
}

jx_size jx_heap_VlaGetSizeRaw(void *ptr
#ifdef JX_HEAP_TRACKER
                              , char *file, int line
#endif
  )
{
  jx_size result = 0;
  if(jx_heap_VlaGetSize(&result, ptr
#ifdef JX_HEAP_TRACKER
                        , file, line
#endif
     ) < 0) {
    result = 0;
  }
  return result;
}

jx_status jx_heap_VlaClone(void **result, void *ptr
#ifdef JX_HEAP_TRACKER
                           , char *file, int line
#endif
  )
{
  jx_heap_vla *vla = NULL;
  register jx_status status = JX_SUCCESS;
  if(ptr) {
    jx_heap_vla *src_vla = (((jx_heap_vla *) ptr) - 1);
    jx_size unit_size = src_vla->unit_size;
    jx_bool auto_zero = src_vla->auto_zero;
    jx_size init_size = src_vla->size;
    void *void_vla = (void *) src_vla;
    if(auto_zero) {
      status = JX__HEAP_VLA_CALLOC(&void_vla,
                                   sizeof(jx_heap_vla) + (unit_size * init_size));
    } else {
      status = JX__HEAP_VLA_MALLOC(&void_vla,
                                   sizeof(jx_heap_vla) + (unit_size * init_size));
    }
    vla = (jx_heap_vla *) void_vla;
    if(JX_OK(status)) {
      vla->alloc = vla->size = init_size;
      vla->unit_size = unit_size;
      vla->auto_zero = auto_zero;
      jx_os_memcpy((void *) (vla + 1), ptr, unit_size * init_size);
      *result = (void *) (vla + 1);
    } else {
      if(void_vla) {
        JX__HEAP_VLA_FREE(&void_vla);
      }
    }
  }
  return status;
}

void *jx_heap_VlaCloneRaw(void *ptr
#ifdef JX_HEAP_TRACKER
                          , char *file, int line
#endif
  )
{
  void *result = NULL;
  if(!JX_OK(JX_HEAP_VLA_CLONE(&result, ptr))) {
    result = NULL;
  }
  return result;
}

void *jx_heap_VlaDeleteRaw(void *ptr, jx_int index, jx_size count
#ifdef JX_HEAP_TRACKER
                           , char *file, int line
#endif
)
{
  if(ptr) {
    jx_heap_vla *vla = ((jx_heap_vla *) ptr) - 1;
    jx_size old_size = vla->size;

    /* failsafe range-handling logic */

    if(index<0) {
      if(index < -old_size)
        index = 0;
      else
        index = old_size + 1 + index;
      if(index<0) index = 0;
    }

    if((count+index) > vla->size) {
      count = vla->size - index;
    }
      
    if((index >= 0) && (count > 0) &&
       (index < vla->size) && ((count + index) <= vla->size)) {
      jx_size new_size = old_size - count;
      jx_char *base = (jx_char *) ptr;
      jx_os_memmove(base + index * vla->unit_size,
                    base + (count + index) * vla->unit_size,
                    ((vla->size - index) - count) * vla->unit_size);

      ptr = jx_heap_VlaSetSizeRaw(ptr, new_size
#ifdef JX_HEAP_TRACKER
                                  , file, line
#endif
                                  );
    }
  }
  return ptr;
}

void *jx_heap_VlaInsertRaw(void *ptr, jx_int index, jx_size count
#ifdef JX_HEAP_TRACKER
                           , char *file, int line
#endif
)
{
  if(ptr) {
    jx_heap_vla *vla = ((jx_heap_vla *) ptr) - 1;
    jx_size old_size = vla->size;

    /* failsafe range-handling logic */

    if(index<0) {
      if(index < -old_size)
        index = 0;
      else
        index = old_size + 1 + index;
      if(index<0) index = 0;
    }
    
    if(index > old_size)
      index = old_size;

    if((index >= 0) && (count > 0) && (index <= old_size)) {
      jx_int new_size = old_size + count;
      
      ptr = jx_heap_VlaSetSizeRaw(ptr, new_size
#ifdef JX_HEAP_TRACKER
                                  , file, line
#endif
                                  );

      if(ptr) {
        jx_char *base = (jx_char *) ptr;
        jx_heap_vla *vla = ((jx_heap_vla *) ptr) - 1;
        jx_os_memmove(base + (index + count) * vla->unit_size,
                base + index * vla->unit_size, (old_size - index) * vla->unit_size);
        if(vla->auto_zero) 
          jx_os_memset(base + index * vla->unit_size, 0, vla->unit_size * count);
      }
    }
  }
  return ptr;
}
