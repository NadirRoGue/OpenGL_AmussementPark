#include "Light.h"

using namespace GraphicEngine;

#include <cstring>

LightSource::LightSource(GLenum srcLight) {
    light = srcLight;
    spotCutOff = 180;
    GLfloat empty[] = {0,0,0,0};
    memcpy(ambient, empty, 4 * sizeof(GLfloat));
    memcpy(diffuse, empty, 4 * sizeof(GLfloat));
    memcpy(specular, empty, 4 * sizeof(GLfloat));
    memcpy(pos, empty, 4 * sizeof(GLfloat));
    spotDirection[0] = spotDirection[1] = 0;
    spotDirection[2] = -1.0;
    init();
}

LightSource::LightSource(GLenum srcLight, GLfloat *amb, GLfloat *dif, GLfloat *spec, GLfloat *p, GLfloat spot) {
    light = srcLight;
    spotCutOff = spot;
    memcpy(ambient, amb, 4 * sizeof(GLfloat));
    memcpy(diffuse, dif, 4 * sizeof(GLfloat));
    memcpy(specular, spec, 4 * sizeof(GLfloat));
    memcpy(pos, p, 4 * sizeof(GLfloat));
    spotDirection[0] = spotDirection[1] = 0;
    spotDirection[2] = -1.0;
    init();
}

void LightSource::set_ambient(GLfloat *amb) {
    memcpy(ambient, amb, 4 * sizeof(GLfloat));
}

void LightSource::set_difusse(GLfloat *dif) {
    memcpy(diffuse, dif, 4 * sizeof(GLfloat));
}

void LightSource::set_specular(GLfloat *spec) {
    memcpy(specular, spec, 4 * sizeof(GLfloat));
}

void LightSource::set_pos(GLfloat *p) {
    memcpy(pos, p, 4 * sizeof(GLfloat));
}

void LightSource::set_spot_cutoff(GLfloat spot){
    spotCutOff = spot;
}

void LightSource::set_spot_direction(GLfloat *spotDir){
    memcpy(spotDirection, spotDir, 3 * sizeof(GLfloat));
}

void LightSource::init() {
    glEnable(light);
    glEnable(GL_LIGHTING);

    glLightfv(light,GL_AMBIENT,ambient);
    glLightfv(light,GL_DIFFUSE,diffuse);
    glLightfv(light,GL_SPECULAR,specular);
    glLightfv(light,GL_POSITION,pos);
    glLightf(light,GL_SPOT_CUTOFF, spotCutOff);
    glLightfv(light,GL_SPOT_DIRECTION,spotDirection);
}

void LightSource::update_me() {
    glLightfv(light,GL_POSITION,pos);
    glLightfv(light,GL_SPOT_DIRECTION,spotDirection);
}
