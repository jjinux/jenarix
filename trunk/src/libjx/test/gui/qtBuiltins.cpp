#include "jx_public.h"
#include "jxobject.h"
#include "qtBuiltins.h"
#include "jxaction.h"
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>


#define OUTXML false
#define OUTJX  true

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
  if (OUTXML) printf("<gui>\n");
  if (OUTJX) {
    printf("(entity VSplitter)\n");
    printf("(entity HSplitter)\n");
    printf("(entity MenuBar)\n");
    printf("(entity MenuItem)\n");
    printf("(entity Navigator)\n");
    printf("(entity Widget)\n");
    printf("(entity OpenGLContext)\n");
    printf("set gui \n");
   }
  jx_ob result = qtTree(qApp->activeWindow());
  if (OUTXML) printf("</gui>\n");
  return result;
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
  //qApp->exit();
  //qApp->quit();
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

void jxend(QString name, QString type) {
  printf("])\n");
}
void jxout(QString name, QString type) {

  QString outname;
  outname = name;
  if        (name == "VSplitter") {
  } else if (name == "HSplitter") {
  } else if (name == "MenuBar") {
  } else if (name == "MenuItem") {
  } else if (name == "Navigator") {
  } else if (name == "Widget") {
  } else if (name == "OpenGLContext") {
  } else {
    outname = '"' + name + '"';
  }
  printf("(%s [\n", outname.toAscii().data());
}

jx_ob qtTree(QObject *parent) {
  //parent->dumpObjectTree();
  QList<QObject *> kids = parent->children();
  QString kname;
  QString ktype;
  for (int i=0; i < kids.size(); ++i) {
    QObject * kid;
    QString ptype = parent->metaObject()->className();
    if (ptype.contains("Splitter")) {
      kid = kids.at(kids.size() - 1 - i); // list is reversed
    } else {
      kid = kids.at(i);
    }
    kname = kid->objectName();
    ktype = kid->metaObject()->className();
    bool interesting = false;
    if (kid->parent() == parent) {
      if (kname.startsWith("qt_")) {
        interesting = false;
      } else if (kname.isNull()) {
        if (ktype == "QWidget") {
/* eliminate cruft */
        } else if (ktype == "QSplitterHandle") {
          interesting = false;
        } else if (ktype == "QTextControl") {
          interesting = false;
        } else if (ktype == "QVBoxLayout") {
          interesting = false;
        } else if (ktype == "QAction") {
          interesting = false;
        }
      } else {
/* the interesting stuff */
        interesting = true;
      }
      if (interesting) {
        if (OUTXML) {
          printf("<%s>%s(%d)\n", kname.replace(' ','_').toAscii().data(), kid->metaObject()->className(), kid->children().count());
        } else if (OUTJX) {
          jxout(kname, ktype);
        }
        qtTree(kid);
      }
    }
    if (interesting) {
      if (OUTXML) {
        printf("</%s>\n", kname.replace(' ','_').toAscii().data());
      } else if (OUTJX) {
        jxend(kname, ktype);
      }
    }
  }
  return jx_ob_from_str("tree");
}
