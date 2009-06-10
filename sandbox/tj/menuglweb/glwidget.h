#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

//! [0]
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
//! [0]

//! [1]
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
//! [1]

//! [2]
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
//! [2]

//! [3]
private:
    GLuint makeObject();
    void normalizeAngle(int *angle);

    GLuint object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;

    void doXRotation(int angle, int update);
    void doYRotation(int angle, int update);
    void doZRotation(int angle, int update);
};
//! [3]

#endif
