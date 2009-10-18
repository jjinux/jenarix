/* hello world test application based on libjx */

#include "jx_public.h"

#include "jx_heap.h"

int main(int argc, char *argv[])
{
  int exit_status = JX_EXIT_FAILURE;

  if( jx_ok( jx_os_process_init(argc, argv) ) ) {
    exit_status = JX_EXIT_SUCCESS;

    jx_printf("heap client initialized\n");

    {
      jx_int *ptr = JX_NULL;
      jx_status status = JX_HEAP_ALLOC( &ptr, jx_int);

      if(jx_ok(status)) {
        jx_printf("successfully allocated an int at %p\n",ptr);
      }

      jx_heap_dump(0);
      
      /* failsafe */
      
      status = JX_HEAP_FREE(&ptr);
      if(jx_ok(status)) {
        jx_printf("successfully released the int (pointer now %p)\n",ptr);
      }
      jx_heap_dump(0);
    }
    jx_os_process_complete();
  }
  return exit_status;
}

