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

void GuiCreator::setOutputType(char *a) 
{
  if (!strncmp(a,"-html",5)) {
    out_type = GUI_HTML;
  } else if (!strncmp(a,"-qt",3)) {
    out_type = GUI_QTUI;
  } else {
    out_type = GUI_NONE;
  } }

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
          return jx_ob_from_str("");
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

void GuiCreator::printFrameHtml(jx_ob widget) {
    int width = getWidth(widget);
    int height = getHeight(widget);
    jx_ob src = getSource(widget);
    printf("  <FRAME SRC=\"%s\"", jx_ob_as_str(&src));
    if (width > 0)  printf(" WIDTH=%d", width);
    if (height > 0) printf(" HEIGHT=%d", height);
    printf(">\n");
}

void GuiCreator::processHSplitter(jx_ob splitter)
{
  jx_ob pane = jx_list_borrow(splitter,1);
  jx_int i,size = jx_list_size(pane);

  if (out_type == GUI_HTML) {
    printf("<FRAMESET COLS=");
    int width = getWidth(jx_list_borrow(pane,0));
    //int height = getHeight(jx_list_borrow(pane,0));
    printVal(width);
    for(i=1;i<size;i++) {
      width = getWidth(jx_list_borrow(pane,i));
      printf (",");
      printVal(width);
    }
    printf (">\n");
  } else if (out_type == GUI_QTUI) {
  } else {
    printf("processing %d horizontal splitter widget...\n", size);
  }

  for(i=0;i<size;i++) {
    processComponents(jx_list_borrow(pane,i));
  }
  if (out_type == GUI_HTML) printf("</FRAMESET>\n");
}

void GuiCreator::processVSplitter(jx_ob splitter)
{
  jx_ob pane = jx_list_borrow(splitter,1);
  jx_int i,size = jx_list_size(pane);

  if (out_type == GUI_HTML) {
    printf("<FRAMESET ROWS=");
    int height = getHeight(jx_list_borrow(pane,0));
    //int width = getWidth(jx_list_borrow(pane,0));
    printVal(height);
    for(i=1;i<size;i++) {
      height = getHeight(jx_list_borrow(pane,i));
      printf (",");
      printVal(height);
    }
    printf (">\n");
  } else if (out_type == GUI_QTUI) {
  } else {
    printf("processing %d horizontal splitter widget...\n", size);
  }

  for(i=0;i<size;i++) {
    processComponents(jx_list_borrow(pane,i));
  }
  if (out_type == GUI_HTML) printf("</FRAMESET>\n");
}

void GuiCreator::processOpenGL(jx_ob widget)
{
  if (out_type == GUI_HTML) {
    printFrameHtml(widget);
  } else if (out_type == GUI_QTUI) {
  } else {
    printf("processing opengl context widget...\n");
  }
}

void GuiCreator::processNavigator(jx_ob widget)
{
  if (out_type == GUI_HTML) {
    printFrameHtml(widget);
  } else if (out_type == GUI_QTUI) {
  } else {
    printf("processing navigator widget...\n");
  }
}

void GuiCreator::processMenuBar(jx_ob widget)
{
  if (out_type == GUI_HTML) {
    printFrameHtml(widget);
  } else if (out_type == GUI_QTUI) {
  } else {
    printf("processing menu bar widget...\n");
  }
}

void GuiCreator::processWidget(jx_ob widget)
{
  if (out_type == GUI_HTML) {
    printFrameHtml(widget);
  } else if (out_type == GUI_QTUI) {
  } else {
    printf("processing generic widget...\n");
  }
}

void GuiCreator::processComponents(jx_ob component)
{
  jx_ob comp_type = jx_list_borrow(component,0);

  if(jx_ob_identical(comp_type, hSplitterType)) {

    processHSplitter(component);

  } else if(jx_ob_identical(comp_type, vSplitterType)) {

    processVSplitter(component);    

  } else if(jx_ob_identical(comp_type, menuBarType)) {
    
    processMenuBar(component);    

  } else if(jx_ob_identical(comp_type, openglContextType)) {
    
    processOpenGL(component);

  } else if(jx_ob_identical(comp_type, navigatorType)) {
    
    processNavigator(component);

  } else { 

    processWidget(component);

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

void GuiCreator::gui_run_from_node(jx_ob node)
{
  
  if(jx_ok( locateKnowns(node) )) {  

    /* get the root gui component */

    jx_ob gui = get_symbol_from_node(node, "gui");
    
    /* dump graph for debuggin' */
    
    //jx_jxon_dump(stdout,"node",node);
    //jx_jxon_dump(stdout,"gui",gui);
    
    /* traverse the graph nondestructively, processing each component in turn */
    
    //printf("processing components...\n");
    
    processComponents(gui);
    
    /* would now run the GUI */
    
    /* when done...free what we took */

    freeKnowns();
    jx_ob_free(gui);

  }
}
