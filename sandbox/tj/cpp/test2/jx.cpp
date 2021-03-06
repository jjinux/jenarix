#include "jx_public.h"
#include "jx.h"

/* Constructors */
jx::Object::Object() {
  jxob = jx_ob_from_null();
};
jx::Object::Object(jx_ob from) {
  jxob = jx_ob_copy(from);
};

/*    could use from_, from_with, etc. methods, but these are cool */
jx::Object::Object(int v) {
  jxob = jx_ob_from_int(v);
};
jx::Object::Object(float v) {
  jxob = jx_ob_from_float(v);
};
jx::Object::Object(double v) {
  jxob = jx_ob_from_float(v);
};
jx::Object::Object(bool v) {
  jxob = jx_ob_from_bool(v);
};
/*    creates string type; ident must use from_ident method */
jx::Object::Object(char * st) {
  jxob = jx_ob_from_str(st);
};
jx::Object::Object(const char * st) {
  jxob = jx_ob_from_str((jx_char *)st);
};
jx::Object::Object(char * st, int stlen) {
  jxob = jx_ob_from_str_with_len(st, stlen);
};
jx::Object::Object(const char * st, int stlen) {
  jxob = jx_ob_from_str_with_len((jx_char *)st, stlen);
};

/* Copy */
jx::Object::Object(const Object& from) {
  jxob = jx_ob_copy(from.jxob);
};

/* Destructor */
jx::Object::~Object() {
  //jx_status status = jx_ob_free(jxob);
  //fprintf(stderr, "free %d\n", status);
  jx_ob_free(jxob);
};

/* meaningful prepositions, for systematic memory management:

   ..._with_...(...) implies a destructive conversion of input object (destroyed)

   ..._from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)

   ..._in_... implies usage of the input object (usually transient)
*/
jx_ob jx::Object::get_jxob() {
  return jxob;
}

jx::Object jx::Object::from_int(int v) {
  jxob = jx_ob_from_int(v);
};
jx::Object jx::Object::from_float(float v) {
  jxob = jx_ob_from_float(v);
};
jx::Object jx::Object::from_float(double v) {
  jxob = jx_ob_from_float(v);
};
jx::Object jx::Object::from_bool(bool v) {
  jxob = jx_ob_from_bool(v);
};
jx::Object jx::Object::from_string(char * st) {
  jxob = jx_ob_from_str(st);
};
jx::Object jx::Object::from_string(const char * st) {
  jxob = jx_ob_from_str((jx_char *)st);
};
jx::Object jx::Object::from_string(char * st, int stlen) {
  jxob = jx_ob_from_str_with_len((jx_char *)st, stlen);
};
jx::Object jx::Object::from_string(const char * st, int stlen) {
  jxob = jx_ob_from_str_with_len((jx_char *)st, stlen);
};
jx::Object jx::Object::from_ident(char * st) {
  jxob = jx_ob_from_ident((jx_char *)st);
};
jx::Object jx::Object::from_ident(const char * st) {
  jxob = jx_ob_from_ident((jx_char *)st);
};
jx::Object jx::Object::from_ident(char * st, int stlen) {
  jxob = jx_ob_from_ident_with_len((jx_char *)st, stlen);
};
jx::Object jx::Object::from_ident(const char * st, int stlen) {
  jxob = jx_ob_from_ident_with_len((jx_char *)st, stlen);
};

/*
jx::Object to_int(jx::Object &from) {
  jx_ob_to_int(from.get_jxob());
};
*/
jx::Object jx::Object::to_int() {
  return jx_ob_to_int(jxob);
};

int    jx::Object::as_int() {
  return jx_ob_as_int(jxob);
};
float  jx::Object::as_float() {
  return jx_ob_as_float(jxob);
};
bool   jx::Object::as_bool() {
  return jx_ob_as_bool(jxob);
};
char * jx::Object::as_str() {
  return (char *)jx_ob_as_str(&jxob);
};
char * jx::Object::as_ident() {
  return (char *)jx_ob_as_ident(&jxob);
};

int jx::Object::size() {
  return jx_ob_as_int(jx_ob_size(jxob));
};
int jx::Object::type() {
  return jx_ob_type(jxob);
}


/* List Object class */
jx::List::List() {
  jxob = jx_list_new();
}
jx::List::List(int size) {
  jxob = jx_list_new_with_size(size);
}
jx::List::List(int size, Object &repeat) {
  jxob = jx_list_new_with_repeat(size, jx_ob_copy(repeat.get_jxob()));
}
jx::List::List(int *array, int size) {
  jxob = jx_list_new_from_int_array(array, size);
}
jx::List::List(float *array, int size) {
  jxob = jx_list_new_from_float_array(array, size);
}
int jx::List::size() {
  return jx_list_size(jxob);
}
