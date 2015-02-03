#include "ParkObjects.h"
#include "Material.h"
#include "Object3D.h"
#include "MeshTable.h"
#include <vector>
#include <cmath>

using namespace AmussementPark;
using namespace GraphicEngine;
using namespace std;

CarrouselRoof::CarrouselRoof():Object3DRevolution(45, true){
    initialize();
    calc_normal();
}

void CarrouselRoof::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf (0.01, 1, 0));
    base.push_back(vertexf (2.5, 0, 0));
    replicate(base);
}

CarrouselAxis::CarrouselAxis():Object3DRevolution(45,true) {
    initialize();
    calc_normal();
}

void CarrouselAxis::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf (0.35,1,0));
    base.push_back(vertexf (0.35,0.8,0));
    base.push_back(vertexf (0.25,0.8,0));
    base.push_back(vertexf (0.25, 0.2, 0));
    base.push_back(vertexf (0.35,0.2,0));
    base.push_back(vertexf (0.35,0,0));
    replicate(base);
}

CarrouselPlatform::CarrouselPlatform():Object3DRevolution(45,true) {
    initialize();
    calc_normal();
}

void CarrouselPlatform::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf (2.3,0.2,0));
    base.push_back(vertexf (2.3,0.1,0));
    replicate(base);
}

CarrouselBase::CarrouselBase():Object3DRevolution(45,true) {
    initialize();
    calc_normal();
}

void CarrouselBase::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf (2.5,0.1,0));
    base.push_back(vertexf (2.5,0,0));
    replicate(base);
}

HobbyHorseInstance::HobbyHorseInstance(GLuint id, GLfloat *dh, GLfloat *dp, bool gUp):
    Hierarchy(id) {
    GLfloat reduct = 0.3;
    GLfloat amb_horse [] = {dh[0]-reduct,dh[1]-reduct,dh[2]-reduct,0};
    GLfloat amb_pole [] = {dp[0]-reduct,dp[1]-reduct,dp[2]-reduct,0};
    GLfloat spec [] = {1,1,1,0};
    hobbyHorse = new Object3DInstance(id, MeshTable::get_instance().get_mesh(string("caballito")));
    hobbyHorse->set_material();
    hobbyHorse->get_material()->set_ambient_color(amb_horse);
    hobbyHorse->get_material()->set_diffuse_color(dh);
    hobbyHorse->get_material()->set_specular_color(spec);
    hobbyHorse->get_material()->set_shineness(30);
    pole = new Object3DInstance(id, MeshTable::get_instance().get_mesh(string("barra")));
    pole->set_material();
    pole->get_material()->set_ambient_color(amb_pole);
    pole->get_material()->set_diffuse_color(dp);
    pole->get_material()->set_specular_color(spec);
    pole->get_material()->set_shineness(30);
    goingUp = gUp;
    curPositionInPole = goingUp? 0 : 0.3;
}

void HobbyHorseInstance::draw() {
    glPushMatrix();
        glRotatef(-90,1,0,0);
        pole->draw();
        glPushMatrix();
            glTranslatef(0,-0.10,curPositionInPole);
            //glTranslatef(0,curPositionInPole,0);
            hobbyHorse->draw();
        glPopMatrix();
    glPopMatrix();
}

void HobbyHorseInstance::draw_to_identify() {
    glPushMatrix();
        glRotatef(-90,1,0,0);
        pole->draw_to_identify();
        glPushMatrix();
            glTranslatef(0,-0.10,curPositionInPole);
            //glTranslatef(0,curPositionInPole,0);
            hobbyHorse->draw_to_identify();
        glPopMatrix();
    glPopMatrix();
}

void HobbyHorseInstance::tick() {
    if(goingUp) {
        curPositionInPole += 0.01;
        if(curPositionInPole >= 0.3)
            goingUp = false;
    } else {
        curPositionInPole -= 0.01;
        if(curPositionInPole <= 0.0)
            goingUp = true;
    }
}

Carrousel::Carrousel(GLuint id, GLfloat x, GLfloat y, GLfloat defaultAngleStep, int h):
    Hierarchy(id,x,y){
    horseCount = h;
    angleStep = defaultAngleStep;
    angle = 0;
    horsePosAngle = (2*M_PI)/horseCount;

    GLfloat horseColors [][4] = {
      {0.3,0.3,0.3,0},                    // caballito negro
      {0.9,0.9,0.9,0},              // caballito blanco
      {0.5,0.5,0.5,0},              // caballito gris
      {0.647059,0.164706,0.164706,0}  // caballito marrón
    };

    GLfloat poleColor [] = {0.858824,0.858824,0.439216,0}; // barra

    int i = 0;
    int color = 0;
    while(i < horseCount) {
        GLfloat *colorData = horseColors[color];
        color = (color + 1) % 4;
        HobbyHorseInstance h(id,colorData,poleColor,i%2==0);
        horses.push_back(h);
        i++;
    }

    GLfloat dif_gray[] = {0.4,0.4,0.4,0};
    GLfloat amb_gray[] = {0.2,0.2,0.2,0};
    GLfloat dif[] = {0.8,0.1,0.1,0};
    GLfloat spec[] = {1,1,1,0};
    GLfloat amb[] = {0.6,0.1,0.1,0};

    roof = new Object3DInstance(id,new CarrouselRoof());
    roof->set_material();
    roof->get_material()->set_ambient_color(amb);
    roof->get_material()->set_diffuse_color(dif);
    roof->get_material()->set_specular_color(spec);
    roof->get_material()->set_shineness(30);
    axis = new Object3DInstance(id,new CarrouselAxis());
    axis->set_material();
    axis->get_material()->set_ambient_color(amb_gray);
    axis->get_material()->set_diffuse_color(dif_gray);
    axis->get_material()->set_specular_color(spec);
    axis->get_material()->set_shineness(30);
    plat = new Object3DInstance(id,new CarrouselPlatform());
    plat->set_material();
    plat->get_material()->set_ambient_color(amb_gray);
    plat->get_material()->set_diffuse_color(dif_gray);
    plat->get_material()->set_specular_color(spec);
    plat->get_material()->set_shineness(30);
    base = new Object3DInstance(id,new CarrouselBase());
    base->set_material();
    base->get_material()->set_ambient_color(amb_gray);
    base->get_material()->set_diffuse_color(dif_gray);
    base->get_material()->set_specular_color(spec);
    base->get_material()->set_shineness(30);
}

void Carrousel::draw() {
    glPushMatrix();
    glTranslatef(this->x,0,this->y);
    base->draw();
    glPushMatrix();
    glRotatef(angle,0,1,0);
    plat->draw();
    glTranslatef(0,0.2,0);
    axis->draw();

    int i = 0;
    double angle = 0;
    const double radius = 2;
    GLfloat x = 0, z = 0;
    while(i < horseCount) {
        x = radius * cos(angle);
        z = radius * sin(angle);

        glPushMatrix();
            glScalef(0.8,0.8,0.8);
            glTranslatef(x,-0.18,z);
            glRotatef(-(angle*(180/M_PI)),0,1,0);
            horses[i].draw();
        glPopMatrix();
        i++;
        angle = angle + horsePosAngle;
    }

    glPushMatrix();
        glTranslatef(0,1,0);
        roof->draw();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void Carrousel::draw_to_identify() {
    glPushMatrix();
    glTranslatef(this->x,0,this->y);
    base->draw_to_identify();
    glPushMatrix();
    glRotatef(angle,0,1,0);
    plat->draw_to_identify();
    glTranslatef(0,0.2,0);
    axis->draw_to_identify();

    int i = 0;
    double angle = 0;
    const double radius = 2;
    GLfloat x = 0, z = 0;
    while(i < horseCount) {
        x = radius * cos(angle);
        z = radius * sin(angle);

        glPushMatrix();
            glScalef(0.8,0.8,0.8);
            glTranslatef(x,-0.18,z);
            glRotatef(-(angle*(180/M_PI)),0,1,0);
            horses[i].draw_to_identify();
        glPopMatrix();
        i++;
        angle = angle + horsePosAngle;
    }

    glPushMatrix();
        glTranslatef(0,1,0);
        roof->draw_to_identify();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void Carrousel::handle_key(int key) {
    switch(key) {
        case '+':
            angleStep = angleStep + 1;
        break;
        case '-':
        if(angleStep > 0)
            angleStep = angleStep - 1;
        break;
    }
}

void Carrousel::tick() {
    angle -= angleStep;
    int i = 0;
    while(i < horseCount) {
        horses[i].tick();
        i++;
    }
}

