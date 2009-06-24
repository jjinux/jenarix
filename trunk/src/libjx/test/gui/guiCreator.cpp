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
#include <QtDebug>
#include <QTextEdit>
#include <QtOpenGL>
#endif

/*
typedef struct {
  jx_ob hSplitterType;
  jx_ob vSplitterType;
  jx_ob menuBarType;
  jx_ob openglContextType;
  jx_ob navigatorType;
} Knowns;
*/


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
    out_type = GUI_HTML;
    return true;
  } else if (!strncmp(a,"-qt",3)) {
    out_type = GUI_QTUI;
    return true;
  } else {
    out_type = GUI_NONE;
    return false;
  }
}

jx_ob GuiCreator::getSource(jx_ob entity) 
{
    jx_ob source = get_symbol_from_node(jx_list_borrow(entity, 2), "source");
    if (jx_null_check(source)) {
      if(jx_ob_identical(jx_list_borrow(entity,0), openglContextType)) {
          return jx_ob_from_str("opengl.html");
      } else if(jx_ob_identical(jx_list_borrow(entity,0), navigatorType)) {
          return jx_ob_from_str("navigator.html");
      } else if(jx_ob_identical(jx_list_borrow(entity,0), menuBarType)) {
          return jx_ob_from_str("menubar.html");
      } else  {
          //return jx_ob_from_str("widget.html");
          return jx_ob_from_str("None");
      }
    }
    return source;
}

int GuiCreator::getWidth(jx_ob entity) 
{
    int width = 0;
    width = jx_ob_as_int(get_symbol_from_node(jx_list_borrow(entity, 2), "width"));
/*
    if (width == 0) {
        width = 10;
    }
*/
    return width;
}
int GuiCreator::getHeight(jx_ob entity) 
{
    int height = 0;
    height = jx_ob_as_int(get_symbol_from_node(jx_list_borrow(entity, 2), "height"));
    if (height == 0) {
      if(jx_ob_identical(jx_list_borrow(entity,0), menuBarType)) {
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
JX_WIDGET * GuiCreator::createQtWidget(jx_ob entity)
{
    int width = getWidth(entity);
    int height = getHeight(entity);
    jx_ob src = getSource(entity);
    qDebug() << "create Widget" << jx_ob_as_str(&src) << width << height;
    if(jx_ob_identical(jx_list_borrow(entity,0), openglContextType)) {
      //GLWidget *w = new GLWidget(window);
      GLWidget *w = new GLWidget;
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      //w->setMinimumSize(width,height);
      //w->swapBuffers();
      return w;
    } else if(jx_ob_identical(jx_list_borrow(entity,0), navigatorType)) {
      //QTextEdit *w = new QTextEdit(window);
      QTextEdit *w = new QTextEdit;
      w->setPlainText(jx_ob_as_str(&src));
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      //w->setMinimumSize(width,height);
      return w;
    } else if(jx_ob_identical(jx_list_borrow(entity,0), menuBarType)) {
      //QTextEdit *w = new QTextEdit(window);
      QTextEdit *w = new QTextEdit;
      w->setPlainText(jx_ob_as_str(&src));
      w->setFixedHeight(height);
      //w->setMinimumHeight(height);
      //w->setMaximumHeight(height);
      return w;
    } else  {
      //QTextEdit *w = new QTextEdit(window);
      QTextEdit *w = new QTextEdit;
      w->setPlainText(jx_ob_as_str(&src));
      if (width > 0)  w->setFixedWidth(width);
      if (height > 0) w->setFixedHeight(height);
      //w->setMinimumSize(width,height);
      return w;
    }
}

JX_SPLITTER * GuiCreator::createQtHsplitter(jx_ob splitter, int size)
{
    int width = getWidth(splitter);
    int height = getHeight(splitter);
    //QSplitter *s = new QSplitter(Qt::Horizontal, window);
    QSplitter *s = new QSplitter(Qt::Horizontal);
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    //s->setMinimumSize(width,height);
    qDebug() << s << width << height;
    return s;
}

JX_SPLITTER * GuiCreator::createQtVsplitter(jx_ob splitter, int size)
{
    int width = getWidth(splitter);
    int height = getHeight(splitter);
    //QSplitter *s = new QSplitter(Qt::Vertical, window);
    QSplitter *s = new QSplitter(Qt::Vertical);
    if (width > 0)  s->setFixedWidth(width);
    if (height > 0) s->setFixedHeight(height);
    //s->setMinimumSize(width,height);
    qDebug() << s << width << height;
    return s;
}
#endif

void GuiCreator::printWidgetInfo(jx_ob entity)
{
  int width = getWidth(entity);
  int height = getHeight(entity);
  jx_ob src = getSource(entity);
  if(jx_ob_identical(jx_list_borrow(entity,0), openglContextType)) {
     printf("OpenGL widget %s %d %d\n", jx_ob_as_str(&src), width, height);
  } else if(jx_ob_identical(jx_list_borrow(entity,0), navigatorType)) {
     printf("Navigator widget %s %d %d\n", jx_ob_as_str(&src), width, height);
  } else if(jx_ob_identical(jx_list_borrow(entity,0), menuBarType)) {
     printf("MenuBar widget %s %d %d\n", jx_ob_as_str(&src), width, height);
  } else {
     printf("Generic widget %s %d %d\n", jx_ob_as_str(&src), width, height);
  }
}

void GuiCreator::printFrameHtml(jx_ob entity)
{
    int width = getWidth(entity);
    int height = getHeight(entity);
    jx_ob src = getSource(entity);
    printf("  <FRAME SRC=\"%s\"", jx_ob_as_str(&src));
    if (width > 0)  printf(" WIDTH=%d", width);
    if (height > 0) printf(" HEIGHT=%d", height);
    printf(">\n");
}

void GuiCreator::printHFramesetHtml(jx_ob pane, int size)
{
    printf("<FRAMESET COLS=");
    int width = getWidth(jx_list_borrow(pane,0));
    //int height = getHeight(jx_list_borrow(pane,0));
    printVal(width);
    for(int i=1;i<size;i++) {
      width = getWidth(jx_list_borrow(pane,i));
      printf (",");
      printVal(width);
    }
    printf (">\n");
}

void GuiCreator::printVFramesetHtml(jx_ob pane, int size)
{
    printf("<FRAMESET ROWS=");
    int height = getHeight(jx_list_borrow(pane,0));
    //int width = getWidth(jx_list_borrow(pane,0));
    printVal(height);
    for(int i=1;i<size;i++) {
      height = getHeight(jx_list_borrow(pane,i));
      printf (",");
      printVal(height);
    }
    printf (">\n");
}

JX_SPLITTER * GuiCreator::processHSplitter(jx_ob splitter)
{
  jx_ob pane = jx_list_borrow(splitter,1);
  jx_int i,size = jx_list_size(pane);

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getWidth(splitter);
  int height = getHeight(splitter);
#endif
  if (out_type == GUI_HTML) {
    printHFramesetHtml(pane,size);
#ifdef JX_QT
  } else if (out_type == GUI_QTUI) {
    s = createQtHsplitter(splitter,size);
    //qDebug() << "create HSplitter" << s;
#endif
  } else {
    printf("processing %d horizontal splitter widget...\n", size);
  }

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    w = processComponents(jx_list_borrow(pane,i));
#ifdef JX_QT
    if (out_type == GUI_QTUI) {
       s->addWidget(w);
       if (height) w->setFixedHeight(height);
       //qDebug() << "  add Widget" << w << " to " << s;
    }
#endif
  }
  if (out_type == GUI_HTML) printf("</FRAMESET>\n");
  return s;
}

JX_SPLITTER * GuiCreator::processVSplitter(jx_ob splitter)
{
  jx_ob pane = jx_list_borrow(splitter,1);
  jx_int i,size = jx_list_size(pane);

  JX_SPLITTER *s;
#ifdef JX_QT
  int width = getWidth(splitter);
  int height = getHeight(splitter);
#endif
  if (out_type == GUI_HTML) {
    printVFramesetHtml(pane, size);
#ifdef JX_QT
  } else if (out_type == GUI_QTUI) {
    s = createQtVsplitter(splitter,size);
    //qDebug() << "create VSplitter" << s;
#endif
  } else {
    printf("processing %d horizontal splitter widget...\n", size);
  }

  JX_WIDGET *w;
  for(i=0;i<size;i++) {
    w = processComponents(jx_list_borrow(pane,i));
#ifdef JX_QT
    if (out_type == GUI_QTUI) {
       s->addWidget(w);
       if (width) w->setFixedWidth(width);
       //qDebug() << "  add Widget" << w << " to " << s;
    }
#endif
  }
  if (out_type == GUI_HTML) printf("</FRAMESET>\n");
  return s;
}

JX_WIDGET * GuiCreator::processWidget(jx_ob widget)
{
  JX_WIDGET *w;
  if (out_type == GUI_HTML) {
    printFrameHtml(widget);
#ifdef JX_QT
  } else if (out_type == GUI_QTUI) {
    w = createQtWidget(widget);
#endif
  } else {
    printWidgetInfo(widget);
  }
  return w;
}

JX_WIDGET * GuiCreator::processComponents(jx_ob component)
{
  jx_ob comp_type = jx_list_borrow(component,0);

  if(jx_ob_identical(comp_type, hSplitterType)) {

    return processHSplitter(component);

  } else if(jx_ob_identical(comp_type, vSplitterType)) {

    return processVSplitter(component);    

  } else { 

    return processWidget(component);

  }
}

jx_ob GuiCreator::get_symbol_from_node(jx_ob node, jx_char *ident)
{
  jx_ob tmp = jx_ob_from_ident(ident);
  jx_ob symbol = jx_hash_get(node,tmp);
  jx_ob_free(tmp);
  return symbol;
}

jx_status GuiCreator::freeKnowns()
{
  jx_ob_free(vSplitterType);
  jx_ob_free(hSplitterType);
  jx_ob_free(menuBarType);
  jx_ob_free(openglContextType);
  return jx_ob_free(navigatorType);
}

jx_status GuiCreator::locateKnowns(jx_ob node)
{
  //jx_os_memset(known,0,sizeof(known));

  vSplitterType = get_symbol_from_node(node, "VSplitter");
  hSplitterType = get_symbol_from_node(node, "HSplitter");
  menuBarType = get_symbol_from_node(node, "MenuBar");
  openglContextType = get_symbol_from_node(node, "OpenGLContext");
  navigatorType = get_symbol_from_node(node, "Navigator");

  return JX_SUCCESS;
}

JX_WIDGET * GuiCreator::gui_run_from_node(jx_ob node)
{
  
  JX_WIDGET *w;
  if(jx_ok( locateKnowns(node) )) {  

    /* get the root gui component */

    jx_ob gui = get_symbol_from_node(node, "gui");
    
    /* dump graph for debuggin' */
    
    //jx_jxon_dump(stdout,"node",node);
    //jx_jxon_dump(stdout,"gui",gui);
    
    /* traverse the graph nondestructively, processing each component in turn */
    
    //printf("processing components...\n");
    
    w = processComponents(gui);
    
    /* would now run the GUI */
    
    /* when done...free what we took */

    freeKnowns();
    jx_ob_free(gui);

    return w;
  }
}
