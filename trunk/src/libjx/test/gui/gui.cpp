#include "jx_public.h"
//#include "jx_main_tools.h"

#include "guiCreator.h"
#include "jxobject.h"
#ifdef JX_QT
#include "glwidget.h"
#include <QDebug>
#include <QWidget>
#include <QApplication>
#include <QVBoxLayout>
#endif

void usage() {
  printf("gui [qt-specific switches][-h][-html][-qt][-v]\n");
  printf("    reads gui description jxon file on stdin\n");
  printf("     -h     help\n");
  printf("     -html  output html framset(s) on stdout\n");
  printf("     -qt    open window with qt widgets\n");
  printf("     -v     verbose output on stderr\n");
}

/* share these with guiCreator */
int out_type = 0;
JX_WIDGET *window;

int main(int argc, char *argv[])
{

  int exit_status = EXIT_FAILURE;
#ifdef JX_QT
  QApplication *app;
  QVBoxLayout * layout;
#endif
  JX_WIDGET *w;
  bool geom = false;
  for (int i=1; i<argc; ++i) {
     if (strlen(argv[i]) == 2 && !strncmp(argv[i], "-h", 2)) {
       usage();
     } else if (!strncmp(argv[i], "-geometry", 9)) {
       geom = true;
     } else {
       out_type = setOutputType(out_type, argv[i]);
     }
  }
  if (out_type & GUI_QTUI) {
#ifdef JX_QT
      app = new QApplication(argc, argv);
      window = new QWidget;
#else
      printf ("qt output not supported in this version\n");
      out_type -= GUI_QTUI;
#endif
  }
  if(jx_ok( jx_os_process_init(argc,argv) )) {
    exit_status = EXIT_SUCCESS;

    jx::Hash *node = new jx::Hash();
    //if(jx_ok( jx_main_exec_in_node(argc,argv,node) )) {
    if(jx_ok( jx_main_exec_in_node(0,NULL,node->ob()) )) {
      w = gui_run_from_node(node);
    }
    delete node;

    if (out_type & GUI_QTUI) {
#ifdef JX_QT
      layout = new QVBoxLayout;
      layout->addWidget(w);
      window->setLayout(layout);
      window->show();
      if (!geom) {
        window->resize(800,600);
      }
      return app->exec();
#endif
    } else {
      jx_os_process_complete();
    }
  }

}
