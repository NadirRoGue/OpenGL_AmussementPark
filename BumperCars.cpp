#include <vector>
#include <cstdlib>
#include "MeshTable.h"
#include "ParkObjects.h"
#include "Object3D.h"
#include "Vertex.h"
#include "Camera.h"
#include "IDFactory.h"

#include <iostream>

using namespace std;
using namespace GraphicEngine;
using namespace AmussementPark;


BumperCarsStructRoof::BumperCarsStructRoof(GLfloat width, GLfloat height):
    bWidth(width), bHeight(height) {
    initialize();
    calc_normal();
}

void BumperCarsStructRoof::initialize() {
    GLfloat tempV [] = {
        0,0,0,
        bWidth,0,0,
        0,0.3,0,
        bWidth,0.3,0,
        0,0,bHeight,
        bWidth,0,bHeight,
        0,0.3,bHeight,
        bWidth,0.3,bHeight,
        bWidth/2,0.8,0.7,
        bWidth/2,0.8,bHeight-0.7
    };

    verticesSize = 10 * 3;
    vertices = new GLfloat[verticesSize];
    memcpy(vertices,tempV,verticesSize * sizeof(GLfloat));

    GLuint tempF [] = {
        0,1,3,
        0,3,2,
        1,5,7,
        1,7,3,
        5,4,6,
        5,6,7,
        4,0,2,
        4,2,6,
        4,1,0,
        4,5,1,
        2,3,8,
        7,6,9,
        3,9,8,
        3,7,9,
        6,2,8,
        6,8,9
    };

    facesSize = 16 * 3;
    faces = new GLuint[facesSize];
    memcpy(faces,tempF,facesSize * sizeof(GLuint));
}

//=============================================================================

BumperCarsStructColumn::BumperCarsStructColumn():Object3DRevolution(16,true) {
    initialize();
    calc_normal();
}

void BumperCarsStructColumn::initialize() {
    vector<vertexf> base;
    base.push_back(vertexf (0.1,2,0));
    base.push_back(vertexf (0.1,0,0));
    replicate(base);
}

//=============================================================================

BumperCarsStructBase::BumperCarsStructBase(GLfloat width, GLfloat height):
    bWidth(width), bHeight(height) {
    initialize();
    calc_normal();
}

void BumperCarsStructBase::initialize() {
    GLfloat w = bWidth + 0.5;
    GLfloat h = bHeight + 0.5;
    GLfloat tempV [] = {
        0,0,0,
        w,0,0,
        0,0.1,0,
        w,0.1,0,
        0,0,h,
        w,0,h,
        0,0.1,h,
        w,0.1,h
    };

    GLuint tempF [] = {
        0,1,3,
        0,3,2,
        1,5,7,
        1,7,3,
        5,4,6,
        5,6,7,
        4,0,2,
        4,2,6,
        2,3,7,
        2,7,6,
        4,5,1,
        4,1,0
    };

    verticesSize = 8 * 3;
    vertices = new GLfloat[verticesSize];
    memcpy(vertices,tempV,verticesSize * sizeof(GLfloat));

    facesSize = 12 * 3;
    faces = new GLuint[facesSize];
    memcpy(faces,tempF,facesSize * sizeof(GLuint));
}

//=============================================================================

BumperCarsBaseBorder::BumperCarsBaseBorder() {
    initialize();
    calc_normal();
}

void BumperCarsBaseBorder::initialize() {
    GLfloat tempV [] = {
        0,0,0,
        0.2,0,0,
        0,0.2,0,
        0.2,0.2,0,
        0,0,0.2,
        0.2,0,0.2,
        0,0.2,0.2,
        0.2,0.2,0.2,
    };

    GLuint tempF [] = {
        0,1,3,
        0,3,2,
        1,5,7,
        1,7,3,
        5,4,6,
        5,6,7,
        4,0,2,
        4,2,6,
        2,3,7,
        2,7,6,
        4,5,1,
        4,1,0
    };

    verticesSize = 8 * 3;
    vertices = new GLfloat[verticesSize];
    memcpy(vertices,tempV,verticesSize*sizeof(GLfloat));
    facesSize = 12 * 3;
    faces = new GLuint[facesSize];
    memcpy(faces,tempF,facesSize*sizeof(GLuint));
}

//=============================================================================

CollidableCar::CollidableCar(GLuint id, Object3D *obj, GLfloat rad):
    CircleCollisionObject(id,obj),radius(rad) {
    x = y = 0;
}

void CollidableCar::update(GLfloat newX, GLfloat newY) {
    x = newX;
    y = newY;
}

GLfloat CollidableCar::get_center_x() {
    return x;
}

GLfloat CollidableCar::get_center_y() {
    return y;
}

GLfloat CollidableCar::get_radius() {
    return radius;
}

//=============================================================================

BorderInstance::BorderInstance(GLuint id, Object3D *obj, GLfloat xmin, GLfloat ymin, GLfloat xmax, GLfloat ymax):
    SquareCollisionObject(id, obj),minX(xmin),minY(ymin),maxX(xmax),maxY(ymax) {
}

GLfloat BorderInstance::min_x() { return minX; }
GLfloat BorderInstance::max_x() { return maxX; }
GLfloat BorderInstance::min_y() { return minY; }
GLfloat BorderInstance::max_y() { return maxY; }

//=============================================================================

BumperCarInstance::BumperCarInstance(GLuint id, Material *mat):
    Hierarchy(id){
    bumperCar = new CollidableCar(id,MeshTable::get_instance().get_mesh("coche"),1);
    pole = new Object3DInstance(id,MeshTable::get_instance().get_mesh("barra"));

    GLfloat dif_black[] = {0.3,0.3,0.3,0};
    GLfloat amb_black[] = {0,0,0,0};
    GLfloat spec[] = {1,1,1,0};
    Material m (amb_black,dif_black,spec,30);
    pole->set_material(&m);
    bumperCar->set_material(mat);

    xPos = yPos = heading = 0;
    speed = 0.1;

    attachedCam = new CarCamera();
}

void BumperCarInstance::draw() {
    glPushMatrix();
        glRotatef(-90,1,0,0);
        pole->draw();
        glPushMatrix();
            glTranslatef(0,-0.6,0);
            bumperCar->draw();
        glPopMatrix();
    glPopMatrix();
}

void BumperCarInstance::draw_to_identify() {
    glPushMatrix();
        glRotatef(-90,1,0,0);
        pole->draw_to_identify();
        glPushMatrix();
            glTranslatef(0,-0.6,0);
            bumperCar->draw_to_identify();
        glPopMatrix();
    glPopMatrix();
}

void BumperCarInstance::set_pos(GLfloat x, GLfloat y, GLfloat z, GLfloat heading) {
    xPos = x;
    yPos = z;
    bumperCar->update(x, z);
    this->heading = heading;
    attachedCam->set_x(xPos);
    attachedCam->set_y(y);
    attachedCam->set_z(yPos);
    attachedCam->set_heading(heading);
}

void BumperCarInstance::tick() {    
    double degree = (M_PI / 180.0) * heading;
    double sin_ = sin(degree);
    double cos_ = cos(degree);
    GLfloat x = (bumperCar->get_radius() * sin_) + xPos;
    GLfloat y = (bumperCar->get_radius() * cos_) + yPos;

    if(bumperCar->check_collisions(x, y)) {
        rotate();
        attachedCam->set_heading(heading);
    }
    else {
        xPos = xPos + (sin_ * speed);
        yPos = yPos + (cos_ * speed);
        bumperCar->update(xPos, yPos);
        attachedCam->set_x(xPos);
        attachedCam->set_z(yPos);
    }
}

GLfloat BumperCarInstance::get_heading() {
    return heading;
}

GLfloat BumperCarInstance::get_x() {
    return xPos;
}

GLfloat BumperCarInstance::get_y() {
    return yPos;
}

void BumperCarInstance::rotate() {
    heading += 1;
}

void BumperCarInstance::update_knownlist(CollisionObject *obj) {
    bumperCar->add_to_knownlist(obj);
}

void BumperCarInstance::update_knownlist(BumperCarInstance &o) {
    bumperCar->add_to_knownlist(o.bumperCar);
}

void BumperCarInstance::handle_key(int key) {
    switch(key) {
    case '+':
        speedUp();
        break;
    case '-':
        slowDown();
        break;
    }
}

void BumperCarInstance::speedUp() {
    speed += 0.1;
}

void BumperCarInstance::slowDown() {
    speed -= 0.1;
}

CarCamera * BumperCarInstance::get_cam() {
    return attachedCam;
}

void CarCamera::set_x(GLfloat x) { xPos = x; }
void CarCamera::set_y(GLfloat y) { yPos = y; }
void CarCamera::set_z(GLfloat z) { zPos = z; }
void CarCamera::set_heading(GLfloat heading) { this->heading = heading; }
GLfloat CarCamera::get_src_x() { return xPos - (0.5*sin((M_PI/180)*heading)); }
GLfloat CarCamera::get_src_y() { return yPos + 0.7; }
GLfloat CarCamera::get_src_z() { return zPos - (0.5*cos((M_PI/180)*heading)); }
GLfloat CarCamera::get_colatitude_angle() { return heading; }
GLfloat CarCamera::get_vision_radius() { return 1; }

//=============================================================================

BumperCarBuild::BumperCarBuild(GLuint id, GLfloat width, GLfloat height, int c, GLfloat x, GLfloat y,
                               vector<CameraInterface*> &cams):
        Hierarchy(id,x,y) {
    carCount = c;
    bWidth = width;
    bHeight = height;
    Object3D *roofObj;
    MeshTable::get_instance().add_mesh("bumper_cars_roof", (roofObj = new BumperCarsStructRoof(width,height)));
    buildRoof = new Object3DInstance(id, roofObj);
    buildRoof->set_material();
    GLfloat dif[] = {0.2,0.2,0.8,0};
    GLfloat amb[] = {0.2,0.2,0.6,0};
    GLfloat spec[] = {1,1,1,0};
    buildRoof->get_material()->set_ambient_color(amb);
    buildRoof->get_material()->set_diffuse_color(dif);
    buildRoof->get_material()->set_specular_color(spec);
    buildRoof->get_material()->set_shineness(30);

    GLfloat dif_m [] = {0.5,0.5,0.5,0};
    GLfloat amb_m [] = {0.4,0.4,0.4,0};
    Material m (amb_m, dif_m, spec, 30);

    Object3D *baseObj;
    MeshTable::get_instance().add_mesh("bumper_cars_base", (baseObj = new BumperCarsStructBase(width, height)));
    buildBase = new Object3DInstance(id, baseObj);
    buildBase->set_material(&m);

    BumperCarsStructColumn *col;
    MeshTable::get_instance().add_mesh("bumper_cars_column", (col = new BumperCarsStructColumn()));
    column = new Object3DInstance(id, col);
    column->set_material(&m);

    GLfloat dif_red[] = {0.8,0.1,0.1,0};
    GLfloat amb_red[] = {0.6,0.1,0.1,0};
    GLfloat dif_blue[] = {0.1,0.1,0.8};
    GLfloat amb_blue[] = {0.1,0.1,0.6};

    Material red (amb_red,dif_red,spec,30);
    Material blue (amb_blue,dif_blue,spec,30);

    int i = 0;
    if(carCount > 8)
        carCount = 8;
    else if(carCount < 0)
        carCount = 0;
    while(i < carCount) {
        BumperCarInstance car (id,(i%2==0? &red : &blue));
        cars.push_back(car);
        cams.push_back(car.get_cam());
        i++;
    }

    BumperCarsBaseBorder *bord;
    MeshTable::get_instance().add_mesh("bumper_cars_border", (bord = new BumperCarsBaseBorder()));

    GLfloat baseOffX = 0.25;
    GLfloat baseOffZ = 0.25;
    GLfloat xOff = this->x + baseOffX;
    GLfloat zOff = this->y + baseOffZ;
    GLfloat bcg = 0.5; // BORDER COLLISION GAP
    BorderInstance b1 (id,bord,xOff+0.2,zOff+0.2-bcg,xOff+bWidth-0.2,zOff+0.2);
    b1.set_material(&m);
    borders.push_back(b1);
    BorderInstance b2 (id,bord,xOff+0.2,zOff+bHeight-0.2-bcg,xOff+bWidth-0.2,zOff+bHeight-0.2);
    b2.set_material(&m);
    borders.push_back(b2);
    BorderInstance b3 (id,bord,xOff+0.2,zOff+0.2,xOff+0.2+bcg,zOff+bHeight-0.2);
    b3.set_material(&m);
    borders.push_back(b3);
    BorderInstance b4 (id,bord,xOff+bWidth-0.2-bcg,zOff+0.2,xOff+bWidth-0.2,zOff+bHeight-0.2);
    b4.set_material(&m);
    borders.push_back(b4);

    xOff += 1.5;
    zOff += 1.5;

    // Inicializar knownlist y posiciones iniciales
    for(unsigned int i = 0; i < cars.size(); i++) {
        GLfloat head = (GLfloat) (rand() % 360);
        if(i % 2 == 0)
            cars[i].set_pos(xOff, 0.1, zOff + i + (i*0.1), head);
        else
            cars[i].set_pos(xOff + 2, 0.1, zOff + i, head);

        for(unsigned int j = 0; j < 4; j++)
            cars[i].update_knownlist(&borders[j]);
        for(unsigned int j = 0; j < cars.size(); j++) {
            if(j != i)
                cars[i].update_knownlist(cars[j]);
        }
    }

    GLfloat amb_l[] = {0,0,0,0};
    GLfloat dif_l[] = {0.8,0.8,0.8,0};
    GLfloat pos[] = {0,0,0,1};
    GLfloat spot[] = {0,0,0};
    ilum = new LightSource(GL_LIGHT1,amb_l,dif_l,spec,pos,90);
    ilum->set_spot_direction(spot);
}

void BumperCarBuild::handle_key(int key) {
    cout << "Aqui" << endl;
    switch(key) {
    case '+':
        for(unsigned int i = 0; i < cars.size(); i++) {
            cars[i].speedUp();
        }
        break;
    case '-':
        for(unsigned int i = 0; i < cars.size(); i++) {
            cars[i].slowDown();
        }
        break;
    }
}

void BumperCarBuild::draw() {
    glPushMatrix();
    int i = 0;
    while(i < carCount) {
        glPushMatrix();
        BumperCarInstance inst = cars[i++];
        glTranslatef(inst.get_x(),0.1,inst.get_y());
        glRotatef(inst.get_heading(),0,1,0);
        inst.draw();
        glPopMatrix();
    }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(this->x,0,this->y);
        buildBase->draw();
        glPushMatrix();
            glTranslatef(0.25,0.1,0.25);
            glPushMatrix();
                glTranslatef(0.2,0,0);
                glScalef((bWidth-0.4)/0.2,1,1);
                borders[0].draw();
                glPushMatrix();
                    glTranslatef(0,0,bHeight-0.2);
                    borders[1].draw();
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,0,0.2);
                glScalef(1,1,(bHeight-0.4)/0.2);
                borders[2].draw();
                glPushMatrix();
                    glTranslatef(bWidth-0.2,0,0);
                    borders[3].draw();
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.1,0,0.1);
               column->draw();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(bWidth-0.1,0,0.1);
                column->draw();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.1,0,bHeight-0.1);
                column->draw();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(bWidth-0.1,0,bHeight-0.1);
                column->draw();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,2,0);
                buildRoof->draw();
                glPushMatrix();
                glTranslatef(bWidth/2,-0.1,bHeight/2);
                ilum->update_me();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void BumperCarBuild::draw_to_identify() {
    glPushMatrix();
    int i = 0;
    while(i < carCount) {
        glPushMatrix();
        BumperCarInstance inst = cars[i++];
        glTranslatef(inst.get_x(),0.1,inst.get_y());
        glRotatef(inst.get_heading(),0,1,0);
        inst.draw_to_identify();
        glPopMatrix();
    }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(this->x,0,this->y);
        buildBase->draw_to_identify();
        glPushMatrix();
            glTranslatef(0.25,0.1,0.25);
            glPushMatrix();
                glTranslatef(0.2,0,0);
                glScalef((bWidth-0.4)/0.2,1,1);
                borders[0].draw_to_identify();
                glPushMatrix();
                    glTranslatef(0,0,bHeight-0.2);
                    borders[1].draw_to_identify();
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,0,0.2);
                glScalef(1,1,(bHeight-0.4)/0.2);
                borders[2].draw_to_identify();
                glPushMatrix();
                    glTranslatef(bWidth-0.2,0,0);
                    borders[3].draw_to_identify();
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.1,0,0.1);
               column->draw_to_identify();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(bWidth-0.1,0,0.1);
                column->draw_to_identify();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.1,0,bHeight-0.1);
                column->draw_to_identify();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(bWidth-0.1,0,bHeight-0.1);
                column->draw_to_identify();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0,2,0);
                buildRoof->draw_to_identify();
                glPushMatrix();
                glTranslatef(bWidth/2,-0.1,bHeight/2);
                ilum->update_me();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void BumperCarBuild::tick() {
    unsigned int i = 0;
    while(i < cars.size()) {
        cars[i++].tick();
    }
}
