/* 
Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

* Neither the name of the DeLano Scientific LLC nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "guiCreator.h"
#ifdef JX_QT
#include "glwidget.h"
#include "jxobject.h"
#include "jxaction.h"
#include <QtDebug>
#include <QTextEdit>
#include <QtOpenGL>
#endif
#include <string>

/*
//can't jx_ob_copy entities?
static jx::Symbol hSplitterType;
static jx::Symbol vSplitterType;
static jx::Symbol menuBarType;
static jx::Symbol menuItemType;
static jx::Symbol openglContextType;
static jx::Symbol navigatorType;
*/

static jx_ob hSplitterType;
static jx_ob vSplitterType;
static jx_ob menuBarType;
static jx_ob menuItemType;
static jx_ob openglContextType;
static jx_ob navigatorType;
extern int OUT_TYPE;
extern JX_WIDGET *window;

int setOutputType(int in_type, char *a) 
{
  int OUT_TYPE = in_type;
  if (!strncmp(a,"-html",5)) {
    OUT_TYPE = in_type | GUI_HTML;
    return true;
  } else if (!strncmp(a,"-qt",3)) {
    OUT_TYPE = in_type | GUI_QTUI;
  } else if (!strncmp(a,"-v",2)) {
    OUT_TYPE = in_type | GUI_PRINT;
  }
  return OUT_TYPE;
}

jx::Ob getAttr(jx::Ob *entity, const char *attr) 
{
/* get "attribute" from entity, which is a list of three items, the last of
   which is a hash of attributes */

    jx::Hash attrs = entity->list_get(2);
    return attrs.get((jx::Ident)attr);
}

jx::Ob getSource(jx::Ob *entity) 
{
    jx::Ob source = getAttr(entity, "source");
    if (source.null_check()) {
      jx::Ob type = entity->list_get(0);
      if(type == openglContextType) {
          return jx::Ob("opengl.html");
      } else if(type == navigatorType) {
          return jx::Ob("navigator.html");
      } else if(type == menuBarType) {
          return jx::Ob("menubar.html");
      } else  {
          return jx::Ob("");
      }
    }
    return source;
}

void printVal(int v) {
  if (v == 0) printf("*");
  else printf("%d", v);
}

#ifdef JX_QT
JX_WIDGET * createQtWidget(jx::Ob *entity)
{
    JX_WIDGET *widget;
    int width = getAttr(entity, "width").as_int();
    int height = getAttr(entity, "height").as_int();
    jx::Ob src = getSource(entity);
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"create Widget %s %dx%d\n", src.as_str(), width, height);
    jx::Ob type = entity->list_get(0);
    if(type == openglContextType) {
      GLWidget *w = new GLWidget;
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    } else if(type == navigatorType) {
      QTextEdit *w = new QTextEdit;
      w->setPlainText(src.as_str());
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    } else if(type == menuBarType) {
      QMenuBar *w = new QMenuBar;
      processMenuItems(entity,0,(JX_MENU *)w);
      widget = w;
    } else if(type == menuItemType) {
    } else  {
      QTextEdit *w = new QTextEdit;
      w->setPlainText(src.as_str());
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    }
    return widget;
}

JX_SPLITTER * createQtHsplitter(jx::Ob *splitter, int size)
{
    int width = getAttr(splitter, "width").as_int();
    int height = getAttr(splitter, "height").as_int();
    QSplitter *s = new QSplitter(Qt::Horizontal);
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr, "HSplitter %dx%d\n", width, height);
    return s;
}

JX_SPLITTER * createQtVsplitter(jx::Ob *splitter, int size)
{
    int width = getAttr(splitter, "width").as_int();
    int height = getAttr(splitter, "height").as_int();
    QSplitter *s = new QSplitter(Qt::Vertical);
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr, "VSplitter %dx%d\n", width, height);
    return s;
}
#endif

void printWidgetInfo(jx::Ob *entity)
{
  int width = getAttr(entity, "width").as_int();
  int height = getAttr(entity, "height").as_int();
  jx::Ob src = getSource(entity);
  jx::Ob type = entity->list_get(0);
  if (type == openglContextType) {
     fprintf(stderr,"OpenGL widget %s %dx%d\n", src.as_str(), width, height);
  } else if(type == navigatorType) {
     fprintf(stderr,"Navigator widget %s %dx%d\n", src.as_str(), width, height);
  } else if(type == menuBarType) {
     if (height == 0) height = GUI_MENUBAR_HEIGHT;
     fprintf(stderr,"MenuBar widget %s %dx%d\n", src.as_str(), width, height);
     if (OUT_TYPE == GUI_PRINT) processMenuItems(entity,0,NULL);
  } else {
     fprintf(stderr,"Generic widget %s %dx%d\n", src.as_str(), width, height);
  }
}


JX_MENU * menuAction(jx::Ob *item, JX_MENU *menu_widget, jx::Ob *label, jx::Ob *callback, jx::Ob *checkbox, jx::Ob *popup)
{
  JX_MENU *sub_menu;
#ifdef JX_QT

  int has_checkbox;
  if (checkbox->null_check()) {
    has_checkbox = 0;
  } else {
    if (!strncmp(checkbox->as_str(), "on", 2)) {
      has_checkbox =  1;
    } else {
      has_checkbox = -1;
    }
  }

  if (OUT_TYPE & GUI_QTUI) {
    jx::Ob sub_menu_items = item->list_get(1);
    if (sub_menu_items.size() > 0) { 
      sub_menu = new JX_MENU(label->as_str());
      menu_widget->addMenu(sub_menu);
    } else {

      JXAction *theAct = new JXAction(label->as_str(), callback, window);
      //Actions.push_back(theAct);
      //theAct->setShortcut(tr("Ctrl+B"));
      menu_widget->addAction(theAct);
      if (!callback->null_check()) {
        if (callback->builtin_callable_check()) {
          if (has_checkbox) {
            QObject::connect(theAct, SIGNAL(triggered(bool)), theAct, SLOT(doCallback(bool)));
          } else if ( !strncmp( popup->as_str(), "fileDialog", 10) ) {
            QObject::connect(theAct, SIGNAL(triggered()), theAct, SLOT(openFile()));
          } else if ( !strncmp( label->as_str(), "Exit", 4) ) {
            QObject::connect(theAct, SIGNAL(triggered()), theAct, SLOT(doExit()));
          } else {
            QObject::connect(theAct, SIGNAL(triggered()), theAct, SLOT(doCallback()));
          }
        }
      }
      if (has_checkbox) {
        theAct->setCheckable(true);
        if (has_checkbox > 0) {
          theAct->setChecked(true);
        } else {
          theAct->setChecked(false);
        }
      }
    }
  }

  return sub_menu;
#endif
}

JX_MENU * processMenuItem(jx::Ob *item, JX_MENU * menu_widget) {
/*
   deal with attributes of a single menu item: label, checkbox, callback.
   This creates and adds an Action if this item is not a list.
   This creates and adds a Menu (sub_menu) if this item is a list.
   This returns any sub_menu that might be created.
   A sub_menu will display a label, but ignore any checkbox or callback.
*/

/* label attribute */
  jx::Ob label = getAttr(item, "label");
  if (OUT_TYPE & GUI_PRINT) fprintf(stderr," label: %s", label.as_str());

/* checkbox attribute */
  jx::Ob checkbox = getAttr(item, "checkbox");
  if ((OUT_TYPE & GUI_PRINT) & !checkbox.null_check()) fprintf(stderr," checkbox: %s", checkbox.as_str());

/* popup attribute */
  jx::Ob popup = getAttr(item, "popup");
  if ((OUT_TYPE & GUI_PRINT) & !popup.null_check()) fprintf(stderr," popup: %s", popup.as_str());

/* callback attribute */
  jx::Ob callback = getAttr(item, "callback");
  if (callback.null_check()) {
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"\n");
  } else if (callback.builtin_callable_check()) {
    if (OUT_TYPE & GUI_PRINT) callback.jxon_dump(stderr, " callback");
  } else {
    if (OUT_TYPE & GUI_PRINT) callback.jxon_dump(stderr, " unknown callback");
  }

  JX_MENU *sub_menu = menuAction(item, menu_widget, &label, &callback, &checkbox, &popup);

  return sub_menu;
}

void processMenuItems(jx::Ob *menu, int depth, JX_MENU *menu_widget)
{
/*
  recursively process menuItemTypes
*/
  std::string blanx(depth*2, ' ');  // for pretty printing
  jx::Ob menu_items = menu->list_get(1);
  for (int i=0; i<menu_items.size(); ++i) {
    jx::Ob item = menu_items.list_get(i);
    jx::Ob type = item.list_get(0);
    if(type == menuItemType) {
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  item", blanx.c_str());
      JX_MENU *sub_menu = processMenuItem(&item, menu_widget);
      processMenuItems(&item, depth+1, (JX_MENU *)sub_menu);
    } else if (item.str_check()) {
#ifdef JX_QT
      if (OUT_TYPE & GUI_QTUI) menu_widget->addAction(item.as_str());
#endif
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  string %s\n", blanx.c_str(), item.as_str());
    } else if (item.null_check()) {
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  null\n", blanx.c_str());
    } else {
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  other type %d\n", blanx.c_str(), item.type());
    }
  }
}

void printFrameHtml(jx::Ob *entity)
{
   int width = getAttr(entity, "width").as_int();
   int height = getAttr(entity, "height").as_int();
   jx::Ob src = getSource(entity);
   printf("  <FRAME SRC=\"%s\"", src.as_str());
   if (width > 0)  printf(" WIDTH=%d", width);
   if (height > 0) printf(" HEIGHT=%d", height);
   printf(">\n");
}

void printHFramesetHtml(jx::Ob *pane, int size)
{
    printf("<FRAMESET COLS=");
    for(int i=0;i<size;i++) {
      jx::Ob tmp = pane->list_get(i);
      int width = getAttr(&tmp, "width").as_int();
      if (i>0) printf (",");
      printVal(width);
    }
    printf (">\n");
}

void printVFramesetHtml(jx::Ob *pane, int size)
{
    printf("<FRAMESET ROWS=");
    for(int i=0;i<size;i++) {
      jx::Ob tmp = pane->list_get(i);
      int height = getAttr(&tmp, "height").as_int();
      if (i>0) printf (",");
      printVal(height);
    }
    printf (">\n");
}

JX_SPLITTER * processHSplitter(jx::Ob *splitter)
{
  jx::Ob pane = splitter->list_get(1);
  int i,size = pane.size();

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getAttr(splitter, "width").as_int();
  int height = getAttr(splitter, "height").as_int();
#endif
  if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"processing horizontal splitter %d widget...\n", size);
  if (OUT_TYPE & GUI_HTML) printHFramesetHtml(&pane,size);
#ifdef JX_QT
  if (OUT_TYPE & GUI_QTUI) {
    s = createQtHsplitter(splitter,size);
  }
#endif

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    jx::Ob tmp = pane.list_get(i);
    w = processComponents(&tmp);
#ifdef JX_QT
    if (OUT_TYPE & GUI_QTUI) {
       s->addWidget(w);
       if (height) w->setFixedHeight(height);
    }
#endif
  }
  if (OUT_TYPE & GUI_HTML) printf("</FRAMESET>\n");
  return s;
}

JX_SPLITTER * processVSplitter(jx::Ob *splitter)
{
  jx::Ob pane = splitter->list_get(1);
  int i,size = pane.size();

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getAttr(splitter, "width").as_int();
  int height = getAttr(splitter, "height").as_int();
#endif
  if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"processing vertical splitter %d widget...\n", size);
  if (OUT_TYPE & GUI_HTML)  printVFramesetHtml(&pane, size);
#ifdef JX_QT
  if (OUT_TYPE & GUI_QTUI) {
    s = createQtVsplitter(splitter,size);
  }
#endif

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    jx::Ob tmp = pane.list_get(i);
    w = processComponents(&tmp);
#ifdef JX_QT
    if (OUT_TYPE & GUI_QTUI) {
       s->addWidget(w);
       if (width) w->setFixedWidth(width);
    }
#endif
  }
  if (OUT_TYPE & GUI_HTML) printf("</FRAMESET>\n");
  return s;
}

JX_WIDGET * processWidget(jx::Ob *widget)
{
  JX_WIDGET *w;
  if (OUT_TYPE & GUI_PRINT) printWidgetInfo(widget);
  if (OUT_TYPE & GUI_HTML)  printFrameHtml(widget);
#ifdef JX_QT
  if (OUT_TYPE & GUI_QTUI) {
    w = createQtWidget(widget);
  }
#endif
  return w;
}

JX_WIDGET * processComponents(jx::Ob *component)
{
  jx::Ob comp_type = component->list_get(0);

  if(comp_type == hSplitterType) {

    return processHSplitter(component);

  } else if(comp_type == vSplitterType) {

    return processVSplitter(component);    

  } else { 

    return processWidget(component);

  }
}

jx_status freeKnowns()
{
  jx_ob_free(vSplitterType);
  jx_ob_free(hSplitterType);
  jx_ob_free(menuBarType);
  jx_ob_free(menuItemType);
  jx_ob_free(openglContextType);
  jx_ob_free(navigatorType);
  return JX_STATUS_SUCCESS;
}

jx_status locateKnowns(jx::Hash *node)
{

  vSplitterType     = node->get(jx::Ident("VSplitter"));
  hSplitterType     = node->get(jx::Ident("HSplitter"));
  menuBarType       = node->get(jx::Ident("MenuBar"));
  menuItemType      = node->get(jx::Ident("MenuItem"));
  openglContextType = node->get(jx::Ident("OpenGLContext"));
  navigatorType     = node->get(jx::Ident("Navigator"));

  return JX_SUCCESS;
}

JX_WIDGET * gui_run_from_node(jx::Hash *node)
{
  
  JX_WIDGET *w;
  if( jx_ok(locateKnowns(node)) ) {  

    /* get the root gui component */

    jx::Ob *gui = new jx::Ob(node->get(jx::Ident("gui")));
    
    /* dump graph for debuggin' */
    
    //gui.jxon_dump(stdout,"node");
    //gui.jxon_dump(stdout,"gui");
    
    /* traverse the graph nondestructively, processing each component in turn */
    
    //printf("processing components...\n");
    
    w = processComponents(gui);
    
    /* would now run the GUI */
    
    /* when done...free what we took */

    delete gui;
    freeKnowns();

    return w;
  }
}
