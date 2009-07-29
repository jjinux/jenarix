#include "jxaction.h"
#include <QDebug>
#include <QApplication>

JXAction::JXAction(const QString & title, jx::Object callback, QWidget *parent)
    : QAction(title, parent)
{
   this->callback = jx_ob_copy(callback.get_jxob());
}

JXAction::~JXAction()
{
   jx_ob_free(this->callback);
}

void JXAction::doCallback(bool checked)
{
    jx_ob fn = jx_ob_copy(callback);
    jx_function_call(fn, fn, jx_ob_from_bool(checked));
    // fn is consumed, somehow
    //jx_ob_free(fn);
}

void JXAction::doCallback(QString s)
{
    jx_ob fn = jx_ob_copy(callback);
    jx_ob arg= jx_ob_from_str(s.toAscii().data());
    jx_function_call(fn, fn, arg);
}

void JXAction::doCallback()
{
    jx_ob fn = jx_ob_copy(callback);
    jx_function_call(fn, fn, jx_ob());
}

void JXAction::doExit()
{
    jx_ob fn = jx_ob_copy(callback);
    jx_function_call(fn, fn, jx_ob());
    freeActions();
    exit(1);
}

void JXAction::openFile()
{
  QString fileName = QFileDialog::getOpenFileName();
  if (!fileName.isEmpty())
    doCallback(fileName);
}

void JXAction::freeActions() {
  QList<JXAction *> kids = parentWidget()->findChildren<JXAction *>();
  for (int i=0; i < kids.size(); ++i) {
    //qDebug() << kids.at(i)->text();
    jx_ob_free(kids.at(i)->callback);
  }
}
