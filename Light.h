#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>

namespace GraphicEngine {
    class LightSource {
    private:
        GLenum light;
        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat specular[4];
        GLfloat pos[4];
        GLfloat spotCutOff;
        GLfloat spotDirection[3];

        void init();
    public:
        LightSource(GLenum srcLight);
        LightSource(GLenum srcLight, GLfloat *amb, GLfloat *dif, GLfloat *spec, GLfloat *p, GLfloat spot=180);
        void set_ambient(GLfloat *amb);
        void set_difusse(GLfloat *dif);
        void set_specular(GLfloat *spec);
        void set_pos(GLfloat *p);
        void set_spot_cutoff(GLfloat spot);
        void set_spot_direction(GLfloat *spotDir);
        void update_me();
    };
}

#endif // LIGHT_H
