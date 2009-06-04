#include <QtOpenGL>
#include "jx_public.h"

namespace Jenarix {

   jx_ob parseJXON  (bool process);
   int   processJXON( jx_ob ob );
   void  printJXON  ( jx_ob ob );

   int sphere(jx_ob ob);
   int colorsphere(jx_ob ob);
   int cylinder(jx_ob ob);
   int triangle(jx_ob ob,  bool clockwise);
   int trianglenormal(jx_ob ob,  bool clockwise);
   void align_from_z(GLdouble a[], GLdouble b[]);
   void color(GLdouble r, GLdouble g, GLdouble b, GLdouble a);
   void make_axes();

};
