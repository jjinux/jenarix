#include "jx.h"
#include <iostream>
using namespace jx;
#include <vector>

#define OUT(t,a) std::cout << t << ":type:" << a.type() << ":size:" << a.size() << ":as_int: " << a.as_int() << std::endl;

int main(int argc, char **argv)
{
  Object obj = Object();
  OUT("Object", obj);
  Object copy = obj;
  OUT("copy", copy);

  Object b = Object(true);
  OUT("bool", b);
  Object i = Object(312);
  OUT("int", i);
  Object j = 399;
  OUT("int", j);
  Object v = Object(27.88);
  OUT("float", v);
  Object s = Object("this is a test");
  OUT("string", s);
  Object s2 = "this is another test";
  OUT("string", s2);
  Object s3 = Object("this is a test", 7);
  OUT("string+len", s3);
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
  List rlist = List(5, i);
  OUT("List repeat", rlist);

  int jlist = (1,2,3,4,5);
  List klist(&jlist,5);
  OUT("List int array", klist);

  int flist = (1.2,3.4,5.6);
  List glist(&flist,3);
  OUT("List float array", glist);


  return 1;
}
