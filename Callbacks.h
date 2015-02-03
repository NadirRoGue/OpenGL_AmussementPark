#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <QGLWidget>

typedef void (*render_func)(QGLWidget *w);

void render_scene(QGLWidget *w);
void render_identify_scene(QGLWidget *w);

#endif // CALLBACKS_H
