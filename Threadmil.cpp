#include "ParkObjects.h"
#include <GL/glu.h>
#include "MeshTable.h"
#include <cmath>

using namespace AmussementPark;
using namespace GraphicEngine;
using namespace std;

ThreadmilPlatform::ThreadmilPlatform():Object3DRevolution(45,true) {
    initialize();
    calc_normal();
}

void ThreadmilPlatform::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf(1.75,0.2,0));
    base.push_back(vertexf(1.75,0,0));
    replicate(base);
}

ThreadmilBase::ThreadmilBase(){
    initialize();
    calc_normal();
}

void ThreadmilBase::initialize() {
    GLfloat tempV [] = {
        0,0,0,
        0.25,0,0,
        0,0,0.25,
        0.25,0,0.25,

        0,4,1,
        0.25,4,1,
        0,3.75,1,
        0.25,3.75,1
    };

    GLuint tempF [] = {
        0,1,5,
        0,5,4,
        1,3,7,
        1,7,5,
        3,2,6,
        3,6,7,
        2,0,4,
        2,4,6,
        4,5,7,
        4,7,6,
        2,3,1,
        2,1,0
    };

    verticesSize = 8 * 3;
    vertices = new GLfloat[verticesSize];
    memcpy(vertices,tempV,verticesSize*sizeof(GLfloat));
    facesSize = 12 * 3;
    faces = new GLuint[facesSize];
    memcpy(faces,tempF,facesSize*sizeof(GLuint));
}

ThreadMilStructPart::ThreadMilStructPart():Object3DRevolution(45,false) {
    initialize();
    calc_normal();
}

void ThreadMilStructPart::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf(3,0,0));
    base.push_back(vertexf(3.1,0,0));
    base.push_back(vertexf(3,0.1,0));
    //base.push_back(vertexf(1.25,0.25,0));
    replicate(base);
}

ThreadMilRadius::ThreadMilRadius():Object3DRevolution(8,true) {
    initialize();
    calc_normal();
}

void ThreadMilRadius::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf(0.01,3,0));
    base.push_back(vertexf(0.05,0,0));
    replicate(base);
}

ThreadMilWagon::ThreadMilWagon() {
    initialize();
    calc_normal();
}

void ThreadMilWagon::initialize() {
    GLfloat tempV [] = {
       0,0,0,
       0.6,0,0,
       0,0.5,0,
       0.6,0.5,0,
       0,0,0.5,
       0.6,0,0.5,
       0,0.5,0.5,
       0.6,0.5,0.5
    };

    GLuint tempF [] = {
        0,1,2,
        1,3,2,
        1,5,3,
        5,7,3,
        5,4,7,
        4,6,7,
        4,0,6,
        0,2,6,
        4,5,0,
        5,1,0
    };

    verticesSize = 8 * 3;
    vertices = new GLfloat[verticesSize];
    memcpy(vertices,tempV,verticesSize*sizeof(GLfloat));
    facesSize = 10 * 3;
    faces = new GLuint[facesSize];
    memcpy(faces,tempF,facesSize*sizeof(GLuint));
}

ThreadMil::ThreadMil(GLuint id, GLfloat x, GLfloat y, int radius, int wagonCount):Hierarchy(id,x,y) {
    this->radiusCount = radius;
    GLfloat dif_gray[] = {0.5,0.5,0.5,0};
    GLfloat amb_gray[] = {0.2,0.2,0.2,0};
    GLfloat spec[] = {1,1,1,0};
    Material m (amb_gray,dif_gray,spec,30);

    plat = new Object3DInstance(id, new ThreadmilPlatform());
    plat->set_material(&m);
    base = new Object3DInstance(id, new ThreadmilBase());
    base->set_material(&m);
    parts = new Object3DInstance(id, new ThreadMilStructPart());
    parts->set_material(&m);
    this->radius = new Object3DInstance(id, new ThreadMilRadius());
    this->radius->set_material(&m);

    Object3D *wagonMesh;
    MeshTable::get_instance().add_mesh("threadmil_wagon", (wagonMesh = new ThreadMilWagon()));

    GLfloat white_dif [] = {0.8,0.8,0.8,0};
    GLfloat white_amb [] = {0.5,0.5,0.5,0};
    GLfloat green_dif [] = {0.2,0.8,0.2,0};
    GLfloat green_amb [] = {0.1,0.5,0.1,0};

    Material white (white_amb, white_dif, spec, 30);
    Material green (green_amb, green_dif, spec, 30);

    int i = 0;
    while(i < wagonCount) {
        Object3DInstance winst (id,wagonMesh);
        winst.set_material((i % 2 == 0? &white : &green));
        wagons.push_back(winst);
        i++;
    }

    twistAngle = 0;
    incr = 1;
}

void ThreadMil::draw_struct_part(GLfloat offset) {
    glPushMatrix();
        glTranslatef(offset,0,0);
        glPushMatrix();
            int i = 0;
            double angle = 360.0 / radiusCount;
            double acum = 0;
            while(i < radiusCount) {
                glPushMatrix();
                    glTranslatef(0,4,1);
                    glRotatef(acum + twistAngle,1,0,0);
                    //glRotatef(twistAngle,1,0,0);
                    radius->draw();
                glPopMatrix();
                acum = acum + angle;
                i++;
            }
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,4,1);
            glRotatef(90,0,0,1);
            glRotatef(twistAngle,0,1,0);
            parts->draw();
        glPopMatrix();
    glPopMatrix();
}

void ThreadMil::draw() {
    glPushMatrix();
        glTranslatef(x,0,y);
        glPushMatrix();
            glTranslatef(0.70,0,1.5);
            plat->draw();
        glPopMatrix();
        glPushMatrix();
            base->draw();
        glPopMatrix();
        glPushMatrix();
            glRotatef(180,0,1,0);
            glTranslatef(-0.25,0,-2);
            base->draw();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1,0,0);
            base->draw();
        glPopMatrix();
        glPushMatrix();
            glRotatef(180,0,1,0);
            glTranslatef(-1.25,0,-2);
            base->draw();
        glPopMatrix();
        glPushMatrix();
            draw_struct_part(0.35);
            draw_struct_part(1);
        glPopMatrix();
        glTranslatef(0.35,3.75,0.60);
        glPushMatrix();
            glRotatef(twistAngle,1,0,0);
            int i = 0;
            const int radius = 3;
            double degreeStep = (2*M_PI) / wagons.size();
            double d = 0;
            while(i < wagons.size()) {
                GLfloat x = radius * cos(d);
                GLfloat y = radius * sin(d);
                d += degreeStep;
                glPushMatrix();
                glTranslatef(0,y,x);
                glPushMatrix();
                    glRotatef(-twistAngle,1,0,0);
                    wagons[0].draw();
                glPopMatrix();
                glPopMatrix();
                i++;
            }
        glPopMatrix();
    glPopMatrix();
}

void ThreadMil::draw_to_identify() {
    glPushMatrix();
        glTranslatef(x,0,y);
        glPushMatrix();
            glTranslatef(0.70,0,1.5);
            plat->draw_to_identify();
        glPopMatrix();
        glPushMatrix();
            base->draw_to_identify();
        glPopMatrix();
        glPushMatrix();
            glRotatef(180,0,1,0);
            glTranslatef(-0.25,0,-2);
            base->draw_to_identify();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1,0,0);
            base->draw_to_identify();
        glPopMatrix();
        glPushMatrix();
            glRotatef(180,0,1,0);
            glTranslatef(-1.25,0,-2);
            base->draw_to_identify();
        glPopMatrix();
        glPushMatrix();
            draw_struct_part(0.35);
            draw_struct_part(1);
        glPopMatrix();
        glTranslatef(0.35,3.75,0.60);
        glPushMatrix();
            glRotatef(twistAngle,1,0,0);
            int i = 0;
            const int radius = 3;
            double degreeStep = (2*M_PI) / wagons.size();
            double d = 0;
            while(i < wagons.size()) {
                GLfloat x = radius * cos(d);
                GLfloat y = radius * sin(d);
                d += degreeStep;
                glPushMatrix();
                glTranslatef(0,y,x);
                glPushMatrix();
                    glRotatef(-twistAngle,1,0,0);
                    wagons[0].draw_to_identify();
                glPopMatrix();
                glPopMatrix();
                i++;
            }
        glPopMatrix();
    glPopMatrix();
}

void ThreadMil::tick() {
    if(twistAngle == 360)
        twistAngle = 1;
    else
        twistAngle += incr;
}

void ThreadMil::handle_key(int key) {
    switch(key) {
    case '+':
        incr += 0.2;
        break;
    case '-':
        incr -= 0.2;
        break;
    }
}
