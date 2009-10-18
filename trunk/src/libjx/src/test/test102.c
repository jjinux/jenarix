

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

#include "jx_public.h"
#include "jx_main_tools.h"

typedef struct {
  jx_ob hSplitterType;
  jx_ob vSplitterType;
  jx_ob menuBarType;
  jx_ob openglContextType;
  jx_ob navigatorType;
} Knowns;

static void processComponents(jx_ob component, Knowns * known);

static void processHSplitter(jx_ob splitter, Knowns * known)
{
  jx_ob pane = jx_list_borrow(splitter, 1);
  jx_int i, size = jx_list_size(pane);
  printf("processing horizontal splitter widget...\n");
  for(i = 0; i < size; i++) {
    processComponents(jx_list_borrow(pane, i), known);
  }
}

static void processVSplitter(jx_ob splitter, Knowns * known)
{
  jx_ob pane = jx_list_borrow(splitter, 1);
  jx_int i, size = jx_list_size(pane);
  printf("processing vertical splitter widget...\n");
  for(i = 0; i < size; i++) {
    processComponents(jx_list_borrow(pane, i), known);
  }
}

static void processComponents(jx_ob component, Knowns * known)
{
  jx_ob comp_type = jx_list_borrow(component, 0);

  if(jx_ob_identical(comp_type, known->hSplitterType)) {

    processHSplitter(component, known);

  } else if(jx_ob_identical(comp_type, known->vSplitterType)) {

    processVSplitter(component, known);

  } else if(jx_ob_identical(comp_type, known->menuBarType)) {

    printf("processing menu bar widget...\n");

  } else if(jx_ob_identical(comp_type, known->openglContextType)) {

    printf("processing opengl context widget...\n");

  } else if(jx_ob_identical(comp_type, known->navigatorType)) {

    printf("processing navigator widget...\n");

  } else {

    printf("processing generic widget...\n");

  }
}

static jx_ob get_symbol_from_node(jx_ob node, jx_char * ident)
{
  jx_ob tmp = jx_ob_from_ident(ident);
  jx_ob symbol = jx_hash_get(node, tmp);
  jx_ob_free(tmp);
  return symbol;
}

static jx_status locateKnowns(jx_ob node, Knowns * known)
{
  jx_os_memset(known, 0, sizeof(Knowns));

  known->vSplitterType = get_symbol_from_node(node, "VSplitter");
  known->hSplitterType = get_symbol_from_node(node, "HSplitter");
  known->menuBarType = get_symbol_from_node(node, "MenuBar");
  known->openglContextType = get_symbol_from_node(node, "OpenGLContext");
  known->navigatorType = get_symbol_from_node(node, "Navigator");

  return JX_SUCCESS;
}

static freeKnowns(Knowns * known)
{
  jx_ob_free(known->vSplitterType);
  jx_ob_free(known->hSplitterType);
  jx_ob_free(known->menuBarType);
  jx_ob_free(known->openglContextType);
  jx_ob_free(known->navigatorType);
}

static void gui_run_from_node(jx_ob node)
{
  Knowns known;

  if(jx_ok(locateKnowns(node, &known))) {

    /* get the root gui component */

    jx_ob gui = get_symbol_from_node(node, "gui");

    /* dump graph for debuggin' */

    jx_jxon_dump(stdout, "gui", gui);

    /* traverse the graph nondestructively, processing each component in turn */

    printf("processing components...\n");

    processComponents(gui, &known);

    /* would now run the GUI */

    /* when done...free what we took */

    jx_ob_free(gui);

    freeKnowns(&known);
  }
}

int main(int argc, char *argv[])
{
  int exit_status = JX_EXIT_FAILURE;

  if(jx_ok(jx_os_process_init(argc, argv))) {
    exit_status = JX_EXIT_SUCCESS;

    jx_ob node = jx_hash_new();

    if(jx_ok(jx_main_exec_in_node(argc, argv, node))) {
      gui_run_from_node(node);
    }

    jx_ob_free(node);
    jx_os_process_complete();
  }
  return exit_status;
}
