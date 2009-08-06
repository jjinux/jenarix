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

int setOutputType(int in_type, char *a);
JX_WIDGET * gui_run_from_node(jx::Hash *node);
void printVal(int v);
void printHFramesetHtml(jx::Ob *pane, int size);
void printVFramesetHtml(jx::Ob *pane, int size);
void printFrameHtml(jx::Ob *widget);
void printWidgetInfo(jx::Ob *widget);
JX_MENU * processMenuItem(jx::Ob *item, JX_MENU *menu_widget);
void processMenuItems(jx::Ob *menu, int depth, JX_MENU *menu_widget);

JX_WIDGET * createQtWidget(jx::Ob *widget);
JX_SPLITTER * createQtHsplitter(jx::Ob *pane, int size);
JX_SPLITTER * createQtVsplitter(jx::Ob *pane, int size);

JX_MENU * menuAction(jx::Ob *item, JX_MENU *menu_widget, jx::Ob *label, jx::Ob *callback, jx::Ob *checkbox, jx::Ob *popup);

jx::Ob getMenuItem(jx::Ob *item, const char *attr);
jx::Ob get_attr_from_entity(jx::Ob *entity, const char *attr);
jx::Ob getSource(jx::Ob *entity);
JX_WIDGET * processComponents(jx::Ob *component);
JX_SPLITTER * processHSplitter(jx::Ob *splitter);
JX_SPLITTER * processVSplitter(jx::Ob *splitter);
JX_WIDGET * processOpenGL(jx::Ob *widget);
JX_WIDGET * processNavigator(jx::Ob *widget);
JX_WIDGET * processMenuBar(jx::Ob *widget);
JX_WIDGET * processWidget(jx::Ob *widget);

jx::Ob getAttr(jx::Ob *item, const char *attr);
bool locateKnowns(jx::Hash *node);

#endif
