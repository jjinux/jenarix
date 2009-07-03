#ifndef GUICREATOR_H
#define GUICREATOR_H

#include "jx_public.h"
#include "jx_main_tools.h"
#ifdef JX_QT
#include <QWidget>
#include <QSplitter>
#include <QMenuBar>
#endif

#define GUI_NONE 0
#define GUI_PRINT 1
#define GUI_HTML 2
#define GUI_QTUI 4
#define GUI_MENUBAR_HEIGHT 50

#ifdef JX_QT
#define JX_WIDGET QWidget
#define JX_SPLITTER QSplitter
#define JX_MENU QMenu
#else
#define JX_WIDGET void
#define JX_SPLITTER void
#define JX_MENU void
#endif

class GuiCreator
{

public:
    GuiCreator(int otype=GUI_NONE);
    ~GuiCreator();
    JX_WIDGET * gui_run_from_node(jx_ob node);
    bool setOutputType(char *a);
    int out_type;
    JX_WIDGET *window;

private:
    void printVal(int v);
    void printHFramesetHtml(jx_ob pane, int size);
    void printVFramesetHtml(jx_ob pane, int size);
    void printFrameHtml(jx_ob widget);
    void printWidgetInfo(jx_ob widget);
    void processMenuItems(jx_ob menu, int depth, JX_MENU *menu_widget);

    JX_WIDGET * createQtWidget(jx_ob widget);
    JX_SPLITTER * createQtHsplitter(jx_ob pane, int size);
    JX_SPLITTER * createQtVsplitter(jx_ob pane, int size);

    jx_ob getMenuItem(jx_ob item, jx_char *attr);
    jx_ob getSource(jx_ob widget);
    int getWidth(jx_ob widget);
    int getHeight(jx_ob widget);
    JX_WIDGET * processComponents(jx_ob component);
    JX_SPLITTER * processHSplitter(jx_ob splitter);
    JX_SPLITTER * processVSplitter(jx_ob splitter);
    JX_WIDGET * processOpenGL(jx_ob widget);
    JX_WIDGET * processNavigator(jx_ob widget);
    JX_WIDGET * processMenuBar(jx_ob widget);
    JX_WIDGET * processWidget(jx_ob widget);

    jx_ob get_symbol_from_node(jx_ob node, jx_char *ident);
    jx_status freeKnowns();
    jx_status locateKnowns(jx_ob node);

    //struct known {
      jx_ob hSplitterType;
      jx_ob vSplitterType;
      jx_ob menuBarType;
      jx_ob menuItemType;
      jx_ob openglContextType;
      jx_ob navigatorType;
    //};

};

#endif
