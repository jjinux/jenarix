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
      Ob(jx_ob);  // don't let users do this, lest borrowed jx_ob's get destructed

    public:
      Ob();
      Ob(const Ob&);

/*    could use from_, from_with, etc. methods, but these are cool */
      Ob(int);
      Ob(float);
      Ob(double);
      Ob(bool);
/*    creates string type; ident must use from_ident method or Ident class */
      Ob(char *);
      Ob(const char *);
      Ob(char *, int);
      Ob(const char *, int);

      ~Ob();

      bool operator == (const Ob&);
      bool operator == (const jx_ob);
      Ob operator[] (int ielement);
      Ob operator[] (const Ob&);

/* meaningful prepositions, for systematic memory management:

   ..._with_...(...) implies a destructive conversion of input object (destroyed)

   ..._from_...(...) implies a constructive copy / conversion of input object (preserved)

   ..._to_...(...) implies a constructive cast resulting in a new copy of the input object (preserved)

   ..._as_...(...) implies a volatile (yet failsafe) cast of in-place borrowed object data (preserved)

   ..._in_... implies usage of the input object (usually transient)
*/
      jx_ob ob();
      void jxonDump(FILE *, const char *);

/* these methods are used, for example:
      jx::Ob a = jx::Ob::fromIdent("VSplitter");
*/
      static Ob fromInt(int);
      static Ob fromFloat(float);
      static Ob fromFloat(double);
      static Ob fromBool(bool);
      static Ob fromString(char *);
      static Ob fromString(const char *);
      static Ob fromString(char *, int);
      static Ob fromString(const char *, int);
      static Ob fromIdent(char *);
      static Ob fromIdent(const char *);
      static Ob fromIdent(char *, int);
      static Ob fromIdent(const char *, int);

      Ob toInt();

      int    asInt();
      float  asFloat();
      bool   asBool();
      char * asStr();
      char * asIdent();

      bool nullCheck();
      bool intCheck();
      bool floatCheck();
      bool boolCheck();
      bool strCheck();
      bool identCheck();
      bool listCheck();
      bool hashCheck();
      bool builtinCallableCheck();

/* useful for Ob that is a list (or jx::List) */
      Ob get(int);
      jx_ob borrow(int);  // return jx_ob, not Ob lest jxob get destructed
/* useful for Ob that is a hash (or jx::Hash) */
      Ob get(const Ob key);
      jx_ob borrow(const Ob key);  // return jx_ob, not Ob lest jxob get destructed

      int size();
      int type();

  };

  class Ident : public Ob {

    private:

    public:
      Ident();
      Ident(const Ident&);
      Ident(char *);
      Ident(const char *);
      Ident(char *, int);
      Ident(const char *, int);

      ~Ident();

  };

  class List : public Ob {

    private:

    public:
      List();
      List(const List&);
      List(int size);
      List(int array[], int size);
      List(float array[], int size);
      List(int size, Ob &fill);

      ~List();

      Ob get(int);
      jx_ob borrow(int);  // return jx_ob, not Ob lest it get destructed
      int size();

      Ob operator[] (int ielement);
  };

  class Hash : public Ob {

    private:

    public:
      Hash();
      Hash(const Hash&);

      ~Hash();

      Ob get(const Ob key);
      jx_ob borrow(const Ob key);  // return jx_ob, not Ob lest it get destructed
      int size();
   };

};

#endif
