#include "jxobject.h"
#include <iostream>
using namespace jx;
#include <vector>

#define OUT(t,a) jx_jxon_dump(stderr, (jx_char *)t, a.ob());

int main(int argc, char **argv)
{
  Ob obj = Ob();
  OUT("Object", obj);
  Ob copy = obj;
  OUT("copy", copy);
  if (obj == copy) {
    fprintf(stderr, "OK: copy is identical\n");
  } else {
    fprintf(stderr, "ERROR: copy is NOT identical\n");
  }

  Ob b = Ob(true);
  OUT("bool", b);
  if (obj == b) {
    fprintf(stderr, "ERROR: b is identical\n");
  } else {
    fprintf(stderr, "OK: b is NOT identical\n");
  }

  Ob i = Ob(312);
  OUT("int", i);
  Ob j = 399;
  OUT("int", j);
  Ob v = Ob(27.88);
  OUT("float", v);
  Ob s = Ob("this is a test");
  OUT("string", s);
  Ob s2 = "this is another test";
  OUT("string", s2);
  Ob s3 = Ob("this is another test", 11);
  OUT("string+len", s3);
/* must use fromIdent for idents, to distinguish from string type
   or Ident derived class */
  Ob nid = Ob::fromIdent("aname");
  OUT("ident", nid);
  Ident xid = "bname";
  OUT("Ident", xid);

  Ob nint = v.toInt();
  OUT("float toInt", nint);

  List alist = List();
  OUT("List", alist);
  List blist = List(11);
  OUT("List", blist);
  List rlist = List(5, i);
  OUT("List fill", rlist);

  int jlist[5] = {1,2,3,4,5};
  List klist(jlist,5);
  OUT("List int array", klist);

  int flist[3] = {1.2,3.4,5.6};
  List glist(flist,3);
  OUT("List float array", glist);

  return 1;
}
