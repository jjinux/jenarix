#ifndef JXOBJECT_H
#define JXOBJECT_H

#include "jx_public.h"
#include <vector>

namespace jx {
  
  static jx_ob env;

  class Object {

    protected:
      jx_ob jxob; // underlying jx object representation 


    public:
      Object();
      Object(const Object&);
      Object(jx_ob);

/*    could use from_, from_with, etc. methods, but these are cool */
      Object(int);
      Object(float);
      Object(double);
      Object(bool);
/*    creates string type; ident must use from_ident method */
      Object(char *);
      Object(const char *);
      Object(char *, int);
      Object(const char *, int);

      ~Object();

      bool operator == (const Object&);

/* meaningful prepositions, for systematic memory management:

   ..._with_...(...) implies a destructive conversion of input object (destroyed)

   ..._from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)

   ..._in_... implies usage of the input object (usually transient)
*/
      jx_ob get_jxob();

/* these methods are used, for example:
      jx::Object a = jx::Object::from_int(47);
*/
      static Object from_int(int);
      static Object from_float(float);
      static Object from_float(double);
      static Object from_bool(bool);
      static Object from_string(char *);
      static Object from_string(const char *);
      static Object from_string(char *, int);
      static Object from_string(const char *, int);
      static Object from_ident(char *);
      static Object from_ident(const char *);
      static Object from_ident(char *, int);
      static Object from_ident(const char *, int);

      //Object to_int(jx::Object &);
      Object to_int();

      int    as_int();
      float  as_float();
      bool   as_bool();
      char * as_str();
      char * as_ident();

      bool null_check();
      bool int_check();
      bool float_check();
      bool bool_check();
      bool str_check();
      bool ident_check();
      bool list_check();
      bool hash_check();

/* useful for Ob that is not declared a List, but is */
      Object list_borrow(int);
/* useful for Ob that is not declared a Hash, but is */
      Object hash_get(const char * key);
      Object hash_get(Object * key);

      int size();
      int type();
  };

  class Ident: public Object {

    public:
      Ident();
      Ident(char *);
      Ident(const char *);
      Ident(char *, int);
      Ident(const char *, int);
  };

  class List: public Object {

    public:
      List();
      List(int size);
      List(int array[], int size);
      List(float array[], int size);
      List(int size, Object &fill);

      Object borrow(int);
      int size();
  };

  class Hash: public Object {

    public:
      Hash();

      Object get(const char *);
      int size();
   };

  //Object Object_from_ident(char *);
};
#endif
