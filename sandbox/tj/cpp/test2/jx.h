#include "jx_public.h"
#include <vector>

namespace jx {
  
  static jx_ob env;

  class Object {

    //protected:
      //jx_ob jxob; // underlying jx object representation 

    public:
      jx_ob jxob; // underlying jx object representation 

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

/* meaningful prepositions, for systematic memory management:

   ..._with_...(...) implies a destructive conversion of input object (destroyed)

   ..._from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)

   ..._in_... implies usage of the input object (usually transient)
*/
      jx_ob get_jxob();

      Object from_int(int);
      Object from_float(float);
      Object from_float(double);
      Object from_bool(bool);
      Object from_string(char *);
      Object from_string(const char *);
      Object from_string(char *, int);
      Object from_string(const char *, int);
      Object from_ident(char *);
      Object from_ident(const char *);
      Object from_ident(char *, int);
      Object from_ident(const char *, int);

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

      int size();
      int type();
  };

  class List: public Object {

    public:
      List();
      List(int size);
      List(int * array, int size);
      List(float * array, int size);
      List(int size, Object *repeat);

      int size();
  };

};
