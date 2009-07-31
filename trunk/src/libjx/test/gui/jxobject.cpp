#include "jx_public.h"
#include "jxobject.h"

/* Constructors */
jx::Object::Object() {
  jxob = jx_ob_from_null();
};
jx::Object::Object(jx_ob from) {
  //jxob = jx_ob_copy(from);
  jxob = from;
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

/* Operators */
bool jx::Object::operator == (const Object & rhs) {
  return jx_ob_identical(jxob, rhs.jxob);
}

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
bool jx::Object::null_check() {
  return jx_null_check(jxob);
}
bool jx::Object::str_check() {
  return jx_str_check(jxob);
}
bool jx::Object::int_check() {
  return jx_int_check(jxob);
}
bool jx::Object::float_check() {
  return jx_float_check(jxob);
}
bool jx::Object::bool_check() {
  return jx_bool_check(jxob);
}
bool jx::Object::list_check() {
  return jx_list_check(jxob);
}
bool jx::Object::hash_check() {
  return jx_hash_check(jxob);
}

/* these static "creators" are used, for example:
    jx::Object i = jx::Object::from_int(123);
*/
jx::Object jx::Object::from_int(int v) {
  return jx_ob_from_int(v);
};
jx::Object jx::Object::from_float(float v) {
  return jx_ob_from_float(v);
};
jx::Object jx::Object::from_float(double v) {
  return jx_ob_from_float(v);
};
jx::Object jx::Object::from_bool(bool v) {
  return jx_ob_from_bool(v);
};
jx::Object jx::Object::from_string(char * st) {
  return jx_ob_from_str(st);
};
jx::Object jx::Object::from_string(const char * st) {
  return jx_ob_from_str((jx_char *)st);
};
jx::Object jx::Object::from_string(char * st, int stlen) {
  return jx_ob_from_str_with_len((jx_char *)st, stlen);
};
jx::Object jx::Object::from_string(const char * st, int stlen) {
  return jx_ob_from_str_with_len((jx_char *)st, stlen);
};
jx::Object jx::Object::from_ident(char * st) {
  return jx_ob_from_ident((jx_char *)st);
};
jx::Object jx::Object::from_ident(const char * st) {
  return jx_ob_from_ident((jx_char *)st);
};
jx::Object jx::Object::from_ident(char * st, int stlen) {
  return jx_ob_from_ident_with_len((jx_char *)st, stlen);
};
jx::Object jx::Object::from_ident(const char * st, int stlen) {
  return jx_ob_from_ident_with_len((jx_char *)st, stlen);
};

/* useful for Ob not declared a List, but is */
jx::Object jx::Object::list_borrow(int ielement) {
  if (jx_list_check(jxob)) {
    return jx_list_borrow(jxob, ielement);
  } else {
    return jx_ob_from_null();
  }
}

/* useful for Ob not declared a Hash, but is */
jx::Object jx::Object::hash_get(const char * ckey) {
  jx_ob key = jx_ob_from_str((jx_char *)ckey);
  if (jx_hash_check(jxob)) {
    return jx_hash_get(jxob, key);
  } else {
    return jx_ob_from_null();
  }
  jx_ob_free(key);
}

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

/* List Object class */
jx::List::List() {
  jxob = jx_list_new();
}
jx::List::List(int size) {
  jxob = jx_list_new_with_size(size);
}
jx::List::List(int size, Object &fill) {
  jxob = jx_list_new_with_fill(size, jx_ob_copy(fill.get_jxob()));
}
jx::List::List(int array[], int size) {
  jxob = jx_list_new_from_int_array(array, size);
}
jx::List::List(float array[], int size) {
  jxob = jx_list_new_from_float_array(array, size);
}

jx::Object jx::List::borrow(int ielement) {
  return ((Object)this).list_borrow(ielement);
}
int jx::List::size() {
  return jx_list_size(jxob);
}

/* Hash Object class */
jx::Hash::Hash() {
  jxob = jx_hash_new();
}

int jx::Hash::size() {
  return jx_hash_size(jxob);
}
jx::Object jx::Hash::get(const char * key) {
  return ((Object)this).hash_get(key);
}
