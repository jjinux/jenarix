#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include "jx_public.h"
#include "jx_main_tools.h"

class Gui : public QWidget
{
    Q_OBJECT

public:
    Gui(QWidget *parent = 0);
    ~Gui();
    void gui_run_from_node(jx_ob node);

private:
    void processComponents(jx_ob component);
    void processHSplitter(jx_ob splitter);
    void processVSplitter(jx_ob splitter);
    jx_ob get_symbol_from_node(jx_ob node, jx_char *ident);
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
