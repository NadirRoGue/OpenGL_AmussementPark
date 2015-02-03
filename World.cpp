#include "ParkObjects.h"
#include <string>
#include <iostream>
#include "Vertex.h"
#include "IDFactory.h"
#include "Light.h"

using namespace std;
using namespace GraphicEngine;
using namespace AmussementPark;

#define KEY_UP 0x01000013
#define KEY_DOWN 0x01000015
#define KEY_RIGHT 0x01000014
#define KEY_LEFT 0x01000012

World * World::INSTANCE = 0;

FloorTile::FloorTile() {
    initialize();
    calc_normal();
}

void FloorTile::initialize() {
    GLfloat vert [] = {
        -15,0,-15,
        15,0,-15,
        15,0,15,
        -15,0,15
    };

    GLuint fac [] = {
        0,1,2,
        0,2,3
    };

    verticesSize = 4*3;
    facesSize = 2*3;
    vertices = new GLfloat[verticesSize];
    memcpy(vertices,vert,verticesSize * sizeof(GLfloat));
    faces = new GLuint[facesSize];
    memcpy(faces,fac,facesSize * sizeof(GLuint));
}

DWORD FloorTile::get_model_height() {
    return 2;
}

DWORD FloorTile::get_model_width() {
    return 2;
}

World::World() {
    initialize();
}

void World::initialize() {
    defaultCamera = new DefaultCameraObject (0,0,-25,50,0,0);
    cameras.push_back(defaultCamera);
    camera.attach_to_object(defaultCamera);
    selectedCam = 0;
    rightClick = false;

    selectedObj = NULL;
    GLuint cId = IdFactory::get_instance().get_next_id();
    selectable[cId] = new Carrousel(cId,1,1,1,8);
    GLuint bId = IdFactory::get_instance().get_next_id();
    selectable[bId] = new BumperCarBuild(bId,5,8,4,-5,6,cameras);
    GLuint tId = IdFactory::get_instance().get_next_id();
    selectable[tId] = new ThreadMil(tId, 5,5,30,6);

    GLfloat green_dif [] = {0.2,0.9,0.2,0};
    GLfloat green_amb [] = {0.2,0.5,0.2,0};
    GLfloat spec [] = {1,1,1,0};
    Material m(green_amb,green_dif,spec,10);
    floor = new Object3DInstance(0xff,new FloorTile());
    floor->set_material(&m);

    GLfloat amb[] = {0,0,0,0};
    GLfloat dif[] = {0.8,0.8,0.8,0};
    GLfloat pos[] = {10,10,0,1};
    lights.push_back(LightSource (GL_LIGHT0,amb,dif,spec,pos));
}

void World::handle_click(int x, int y, bool rightClick, GLuint hits, GLuint *selectBuf) {
    if((this->rightClick = rightClick)) {
        if(selectedCam == 0)
            defaultCamera->update_angles(x, y,false);
    } else {
        if(hits == 0)
            return;
        GLuint name_count = *(selectBuf);
        cout << name_count << endl;
        if(name_count <= 0)
            return;
        GLuint objId = *(selectBuf+2+name_count);

        cout << "Obj: " << objId << endl;
        map<GLuint,Hierarchy*>::iterator it = selectable.find(objId);
        if(it != selectable.end()) {
            cout << "Found" << endl;
            selectedObj = it->second;
        }
    }
}

void World::handle_release_click(bool rightClick) {
    if(rightClick)
        this->rightClick = false;
}

void World::handle_key(int key) {
    if(key == KEY_UP && selectedCam == 0)
        defaultCamera->update_y(0.1);
    else if(key == KEY_DOWN)
        defaultCamera->update_y(-0.1);
    else if(key == KEY_LEFT)
        defaultCamera->update_x(-0.1);
    else if(key == KEY_RIGHT)
        defaultCamera->update_x(0.1);
    else if(key >= '0' && key <= '9') {
        unsigned char c = (unsigned char) (key - '0');
        if(c != selectedCam) {
            selectedCam = c;
            if(selectedCam > cameras.size())
                selectedCam = cameras.size();
            camera.attach_to_object(cameras[selectedCam]);
        }
    }
    else if(selectedObj != NULL)
        selectedObj->handle_key(key);
}

void World::handle_move(int x, int y) {
    if(selectedCam == 0) {
        defaultCamera->update_angles(x, y,true);
        camera.update_dst_3D();
    }
}

void World::handle_wheel(int delta) {
    if(selectedCam == 0) {
        if(delta > 0)
            defaultCamera->update_z(1);
        else
            defaultCamera->update_z(-1);
    }
}

void World::render_world() {

    camera.project();

    floor->draw();

    for(unsigned int i = 0; i < lights.size(); i++)
        lights[i].update_me();

    map<GLuint,Hierarchy*>::iterator it = selectable.begin();
    while(it != selectable.end()) {
        glPushMatrix();
            it->second->draw();
        glPopMatrix();
        it++;
    }
}

void World::identify_world() {

    camera.project();

    for(unsigned int i = 0; i < lights.size(); i++)
        lights[i].update_me();

    map<GLuint,Hierarchy*>::iterator it = selectable.begin();
    while(it != selectable.end()) {
        glPushMatrix();
            it->second->draw_to_identify();
        glPopMatrix();
        it++;
    }

}

void World::tick_timer() {
    map<GLuint,Hierarchy*>::iterator it = selectable.begin();
    while(it != selectable.end()) {
        glPushMatrix();
            it->second->tick();
        glPopMatrix();
        it++;
    }

    if(selectedCam > 0)
        camera.update_dst_2D();
}

void World::register_light(LightSource &l) {
    lights.push_back(l);
}

void World::register_camera(CameraInterface *cam) {
    cameras.push_back(cam);
}

World & World::get_instance() {
    if(World::INSTANCE == 0)
        World::INSTANCE = new World();

    return *(World::INSTANCE);
}
