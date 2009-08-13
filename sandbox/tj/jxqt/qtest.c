#include "qtBuiltins.h"
#include "jx_public.h"
#include "jx_main_tools.h"

int main(int argc, char *argv[])
{

  int exit_status = EXIT_FAILURE;

  if(jx_ok( jx_os_process_init(argc,argv) )) {
    exit_status = EXIT_SUCCESS;

    jx_ob node = jx_hash_new();
    jx_main_expose_builtins_in_node(node);
    addQtBuiltins(node);
    if(jx_ok( jx_main_exec_in_node(0,NULL,node) )) {
      jx_ob_free(node);
    }
    jx_os_process_complete();
  }

}
