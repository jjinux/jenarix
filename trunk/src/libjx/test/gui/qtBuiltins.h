#ifndef QTBUILTINS_H
#define QTBUILTINS_H

#include "jx_public.h"
#include "jxobject.h"
#include <QObject>

void addQtBuiltins(jx::Ob * node);
void exposeQtBuiltins(jx_ob names);
static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char * ident, jx_native_fn fn);
jx_ob qtTree(QObject * parent);

extern "C" jx_ob qtAlert(jx_env *E, jx_ob payload);
extern "C" jx_ob qtExit(jx_env *E, jx_ob payload);
extern "C" jx_ob qtFile(jx_env *E, jx_ob payload);
extern "C" jx_ob qtJxon(jx_env *E, jx_ob payload);

#endif
