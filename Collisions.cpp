#include <cmath>
#include "Collisions.h"

#include <iostream>

using namespace std;
using namespace GraphicEngine;

CollisionObject::CollisionObject(DWORD32 id, Object3D *o):
    Object3DInstance(id, o) {

}

void CollisionObject::add_to_knownlist(CollisionObject *o) {
    knownList.push_back(o);
}

bool CollisionObject::check_collisions(GLfloat x, GLfloat y) {
    vector<CollisionObject*>::iterator it;
    CollisionObject *obj;
    for(it = knownList.begin(); it != knownList.end(); it++) {
        obj = *it;
        if(obj->collides(x, y))
            return true;
    }
    return false;
}

CircleCollisionObject::CircleCollisionObject(DWORD32 id, Object3D *o):
    CollisionObject(id, o){
}

bool CircleCollisionObject::collides(GLfloat x, GLfloat y) {
    double difx = get_center_x() - x;
    double dify = get_center_y() - y;
    double dist = std::sqrt((difx*difx)+(dify*dify));
    return (dist <= get_radius());
}

SquareCollisionObject::SquareCollisionObject(DWORD32 id, Object3D *o):
    CollisionObject(id, o){
}

bool SquareCollisionObject::collides(GLfloat x, GLfloat y) {
    return ((x >= min_x() && y >= min_y())
            && (x <= max_x() && y <= max_y()));
}
