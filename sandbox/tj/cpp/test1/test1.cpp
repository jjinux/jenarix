#include "jx.h"
#include <iostream>
using namespace std;
using namespace jx;
#include <vector>

#define OUT(t,a) cout << t << ":: value:" << a.value() << " type:" << a.type() << " size:" << a.size() << endl;

int main(int argc, char **argv)
{
  Object obj = Object();
  OUT("object", obj);
  Object copy = Object(obj);
  OUT("object copy", copy);
  Object bcopy = obj;
  OUT("object bcopy", bcopy);

  Float x = Float(435.22);
  OUT("Float", x);
  Object x2 = x;
  OUT("Object x2", x2);
  Float x3;
  OUT("Float x3", x3);

  Integer y = Integer(7765);
  OUT("Integer", y);
  Integer y2;
  OUT("Integer y2", y2);
  Integer x4 = x.value();
  OUT("Integer x4", x4);

  Integer z = 1982;
  OUT("Integer", z);

  Bool q = true;
  OUT("Bool", q);

  String r;
  OUT("String", r);
  String s = "this is a test";
  OUT("String", s);
  String t = String("this is a test",11);
  OUT("String len", t);
/*
  Integer tlen = t.str_size();
  OUT("str_size", t);
*/
  String u = s.concat(" extended by concat");
  OUT("String concat", u);

  List l0;
  OUT("list", l0);

  List l1 = List(12);
  OUT("list", l1);

  std::vector<int> v2(18,33);
  IntegerList l2 = IntegerList(v2);
  OUT("vla list", l2);
  IntegerList l3 = l2;
  OUT("vla list copy", l3);

  return 1;
}
