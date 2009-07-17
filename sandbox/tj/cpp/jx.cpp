#include "jx.h"
using namespace jx;
/*
jx::jx() {
}
jx::~jx() {
}
*/

// Base class
Object::Object() {
  jxob = jx_ob();
}
Object::Object(const Object & obj) {
  jxob = jx_ob_copy(obj.jxob);
}
Object::Object(jx_ob obj) {
  jxob = jx_ob_copy(obj);
}
Object::~Object() {
  jx_ob_free(jxob);
}
int Object::size() {
  return jx_ob_as_int(jx_ob_size(jxob));
}
int Object::type() {
  return jx_ob_type(jxob);
}
bool Object::value() {
  if (jx_null_check(jxob)) return false;
  else return true;
}

// Derived classes
Integer::Integer(int v) {
  jxob = jx_ob_from_int(v);
}
int Integer::value() {
  return jx_ob_as_int(jxob);
}

Float::Float(float v) {
  jxob = jx_ob_from_float(v);
}
float Float::value() {
  return jx_ob_as_float(jxob);
}

Bool::Bool(bool v) {
  jxob = jx_ob_from_bool(v);
}
bool Bool::value() {
  return jx_ob_as_bool(jxob);
}

String::String() {
  //jxob = jx_ob();
  jxob = jx_ob_from_str((jx_char *)"");
}
String::String(const char * st) {
  jxob = jx_ob_from_str((jx_char *)st);
}
String::String(const char * st, int stlen) {
  jxob = jx_ob_from_str_with_len((jx_char *)st, stlen);
}
char * String::value() {
  return jx_ob_as_str(&jxob);
}
/*
Integer String::str_size() {
  return jx_str_size(jxob);
}
*/
String String::concat(String st) {
  jx_str_concat(jxob, st.jxob);
}

List::List() {
  jxob = jx_list_new();
}
List::List(int size) {
  jxob = jx_list_new_with_size(size);
}
int List::size() {
  return jx_list_size(jxob);
}

IntegerList::IntegerList(std::vector<int> vec) {
  jx_int *array = jx_int_vla_new(vec.size());
  int i=0;
  for( std::vector<int>::iterator iter = vec.begin(); iter != vec.end(); iter++, ++i) {
    array[i] = *iter;
  }
  jxob = jx_list_new_with_int_vla(&array);
}
