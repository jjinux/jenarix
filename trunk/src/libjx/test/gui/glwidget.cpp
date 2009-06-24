#include <QtGui>
#include <QtOpenGL>

#include <math.h>

//#include "jenarix.h"
#include "glwidget.h"

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    object = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
    makeCurrent();
    glDeleteLists(object, 1);
}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(500, 500);
}
//! [4]

//! [5]
void GLWidget::setXRotation(int angle) {
    doXRotation(angle,1);
}
void GLWidget::setYRotation(int angle) {
    doYRotation(angle,1);
}
void GLWidget::setZRotation(int angle) {
    doZRotation(angle,1);
}
void GLWidget::doXRotation(int angle, int update)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        if (update) updateGL();
    }
}
//! [5]

void GLWidget::doYRotation(int angle, int update)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        if (update) updateGL();
    }
}

void GLWidget::doZRotation(int angle, int update)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        if (update) updateGL();
    }
}

//! [6]
void GLWidget::initializeGL()
{
    GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 0.5, 1.0, 1.0, 0.0 };
    GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.2, 0.3, 0.4, 0.0);
    object = makeObject();
}
//! [6]

//! [7]

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    glCallList(object);
    //swapBuffers();
}

//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    GLint side = qMin(width, height);
    glViewport((GLint)((width - side) / 2), (GLint)((height - side) / 2), side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8.0, +8.0, -8.0, +8.0, -20.0, +20.0);
    glMatrixMode(GL_MODELVIEW);
}
//! [8]

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        doXRotation(xRot + 8 * dy,0);
        doYRotation(yRot + 8 * dx,0);
    } else if (event->buttons() & Qt::RightButton) {
        doXRotation(xRot + 8 * dy,0);
        doZRotation(zRot + 8 * dx,0);
    }
    lastPos = event->pos();
    if (dx || dy) updateGL();
}
//! [10]

GLuint GLWidget::makeObject()
{
    GLuint list = 1;
    glNewList(list, GL_COMPILE_AND_EXECUTE);
    make_cube();
    glEndList();
    return list;
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

void GLWidget::make_cube()
{
    GLfloat size = 3.0;
    
     // face 1 front in x y plane 
    glNormal3d(0.0, 0.0, 1.0);
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);     
    glVertex3f(-size, -size, size);
    glVertex3f( size, -size, size);
    glVertex3f( size,  size, size);
    glVertex3f(-size,  size, size);
    glEnd();
     // face 2 right, y z plane
    glNormal3d(1.0, 0.0, 0.0);  
    glColor3f(0.0f, 1.0f, 0.0f);  // green
    glBegin(GL_QUADS);   // face 2
    glVertex3f( size, -size,  size);
    glVertex3f( size, -size, -size);
    glVertex3f( size,  size, -size);
    glVertex3f( size,  size,  size);
    glEnd();
     // face 3, back, x y plane
    glNormal3d(0.0, 0.0, -1.0);
    glColor3f(1.0f, 0.0f, 1.0f); // magenta
    glBegin(GL_QUADS);   // face 3
    glVertex3f( size, -size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size,  size, -size);
    glVertex3f( size,  size, -size);
    glEnd();
     // face 4, left, yz plane
    glNormal3d(-1.0, 0.0, 0.0);
    glColor3f(1.0f, 1.0f, 0.0f);  // yellow
    glBegin(GL_QUADS);   // face 4
    glVertex3f(-size,  size,  size);
    glVertex3f(-size,  size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size,  size);
    glEnd();
     // face 5 top xz plane
    glNormal3d(0.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 1.0f);  // blue
    glBegin(GL_QUADS);   
    glVertex3f( size, size,  size);
    glVertex3f( size, size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size,  size);
    glEnd();
     // face 6, bottom, xz plane
    glNormal3d(0.0,-1.0, 0.0);
    glColor3f(0.0f, 1.0f, 1.0f);  // cyan
    glBegin(GL_QUADS);   // face 6
    glVertex3f( size, -size, -size);
    glVertex3f( size, -size,  size);
    glVertex3f(-size, -size,  size);
    glVertex3f(-size, -size, -size);
    glEnd();
}

void GLWidget::make_axes()
{
    glBegin(GL_LINES);
    glColor4f(1.0, 0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 10.0);
    glColor4f(0.0, 1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 10.0, 0.0);
    glColor4f(0.0, 0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(10.0, 0.0, 0.0);
    glEnd();
}
