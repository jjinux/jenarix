#include "jx_public.h"
#include "jxobject.h"

/* Constructors */
jx::Ob::Ob() {
  jxob = jx_ob_from_null();
}
jx::Ob::Ob(jx_ob from) {
  jxob = from;
}

/*    could use from_, from_with, etc. methods, but these are cool */
jx::Ob::Ob(int v) {
  jxob = jx_ob_from_int(v);
}
jx::Ob::Ob(float v) {
  jxob = jx_ob_from_float(v);
}
jx::Ob::Ob(double v) {
  jxob = jx_ob_from_float(v);
}
jx::Ob::Ob(bool v) {
  jxob = jx_ob_from_bool(v);
}
/*    creates string type; ident must use from_ident method */
jx::Ob::Ob(char * st) {
  jxob = jx_ob_from_str(st);
}
jx::Ob::Ob(const char * st) {
  jxob = jx_ob_from_str((jx_char *)st);
}
jx::Ob::Ob(char * st, int stlen) {
  jxob = jx_ob_from_str_with_len(st, stlen);
}
jx::Ob::Ob(const char * st, int stlen) {
  jxob = jx_ob_from_str_with_len((jx_char *)st, stlen);
}

/* Operators */
bool jx::Ob::operator == (const Ob & rhs) {
  return jx_ob_identical(jxob, rhs.jxob);
}
bool jx::Ob::operator == (const jx_ob rhs) {
  return jx_ob_identical(jxob, rhs);
}

/* Copy */
jx::Ob::Ob(const Ob& from) {
  jxob = jx_ob_copy(from.jxob);
}

/* Destructor */
jx::Ob::~Ob() {
  //fprintf(stderr, "Ob    free %x type %x ", jxob, jx_ob_type(jxob));
  //jx_jxon_dump(stderr, "free", jxob);
  jx_ob_free(jxob);
}

/* meaningful prepositions, for systematic memory management:

   ..._with_...(...) implies a destructive conversion of input object (destroyed)

   ..._from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)

   ..._in_... implies usage of the input object (usually transient)
*/
jx_ob jx::Ob::ob() {
  return jxob;
}
void jx::Ob::jxonDump(FILE *f, const char *prefix) {
  jx_jxon_dump(f, (jx_char *)prefix, jxob);
}
bool jx::Ob::nullCheck() {
  return jx_null_check(jxob);
}
bool jx::Ob::strCheck() {
  return jx_str_check(jxob);
}
bool jx::Ob::intCheck() {
  return jx_int_check(jxob);
}
bool jx::Ob::floatCheck() {
  return jx_float_check(jxob);
}
bool jx::Ob::boolCheck() {
  return jx_bool_check(jxob);
}
bool jx::Ob::listCheck() {
  return jx_list_check(jxob);
}
bool jx::Ob::hashCheck() {
  return jx_hash_check(jxob);
}
bool jx::Ob::builtinCallableCheck() {
  return jx_builtin_callable_check(jxob);
}

/* these static "creators" are used, for example:
    jx::Ob i = jx::Ob::from_int(123);
*/
jx::Ob jx::Ob::makeOb(jx_ob from) {
  return from;
}
jx::Ob jx::Ob::fromInt(int v) {
  return jx_ob_from_int(v);
}
jx::Ob jx::Ob::fromFloat(float v) {
  return jx_ob_from_float(v);
}
jx::Ob jx::Ob::fromFloat(double v) {
  return jx_ob_from_float(v);
}
jx::Ob jx::Ob::fromBool(bool v) {
  return jx_ob_from_bool(v);
}
jx::Ob jx::Ob::fromString(char * st) {
  return jx_ob_from_str(st);
}
jx::Ob jx::Ob::fromString(const char * st) {
  return jx_ob_from_str((jx_char *)st);
}
jx::Ob jx::Ob::fromString(char * st, int stlen) {
  return jx_ob_from_str_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::fromString(const char * st, int stlen) {
  return jx_ob_from_str_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::fromIdent(char * st) {
  return jx_ob_from_ident((jx_char *)st);
}
jx::Ob jx::Ob::fromIdent(const char * st) {
  return jx_ob_from_ident((jx_char *)st);
}
jx::Ob jx::Ob::fromIdent(char * st, int stlen) {
  return jx_ob_from_ident_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::fromIdent(const char * st, int stlen) {
  return jx_ob_from_ident_with_len((jx_char *)st, stlen);
}

/* useful for Ob not declared a List, but is */
jx::Ob jx::Ob::listGet(int ielement) {
  return jx_list_get(jxob, ielement);
}
jx_ob jx::Ob::listBorrow(int ielement) {
  return jx_list_borrow(jxob, ielement);
}

/* useful for Ob not declared a Hash, but is */
jx::Ob jx::Ob::hashGet(Ob key) {
  return jx_hash_get(jxob, key.ob());
}
jx_ob jx::Ob::hashBorrow(Ob key) {
  return jx_hash_get(jxob, key.ob());
}

jx::Ob jx::Ob::toInt() {
  return jx_ob_to_int(jxob);
}

int    jx::Ob::asInt() {
  return jx_ob_as_int(jxob);
}
float  jx::Ob::asFloat() {
  return jx_ob_as_float(jxob);
}
bool   jx::Ob::asBool() {
  return jx_ob_as_bool(jxob);
}
char * jx::Ob::asStr() {
  return (char *)jx_ob_as_str(&jxob);
}
char * jx::Ob::asIdent() {
  return (char *)jx_ob_as_ident(&jxob);
}

int jx::Ob::size() {
  return jx_ob_as_int(jx_ob_size(jxob));
}
int jx::Ob::type() {
  return jx_ob_type(jxob);
}


/* Ident Oject class */
jx::Ident::Ident() {
  jxob = jx_ob_from_null();
}
jx::Ident::Ident(char * st) {
  jxob = jx_ob_from_ident((jx_char *)st);
}
jx::Ident::Ident(const char * st) {
  jxob = jx_ob_from_ident((jx_char *)st);
}
jx::Ident::Ident(char * st, int stlen) {
  jxob = jx_ob_from_ident_with_len((jx_char *)st, stlen);
}
jx::Ident::Ident(const char * st, int stlen) {
  jxob = jx_ob_from_ident_with_len((jx_char *)st, stlen);
}
/* Copy */
jx::Ident::Ident(const Ident& from) {
  jxob = jx_ob_copy(from.jxob);
}
/* Destructor */
jx::Ident::~Ident() {
  // let ~Ob() do it
}
/*
jx_ob jx::Ident::ob() {
  return jxob;
}
*/

/* List Ob class */
jx::List::List() {
  jxob = jx_list_new();
}
jx::List::List(int size) {
  jxob = jx_list_new_with_size(size);
}
jx::List::List(int size, Ob &fill) {
  jxob = jx_list_new_with_fill(size, jx_ob_copy(fill.ob()));
}
jx::List::List(int array[], int size) {
  jxob = jx_list_new_from_int_array(array, size);
}
jx::List::List(float array[], int size) {
  jxob = jx_list_new_from_float_array(array, size);
}
/* Copy */
jx::List::List(const List& from) {
  jxob = jx_ob_copy(from.jxob);
}
/* Destructor */
jx::List::~List() {
  // let Ob() do it
}
/*
jx_ob jx::List::ob() {
  return jxob;
}
*/

jx::Ob jx::List::get(int ielement) {
  return makeOb(jx_list_get(jxob, ielement));
}
jx_ob jx::List::borrow(int ielement) {
  return jx_list_get(jxob, ielement);
}
int jx::List::size() {
  return jx_list_size(jxob);
}

/* Hash Ob class */
jx::Hash::Hash() {
  jxob = jx_hash_new();
}
/*
jx::Hash::Hash(jx_ob from) {
  jxob = from;
}
*/
/* Copy */
jx::Hash::Hash(const Hash& from) {
  jxob = jx_ob_copy(from.jxob);
}
/* Destructor */
jx::Hash::~Hash() {
  // let ~Ob() do it
}
/*
jx_ob jx::Hash::ob() {
  return jxob;
}
*/

int jx::Hash::size() {
  return jx_hash_size(jxob);
}
jx::Ob jx::Hash::get(Ob key) {
  return makeOb(jx_hash_get(jxob, key.ob()));
}
jx::Ob jx::Hash::get(Ident key) {
  return makeOb(jx_hash_get(jxob, key.ob()));
}
jx_ob jx::Hash::borrow(Ob key) {
  return jx_hash_get(jxob, key.ob());
}
jx_ob jx::Hash::borrow(Ident key) {
  return jx_hash_get(jxob, key.ob());
}
