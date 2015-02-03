#include "Material.h"
#include <cstring>

using namespace GraphicEngine;

Material::Material(){
    GLfloat empty [] = {0,0,0,0};
    initialize(empty,empty,empty,0);
}

Material::Material(GLfloat *amb, GLfloat *dif, GLfloat *spec, GLfloat shine) {
    initialize(amb,dif,spec,shine);
}

Material::Material(Material *other) {
    initialize(other->ambient,other->diffuse,other->specular,other->shineness);
}

void Material::initialize(GLfloat *amb, GLfloat *dif, GLfloat *spec, GLfloat shine) {
    memcpy(ambient,amb,4*sizeof(GLfloat));
    memcpy(diffuse,dif,4*sizeof(GLfloat));
    memcpy(specular,spec,4*sizeof(GLfloat));
    shineness = shine;
}

void Material::set_ambient_color(GLfloat *amb) {
    memcpy(ambient,amb,4*sizeof(GLfloat));
}

void Material::set_diffuse_color(GLfloat *dif) {
    memcpy(diffuse,dif,4*sizeof(GLfloat));
}

void Material::set_specular_color(GLfloat *spec) {
    memcpy(specular,spec,4*sizeof(GLfloat));
}

void Material::set_shineness(GLfloat shine) {
    shineness = shine;
}

void Material::apply_me() {
    glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
    glMaterialf(GL_FRONT,GL_SHININESS,shineness);
}
