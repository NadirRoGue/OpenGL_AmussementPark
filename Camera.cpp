#include <cmath>
#include <GL/glu.h>
#include "Camera.h"

using namespace GraphicEngine;

Camera::Camera() {
}

void Camera::attach_to_object(CameraInterface *ci) {
    obj = ci;
    dst_x = dst_y = dst_z = 0;
}

void Camera::update_dst_2D() {
    GLfloat radians = (M_PI/180.0) * obj->get_colatitude_angle();

    dst_x = sin(radians) * obj->get_vision_radius() + obj->get_src_x();
    dst_z = cos(radians) * obj->get_vision_radius() + obj->get_src_z();
    dst_y = obj->get_src_y();
}

void Camera::update_dst_3D() {
    GLfloat rad_aci = (M_PI/180.0) * obj->get_acimut();
    GLfloat radians = (M_PI/180.0) * obj->get_colatitude_angle();

    dst_x = sin(radians) * obj->get_vision_radius() /*+ obj->get_src_x()*/;
    dst_z = cos(radians) * obj->get_vision_radius() /*+ obj->get_src_y()*/;
    dst_y = cos(rad_aci) * obj->get_vision_radius() /*+ obj->get_src_z()*/;
}

void Camera::project() {
    gluLookAt(obj->get_src_x(), obj->get_src_y(), obj->get_src_z(),
              dst_x, dst_y, dst_z,
              0, 1, 0);
}

// =============================================================

CameraInterface::CameraInterface() {

}

GLfloat CameraInterface::get_acimut() {
    return 0;
}

// =============================================================

DefaultCameraObject::DefaultCameraObject(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, GLfloat aci, GLfloat col) {
    this->x = x;
    this->y = y;
    this->z = z;
    radius = rad;
    acimut = aci;
    colatitude = col;
    xLastData = yLastData = 0;
}

void DefaultCameraObject::update_angles(int xData, int yData, bool updateViewAngles) {
    if(updateViewAngles) {
        int dX = (xData - xLastData);
        int dY = (yData - yLastData);
        acimut += dY;
        colatitude += dX;
    }

    xLastData = xData;
    yLastData = yData;
}

void DefaultCameraObject::update_x(GLfloat x) { this->x += x; }
void DefaultCameraObject::update_y(GLfloat y) { this->y += y; }
void DefaultCameraObject::update_z(GLfloat z) { this->z += z; }
GLfloat DefaultCameraObject::get_src_x() { return x; }
GLfloat DefaultCameraObject::get_src_y() { return y; }
GLfloat DefaultCameraObject::get_src_z() { return z; }
GLfloat DefaultCameraObject::get_colatitude_angle() { return colatitude; }
GLfloat DefaultCameraObject::get_acimut() { return acimut; }
GLfloat DefaultCameraObject::get_vision_radius() { return radius; }
