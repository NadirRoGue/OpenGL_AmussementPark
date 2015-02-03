#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <GL/gl.h>
#include "Object3D.h"
#include <vector>

namespace GraphicEngine {
    class CollisionObject: public Object3DInstance {
    private:
        std::vector<CollisionObject*> knownList;
    public:
        CollisionObject(DWORD32 id, Object3D *o);
        void add_to_knownlist(CollisionObject *);
        bool check_collisions(GLfloat x, GLfloat y);
        virtual bool collides(GLfloat x, GLfloat y) = 0;
    };

    class CircleCollisionObject: public CollisionObject {
    public:
        CircleCollisionObject(DWORD32 id, Object3D *o);
        bool collides(GLfloat x, GLfloat y);
        virtual GLfloat get_center_x() = 0;
        virtual GLfloat get_center_y() = 0;
        virtual GLfloat get_radius() = 0;
    };

    class SquareCollisionObject: public CollisionObject {
    public:
        SquareCollisionObject(DWORD32 id, Object3D *o);
        bool collides(GLfloat x, GLfloat y);
        virtual GLfloat min_x() = 0;
        virtual GLfloat min_y() = 0;
        virtual GLfloat max_x() = 0;
        virtual GLfloat max_y() = 0;
    };
}

#endif // COLLISIONS_H
