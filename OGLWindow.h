#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include <QtOpenGL/QGLWidget>
#include "ParkObjects.h"
#include "Object3D.h"

class OGLWindow: public QGLWidget {
    Q_OBJECT
private:
    AmussementPark::World *world;
    int viewPortWidth;
    int viewPortHeight;
protected:
    void initializeGL();
    void resizeGL (int width, int height);
    void paintGL();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    GLuint render_identify_scene(int x, int y, GLuint *selectBuf, GLuint bufSize);
public:
    OGLWindow(QWidget *parent=0);
    void keyPressEvent (QKeyEvent * event);
    void wheelEvent(QWheelEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void timerEvent(QTimerEvent * event);
};

class DesktopWindow: public QWidget {
    Q_OBJECT
private:
    OGLWindow *widget;
protected:
    void keyPressEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
public:
    DesktopWindow();
};

#endif // OGLWINDOW_H
