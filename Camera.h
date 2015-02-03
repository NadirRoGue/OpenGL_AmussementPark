#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>

namespace GraphicEngine {

    class CameraInterface {
    public:
        CameraInterface();
        virtual GLfloat get_src_x() = 0;
        virtual GLfloat get_src_y() = 0;
        virtual GLfloat get_src_z() = 0;
        virtual GLfloat get_vision_radius() = 0;
        virtual GLfloat get_colatitude_angle() = 0;
        virtual GLfloat get_acimut();
    };

    class Camera {
    private:
        CameraInterface *obj;
        GLfloat dst_x, dst_y, dst_z;
    public:
        Camera();
        void attach_to_object(CameraInterface *ci);
        void update_dst_2D();
        void update_dst_3D();
        void project();
    };

    class DefaultCameraObject: public CameraInterface {
    private:
        GLfloat x,y,z;
        int xLastData, yLastData;
        GLfloat radius;
        GLfloat acimut;
        GLfloat colatitude;
    public:
        DefaultCameraObject(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, GLfloat aci, GLfloat col);
        void update_x(GLfloat x);
        void update_y(GLfloat y);
        void update_z(GLfloat z);
        void update_angles(int xData, int yData, bool updateViewAngles);
        GLfloat get_src_x();
        GLfloat get_src_y();
        GLfloat get_src_z();
        GLfloat get_vision_radius();
        GLfloat get_colatitude_angle();
        GLfloat get_acimut();
    };
}

#endif // CAMERA_H
