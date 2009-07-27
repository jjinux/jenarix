#include "jx.h"
#include <iostream>
using namespace std;
using namespace jx;
#include <vector>

#define OUT(t,a) cout << t << ":type:" << a.type() << ":size:" << a.size() << ":as_int: " << a.as_int() << endl;

int main(int argc, char **argv)
{
  Object obj = Object();
  OUT("Object", obj);
  Object copy = obj;
  OUT("copy", copy);

  Object b = Object(true);
  OUT("bool", b);
  Object i = Object(12);
  OUT("int", i);
  Object v = Object(27.88);
  OUT("float", v);
  Object s = Object("this is a test");
  OUT("string", s);
  Object s2 = Object("this is a test", 7);
  OUT("string+len", s2);
/* must use from_ident for idents, to distinguish from string type */
  Object nid = Object();
  nid.from_ident("aname");
  OUT("ident", nid);

  //Object nint = Object(v.to_int());
  Object nint = v.to_int();
  OUT("float to_int", nint);

  List alist = List();
  OUT("List", alist);
  List blist = List(11);
  OUT("List", blist);
  List rlist = List(7, &i);
  OUT("List repeat", rlist);

  return 1;
}
