#ifndef JXOBJECT_H
#define JXOBJECT_H

#include "jx_public.h"
#include <vector>

namespace jx {
  
  //static jx_ob env;

  class Ob {

    protected:
      jx_ob jxob; // underlying jx object representation

    private:

    public:
      Ob();
      Ob(const Ob&);
      Ob(jx_ob);

/*    could use from_, from_with, etc. methods, but these are cool */
      Ob(int);
      Ob(float);
      Ob(double);
      Ob(bool);
/*    creates string type; ident must use from_ident method */
      Ob(char *);
      Ob(const char *);
      Ob(char *, int);
      Ob(const char *, int);

      ~Ob();

      bool operator == (const Ob&);
      bool operator == (const jx_ob);

/* meaningful prepositions, for systematic memory management:

   ..._with_...(...) implies a destructive conversion of input object (destroyed)

   ..._from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)

   ..._in_... implies usage of the input object (usually transient)
*/
      jx_ob ob();
      void jxon_dump(FILE *, const char *);

/* these methods are used, for example:
      jx::Ob a = jx::Ob::from_int(47);
*/
      static Ob from_int(int);
      static Ob from_float(float);
      static Ob from_float(double);
      static Ob from_bool(bool);
      static Ob from_string(char *);
      static Ob from_string(const char *);
      static Ob from_string(char *, int);
      static Ob from_string(const char *, int);
      static Ob from_ident(char *);
      static Ob from_ident(const char *);
      static Ob from_ident(char *, int);
      static Ob from_ident(const char *, int);
      static Ob from_hash(jx_ob);

      //Ob to_int(jx::Ob &);
      Ob to_int();

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
      bool builtin_callable_check();

/* useful for Ob that is not declared a List, but is */
      Ob list_get(int);
/* useful for Ob that is not declared a Hash, but is */
      Ob hash_get(const char * key);
      Ob hash_get(const Ob key);

      int size();
      int type();
  };

  class Ident : public Ob {

    public:
      Ident();
      Ident(const Ident&);
      Ident(char *);
      Ident(const char *);
      Ident(char *, int);
      Ident(const char *, int);
      Ident(jx_ob);

      ~Ident();

  };

  class List : public Ob {

    public:
      List();
      List(const List&);
      List(int size);
      List(int array[], int size);
      List(float array[], int size);
      List(int size, Ob &fill);
      List(jx_ob);

      ~List();

      Ob get(int);
      int size();
  };

  class Hash : public Ob {

    public:
      Hash();
      Hash(const Hash&);
      Hash(jx_ob);

      ~Hash();

      Ob get(const Ob key);
      Ob get(const Ident key);
      Ob get(const char *);
      int size();
   };

  //Ob Ob_from_ident(char *);
};
#endif
