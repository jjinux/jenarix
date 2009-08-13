#ifndef JXACTION_H
#define JXACTION_H

#include <QAction>
#include <QFileDialog>
#include "jx_public.h"
#include "jxobject.h"

class JXAction : public QAction
{
    Q_OBJECT

public:
    JXAction(const QString & title, jx::Ob *callback, QWidget *parent = 0);
    ~JXAction();

private:
    jx_ob callback;

public slots:
    void doCallback(bool checked);
    void doCallback(QString s);
    void doCallback();
    //void doExit();
    void openFile();
    static void freeActions();

};
#endif
