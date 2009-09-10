#include "jxaction.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>

JXAction::JXAction(const QString & title, jx::Ob *acallback, QWidget *parent)
    : QAction(title, parent)
{
   callback = jx_ob_copy(acallback->ob());
}

JXAction::~JXAction()
{
   jx_ob_free(callback);
}

jx_ob JXAction::getCallback() { return callback; }

void JXAction::callCallback(jx_ob arg) {
    if (jx_builtin_callable_check(callback)) {
      jx_ob fn = jx_ob_copy(callback);
      jx_function_call(jx_ob_from_null(), fn, arg);
    } else if (jx_ident_check(callback)) {
      //printf ("unbound function %s\n", jx_ob_as_ident(&callback));
      QMessageBox msgBox;
      msgBox.setText(QString("unbound function ") + QString(jx_ob_as_ident(&callback)));
      msgBox.exec();
    }
}

void JXAction::doCallback(bool checked)
{
    //jx_ob fn = jx_ob_copy(callback);
    //jx_function_call(jx_ob_from_null(), fn, jx_ob_from_bool(checked)); // fn is consumed, somehow
    callCallback(jx_ob_from_bool(checked)); // fn is consumed, somehow
}

void JXAction::doCallback(QString s)
{
    jx_ob arg= jx_ob_from_str(s.toAscii().data());
    //jx_function_call(jx_ob_from_null(), fn, arg);
    callCallback(arg);
}

void JXAction::doCallback()
{
    //jx_ob fn = jx_ob_copy(callback);
    //jx_function_call(jx_ob_from_null(), fn, jx_ob());
    callCallback(jx_ob());
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
