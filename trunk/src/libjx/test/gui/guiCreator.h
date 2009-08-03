#ifndef GUICREATOR_H
#define GUICREATOR_H

#include "jx_public.h"
#include "jx_main_tools.h"
//#include <list>
#ifdef JX_QT
#include "jxaction.h"
#include "jxobject.h"
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

/*
    static jx::Object hSplitterType;
    static jx::Object vSplitterType;
    static jx::Object menuBarType;
    static jx::Object menuItemType;
    static jx::Object openglContextType;
    static jx::Object navigatorType;
*/

class GuiCreator
{
public:

    GuiCreator(int otype=GUI_NONE);
    ~GuiCreator();
    JX_WIDGET * gui_run_from_node(jx::Hash *node);
    bool setOutputType(char *a);
    int out_type;
    JX_WIDGET *window;

/*
    jx::Object vSplitterType;
    jx::Object hSplitterType;
    jx::Object menuBarType;
    jx::Object menuItemType;
    jx::Object openglContextType;
    jx::Object navigatorType;
*/

private:
    jx::Object gui;

    void printVal(int v);
    void printHFramesetHtml(jx::Object *pane, int size);
    void printVFramesetHtml(jx::Object *pane, int size);
    void printFrameHtml(jx::Object *widget);
    void printWidgetInfo(jx::Object *widget);
    JX_MENU * processMenuItem(jx::Object *item, JX_MENU *menu_widget);
    void processMenuItems(jx::Object *menu, int depth, JX_MENU *menu_widget);

    JX_WIDGET * createQtWidget(jx::Object *widget);
    JX_SPLITTER * createQtHsplitter(jx::Object *pane, int size);
    JX_SPLITTER * createQtVsplitter(jx::Object *pane, int size);

    JX_MENU * menuAction(jx::Object *item, JX_MENU *menu_widget, jx::Object *label, jx::Object *callback, jx::Object *checkbox, jx::Object *popup);

    jx::Object getMenuItem(jx::Object *item, const char *attr);
    jx::Object get_attr_from_entity(jx::Object *entity, const char *attr);
    jx::Object getSource(jx::Object *entity);
    JX_WIDGET * processComponents(jx::Object *component);
    JX_SPLITTER * processHSplitter(jx::Object *splitter);
    JX_SPLITTER * processVSplitter(jx::Object *splitter);
    JX_WIDGET * processOpenGL(jx::Object *widget);
    JX_WIDGET * processNavigator(jx::Object *widget);
    JX_WIDGET * processMenuBar(jx::Object *widget);
    JX_WIDGET * processWidget(jx::Object *widget);

    //static jx::Object get_symbol_from_node(jx::Hash *node, const char *ident);
    jx::Object getAttr(jx::Object *item, const char *attr);
    jx_status freeKnowns();
    jx_status locateKnowns(jx::Hash *node);

};

#endif
