/*
 * author: Nadir Román Guerrero
 * e-mail nadir dot ro dot gue at gmail dot com
 */

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/gl.h>
#include <vector>
#include <string>

#include "Material.h"
#include "Texture.h"
#include "Vertex.h"
#include "IDFactory.h"


namespace GraphicEngine {

    typedef unsigned long DWORD;

// ============================ Object3D ===========================================
    class Object3D {
    protected:
        GLfloat *vertices;
        GLfloat *normals;
        GLuint *faces;
        unsigned long facesSize;
        unsigned long verticesSize;

        virtual void initialize() = 0;
        void calc_normal();
    public:
        Object3D();
        void draw_me(GLenum pMode=GL_FRONT_AND_BACK, GLenum dMode=GL_FILL);
        virtual DWORD get_model_height();
        virtual DWORD get_model_width();
        GLfloat * get_vertices();
    };

// ============================ Object3DPly ========================================
    class Object3DPly: public Object3D {
    private:
        std::string fileName;
    protected:
        void initialize();
    public:
        Object3DPly(std::string file);
    };

// ============================ Object3DRevolution ========================================
    class Object3DRevolution: public Object3D {
    private:
        int revolutions;
        bool topBottomClose;
        void rotate_coords(GLfloat src_x, GLfloat src_y, double angle, GLfloat *x, GLfloat *y);
        void build_face_top_bottom(std::vector<GLuint> &faceList, DWORD profSize, DWORD vSize, GLfloat y0, GLfloat yN);
        void build_faces(std::vector<GLuint> &faceList, DWORD profSize, DWORD vSize);
    protected:
        void replicate(std::vector<vertexf> &base);
    public:
        Object3DRevolution(int revs, bool topBottom);
    };

// ============================ Object3DInstance ===================================
    class Object3DInstance {
    private:
        Object3D *obj;
        GLuint id;
        Material *mat;
    public:
        Object3DInstance(GLuint id, Object3D *o);
        bool operator == (const Object3DInstance &);
        bool operator == (const GLuint oId);
        void handle_key(int key);
        virtual void draw();
        void draw_to_identify();
        void set_material(Material *m=new Material());
        Material * get_material();
    }; 

// ============================ Object3DTexturedInstance ==========================
    class Object3DTexturedInstance: public Object3DInstance {
    private:

    public:
        Object3DTexturedInstance(GLuint id, Object3D *o, std::string texture);
        Texture *text;
        void draw();
    };

// ============================ Hierarchy ===================================
    class Hierarchy {
    protected:
        GLuint id;
        GLfloat x, y;
    public:
        Hierarchy(GLuint id, GLfloat xPos=0, GLfloat yPos=0);
        virtual void draw() = 0;
        virtual void draw_to_identify() = 0;
        virtual void tick();
        virtual bool check_collisions();
        virtual void handle_key(int key);
    };
}

#endif // OBJECT3D_H
