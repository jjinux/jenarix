#include <json/json.h>
#include <stdio.h>
#include <assert.h>
#include "jenarix.h"

#if defined(_MSC_VER)  &&  _MSC_VER >= 1310
# pragma warning( disable: 4996 )     // disable fopen deprecation warning
#endif

int
Jenarix::processJSON( Json::Value &value )
{
/* expect an array of primitives,
 * each with a name, followed by an array of values
 */
   assert (value.type() == Json::arrayValue);
   int nval = value.size();
   int nprimitives = 0;
   for ( int index = 0; index < nval; ++index ) {
      assert (value[index].type() == Json::stringValue);
      
      if ( !value[index].asString().compare("cylinder") ) {
        ++index;
        color(0.5, 1.0, 0.5, 0.0);
        Jenarix::cylinder( value[index] );
      } else if ( !value[index].asString().compare("sphere") ) {
        ++index;
        color(0.5, 0.5, 1.0, 0.0);
        Jenarix::sphere( value[index] );
      } else {
        printf ("unrecognized primitive type '%s'\n", value[index].asString().c_str() );
      }
      ++nprimitives;
      //printf("%d\n", index);

   }

  return nprimitives;
}

bool
Jenarix::parseJSON( Json::Value &root )
{
   Json::Reader reader;
   bool parsingSuccessful = reader.parse( std::cin, root );
   if ( !parsingSuccessful )
   {
      printf( "Failed to parse <stdin> %s\n", 
              reader.getFormatedErrorMessages().c_str() );
      return false;
   }

   return true;
}

int get_double( Json::Value &value, GLdouble &q ) {
    assert (value.type() == Json::realValue);
    q = value.asDouble();
    return 0;
}

int get_3double( Json::Value &value, GLdouble xyz[3] ) {

    assert (value.type() == Json::arrayValue);
    assert (value.size() == 3);
    for (int index=0; index < 3; ++index) {
      xyz[index] = value[index].asDouble();
    }

  return value.size();
}

int Jenarix::sphere( Json::Value &value )
{

    GLdouble xyz[3], r;
    unsigned int ixyz = 0;
    unsigned int irad = 1;
    assert (value.type() == Json::arrayValue);
    int nval = value.size();
    //printf ("%d sphere\n", nval);

    for ( int index = 0; index < nval; ++index ) {
      glPushMatrix();
      //printf ("%d\n", value[index].size());
      get_3double((value[index])[ixyz], xyz);
      glTranslated(xyz[0], xyz[1], xyz[2]);
      GLUquadricObj* quadric = gluNewQuadric();
      gluQuadricDrawStyle(quadric, GLU_FILL);
      gluQuadricOrientation(quadric, GLU_OUTSIDE);
      gluQuadricNormals(quadric, GLU_SMOOTH);
      //printf ("%f %f %f\n", xyz[0], xyz[1], xyz[2]);
      get_double((value[index])[irad], r);
      gluSphere(quadric, r/3.0, 20, 20);
      //printf ("%f\n", r);
      gluDeleteQuadric(quadric);
      glPopMatrix();
    }

   return nval;
}

int Jenarix::cylinder( Json::Value &value )
{
    GLdouble bxyz[3], txyz[3], r;
    GLdouble dx, dy, dz, h;

    unsigned int ibxyz = 0;
    unsigned int itxyz = 1;
    unsigned int irad = 2;
    assert (value.type() == Json::arrayValue);
    int nval = value.size();
    //printf ("%d cylinder\n", nval);
    for ( int index = 0; index < nval; ++index ) {
      //printf ("%d\n", value[index].size());
      get_3double((value[index])[ibxyz], bxyz);
      //printf ("%f %f %f\n", bxyz[0], bxyz[1], bxyz[2]);
      get_3double((value[index])[itxyz], txyz);
      //printf ("%f %f %f\n", txyz[0], txyz[1], txyz[2]);
      dx = txyz[0] - bxyz[0];
      dy = txyz[1] - bxyz[1];
      dz = txyz[2] - bxyz[2];
      h = sqrt(dx*dx + dy*dy + dz*dz);
      glPushMatrix();
      align_from_z(bxyz, txyz);
      GLUquadricObj* quadric = gluNewQuadric();
      gluQuadricDrawStyle(quadric, GLU_FILL);
      gluQuadricOrientation(quadric, GLU_OUTSIDE);
      gluQuadricNormals(quadric, GLU_SMOOTH);
      get_double((value[index])[irad], r);
      //printf ("%f\n", r);
      gluCylinder(quadric, r, r, h, 20, 20);
      gluDeleteQuadric(quadric);
      glPopMatrix();
    }

    return nval;
}

void Jenarix::align_from_z(GLdouble a[], GLdouble b[])
{
    GLdouble delx, dely, delz, vlen, v;
    GLdouble dircosx, dircosy, dircosz;
    GLdouble xangle, yangle;

    delx = b[0] - a[0];
    dely = b[1] - a[1];
    delz = b[2] - a[2];
    vlen = sqrt(delx*delx + dely*dely + delz*delz);
    dircosx = delx / vlen;
    dircosy = dely / vlen;
    dircosz = delz / vlen;
    v = sqrt(dircosz*dircosz + dircosy*dircosy);
    if (v == 0.0) {
      xangle = 0;
      //yangle = M_PI_2;
      yangle = 90.0;
    } else {
      xangle = (180.0 / M_PI ) * acos(dircosz/v);
      yangle = (180.0 / M_PI ) * acos(v);
    }
    if (dely < 0.0) xangle = -xangle;
    if (delx > 0.0) yangle = -yangle;

    glTranslated(a[0], a[1], a[2]);
    glRotated(-xangle, 1.0, 0.0, 0.0);
    glRotated(-yangle, 0.0, 1.0, 0.0);
}


void Jenarix::color(GLdouble r, GLdouble g, GLdouble b, GLdouble a)
{
    glColor4f(r,g,b,a);
}

void Jenarix::make_axes()
{
    glBegin(GL_LINES);
    color(1.0, 0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 1.0);
    color(0.0, 1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);
    color(0.0, 0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);
    glEnd();
}
