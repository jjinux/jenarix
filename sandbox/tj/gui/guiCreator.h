#ifndef GUICREATOR_H
#define GUICREATOR_H

#include "jx_public.h"
#include "jx_main_tools.h"
#include <QWidget>
#include <QSplitter>

#define GUI_NONE 0
#define GUI_HTML 1
#define GUI_QTUI 2
#define GUI_MENUBAR_HEIGHT 50

class GuiCreator
{

public:
    GuiCreator(int otype=GUI_NONE);
    ~GuiCreator();
    QWidget * gui_run_from_node(jx_ob node);
    void setOutputType(char *a);
    int out_type;
    QWidget *window;

private:
    void printVal(int v);
    void printHFramesetHtml(jx_ob pane, int size);
    void printVFramesetHtml(jx_ob pane, int size);
    void printFrameHtml(jx_ob widget);
    void printWidgetInfo(jx_ob widget);

    QWidget * createQtWidget(jx_ob widget);
    QSplitter * createQtHsplitter(jx_ob pane, int size);
    QSplitter * createQtVsplitter(jx_ob pane, int size);

    jx_ob getSource(jx_ob widget);
    int getWidth(jx_ob widget);
    int getHeight(jx_ob widget);
    QWidget * processComponents(jx_ob component);
    QSplitter * processHSplitter(jx_ob splitter);
    QSplitter * processVSplitter(jx_ob splitter);
    QWidget * processOpenGL(jx_ob widget);
    QWidget * processNavigator(jx_ob widget);
    QWidget * processMenuBar(jx_ob widget);
    QWidget * processWidget(jx_ob widget);

    jx_ob get_symbol_from_node(jx_ob node, jx_char *ident);
    jx_status freeKnowns();
    jx_status locateKnowns(jx_ob node);

    //struct known {
      jx_ob hSplitterType;
      jx_ob vSplitterType;
      jx_ob menuBarType;
      jx_ob openglContextType;
      jx_ob navigatorType;
    //};

};

#endif
