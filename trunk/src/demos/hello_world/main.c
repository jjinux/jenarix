/* hello world test application based on libjx */

#include "jx_public.h"

int main(int argc, char *argv[])
{
  int exit_status = JX_EXIT_FAILURE;

  if( jx_ok( jx_os_process_init(argc, argv) ) ) {
    exit_status = JX_EXIT_SUCCESS;

    jx_os_printf("hello world (from the jenarix environment)\n");

    jx_os_process_complete();
  }
  return exit_status;
}

