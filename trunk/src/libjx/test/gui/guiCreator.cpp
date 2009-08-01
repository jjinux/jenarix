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

//static struct KnownTypes {
  jx_ob hSplitterType;
  jx_ob vSplitterType;
  jx_ob menuBarType;
  jx_ob menuItemType;
  jx_ob openglContextType;
  jx_ob navigatorType;
//} guiType;

GuiCreator::GuiCreator(int otype)
{
  out_type = otype;
}
GuiCreator::~GuiCreator()
{
}

bool GuiCreator::setOutputType(char *a) 
{
  if (!strncmp(a,"-html",5)) {
    out_type |= GUI_HTML;
    return true;
  } else if (!strncmp(a,"-qt",3)) {
    out_type |= GUI_QTUI;
    return true;
  } else if (!strncmp(a,"-v",2)) {
    out_type |= GUI_PRINT;
    return true;
  }
  return false;
}

jx::Object GuiCreator::getMenuItem(jx::Object item, const char *attr) 
{
   return getAttr(item, attr);
}
jx::Object GuiCreator::getAttr(jx::Object item, const char *attr) 
{
    //return get_symbol_from_node(jx_list_borrow(item.get_jxob(), 2), attr);
    jx::Object tmp = item.list_borrow(2);
    return tmp.hash_get(attr);
}

jx::Object GuiCreator::getSource(jx::Object entity) 
{
    jx::Object source = getAttr(entity, "source");
    jx::Object type = entity.list_borrow(0);
    if (source.null_check()) {
      //if(jx_list_borrow(entity.get_jxob(),0) == openglContextType) {
      if(type == openglContextType) {
          return jx::Object("opengl.html");
      } else if(type == navigatorType) {
          return jx::Object("navigator.html");
      } else if(type == menuBarType) {
          return jx::Object("menubar.html");
      } else  {
          return jx::Object("");
      }
    }
    return source;
}

int GuiCreator::getWidth(jx::Object entity) 
{
    int width = getAttr(entity, "width").as_int();
/*
    if (width == 0) {
        width = 10;
    }
*/
    return width;
}
int GuiCreator::getHeight(jx::Object entity) 
{
    int height = getAttr(entity, "height").as_int();
    if (height == 0) {
      if(entity.list_borrow(0) == menuBarType) {
        height = GUI_MENUBAR_HEIGHT;
      }
    }
    return height;
}

void GuiCreator::printVal(int v) {
  if (v == 0) printf("*");
  else printf("%d", v);
}

#ifdef JX_QT
JX_WIDGET * GuiCreator::createQtWidget(jx::Object entity)
{
    JX_WIDGET *widget;
    int width = getWidth(entity);
    int height = getHeight(entity);
    jx::Object src = getSource(entity);
    if (out_type & GUI_PRINT) fprintf(stderr,"create Widget %s %dx%d\n", src.as_str(), width, height);
    if(entity.list_borrow(0) == openglContextType) {
      GLWidget *w = new GLWidget;
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    } else if(entity.list_borrow(0) == navigatorType) {
      QTextEdit *w = new QTextEdit;
      w->setPlainText(src.as_str());
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    } else if(entity.list_borrow(0) == menuBarType) {
      QMenuBar *w = new QMenuBar;
      processMenuItems(entity,0,(JX_MENU *)w);
      widget = w;
    } else if(entity.list_borrow(0) == menuItemType) {
    } else  {
      QTextEdit *w = new QTextEdit;
      w->setPlainText(src.as_str());
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      widget = w;
    }
    //jx_ob_free(src);
    return widget;
}

JX_SPLITTER * GuiCreator::createQtHsplitter(jx::Object splitter, int size)
{
    int width = getWidth(splitter);
    int height = getHeight(splitter);
    QSplitter *s = new QSplitter(Qt::Horizontal);
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    if (out_type & GUI_PRINT) fprintf(stderr, "HSplitter %dx%d\n", width, height);
    return s;
}

JX_SPLITTER * GuiCreator::createQtVsplitter(jx::Object splitter, int size)
{
    int width = getWidth(splitter);
    int height = getHeight(splitter);
    QSplitter *s = new QSplitter(Qt::Vertical);
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    if (out_type & GUI_PRINT) fprintf(stderr, "VSplitter %dx%d\n", width, height);
    return s;
}
#endif

void GuiCreator::printWidgetInfo(jx::Object entity)
{
  int width = getWidth(entity);
  int height = getHeight(entity);
  jx::Object src = getSource(entity);
  if(entity.list_borrow(0) == openglContextType) {
     fprintf(stderr,"OpenGL widget %s %dx%d\n", src.as_str(), width, height);
  } else if(entity.list_borrow(0) == navigatorType) {
     fprintf(stderr,"Navigator widget %s %dx%d\n", src.as_str(), width, height);
  } else if(entity.list_borrow(0) == menuBarType) {
     fprintf(stderr,"MenuBar widget %s %dx%d\n", src.as_str(), width, height);
     if (out_type == GUI_PRINT) processMenuItems(entity,0,NULL);
  } else {
     fprintf(stderr,"Generic widget %s %dx%d\n", src.as_str(), width, height);
  }
  //jx_ob_free(src);
}


JX_MENU * GuiCreator::menuAction(jx::Object item, JX_MENU *menu_widget, jx::Object label, jx::Object callback, jx::Object checkbox, jx::Object popup)
{
  JX_MENU *sub_menu;
#ifdef JX_QT

  int has_checkbox;
  if (checkbox.null_check()) {
    has_checkbox = 0;
  } else {
    if (!strncmp(checkbox.as_str(), "on", 2)) {
      has_checkbox =  1;
    } else {
      has_checkbox = -1;
    }
  }

  if (out_type & GUI_QTUI) {
    if (item.list_borrow(1).size() > 0) { 
      sub_menu = new JX_MENU(label.as_str());
      menu_widget->addMenu(sub_menu);
    } else {

      JXAction *theAct = new JXAction(label.as_str(), callback, window);
      //Actions.push_back(theAct);
      //theAct->setShortcut(tr("Ctrl+B"));
      menu_widget->addAction(theAct);
      if (!callback.null_check()) {
        if (jx_builtin_callable_check(callback.get_jxob())) {
          if (has_checkbox) {
            QObject::connect(theAct, SIGNAL(triggered(bool)), theAct, SLOT(doCallback(bool)));
          } else if ( !strncmp( popup.as_str(), "fileDialog", 10) ) {
            QObject::connect(theAct, SIGNAL(triggered()), theAct, SLOT(openFile()));
          } else if ( !strncmp( label.as_str(), "Exit", 4) ) {
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

JX_MENU * GuiCreator::processMenuItem(jx::Object item, JX_MENU * menu_widget) {
/*
   deal with attributes of a single menu item: label, checkbox, callback.
   This creates and adds an Action if this item is not a list.
   This creates and adds a Menu (sub_menu) if this item is a list.
   This returns any sub_menu that might be created.
   A sub_menu will display a label, but ignore any checkbox or callback.
*/
  //jx_ob checkbox=jx_ob(), callback=jx_ob(), label=jx_ob(), popup=jx_ob();

/* label attribute */
  jx::Object label = getMenuItem(item, "label");
  if (out_type & GUI_PRINT) fprintf(stderr," label: %s", label.as_str());

/* checkbox attribute */
  jx::Object checkbox = getMenuItem(item, "checkbox");
  if ((out_type & GUI_PRINT) & !checkbox.null_check()) fprintf(stderr," checkbox: %s", checkbox.as_str());

/* callback attribute */
  jx::Object callback = getMenuItem(item, "callback");
  jx::Object popup = getMenuItem(item, "popup");
  if (callback.null_check()) {
    if (out_type & GUI_PRINT) fprintf(stderr,"\n");
  } else if (jx_builtin_callable_check(callback.get_jxob())) {
    if (out_type & GUI_PRINT) callback.jxon_dump(stderr, " callback");
  } else {
    if (out_type & GUI_PRINT) callback.jxon_dump(stderr, " unknown callback");
  }

  
  JX_MENU *sub_menu = menuAction(item, menu_widget, label, callback, checkbox, popup);
/*
  jx_ob_free(label);
  jx_ob_free(checkbox);
  jx_ob_free(popup);
*/
  // needed for later callbacks.  see also freeActions()
  //if (out_type & GUI_QTUI); else jx_ob_free(callback);

  return sub_menu;
}

void GuiCreator::processMenuItems(jx::Object menu, int depth, JX_MENU *menu_widget)
{
/*
  recursively process menuItemTypes
*/
  std::string blanx(depth*2, ' ');  // for pretty printing
  jx::Object menu_items = menu.list_borrow(1);
  for (int i=0; i<menu_items.size(); ++i) {
    jx::Object item = menu_items.list_borrow(i);
    if(item.list_borrow(0) == menuItemType) {
      if (out_type & GUI_PRINT) fprintf(stderr,"%s  item", blanx.c_str());
      JX_MENU *sub_menu = processMenuItem(item, menu_widget);
      processMenuItems(item, depth+1, (JX_MENU *)sub_menu);
    } else if (item.str_check()) {
#ifdef JX_QT
      if (out_type & GUI_QTUI) menu_widget->addAction(item.as_str());
#endif
      if (out_type & GUI_PRINT) fprintf(stderr,"%s  string %s\n", blanx.c_str(), item.as_str());
    } else if (item.null_check()) {
      if (out_type & GUI_PRINT) fprintf(stderr,"%s  null\n", blanx.c_str());
    } else {
      if (out_type & GUI_PRINT) fprintf(stderr,"%s  other type %d\n", blanx.c_str(), item.type());
    }
  }
}

void GuiCreator::printFrameHtml(jx::Object entity)
{
   int width = getWidth(entity);
   int height = getHeight(entity);
   jx::Object src = getSource(entity);
   printf("  <FRAME SRC=\"%s\"", src.as_str());
   if (width > 0)  printf(" WIDTH=%d", width);
   if (height > 0) printf(" HEIGHT=%d", height);
   printf(">\n");
   //jx_ob_free(src);
}

void GuiCreator::printHFramesetHtml(jx::Object pane, int size)
{
    printf("<FRAMESET COLS=");
    int width = getWidth(pane.list_borrow(0));
    //int height = getHeight(jx_list_borrow(pane,0));
    printVal(width);
    for(int i=1;i<size;i++) {
      width = getWidth(pane.list_borrow(i));
      printf (",");
      printVal(width);
    }
    printf (">\n");
}

void GuiCreator::printVFramesetHtml(jx::Object pane, int size)
{
    printf("<FRAMESET ROWS=");
    int height = getHeight(pane.list_borrow(0));
    //int width = getWidth(jx_list_borrow(pane,0));
    printVal(height);
    for(int i=1;i<size;i++) {
      height = getHeight(pane.list_borrow(i));
      printf (",");
      printVal(height);
    }
    printf (">\n");
}

JX_SPLITTER * GuiCreator::processHSplitter(jx::Object splitter)
{
  jx::Object pane = splitter.list_borrow(1);
  jx_int i,size = pane.size();

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getWidth(splitter);
  int height = getHeight(splitter);
#endif
  if (out_type & GUI_PRINT) fprintf(stderr,"processing horizontal splitter %d widget...\n", size);
  if (out_type & GUI_HTML) printHFramesetHtml(pane,size);
#ifdef JX_QT
  if (out_type & GUI_QTUI) {
    s = createQtHsplitter(splitter,size);
  }
#endif

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    w = processComponents(pane.list_borrow(i));
#ifdef JX_QT
    if (out_type & GUI_QTUI) {
       s->addWidget(w);
       if (height) w->setFixedHeight(height);
    }
#endif
  }
  if (out_type & GUI_HTML) printf("</FRAMESET>\n");
  return s;
}

JX_SPLITTER * GuiCreator::processVSplitter(jx::Object splitter)
{
  jx::Object pane = splitter.list_borrow(1);
  jx_int i,size = pane.size();

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getWidth(splitter);
  int height = getHeight(splitter);
#endif
  if (out_type & GUI_PRINT) fprintf(stderr,"processing vertical splitter %d widget...\n", size);
  if (out_type & GUI_HTML)  printVFramesetHtml(pane, size);
#ifdef JX_QT
  if (out_type & GUI_QTUI) {
    s = createQtVsplitter(splitter,size);
  }
#endif

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    w = processComponents(pane.list_borrow(i));
#ifdef JX_QT
    if (out_type & GUI_QTUI) {
       s->addWidget(w);
       if (width) w->setFixedWidth(width);
    }
#endif
  }
  if (out_type & GUI_HTML) printf("</FRAMESET>\n");
  return s;
}

JX_WIDGET * GuiCreator::processWidget(jx::Object widget)
{
  JX_WIDGET *w;
  if (out_type & GUI_PRINT) printWidgetInfo(widget);
  if (out_type & GUI_HTML)  printFrameHtml(widget);
#ifdef JX_QT
  if (out_type & GUI_QTUI) {
    w = createQtWidget(widget);
  }
#endif
  return w;
}

JX_WIDGET * GuiCreator::processComponents(jx::Object component)
{
  jx::Object comp_type = component.list_borrow(0);

  comp_type.jxon_dump(stderr, "comp_type");

  if(comp_type == hSplitterType) {

    return processHSplitter(component);

  } else if(comp_type == vSplitterType) {

    return processVSplitter(component);    

  } else { 

    return processWidget(component);

  }
}

jx_ob GuiCreator::get_symbol_from_node(jx::Hash node, const char *ident)
{
/* either style of creating tmp works */
  //jx::Object tmp = jx::Object::from_ident(ident);
  jx::Ident tmp = ident;
  jx_ob symbol = jx_hash_get(node.get_jxob(),tmp.get_jxob());
  return symbol;
}

/*
jx_status GuiCreator::freeKnowns()
{
  jx_ob_free vSplitterType;
  jx_ob_free hSplitterType;
  jx_ob_free menuBarType;
  jx_ob_free menuItemType;
  jx_ob_free openglContextType;
  jx_ob_free navigatorType;
  return JX_STATUS_SUCCESS;
}
*/

jx_status GuiCreator::locateKnowns(jx::Hash node)
{

  vSplitterType     = get_symbol_from_node(node, "VSplitter");
  hSplitterType     = get_symbol_from_node(node, "HSplitter");
  menuBarType       = get_symbol_from_node(node, "MenuBar");
  menuItemType      = get_symbol_from_node(node, "MenuItem");
  openglContextType = get_symbol_from_node(node, "OpenGLContext");
  navigatorType     = get_symbol_from_node(node, "Navigator");

    return JX_SUCCESS;
}

JX_WIDGET * GuiCreator::gui_run_from_node(jx::Hash node)
{
  
  JX_WIDGET *w;
  if(jx_ok( locateKnowns(node) )) {  

    /* get the root gui component */

    jx::Object gui = get_symbol_from_node(node, "gui");
    this->gui = gui;
    
    /* dump graph for debuggin' */
    
    //gui.jxon_dump(stdout,"node");
    //gui.jxon_dump(stdout,"gui");
    
    /* traverse the graph nondestructively, processing each component in turn */
    
    //printf("processing components...\n");
    
    w = processComponents(gui);
    
    /* would now run the GUI */
    
    /* when done...free what we took */

    //freeKnowns();
    //jx_ob_free(gui);

    return w;
  }
}
