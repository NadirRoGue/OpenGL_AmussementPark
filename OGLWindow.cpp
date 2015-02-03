#include <QKeyEvent>
#include <QMouseEvent>
#include <QBoxLayout>
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QColor>
#include <QWheelEvent>
#include <QTimer>
#include <climits>

#include <iostream>
#include <string>

#include "OGLWindow.h"
#include "ParkObjects.h"
#include "Object3D.h"
#include "MeshTable.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

using namespace Qt;
using namespace std;
using namespace GraphicEngine;
using namespace AmussementPark;

OGLWindow::OGLWindow(QWidget *parent): QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent) {
    viewPortWidth = viewPortHeight = 800;
    MeshTable::get_instance();
    startTimer(17);
}

void OGLWindow::timerEvent(QTimerEvent *event) {
    World::get_instance().tick_timer();
    updateGL();
}

QSize OGLWindow::sizeHint() const {
    return QSize(800, 800);
}

QSize OGLWindow::minimumSizeHint() const {
    return QSize(50, 50);
}

void OGLWindow::resizeGL(int width, int height) {
    viewPortWidth = width;
    viewPortHeight = height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 1000.0);
    glViewport(0,0,width,height);
    updateGL();
}

void OGLWindow::paintGL() {
    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    World::get_instance().render_world();
}

GLuint OGLWindow::render_identify_scene(int x, int y, GLuint *selectBuf, GLuint bufSize) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(bufSize, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(x, viewport[3] - y, 10, 10, viewport);
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 1000.0);
    World::get_instance().identify_world();
    glFlush();
    GLuint hits = glRenderMode(GL_RENDER);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 1000.0);
    glPopName();
    return hits;
}

void OGLWindow::initializeGL() {
    setAutoBufferSwap(true);
    glShadeModel(GL_SMOOTH);
    //glFrontFace(GL_CCW);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 1000.0);
    glViewport(0, 0, viewPortWidth, viewPortHeight);
    glEnable(GL_LIGHTING);

    World::get_instance();
}

void OGLWindow::keyPressEvent(QKeyEvent *event) {
    World::get_instance().handle_key(event->key());
    updateGL();
}

void OGLWindow::mousePressEvent(QMouseEvent *event) {
    bool isRight = event->button() == Qt::RightButton;
    GLuint *selectBuf;
    GLuint hits;
    if(!isRight) {
        selectBuf = new GLuint[70];
        hits = render_identify_scene(event->x(), event->y(), selectBuf, 70);
    }
    World::get_instance().handle_click(event->x(), event->y(), isRight, hits, selectBuf);
}

void OGLWindow::mouseReleaseEvent(QMouseEvent * event) {
    World::get_instance().handle_release_click(event->button() == Qt::RightButton);
}

void OGLWindow::mouseMoveEvent(QMouseEvent * event) {
    World::get_instance().handle_move(event->x(), event->y());
}

void OGLWindow::wheelEvent(QWheelEvent *event) {
    World::get_instance().handle_wheel(event->delta());
    updateGL();
}

//================================================================================

DesktopWindow::DesktopWindow() {
    widget = new OGLWindow;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(widget);
    setLayout(mainLayout);
    setWindowTitle(tr("Sistemas Gráficos, Practica 1"));
}

void DesktopWindow::keyPressEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_Escape && isFullScreen())
        showMaximized();
    else
        widget->keyPressEvent(e);
}

void DesktopWindow::wheelEvent(QWheelEvent *event) {
    widget->wheelEvent(event);
}

void DesktopWindow::mousePressEvent(QMouseEvent *event) {
    widget->mousePressEvent(event);
}

void DesktopWindow::mouseReleaseEvent(QMouseEvent *event) {
    widget->mouseReleaseEvent(event);
}

void DesktopWindow::mouseMoveEvent(QMouseEvent *event) {
    widget->mouseMoveEvent(event);
}
