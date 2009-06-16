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

#include "gui.h"

/*
typedef struct {
  jx_ob hSplitterType;
  jx_ob vSplitterType;
  jx_ob menuBarType;
  jx_ob openglContextType;
  jx_ob navigatorType;
} Knowns;
*/


Gui::Gui(QWidget *parent)
{
}
Gui::~Gui()
{
}

void Gui::processHSplitter(jx_ob splitter)
{
  jx_ob pane = jx_list_borrow(splitter,1);
  jx_int i,size = jx_list_size(pane);
  printf("processing horizontal splitter widget...\n");
  for(i=0;i<size;i++) {
    processComponents(jx_list_borrow(pane,i));
  }
}

void Gui::processVSplitter(jx_ob splitter)
{
  jx_ob pane = jx_list_borrow(splitter,1);
  jx_int i,size = jx_list_size(pane);
    printf("processing vertical splitter widget...\n");
  for(i=0;i<size;i++) {
    processComponents(jx_list_borrow(pane,i));
  }
}

void Gui::processComponents(jx_ob component)
{
  jx_ob comp_type = jx_list_borrow(component,0);

  if(jx_ob_identical(comp_type, hSplitterType)) {

    processHSplitter(component);

  } else if(jx_ob_identical(comp_type, vSplitterType)) {

    processVSplitter(component);    

  } else if(jx_ob_identical(comp_type, menuBarType)) {
    
    printf("processing menu bar widget...\n");

  } else if(jx_ob_identical(comp_type, openglContextType)) {
    
    printf("processing opengl context widget...\n");

  } else if(jx_ob_identical(comp_type, navigatorType)) {
    
    printf("processing navigator widget...\n");

  } else { 

    printf("processing generic widget...\n");

  }
}

jx_ob Gui::get_symbol_from_node(jx_ob node, jx_char *ident)
{
  jx_ob tmp = jx_ob_from_ident(ident);
  jx_ob symbol = jx_hash_get(node,tmp);
  jx_ob_free(tmp);
  return symbol;
}

jx_status Gui::locateKnowns(jx_ob node)
{
  //jx_os_memset(known,0,sizeof(known));

  vSplitterType = get_symbol_from_node(node, "VSplitter");
  hSplitterType = get_symbol_from_node(node, "HSplitter");
  menuBarType = get_symbol_from_node(node, "MenuBar");
  openglContextType = get_symbol_from_node(node, "OpenGLContext");
  navigatorType = get_symbol_from_node(node, "Navigator");

  return JX_SUCCESS;
}

void Gui::gui_run_from_node(jx_ob node)
{
  
  if(jx_ok( locateKnowns(node) )) {  

    /* get the root gui component */

    jx_ob gui = get_symbol_from_node(node, "gui");
    
    /* dump graph for debuggin' */
    
    jx_jxon_dump(stdout,"node",node);
    jx_jxon_dump(stdout,"gui",gui);
    
    /* traverse the graph nondestructively, processing each component in turn */
    
    printf("processing components...\n");
    
    processComponents(gui);
    
    /* would now run the GUI */
    
    /* when done...free what we took */

    jx_ob_free(gui);

  }
}
