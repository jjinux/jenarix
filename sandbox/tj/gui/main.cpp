#include <QApplication>
#include <QWidget>

#include "jx_public.h"
#include "jx_main_tools.h"

#include "gui.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QWidget window;

  int exit_status = EXIT_FAILURE;
  Gui *g = new Gui(0);
  if(jx_ok( jx_os_process_init(argc,argv) )) {
    exit_status = EXIT_SUCCESS;

    jx_ob node = jx_hash_new();
    
    if(jx_ok( jx_main_exec_in_node(argc,argv,node) )) {
      g->gui_run_from_node(node);
    }
    
    jx_ob_free(node);
    //jx_os_process_complete();
  }

  window.show();
  return app.exec();
}
