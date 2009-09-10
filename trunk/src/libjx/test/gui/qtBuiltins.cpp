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
QString jxout;

void QtBuiltins::add(jx::Ob * node) {

  jx_ob names = jx_hash_new();
  jx_code_expose_secure_builtins(names);
  expose(names);
  jx_hash_set(node->ob(), jx_builtins(), jx_ob_copy(names));
  jx_ob_free(names);
}

void QtBuiltins::expose(jx_ob names) {

  jx_bool ok = JX_TRUE;
  ok = jx_declare(ok, names, (jx_char *)"qtAlert", qtAlert);
  ok = jx_declare(ok, names, (jx_char *)"qtExit",  qtExit);
  ok = jx_declare(ok, names, (jx_char *)"qtFile",  qtFile);
  ok = jx_declare(ok, names, (jx_char *)"qtJxon",  qtJxon);

}

jx_ob qtJxon(jx_env *E, jx_ob payload) {
  jxout.clear();
  jx_ob_free(payload);
  if (OUTXML) jxout.append("<gui>\n");
  if (OUTJX) {
    jxout.append("(entity VSplitter)\n");
    jxout.append("(entity HSplitter)\n");
    jxout.append("(entity MenuBar)\n");
    jxout.append("(entity MenuItem)\n");
    jxout.append("(entity Navigator)\n");
    jxout.append("(entity Widget)\n");
    jxout.append("(entity OpenGLContext)\n");
    jxout.append("set gui \n");
   }
  QtBuiltins::tree(qApp->activeWindow());
  if (OUTXML) jxout.append("</gui>\n");
  QFile save("guiout.jx");
  save.open(QIODevice::WriteOnly);
  save.write(jxout.toAscii().data());
  save.close();
  //return jx_ob_from_str(jxout.toAscii().data());
  return jx_ob_from_str("saved to file guiout.jx");
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

QString callback_attr(JXAction *w) {
  QString attr = "callback: ";
  jx_ob callback = w->getCallback();
  if (jx_builtin_callable_check(callback)) {
    jx_ob ftmp = jx_function_to_impl(callback);
    jx_ob fid = jx_list_borrow(ftmp,0);
    if (jx_null_check(fid)) {
     attr.append("nop");
    } else {
     char * fname = jx_ob_as_ident(&fid);
     attr.append(fname);
    }
    jx_ob_free(ftmp);
  } else if (jx_ident_check(callback)) {
     attr.append(jx_ob_as_ident(&callback));
  } else {
   attr.append("nop");
  }
  //jx_ob_free(callback);
  return attr;
}
QString check_attr(QAction *w) {
  QString attr;
  if (w->isCheckable()) {
    attr = "checkbox: ";
    if (w->isChecked()) {
      attr.append("\"on\"");
    } else {
      attr.append("\"off\"");
    }
  }
  return attr;
}
QString obout( QObject *w) {
  QString attr; // return value

  QString wname = w->objectName();
  QString wtype = w->metaObject()->className();
  //qDebug() << wname << wtype;

  if (OUTXML) {
    //printf("<%s>\n", wname.replace(' ','_').toAscii().data());
    jxout += "<" + wname.replace(' ','_') + " >\n";
    return wname;
  }

  QString outname;
/* eliminate cruft */
  if (wname.startsWith("qt_")) {
  } else if (wname.isNull()) {
    if (wtype == "QWidget") {
    } else if (wtype == "QSplitterHandle") {
    } else if (wtype == "QTextControl") {
    } else if (wtype == "QVBoxLayout") {
    } else if (wtype == "QAction") {
    }

  } else if (wname == "VSplitter") {
    outname = wname;
    attr = " ";
  } else if (wname == "HSplitter") {
    outname = wname;
    attr = " ";
  } else if (wname == "MenuBar") {
    outname = wname;
    attr = " ";
  } else if (wname == "MenuItem") {
    outname = wname;
    attr = " ";
  } else if (wname == "Navigator") {
    outname = wname;
    attr = " ";
  } else if (wname == "Widget") {
    outname = wname;
    attr = " ";
  } else if (wname == "OpenGLContext") {
    outname = wname;
    attr = " ";
  } else if (wtype == "QMenu") {
    outname = "MenuItem";
    attr = "label: "; attr.append('"'); attr.append(wname); attr.append('"');
  } else if (wtype == "QAction") {
    outname = "MenuItem";
    attr = "label: "; attr.append('"'); attr.append(wname); attr.append('"');
    QString check = check_attr((QAction *)w);
    if (!check.isNull()) attr.append(","+check);
  } else if (wtype == "JXAction") {
    outname = "MenuItem";
    attr = "label: "; attr.append('"'); attr.append(wname); attr.append('"');
    QString cback = callback_attr((JXAction *)w);
    if (!cback.isNull()) attr.append(","+cback);
    QString check = check_attr((QAction *)w);
    if (!check.isNull()) attr.append(","+check);
  } else {
    attr = " ";
  }

  if (!outname.isNull()) {
    //printf("(%s [\n", outname.toAscii().data());
    jxout += "( " + outname + " [\n";
  }
  return attr;

}
void obend(QString attr) {
  if (OUTXML) {
    //printf("</%s>\n", attr.toAscii().data());
    jxout += "</" + attr + ">\n";
  } else if (OUTJX) {
    if (!attr.isNull()) {
      //printf("] {%s})\n", attr.toAscii().data());
      jxout += "] {" + attr + "})\n";
    }
  }
}

void QtBuiltins::tree(QObject *parent) {

  //parent->dumpObjectTree();
  QString attr = obout(parent);

  QString ptype = parent->metaObject()->className();
  QList<QObject *> kids = parent->children();
  for (int i=0; i < kids.size(); ++i) {
    QObject * kid;
    if (ptype.contains("Splitter")) {
      kid = kids.at(kids.size() - 1 - i); // list is reversed
    } else {
      kid = kids.at(i);
    }
    tree(kid);
  }

  obend(attr);

  return;
}
