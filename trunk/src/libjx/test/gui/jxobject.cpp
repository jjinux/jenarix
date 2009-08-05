#include "jx_public.h"
#include "jxobject.h"

/* Constructors */
jx::Ob::Ob() {
  jxob = jx_ob_from_null();
}
jx::Ob::Ob(jx_ob from) {
  //jx_jxon_dump(stderr, "from", from);
  //jxob = jx_ob_copy(from);
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
void jx::Ob::jxon_dump(FILE *f, const char *prefix) {
  jx_jxon_dump(f, (jx_char *)prefix, jxob);
}
bool jx::Ob::null_check() {
  return jx_null_check(jxob);
}
bool jx::Ob::str_check() {
  return jx_str_check(jxob);
}
bool jx::Ob::int_check() {
  return jx_int_check(jxob);
}
bool jx::Ob::float_check() {
  return jx_float_check(jxob);
}
bool jx::Ob::bool_check() {
  return jx_bool_check(jxob);
}
bool jx::Ob::list_check() {
  return jx_list_check(jxob);
}
bool jx::Ob::hash_check() {
  return jx_hash_check(jxob);
}
bool jx::Ob::builtin_callable_check() {
  return jx_builtin_callable_check(jxob);
}

/* these static "creators" are used, for example:
    jx::Ob i = jx::Ob::from_int(123);
*/
jx::Ob jx::Ob::from_int(int v) {
  return jx_ob_from_int(v);
}
jx::Ob jx::Ob::from_float(float v) {
  return jx_ob_from_float(v);
}
jx::Ob jx::Ob::from_float(double v) {
  return jx_ob_from_float(v);
}
jx::Ob jx::Ob::from_bool(bool v) {
  return jx_ob_from_bool(v);
}
jx::Ob jx::Ob::from_string(char * st) {
  return jx_ob_from_str(st);
}
jx::Ob jx::Ob::from_string(const char * st) {
  return jx_ob_from_str((jx_char *)st);
}
jx::Ob jx::Ob::from_string(char * st, int stlen) {
  return jx_ob_from_str_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::from_string(const char * st, int stlen) {
  return jx_ob_from_str_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::from_ident(char * st) {
  return jx_ob_from_ident((jx_char *)st);
}
jx::Ob jx::Ob::from_ident(const char * st) {
  return jx_ob_from_ident((jx_char *)st);
}
jx::Ob jx::Ob::from_ident(char * st, int stlen) {
  return jx_ob_from_ident_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::from_ident(const char * st, int stlen) {
  return jx_ob_from_ident_with_len((jx_char *)st, stlen);
}
jx::Ob jx::Ob::from_hash(jx_ob from) {
  return jx_ob_copy(from);
}

/* useful for Ob not declared a List, but is */
jx_ob jx::Ob::list_get(int ielement) {
  if (jx_list_check(jxob)) {
    return jx_list_get(jxob, ielement);
  } else {
    return jx_ob_from_null();
  }
}

/* useful for Ob not declared a Hash, but is */
jx_ob jx::Ob::hash_get(Ob key) {
  if (jx_hash_check(jxob)) {
    return jx_hash_get(jxob, key.ob());
  } else {
    return jx_ob_from_null();
  }
}

jx::Ob jx::Ob::to_int() {
  return jx_ob_to_int(jxob);
}

int    jx::Ob::as_int() {
  return jx_ob_as_int(jxob);
}
float  jx::Ob::as_float() {
  return jx_ob_as_float(jxob);
}
bool   jx::Ob::as_bool() {
  return jx_ob_as_bool(jxob);
}
char * jx::Ob::as_str() {
  return (char *)jx_ob_as_str(&jxob);
}
char * jx::Ob::as_ident() {
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
jx::Ident::Ident(jx_ob from) {
  //jxob = jx_ob_copy(from);
  jxob = from;
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
jx::Ident::~Ident() {
  jx_ob_free(jxob);
}
/* Copy */
jx::Ident::Ident(const Ident& from) {
  jxob = jx_ob_copy(from.jxob);
}
jx_ob jx::Ident::ob() {
  return jxob;
}

/* List Ob class */
jx::List::List() {
  jxob = jx_list_new();
}
jx::List::List(jx_ob from) {
  //jxob = jx_ob_copy(from);
  jxob = from;
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
jx::List::~List() {
  jx_ob_free(jxob);
}
/* Copy */
jx::List::List(const List& from) {
  jxob = jx_ob_copy(from.jxob);
}
jx_ob jx::List::ob() {
  return jxob;
}

jx_ob jx::List::get(int ielement) {
  return jx_list_get(jxob, ielement);
}
int jx::List::size() {
  return jx_list_size(jxob);
}

/* Hash Ob class */
jx::Hash::Hash() {
  jxob = jx_hash_new();
}
jx::Hash::Hash(jx_ob from) {
  //jx_jxon_dump(stderr, "from", from);
  //jxob = jx_ob_copy(from);
  jxob = from;
}
jx::Hash::~Hash() {
  jx_ob_free(jxob);
}
/* Copy */
jx::Hash::Hash(const Hash& from) {
  jxob = jx_ob_copy(from.jxob);
}
jx_ob jx::Hash::ob() {
  return jxob;
}

int jx::Hash::size() {
  return jx_hash_size(jxob);
}
jx_ob jx::Hash::get(Ob key) {
  return jx_hash_get(jxob, key.ob());
}
jx_ob jx::Hash::get(Ident key) {
  return jx_hash_get(jxob, key.ob());
}
