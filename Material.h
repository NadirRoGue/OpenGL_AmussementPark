#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/gl.h>

namespace GraphicEngine {
    class Material {
    private:
        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat specular[4];
        GLfloat shineness;

        void initialize(GLfloat *amb, GLfloat *dif, GLfloat *spec, GLfloat shine);
    public:
        Material();
        Material(GLfloat *amb, GLfloat *dif, GLfloat *spec, GLfloat shine);
        Material(Material *other);
        void set_ambient_color(GLfloat *amb);
        void set_diffuse_color(GLfloat *dif);
        void set_specular_color(GLfloat *spec);
        void set_shineness(GLfloat shine);
        void apply_me();
    };
}

#endif // MATERIAL_H
