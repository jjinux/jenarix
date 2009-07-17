#include "jx_public.h"
#include <vector>

namespace jx {
  
  static jx_ob env; // global namespace 
/*
class jx {
  public:
    jx();
    ~jx();
*/

  class Object {

    protected:
      jx_ob jxob; // underlying jx object representation 

    public:
      Object();
      Object(const Object&);
      Object(jx_ob);

      ~Object();

      bool value();
      int size();
      int type();
  };

  class Integer : public Object {

    public:
      Integer(int v);

      int value();

  };

  class Float : public Object {

    public:
      Float(float v);

      float value();

  };

  class Bool : public Object {

    public:
      Bool(bool v);

      bool value();

  };

  class String : public Object {

    public:
      String();
      String(const char * st);
      String(const char * st, int stlen);

      char * value();

      //Integer str_size();
      String concat(String st);

  };

  class List : public Object {

    public:
      List();
      List(int);

      int size(); // uses jx_list_size, though jx_size returns same

    };

  class IntegerList : public List {

    public:
      IntegerList(std::vector<int>);

  };

};
