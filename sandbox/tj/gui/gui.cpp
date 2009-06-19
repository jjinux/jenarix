#include "jx_public.h"
#include "jx_main_tools.h"

#include "guiCreator.h"
//#include <QDebug>
#include <QWidget>
#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{

  int exit_status = EXIT_FAILURE;
  QApplication *app;
  QWidget *w;
  QVBoxLayout * layout;
  GuiCreator gui = GuiCreator();
  if (argc > 1) gui.setOutputType(argv[1]);
  if (gui.out_type == GUI_QTUI) {
      app = new QApplication(argc, argv);
      gui.window = new QWidget();
  }
  //if(jx_ok( jx_os_process_init(argc,argv) )) {
  if(jx_ok( jx_os_process_init(0,NULL) )) {
    exit_status = EXIT_SUCCESS;

    jx_ob node = jx_hash_new();
    
    //if(jx_ok( jx_main_exec_in_node(argc,argv,node) )) {
    if(jx_ok( jx_main_exec_in_node(0,NULL,node) )) {
      w = gui.gui_run_from_node(node);
    }
    
    jx_ob_free(node);
    if (gui.out_type == GUI_QTUI) {
      //qDebug() << w;
      layout = new QVBoxLayout();
      layout->addWidget(w);
      gui.window->setLayout(layout);
      //gui.window->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
      gui.window->show();
      return app->exec();
    } else {
      jx_os_process_complete();
    }
  }

}
