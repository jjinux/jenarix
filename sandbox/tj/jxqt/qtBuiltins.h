#ifndef QTBUILTINS_H
#define QTBUILTINS_H

#include "jx_public.h"

void addQtBuiltins(jx_ob node);
void exposeQtBuiltins(jx_ob names);
static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char * ident, jx_native_fn fn);

extern "C" {
jx_ob qtPrint(jx_env *E, jx_ob payload);
}

#endif
