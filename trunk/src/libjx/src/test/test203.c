

/* 
Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

* Neither the name of the DeLano Scientific LLC nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "jx_private.h"

#define N_THREAD 12

typedef struct {
  jx_int id;
  jx_ob list;
} thread_info;

void *thread_fn(void *id_ptr)
{
  thread_info *info = (thread_info *) id_ptr;

  printf("# thread %d started\n", info->id);

  {
    jx_int i;
    for(i = 0; i < 100; i++) {
      jx_list_append(info->list, jx_ob_from_int(info->id));
    }
    for(i = 0; i < 100000; i++) {
      jx_list_append(info->list, jx_ob_from_int(info->id));
      jx_list_del(info->list, 0);
    }
  }

  printf("# thread %d complete\n", info->id);
  return NULL;
}

jx_status run_test(void)
{
  jx_os_thread *thread_array = NULL;
  thread_info thread_info[N_THREAD];

  jx_status status;
  jx_ob list = jx_list_new();

  jx_ob_set_synchronized(list, JX_TRUE, JX_TRUE);

  if(JX_IS_OK(jx_os_thread_array_new(&thread_array, N_THREAD))) {

    {
      jx_int i;
      for(i = 0; i < N_THREAD; i++) {
        jx_os_thread *thread = jx_os_thread_array_entry(thread_array, i);

        thread_info[i].id = i;
        thread_info[i].list = list;

        JX_OK_DO(jx_os_thread_start(thread, thread_fn, thread_info + i));
      }
    }
    {
      jx_int i;
      for(i = 0; i < N_THREAD; i++) {
        jx_os_thread *thread = jx_os_thread_array_entry(thread_array, i);

        JX_OK_DO(jx_os_thread_join(thread));
      }
    }
    JX_OK_DO(jx_os_thread_array_free(&thread_array));
  }
  jx_jxon_dump(stdout, "# output", list);
  jx_ob_free(list);
  return status;
}

int main(int argc, char **argv)
{
  if(jx_ok(jx_os_process_init(argc, argv))) {

    jx_status status = run_test();

    status = jx_os_process_complete();
    return JX_OK(status) ? JX_EXIT_SUCCESS : JX_EXIT_FAILURE;
  }
  return JX_EXIT_FAILURE;
}
