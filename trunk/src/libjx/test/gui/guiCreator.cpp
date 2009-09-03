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

namespace Known {
/*
// doesn't work ??
  jx::Ob vSplitterType;
  jx::Ob hSplitterType;
  jx::Ob menuBarType;
  jx::Ob menuItemType;
  jx::Ob openglContextType;
  jx::Ob navigatorType;
*/
  jx_ob vSplitterType;
  jx_ob hSplitterType;
  jx_ob menuBarType;
  jx_ob menuItemType;
  jx_ob openglContextType;
  jx_ob navigatorType;
};

extern int OUT_TYPE;
extern JX_WIDGET *window;

int setOutputType(int in_type, char *a) 
{
  int out_type = in_type;
  if (!strncmp(a,"-html",5)) {
    out_type |= GUI_HTML;
  } else if (!strncmp(a,"-qt",3)) {
    out_type |=  GUI_QTUI;
  } else if (!strncmp(a,"-v",2)) {
    out_type |= GUI_PRINT;
  }
  return out_type;
}

int getWidth(jx::Ob *entity) {
  return getAttr(entity, "width").asInt();
}
int getHeight(jx::Ob *entity) {
  int height = getAttr(entity, "height").asInt();
  if (height < 1) {
    //jx::Ob type = entity->listGet(0);
    //jx::Ob type = (*entity)[0];
    if ((*entity)[0] == Known::menuBarType) height = 50;
  }
  return height;
}

jx::Ob getAttr(jx::Ob *entity, const char *attr) 
{
/* get "attribute" from entity, which is a list of three items, the last of
   which is a hash of attributes with idents as keys */

/* this mehtod demonstrates the use of the [] operator for lists and hashes */
    //jx::Ob attrs = entity->listGet(2);
    jx::Ob attrs = (*entity)[2];
    //return attrs.hashGet((jx::Ident)attr);
    //return (*entity)[2].hashGet((jx::Ident)attr);
    //return attrs[jx::Ob::fromIdent(attr)];
    return attrs[jx::Ident(attr)];
}

jx::Ob getSource(jx::Ob *entity) 
{
    jx::Ob source = getAttr(entity, "source");
    if (source.nullCheck()) {
      jx::Ob type = (*entity)[0];
      if(type == Known::openglContextType) {
          return jx::Ob("opengl.html");
      } else if(type == Known::navigatorType) {
          return jx::Ob("navigator.html");
      } else if(type == Known::menuBarType) {
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
    int width = getWidth(entity);
    int height = getHeight(entity);
    jx::Ob src = getSource(entity);
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"create Widget %s %dX%d\n", src.asStr(), width, height);
    jx::Ob type = (*entity)[0];
    if(type == Known::openglContextType) {
      GLWidget *w = new GLWidget;
      w->setObjectName("OpenGLContext");
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    } else if(type == Known::navigatorType) {
      QTextEdit *w = new QTextEdit;
      w->setObjectName("Navigator");
      w->setPlainText(src.asStr());
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    } else if(type == Known::menuBarType) {
      QMenuBar *w = new QMenuBar;
      w->setObjectName("MenuBar");
      processMenuItems(entity,0,(JX_MENU *)w);
      widget = w;
    } else if(type == Known::menuItemType) {
    } else  {
      QTextEdit *w = new QTextEdit;
      w->setPlainText(src.asStr());
      w->setObjectName("Widget");
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    }
    return widget;
}

JX_SPLITTER * createQtHsplitter(jx::Ob *splitter, int size)
{
    int width = getWidth(splitter);
    int height = getHeight(splitter);
    QSplitter *s = new QSplitter(Qt::Horizontal);
    s->setObjectName("HSplitter");
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr, "HSplitter %dX%d\n", width, height);
    return s;
}

JX_SPLITTER * createQtVsplitter(jx::Ob *splitter, int size)
{
    int width = getWidth(splitter);
    int height = getHeight(splitter);
    QSplitter *s = new QSplitter(Qt::Vertical);
    s->setObjectName("VSplitter");
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr, "VSplitter %dX%d\n", width, height);
    return s;
}
#endif

void printWidgetInfo(jx::Ob *entity)
{
  int width = getWidth(entity);
  int height = getHeight(entity);
  jx::Ob src = getSource(entity);
  jx::Ob type = (*entity)[0];
  if (type == Known::openglContextType) {
     fprintf(stderr,"OpenGL widget %s %dX%d\n", src.asStr(), width, height);
  } else if(type == Known::navigatorType) {
     fprintf(stderr,"Navigator widget %s %dX%d\n", src.asStr(), width, height);
  } else if(type == Known::menuBarType) {
     if (height == 0) height = GUI_MENUBAR_HEIGHT;
     fprintf(stderr,"MenuBar widget %s %dX%d\n", src.asStr(), width, height);
     if (OUT_TYPE == GUI_PRINT) processMenuItems(entity,0,NULL);
  } else {
     fprintf(stderr,"Generic widget %s %dX%d\n", src.asStr(), width, height);
  }
}


JX_MENU * menuAction(jx::Ob *item, JX_MENU *menu_widget, jx::Ob *label, jx::Ob *callback, jx::Ob *checkbox, jx::Ob *popup)
{
  JX_MENU *sub_menu;
#ifdef JX_QT

  int has_checkbox;
  if (checkbox->nullCheck()) {
    has_checkbox = 0;
  } else {
    if (!strncmp(checkbox->asStr(), "on", 2)) {
      has_checkbox =  1;
    } else {
      has_checkbox = -1;
    }
  }

  if (OUT_TYPE & GUI_QTUI) {
    jx::Ob sub_menu_items = (*item)[1];
    if (sub_menu_items.size() > 0) { 
      //sub_menu = new JX_MENU(label->asStr());
      sub_menu = new JX_MENU(label->asStr(), menu_widget);
      sub_menu->setObjectName(label->asStr());
      menu_widget->addMenu(sub_menu);
    } else {

      //JXAction *theAct = new JXAction(label->asStr(), callback, window);
      JXAction *theAct = new JXAction(label->asStr(), callback, menu_widget);
      theAct->setObjectName(label->asStr());
      menu_widget->addAction(theAct);
      //Actions.push_back(theAct);
      //theAct->setShortcut(tr("Ctrl+B"));
      if (!callback->nullCheck()) {
        if (callback->builtinCallableCheck()) {
          if (has_checkbox) {
            QObject::connect(theAct, SIGNAL(triggered(bool)), theAct, SLOT(doCallback(bool)));
/*
          } else if ( !strncmp( popup->asStr(), "fileDialog", 10) ) {
            QObject::connect(theAct, SIGNAL(triggered()), theAct, SLOT(openFile()));
*/
/*
          } else if ( !strncmp( label->asStr(), "Exit", 4) ) {
            QObject::connect(theAct, SIGNAL(triggered()), theAct, SLOT(doExit()));
*/
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
  if (OUT_TYPE & GUI_PRINT) fprintf(stderr," label: %s", label.asStr());

/* checkbox attribute */
  jx::Ob checkbox = getAttr(item, "checkbox");
  if ((OUT_TYPE & GUI_PRINT) & !checkbox.nullCheck()) fprintf(stderr," checkbox: %s", checkbox.asStr());

/* popup attribute */
  jx::Ob popup = getAttr(item, "popup");
  if ((OUT_TYPE & GUI_PRINT) & !popup.nullCheck()) fprintf(stderr," popup: %s", popup.asStr());

/* callback attribute */
  jx::Ob callback = getAttr(item, "callback");
  if (callback.nullCheck()) {
    if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"\n");
  } else if (callback.builtinCallableCheck()) {
    if (OUT_TYPE & GUI_PRINT) callback.jxonDump(stderr, " callback");
  } else {
    if (OUT_TYPE & GUI_PRINT) callback.jxonDump(stderr, " unknown callback");
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
  jx::Ob menu_items = (*menu)[1];
  for (int i=0; i<menu_items.size(); ++i) {
    jx::Ob item = menu_items[i];
    jx::Ob type = item[0];
    if(type == Known::menuItemType) {
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  item", blanx.c_str());
      JX_MENU *sub_menu = processMenuItem(&item, menu_widget);
      processMenuItems(&item, depth+1, (JX_MENU *)sub_menu);
    } else if (item.strCheck()) {
#ifdef JX_QT
      if (OUT_TYPE & GUI_QTUI) {
        QAction *menu_string = new QAction(item.asStr(), menu_widget);
        menu_string->setObjectName(item.asStr());
        menu_widget->addAction(menu_string);
      }
#endif
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  string %s\n", blanx.c_str(), item.asStr());
    } else if (item.nullCheck()) {
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  null\n", blanx.c_str());
    } else {
      if (OUT_TYPE & GUI_PRINT) fprintf(stderr,"%s  other type %d\n", blanx.c_str(), item.type());
    }
  }
}

void printFrameHtml(jx::Ob *entity)
{
   int width = getWidth(entity);
   int height = getHeight(entity);
   jx::Ob src = getSource(entity);
   printf("  <FRAME SRC=\"%s\"", src.asStr());
   if (width > 0)  printf(" WIDTH=%d", width);
   if (height > 0) printf(" HEIGHT=%d", height);
   printf(">\n");
}

void printHFramesetHtml(jx::Ob *pane, int size)
{
    printf("<FRAMESET COLS=");
    for(int i=0;i<size;i++) {
      jx::Ob tmp = (*pane)[i];
      int width = getWidth(&tmp);
      if (i>0) printf (",");
      printVal(width);
    }
    printf (">\n");
}

void printVFramesetHtml(jx::Ob *pane, int size)
{
    printf("<FRAMESET ROWS=");
    for(int i=0;i<size;i++) {
      jx::Ob tmp = (*pane)[i];
      int height = getHeight(&tmp);
      if (i>0) printf (",");
      printVal(height);
    }
    printf (">\n");
}

JX_SPLITTER * processHSplitter(jx::Ob *splitter)
{
  jx::Ob pane = (*splitter)[1];
  int i,size = pane.size();

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getWidth(splitter);
  int height = getHeight(splitter);
#endif
  if (OUT_TYPE & GUI_HTML) printHFramesetHtml(&pane,size);
#ifdef JX_QT
  if (OUT_TYPE & GUI_QTUI) {
    s = createQtHsplitter(splitter,size);
  }
#endif

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    jx::Ob tmp = pane[i];
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
  jx::Ob pane = (*splitter)[1];
  int i,size = pane.size();

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getWidth(splitter);
  int height = getHeight(splitter);
#endif
  if (OUT_TYPE & GUI_HTML)  printVFramesetHtml(&pane, size);
#ifdef JX_QT
  if (OUT_TYPE & GUI_QTUI) {
    s = createQtVsplitter(splitter,size);
  }
#endif

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    jx::Ob tmp = pane[i];
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
  jx::Ob comp_type = (*component)[0];

  if(comp_type == Known::hSplitterType) {

    return processHSplitter(component);

  } else if(comp_type == Known::vSplitterType) {

    return processVSplitter(component);    

  } else { 

    return processWidget(component);

  }
}

bool locateKnowns(jx::Ob *node)
{

/*
// doesn't work ??
  Known::vSplitterType     = node->get(jx::Ident("VSplitter"));
  Known::hSplitterType     = node->get(jx::Ident("HSplitter"));
  Known::menuBarType       = node->get(jx::Ident("MenuBar"));
  Known::menuItemType      = node->get(jx::Ident("MenuItem"));
  Known::openglContextType = node->get(jx::Ident("OpenGLContext"))l
  Known::navigatorType     = node->get(jx::Ident("Navigator"));
*/
/* these are borrowed, so I'm not responsible for freeing them (shouldn't free them)
   they will be freed with delete node, in main */
  Known::vSplitterType     = node->borrow(jx::Ident("VSplitter"));
  Known::hSplitterType     = node->borrow(jx::Ident("HSplitter"));
  Known::menuBarType       = node->borrow(jx::Ident("MenuBar"));
  Known::menuItemType      = node->borrow(jx::Ident("MenuItem"));
  Known::openglContextType = node->borrow(jx::Ident("OpenGLContext"));
  Known::navigatorType     = node->borrow(jx::Ident("Navigator"));

  if (jx_null_check(Known::vSplitterType) ||
      jx_null_check(Known::hSplitterType) ||
      jx_null_check(Known::menuBarType) ||
      jx_null_check(Known::menuItemType) ||
      jx_null_check(Known::openglContextType) ||
      jx_null_check(Known::navigatorType) ) {
    return false;
  } else {
    return true;
  }
}

JX_WIDGET * gui_run_from_node(jx::Ob *node)
{
  
  JX_WIDGET *w;
  if( locateKnowns(node) ) {  

    /* get the root gui component */

    jx::Ob *gui = new jx::Ob(node->get(jx::Ident("gui")));
    
    /* dump graph for debuggin' */
    
    //node->jxonDump(stdout,"node");
    //gui->jxonDump(stdout,"gui");
    
    /* traverse the graph nondestructively, processing each component in turn */
    
    //printf("processing components...\n");
    
    w = processComponents(gui);
    
    /* would now run the GUI */
    
    /* when done...free what we took */

    delete gui;

    return w;
  }
}
