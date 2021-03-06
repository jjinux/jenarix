#include <json/json.h>
#include <QtOpenGL>

namespace Jenarix {

   bool parseJSON  ( Json::Value &value );
   int  processJSON( Json::Value &value );
   void printJSON  ( Json::Value &value );

   int sphere(Json::Value &value );
   int colorsphere(Json::Value &value );
   int cylinder( Json::Value &value );
   int triangle( Json::Value &value, bool clockwise);
   int trianglenormal( Json::Value &value, bool clockwise);
   void align_from_z(GLdouble a[], GLdouble b[]);
   void color(GLdouble r, GLdouble g, GLdouble b, GLdouble a);
   void make_axes();

};
