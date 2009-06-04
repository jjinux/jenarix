#include <stdio.h>
#include <assert.h>
#include "jenarix.h"

jx_ob Jenarix::parseJXON(bool process)
{
    jx_ob result;
  
    jx_ob anamespace = jx_hash_new();
    jx_ob scanner = jx_jxon_scanner_new_with_file(stdin);
    jx_ob node = jx_hash_new();

    jx_code_expose_secure_builtins(anamespace);

    {
      jx_ob source = JX_OB_NULL;
      jx_status status;
      jx_bool done = JX_FALSE;
      while( !done ) {
        status = jx_jxon_scanner_next_ob(&source, scanner);
        switch(status) {
        case JX_YES:
          {
            jx_ob code = jx_code_bind_with_source(anamespace, source);
            source = jx_ob_from_null();
            result = jx_code_eval(node,code);
            //printf("status = %d\n", status);
            //show_ob(result,0);
            //jx_ob_free(result);
            jx_ob_free(code);
            if (process) {
              processJXON(result);
            } else {
              return result;
            }
          }
          break;
        case JX_STATUS_SYNTAX_ERROR: /* catch this error */
          {
            jx_ob message = jx_jxon_scanner_get_error_message(scanner);
            printf("Error: invalid syntax\n");
            if(jx_str_check(message))
              printf("%s\n",jx_ob_as_str(&message));
              jx_jxon_scanner_purge_input(scanner);
              jx_ob_free(message);
          }
          break;
        default: /* about on all other errors */
          done = JX_TRUE;
          break;
        }
      }
      jx_ob_free(source);
    }
    jx_ob_free(node);
    jx_ob_free(scanner);
    jx_ob_free(anamespace);

    return result;
}

int
Jenarix::processJXON( jx_ob ob )
{
/* expect a list of primitives,
 * each with a name, followed by an array of values
 */
   assert (jx_list_check(ob));
   int nval = jx_list_size(ob);
   int nprimitives = 0;
   for ( int index = 0; index < nval; ++index ) {
      //jx_ob item = jx_list_get(ob,index);
      jx_ob item = jx_list_shift(ob);
      assert (jx_str_check(item));
      std::string ptype = jx_ob_as_str(&item);
      
      if ( ptype == "cylinder" ) {
        ++index;
        //color(0.25, 1.0, 0.5, 0.0);
        Jenarix::cylinder( jx_list_shift(ob) );

      } else if ( ptype == "sphere" ) {
        ++index;
        //color(0.5, 0.5, 1.0, 0.0);
        Jenarix::sphere( jx_list_shift(ob) );

      } else if ( ptype == "colorsphere" ) {
        ++index;
        Jenarix::colorsphere( jx_list_shift(ob) );

      } else if ( ptype == "triangle" ) {
        ++index;
        Jenarix::triangle( jx_list_shift(ob), true );

      } else if ( ptype == "cctriangle" ) {
        ++index;
        Jenarix::triangle( jx_list_shift(ob), false );

      } else if ( ptype == "cwtriangle" ) {
        ++index;
        Jenarix::triangle( jx_list_shift(ob), true );

      } else if ( ptype == "trianglenormal" ) {
        ++index;
        Jenarix::trianglenormal( jx_list_shift(ob), true );

      } else if ( ptype == "cctrianglenormal" ) {
        ++index;
        Jenarix::trianglenormal( jx_list_shift(ob), false );

      } else if ( ptype == "cwtrianglenormal" ) {
        ++index;
        Jenarix::trianglenormal( jx_list_shift(ob), true );

      } else {
        printf ("unrecognized primitive type '%s'\n", ptype.c_str() );
      }
      ++nprimitives;

   }

  return nprimitives;
}

GLdouble get_real( jx_ob ob ) {
    assert (jx_float_check(ob));
    return jx_ob_as_float(ob);
}

int get_point( jx_ob ob, GLdouble xyz[3] ) {

    assert (jx_list_check(ob));
    assert (jx_list_size(ob) == 3);
    for (unsigned int index=0; index < 3; ++index) {
      xyz[index] = get_real(jx_list_shift(ob));
    }

  return 3;
}

int get_point_normal( jx_ob ob, GLdouble xyz[3], GLdouble nml[3] ) {

    assert (jx_list_check(ob));
    assert (jx_list_size(ob) == 2);
    unsigned int ivertex = 0;
    unsigned int inormal = 1;
    get_point( jx_list_get(ob,ivertex), xyz );
    get_point( jx_list_get(ob,inormal), nml );

  return 2;
}

int Jenarix::sphere( jx_ob ob )
{

    GLdouble xyz[3], r;
    unsigned int ixyz = 0;
    unsigned int irad = 1;
    assert (jx_list_check(ob));
    int nval = jx_list_size(ob);
    //printf ("%d sphere\n", nval);

    for ( int index = 0; index < nval; ++index ) {
      jx_ob xyz_rad = jx_list_shift(ob);
      glPushMatrix();
      //printf ("%d\n", value[index].size());
      get_point(jx_list_get(xyz_rad, ixyz), xyz);
      glTranslated(xyz[0], xyz[1], xyz[2]);
      GLUquadricObj* quadric = gluNewQuadric();
      gluQuadricDrawStyle(quadric, GLU_FILL);
      gluQuadricOrientation(quadric, GLU_OUTSIDE);
      gluQuadricNormals(quadric, GLU_SMOOTH);
      //printf ("%f %f %f\n", xyz[0], xyz[1], xyz[2]);
      r = get_real(jx_list_get(xyz_rad, irad));
      gluSphere(quadric, r, 20, 20);
      //printf ("%f\n", r);
      gluDeleteQuadric(quadric);
      glPopMatrix();
    }

   return nval;
}

int Jenarix::triangle( jx_ob ob, bool clockwise)
{

    GLdouble xyz[3][3];
    assert (jx_list_check(ob));
    int nval = jx_list_size(ob);
    //printf ("%d triangle\n", nval);

    glBegin(GL_TRIANGLES);
    for ( int index = 0; index < nval; ++index ) {
      jx_ob triangle = jx_list_shift(ob);
      //printf ("%d\n", value[index].size());
      for (unsigned int itri=0; itri<3; ++itri) {
        get_point(jx_list_get(triangle, itri), xyz[itri]);
      }
      for (unsigned int itri=0; itri<3; ++itri) {
        if (clockwise) {
          glVertex3d(xyz[2-itri][0], xyz[2-itri][1], xyz[2-itri][2]);
        } else {
          glVertex3d(xyz[itri][0], xyz[itri][1], xyz[itri][2]);
        }
      }
    }
    glEnd();

   return nval;

}

int Jenarix::trianglenormal( jx_ob ob, bool clockwise)
{

    GLdouble xyz[3][3], nml[3][3];
    assert (jx_list_check(ob));
    int nval = jx_list_size(ob);
    //printf ("%d trianglenormal\n", nval);

    glBegin(GL_TRIANGLES);
    for ( int index = 0; index < nval; ++index ) {
      jx_ob triangle_nml = jx_list_shift(ob);
      //printf ("%d\n", value[index].size());
      for (unsigned int itri=0; itri<3; ++itri) {
        get_point_normal(jx_list_get(triangle_nml, itri), xyz[itri], nml[itri]);
      }
      for (unsigned int itri=0; itri<3; ++itri) {
        if (clockwise) {
          glNormal3d(nml[2-itri][0], nml[2-itri][1], nml[2-itri][2]);
          glVertex3d(xyz[2-itri][0], xyz[2-itri][1], xyz[2-itri][2]);
        } else {
          glNormal3d(nml[itri][0], nml[itri][1], nml[itri][2]);
          glVertex3d(xyz[itri][0], xyz[itri][1], xyz[itri][2]);
        }
      }
    }
    glEnd();

   return nval;

}

int Jenarix::colorsphere( jx_ob ob )
{

    GLdouble xyz[3], r, rgb[3];
    unsigned int ixyz = 0;
    unsigned int irgb = 1;
    unsigned int irad = 2;
    assert (jx_list_check(ob));
    int nval = jx_list_size(ob);
    //printf ("%d sphere\n", nval);

    for ( int index = 0; index < nval; ++index ) {
      jx_ob xyz_rgb_rad = jx_list_shift(ob);
      glPushMatrix();
      //printf ("%d\n", value[index].size());
      get_point(jx_list_get(xyz_rgb_rad, ixyz), xyz);
      glTranslated(xyz[0], xyz[1], xyz[2]);
      get_point(jx_list_get(xyz_rgb_rad, irgb), rgb);
      color(rgb[0], rgb[1], rgb[2], 0.0);
      GLUquadricObj* quadric = gluNewQuadric();
      gluQuadricDrawStyle(quadric, GLU_FILL);
      gluQuadricOrientation(quadric, GLU_OUTSIDE);
      gluQuadricNormals(quadric, GLU_SMOOTH);
      //printf ("%f %f %f\n", xyz[0], xyz[1], xyz[2]);
      r = get_real(jx_list_get(xyz_rgb_rad, irad));
      gluSphere(quadric, r, 20, 20);
      //printf ("%f\n", r);
      gluDeleteQuadric(quadric);
      glPopMatrix();
    }

   return nval;
}

int Jenarix::cylinder( jx_ob ob) 
{
    GLdouble bxyz[3], txyz[3], r;
    GLdouble dx, dy, dz, h;

    unsigned int ibxyz = 0;
    unsigned int itxyz = 1;
    unsigned int irad = 2;
    assert (jx_list_check(ob));
    int nval = jx_list_size(ob);
    //printf ("%d cylinder\n", nval);
    for ( int index = 0; index < nval; ++index ) {
      jx_ob base_tip_rad = jx_list_shift(ob);
      //printf ("%d\n", value[index].size());
      get_point(jx_list_get(base_tip_rad,ibxyz), bxyz);
      //printf ("%f %f %f\n", bxyz[0], bxyz[1], bxyz[2]);
      get_point(jx_list_get(base_tip_rad,itxyz), txyz);
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
      r = get_real(jx_list_get(base_tip_rad, irad));
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
    glVertex3d(0.0, 0.0, 10.0);
    color(0.0, 1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 10.0, 0.0);
    color(0.0, 0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(10.0, 0.0, 0.0);
    glEnd();
}
