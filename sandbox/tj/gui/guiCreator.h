#ifndef GUICREATOR_H
#define GUICREATOR_H

#include "jx_public.h"
#include "jx_main_tools.h"

#define GUI_NONE 0
#define GUI_HTML 1
#define GUI_QTUI 2
class GuiCreator
{

public:
    GuiCreator(int otype=GUI_NONE);
    ~GuiCreator();
    void gui_run_from_node(jx_ob node);
    void setOutputType(char *a);

private:
    int out_type;
    void printVal(int v);
    void getSize(int *width, int *height, jx_ob widget);
    void processComponents(jx_ob component);
    void processHSplitter(jx_ob splitter);
    void processVSplitter(jx_ob splitter);
    void processOpenGL(jx_ob widget);
    void processNavigator(jx_ob widget);
    void processMenuBar(jx_ob widget);
    void processWidget(jx_ob widget);

    jx_ob get_symbol_from_node(jx_ob node, jx_char *ident);
    jx_status freeKnowns();
    jx_status locateKnowns(jx_ob node);

    //struct known {
      jx_ob hSplitterType;
      jx_ob vSplitterType;
      jx_ob menuBarType;
      jx_ob openglContextType;
      jx_ob navigatorType;
    //};

};

#endif
