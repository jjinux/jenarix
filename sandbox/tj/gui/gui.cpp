#include "jx_public.h"
#include "jx_main_tools.h"

#include "guiCreator.h"
#ifdef JX_QT
#include "glwidget.h"
#include <QDebug>
#include <QWidget>
#include <QApplication>
#include <QVBoxLayout>
#endif

void usage() {
  printf("gui [qt-specific switches][-h][-html][-qt]\n");
  printf("    reads gui description jxon file on stdin\n");
  printf("     -h     help\n");
  printf("     -html  output html framset(s) on stdout\n");
  printf("     -qt    open window with qt widgets\n");
}

int main(int argc, char *argv[])
{

  int exit_status = EXIT_FAILURE;
#ifdef JX_QT
  QApplication *app;
  QVBoxLayout * layout;
#endif
  JX_WIDGET *w;
  GuiCreator gui = GuiCreator();
  bool geom = false;
  for (int i=1; i<argc; ++i) {
     if (strlen(argv[i]) == 2 && !strncmp(argv[i], "-h", 2)) {
       usage();
     } else if (!strncmp(argv[i], "-geometry", 9)) {
       geom = true;
     } else {
       if (gui.out_type == GUI_NONE) gui.setOutputType(argv[i]);
     }
  }
  if (gui.out_type == GUI_QTUI) {
#ifdef JX_QT
      app = new QApplication(argc, argv);
      gui.window = new QWidget;
#else
      printf ("qt output not supported in this version\n");
#endif
  }
  if(jx_ok( jx_os_process_init(argc,argv) )) {
    exit_status = EXIT_SUCCESS;

    jx_ob node = jx_hash_new();
    
    //if(jx_ok( jx_main_exec_in_node(argc,argv,node) )) {
    if(jx_ok( jx_main_exec_in_node(0,NULL,node) )) {
      w = gui.gui_run_from_node(node);
    }
    
    jx_ob_free(node);
#ifdef JX_QT
    if (gui.out_type == GUI_QTUI) {
      //qDebug() << w;
      layout = new QVBoxLayout;
      layout->addWidget(w);
      gui.window->setLayout(layout);
      //gui.window->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
      gui.window->show();
      if (geom) {
        qDebug() << gui.window->size();
      } else {
        gui.window->resize(800,600);
      }
      return app->exec();
#else
    if (0) {
#endif
    } else {
      jx_os_process_complete();
    }
  }

}
