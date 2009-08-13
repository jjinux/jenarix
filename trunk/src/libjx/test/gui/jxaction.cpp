#include "jxaction.h"
#include <QDebug>
#include <QApplication>

JXAction::JXAction(const QString & title, jx::Ob *acallback, QWidget *parent)
    : QAction(title, parent)
{
   callback = jx_ob_copy(acallback->ob());
}

JXAction::~JXAction()
{
   jx_ob_free(callback);
}

void JXAction::doCallback(bool checked)
{
    jx_ob fn = jx_ob_copy(callback);
    jx_function_call(jx_ob_from_null(), fn, jx_ob_from_bool(checked)); // fn is consumed, somehow
}

void JXAction::doCallback(QString s)
{
    jx_ob fn = jx_ob_copy(callback);
    jx_ob arg= jx_ob_from_str(s.toAscii().data());
    jx_function_call(jx_ob_from_null(), fn, arg);
}

void JXAction::doCallback()
{
    jx_ob fn = jx_ob_copy(callback);
    jx_function_call(jx_ob_from_null(), fn, jx_ob());
}

/*
void JXAction::doExit()
{
    jx_ob fn = jx_ob_copy(callback);
    jx_function_call(jx_ob_from_null(), fn, jx_ob());
    freeActions();
    exit(1);
}
*/

/*
void JXAction::openFile()
{
  QString fileName = QFileDialog::getOpenFileName();
  if (!fileName.isEmpty())
    doCallback(fileName);
}
*/

void JXAction::freeActions() {
/* called from qtBuiltins */
  //QList<JXAction *> kids = parentWidget()->findChildren<JXAction *>();
  QList<JXAction *> kids = qApp->activeWindow()->findChildren<JXAction *>();
  for (int i=0; i < kids.size(); ++i) {
    //qDebug() << kids.at(i)->text();
    jx_ob_free(kids.at(i)->callback);
    /* don't delete exit action - it called this! */
    //delete kids.at(i);
  }
}
