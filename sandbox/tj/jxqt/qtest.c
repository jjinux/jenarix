#include "jx_main_tools.h"

#include "qtBuiltins.h"

int main(int argc, char *argv[])
{

  int exit_status = EXIT_FAILURE;

  if(jx_ok( jx_os_process_init(argc,argv) )) {
    exit_status = EXIT_SUCCESS;

    jx_ob node = jx_hash_new();
    addQtBuiltins(node);
    jx_jxon_dump(stderr, "node", node);
    if(jx_ok( jx_main_exec_in_node(0,NULL,node) )) {
    }
    jx_ob_free(node);
    jx_os_process_complete();
  }

}
