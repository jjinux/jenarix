#include "jx_public.h"
#include "jx_main_tools.h"

#include "guiCreator.h"

int main(int argc, char *argv[])
{

  int exit_status = EXIT_FAILURE;
  GuiCreator gui = GuiCreator();
  if (argc > 1) gui.setOutputType(argv[1]);
  //if(jx_ok( jx_os_process_init(argc,argv) )) {
  if(jx_ok( jx_os_process_init(0,NULL) )) {
    exit_status = EXIT_SUCCESS;

    jx_ob node = jx_hash_new();
    
    //if(jx_ok( jx_main_exec_in_node(argc,argv,node) )) {
    if(jx_ok( jx_main_exec_in_node(0,NULL,node) )) {
      gui.gui_run_from_node(node);
    }
    
    jx_ob_free(node);
    jx_os_process_complete();
  }

}
