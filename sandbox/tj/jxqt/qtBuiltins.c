#include "jx_public.h"
#include "qtBuiltins.h"

void addQtBuiltins(jx_ob node) {

  jx_ob names = jx_hash_new();
  jx_code_expose_secure_builtins(names);
  exposeQtBuiltins(names);
  jx_hash_set(node, jx_builtins(), jx_ob_copy(names));
  jx_ob_free(names);
}

void exposeQtBuiltins(jx_ob names) {

  jx_bool ok = JX_TRUE;
  ok = jx_declare(ok, names, (jx_char *)"qtprint", qtPrint);

}

jx_ob qtPrint(jx_env *E, jx_ob payload) {
  jx_jxon_dump(stderr, (jx_char *)"qtprint", payload);
  jx_ob_free(payload);
  return jx_ob_from_null();
}

static jx_bool jx_declare(jx_bool ok, jx_ob names, jx_char * ident, jx_native_fn fn)
{
  if(ok)
    ok = jx_ok(jx_hash_set(names, jx_ob_from_ident(ident),
                           jx_builtin_new_from_native_fn(fn)));
  return ok;
}
