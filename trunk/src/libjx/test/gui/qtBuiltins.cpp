#include "jx_public.h"
#include "jxobject.h"
#include "qtBuiltins.h"
#include "jxaction.h"
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>

void addQtBuiltins(jx::Ob * node) {

  jx_ob names = jx_hash_new();
  jx_code_expose_secure_builtins(names);
  exposeQtBuiltins(names);
  jx_hash_set(node->ob(), jx_builtins(), jx_ob_copy(names));
  jx_ob_free(names);
}

void exposeQtBuiltins(jx_ob names) {

  jx_bool ok = JX_TRUE;
  ok = jx_declare(ok, names, (jx_char *)"qtAlert", qtAlert);
  ok = jx_declare(ok, names, (jx_char *)"qtExit",  qtExit);
  ok = jx_declare(ok, names, (jx_char *)"qtFile",  qtFile);
  ok = jx_declare(ok, names, (jx_char *)"qtJxon",  qtJxon);

}

jx_ob qtJxon(jx_env *E, jx_ob payload) {
  jx_ob_free(payload);
  qDebug() << "(";
  return qtTree(qApp->activeWindow());
}

jx_ob qtFile(jx_env *E, jx_ob payload) {
  QString fileName = QFileDialog::getOpenFileName();
  jx_ob_free(payload);
  return jx_ob_from_str(fileName.toAscii().data());
}

jx_ob qtExit(jx_env *E, jx_ob payload) {

  // free callback jx_ob's
  JXAction::freeActions();
  qApp->closeAllWindows();
  qApp->exit();
  qApp->quit();
  jx_ob_free(payload);
  return jx_ob_from_str((jx_char *)"exit return value");
}

jx_ob qtAlert(jx_env *E, jx_ob payload) {
  
  QMessageBox msgBox;
  jx_ob msg = jx_list_borrow(payload,0);
  msgBox.setText(jx_ob_as_str(&msg));
  msgBox.exec();
  jx_ob_free(payload);
  return jx_ob_from_str((jx_char *)"test return value");
}

static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char * ident, jx_native_fn fn)
{
  if(ok)
    ok = jx_ok(jx_hash_set(names, jx_ob_from_ident(ident),
                           jx_builtin_new_from_native_fn(fn)));
  return ok;
}

jx_ob qtTree(QObject *parent) {
  //parent->dumpObjectTree();
  QList<QObject *> kids = parent->children();
  for (int i=0; i < kids.size(); ++i) {
    QObject * kid = kids.at(kids.size() - 1 - i); // list is reversed
    QString name = kid->objectName();
    if (kid->parent() == parent && !name.startsWith("qt_") && !name.isNull()) {
      qDebug() << name << "(";
      qtTree(kid);
    }
  }
  qDebug() << ")";
  return jx_ob_from_str("tree");
}
