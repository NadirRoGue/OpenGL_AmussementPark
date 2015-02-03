#include <map>
#include <string>
#include <cmath>
#include "Vertex.h"
#include "Object3D.h"
#include "PlyFile.h"

#include <iostream>


using namespace GraphicEngine;
using namespace std;

void msg(char *msg) {
    cout << msg << endl;
}

void save_vector_product(map<GLuint, vector<GLuint> > &myMap, GLuint key, GLuint face) {
    map<GLuint, vector<GLuint> >::iterator it = myMap.find(key);
    if(it == myMap.end()) {
        vector<GLuint> normals;
        normals.push_back(face);
        myMap[key] = normals;
    } else {
        vector<GLuint> v = it->second;
        v.push_back(face);
        myMap[key] = v;
    }
}

// ============================ Object3D ===========================================

Object3D::Object3D() {
}

void Object3D::calc_normal() {
    normals = new GLfloat[verticesSize];

    double v1x, v1y, v1z;
    double v2x, v2y, v2z;
    double v3x, v3y, v3z;
    GLuint f1, f2, f3;

    map<GLuint, vector<GLuint> > verticesData;
    vector<vertexf> faceNormal;
    GLuint faceNormalWrt = 0;

    for(DWORD i = 0; i < facesSize; i = i+3) {
        f1 = faces[i];
        f2 = faces[i+1];
        f3 = faces[i+2];

        v1x = vertices[f1*3]; v1y = vertices[(f1*3)+1]; v1z = vertices[(f1*3)+2];
        v2x = vertices[f2*3]; v2y = vertices[(f2*3)+1]; v2z = vertices[(f2*3)+2];
        v3x = vertices[f3*3]; v3y = vertices[(f3*3)+1]; v3z = vertices[(f3*3)+2];

        vertexf BA (v2x - v1x, v2y - v1y, v2z - v1z);
        vertexf CB (v2x - v3x, v2y - v3y, v2z - v3z);

        vertexf product = BA * CB;
        product.normailize();

        faceNormal.push_back(product);
        save_vector_product(verticesData,f1,faceNormalWrt);
        save_vector_product(verticesData,f2,faceNormalWrt);
        save_vector_product(verticesData,f3,faceNormalWrt);
        faceNormalWrt++;
    }

    DWORD size = verticesSize / 3;
    for(DWORD i = 0; i < size; i++) {
        vector<GLuint> vlist = verticesData.find(i)->second;
        GLfloat x = 0, y = 0, z = 0;
        DWORD j = 0;
        while(j < vlist.size()) {
            vertexf temp = faceNormal[vlist[j]];
            x += temp.get_x();
            y += temp.get_y();
            z += temp.get_z();
            j++;
        }

        vertexf final(x,y,z);
        final.normailize();
        DWORD index = i * 3;
        normals[index] = final.get_x();
        normals[index+1] = final.get_y();
        normals[index+2] = final.get_z();
    }
}

void Object3D::draw_me(GLenum pMode, GLenum dMode) {
    glPolygonMode(pMode, dMode);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,normals);
    glDrawElements(GL_TRIANGLES,facesSize, GL_UNSIGNED_INT, faces);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

DWORD Object3D::get_model_height() {
    return 0;
}

DWORD Object3D::get_model_width() {
    return 0;
}

GLfloat * Object3D::get_vertices() {
    return vertices;
}

// ============================ Object3DPly ========================================

Object3DPly::Object3DPly(string file):fileName(file) {
    initialize();
    calc_normal();
}

void Object3DPly::initialize() {
    PlyReader<GLfloat, GLuint> loader (fileName);
    verticesSize = loader.get_vertices_size();
    vertices = new GLfloat[verticesSize];
    loader.dump_vertices(vertices);
    facesSize = loader.get_face_size();
    faces = new GLuint[facesSize];
    loader.dump_faces(faces);
}

// ============================ Object3DRevolution ========================================

Object3DRevolution::Object3DRevolution(int revs, bool topBottom):revolutions(revs),
    topBottomClose(topBottom) {
}

void Object3DRevolution::replicate(vector<vertexf> &base) {
    vector<GLfloat> rawV;
    for(DWORD k = 0; k < base.size(); k++) {
        vertexf v = base[k];
        rawV.push_back(v.get_x());
        rawV.push_back(v.get_y());
        rawV.push_back(v.get_z());
    }

    int i = 1;
    double angleStep = (2*M_PI) / revolutions;
    DWORD profileSize = rawV.size();
    GLfloat oldX, oldZ;

    while(i < revolutions) {
        for(DWORD j = 0; j < profileSize; j = j+3) {
            DWORD incr = profileSize * (i-1);
            oldX = rawV[incr+j]; oldZ = rawV[incr+(j+2)];
            GLfloat newX, newZ;
            rotate_coords(oldX,oldZ,angleStep,&newX,&newZ);
            rawV.push_back(newX);
            rawV.push_back(rawV[incr+j+1]);
            rawV.push_back(newZ);
        }
        i++;
    }

    vector<GLuint> faces;

    build_faces(faces, profileSize, rawV.size());

    if(topBottomClose) {
        GLfloat yMaxRef = 0, yMinRef = 0;
        GLfloat yPivot;
        for(DWORD j = 0; j < profileSize; j=j+3) {
            yPivot = rawV[j+1];
            if(yPivot >= yMaxRef)
                yMaxRef = yPivot;

            if(yPivot <= yMinRef)
                yMinRef = yPivot;
        }

        rawV.push_back(0);rawV.push_back(yMaxRef);rawV.push_back(0);
        rawV.push_back(0);rawV.push_back(yMinRef);rawV.push_back(0);
        build_face_top_bottom(faces, profileSize, (DWORD)rawV.size(), rawV[1], rawV[profileSize-2]);
    }

    // Store data
    verticesSize = rawV.size();
    vertices = new GLfloat[verticesSize];
    memcpy(vertices, &rawV[0], verticesSize * sizeof(GLfloat));
    facesSize = faces.size();
    this->faces = new GLuint[facesSize];
    memcpy(this->faces, &faces[0], facesSize * sizeof(GLuint));
}

void Object3DRevolution::rotate_coords(GLfloat src_x, GLfloat src_y, double angle, GLfloat *x, GLfloat *y) {
    *x = (src_x * cos(angle)) + (src_y * sin(angle));
    *y = (-src_x * sin(angle)) + (src_y * cos(angle));
}

void Object3DRevolution::build_faces(vector<GLuint> &faceList, DWORD profSize, DWORD vSize) {

    const DWORD profileSize = profSize / 3;
    const DWORD profSizeIterator = profileSize - 1;
    const DWORD realVSize = vSize / 3;
    GLuint a;
    GLuint b;
    GLuint c;
    GLuint d;

    /*

    c*----*a
     |\   |
     | \  |
     |  \ |
    d*----*b

    */

    for(int i = 0; i < revolutions; i++) {
        for(DWORD j = 0; j < profSizeIterator; j++){
            a = j + (i*profileSize);
            b = a + 1;
            c = (a + profileSize) % realVSize;
            d = c + 1;

            faceList.push_back(a);faceList.push_back(c);faceList.push_back(b);
            faceList.push_back(b);faceList.push_back(c);faceList.push_back(d);
        }
    }
}

void Object3DRevolution::build_face_top_bottom(vector<GLuint> &faceList, DWORD profSize, DWORD vSize,
                                               GLfloat y0, GLfloat yN) {

    DWORD profIterator = profSize / 3;
    DWORD topIndex = 0, lowerIndex = 0;
    if(y0 > yN)
        lowerIndex = profIterator - 1;
    else
        topIndex = profIterator - 1;

    DWORD profStep;
    DWORD lastIndices = vSize / 3;
    //top faces
    for(int i = 0; i < revolutions; i++) {
        profStep = i*profIterator;
        GLuint a;
        GLuint b;
        GLuint c;
        GLuint d;

        a = profStep + topIndex;
        b = (a + profIterator) % (lastIndices - 2);
        faceList.push_back(a);faceList.push_back(lastIndices-2);faceList.push_back(b);

        c = profStep + lowerIndex;
        d = (c + profIterator) % (lastIndices - 2);
        faceList.push_back(c);faceList.push_back(lastIndices-1);faceList.push_back(d);
    }
}


// ============================ Object3DInstance ===================================
Object3DInstance::Object3DInstance(GLuint id, Object3D *o) {
   // memcpy(colorId,id,4*sizeof(GLfloat));
    obj = o;
    mat = NULL;
}

bool Object3DInstance::operator == (const Object3DInstance &other) {
    return id == other.id;
}

bool Object3DInstance::operator == (const GLuint id) {
    return this->id == id;
}

void Object3DInstance::handle_key(int key) {
    cout << "No '" << (char)key << "'' handler implemented yet." << endl;
}

void Object3DInstance::set_material(Material *m) {
    if(m == NULL)
        mat = new Material();
    else {
        /*if(mat != NULL)
            free(mat);*/
        mat = new Material(m);
    }
}

Material * Object3DInstance::get_material() {
    return mat;
}

void Object3DInstance::draw() {
    mat->apply_me();
    obj->draw_me();
}

void Object3DInstance::draw_to_identify() {
    glLoadName(id);
    draw();
}

// ============================ Object3DTexturedInstance ==========================
Object3DTexturedInstance::Object3DTexturedInstance(GLuint id, Object3D *o, string texture):
    Object3DInstance(id,o) {
    text = new Texture(texture);
    text->calc(o->get_vertices(),o->get_model_height(),o->get_model_width());
}

void Object3DTexturedInstance::draw() {
    text->pre_render();
    Object3DInstance::draw();
    text->post_render();
}

//============================================================
Hierarchy::Hierarchy(GLuint id, GLfloat xPos, GLfloat yPos):
    id(id),x(xPos),y(yPos){
}

void Hierarchy::tick() {
}

bool Hierarchy::check_collisions() {
    return false;
}

void Hierarchy::handle_key(int key) {
    cout << "No handler for " << (char)key << " registered" << endl;
}
