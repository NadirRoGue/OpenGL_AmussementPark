#ifndef PARKOBJECTS_H
#define PARKOBJECTS_H

#include "Object3D.h"
#include "Collisions.h"
#include "Material.h"
#include "IDFactory.h"
#include "Light.h"
#include "Camera.h"
#include <map>

namespace AmussementPark {

    class FloorTile: public GraphicEngine::Object3D {
    public:
        FloorTile();
        void initialize();
        DWORD get_model_height();
        DWORD get_model_width();
    };

    //=============================================================================
    //=============================================================================
    class CarrouselRoof: public GraphicEngine::Object3DRevolution {
    public:
        CarrouselRoof();
        void initialize();
    };

    class CarrouselAxis: public GraphicEngine::Object3DRevolution {
    public:
        CarrouselAxis();
        void initialize();
    };

    class CarrouselPlatform: public GraphicEngine::Object3DRevolution {
    public:
        CarrouselPlatform();
        void initialize();
    };

    class CarrouselBase: public GraphicEngine::Object3DRevolution {
    public:
        CarrouselBase();
        void initialize();
    };

    class HobbyHorseInstance: public GraphicEngine::Hierarchy {
    private:
        GLfloat curPositionInPole; // y axis value
        GraphicEngine::Object3DInstance *hobbyHorse;
        GraphicEngine::Object3DInstance *pole;
        bool goingUp;
    public:
        HobbyHorseInstance(GLuint id, GLfloat *difuse_horse, GLfloat *difuse_pole, bool gUp);
        void draw();
        void draw_to_identify();
        void tick();
    };

    class Carrousel: public GraphicEngine::Hierarchy {
    private:
        GLfloat angleStep, angle;
        std::vector<HobbyHorseInstance> horses;
        double horsePosAngle;
        int horseCount;
        GraphicEngine::Object3DInstance *roof, *axis, *plat, *base;
    public:
        Carrousel(GLuint id, GLfloat x=0, GLfloat y=0, GLfloat defaultAngleStep=1, int horses=6);
        void draw();
        void draw_to_identify();
        void handle_key(int key);
        void tick();
    };

    //=============================================================================
    //=============================================================================
    class ThreadmilPlatform: public GraphicEngine::Object3DRevolution {
    public:
        ThreadmilPlatform();
        void initialize();
    };

    class ThreadmilBase: public GraphicEngine::Object3D {
    public:
        ThreadmilBase();
        void initialize();
    };

    class ThreadMilStructPart: public GraphicEngine::Object3DRevolution {
    public:
        ThreadMilStructPart();
        void initialize();
    };

    class ThreadMilRadius: public GraphicEngine::Object3DRevolution {
    public:
        ThreadMilRadius();
        void initialize();
    };

    class ThreadMilWagon: public GraphicEngine::Object3D {
    public:
        ThreadMilWagon();
        void initialize();
    };

    class ThreadMil: public GraphicEngine::Hierarchy {
    private:
        std::vector<GraphicEngine::Object3DInstance> wagons;
        int radiusCount;
        GraphicEngine::Object3DInstance *plat;
        GraphicEngine::Object3DInstance *base;
        GraphicEngine::Object3DInstance *parts;
        GraphicEngine::Object3DInstance *radius;
        GLfloat twistAngle;
        GLfloat incr;
        void draw_struct_part(GLfloat offset);
    public:
        ThreadMil(GLuint id, GLfloat x, GLfloat y, int radius, int wagonCount);
        void draw();
        void draw_to_identify();
        void tick();
        void handle_key(int key);
    };

    //=============================================================================
    //=============================================================================
    class BumperCarsStructRoof: public GraphicEngine::Object3D {
    private:
        GLfloat bWidth, bHeight;
    public:
        BumperCarsStructRoof(GLfloat width, GLfloat height);
        void initialize();
    };

    class BumperCarsStructColumn: public GraphicEngine::Object3DRevolution {
    public:
        BumperCarsStructColumn();
        void initialize();
    };

    class BumperCarsStructBase: public GraphicEngine::Object3D {
    private:
        GLfloat bWidth, bHeight;
    public:
        BumperCarsStructBase(GLfloat width, GLfloat height);
        void initialize();
    };

    class BumperCarsBaseBorder: public GraphicEngine::Object3D {
    public:
        BumperCarsBaseBorder();
        void initialize();
    };

    class CollidableCar: public GraphicEngine::CircleCollisionObject {
    private:
        GLfloat x,y,radius;
    public:
        CollidableCar(GLuint id,  GraphicEngine::Object3D *obj, GLfloat radius);
        void update(GLfloat newX, GLfloat newY);
        GLfloat get_center_x();
        GLfloat get_center_y();
        GLfloat get_radius();
    };

    class CarCamera: public GraphicEngine::CameraInterface {
    private:
        GLfloat xPos, yPos, zPos;
        GLfloat heading;
    public:
        void set_x(GLfloat x);
        void set_y(GLfloat y);
        void set_z(GLfloat z);
        void set_heading(GLfloat heading);
        GLfloat get_src_x();
        GLfloat get_src_y();
        GLfloat get_src_z();
        GLfloat get_colatitude_angle();
        GLfloat get_vision_radius();
    };

    class BumperCarInstance: public GraphicEngine::Hierarchy {
    private:
        GLfloat xPos, yPos;
        GLfloat heading;
        GLfloat speed;
        CollidableCar *bumperCar;
        GraphicEngine::Object3DInstance *pole;
        CarCamera *attachedCam;
        void rotate();
    public:
        BumperCarInstance(GLuint id,GraphicEngine::Material *m);
        void draw();
        void draw_to_identify();
        void tick();
        void set_pos(GLfloat x, GLfloat y, GLfloat z, GLfloat heading);
        void speedUp();
        void slowDown();
        GLfloat get_heading();
        GLfloat get_x();
        GLfloat get_y();
        void update_knownlist(GraphicEngine::CollisionObject *obj);
        void update_knownlist(BumperCarInstance &o);
        void handle_key(int key);
        CarCamera * get_cam();
    };

    class BorderInstance: public GraphicEngine::SquareCollisionObject {
    private:
        GLfloat minX, minY, maxX, maxY;
    public:
        BorderInstance(GLuint id, GraphicEngine::Object3D *obj, GLfloat xmin, GLfloat ymin, GLfloat xmax, GLfloat ymax);
        void update(GLfloat xt, GLfloat yt);
        GLfloat min_x();
        GLfloat min_y();
        GLfloat max_x();
        GLfloat max_y();
    };

    class BumperCarBuild: public GraphicEngine::Hierarchy {
    private:
        std::vector<BumperCarInstance> cars;
        int carCount;
        GraphicEngine::Object3DInstance *buildBase;
        GraphicEngine::Object3DInstance *column;
        GraphicEngine::Object3DInstance *buildRoof;
        GraphicEngine::Object3DInstance *lightSources;
        std::vector<BorderInstance> borders;
        GLfloat bWidth;
        GLfloat bHeight;
        GraphicEngine::LightSource *ilum;
    public:
        BumperCarBuild(GLuint id, GLfloat widht, GLfloat height, int cars, GLfloat x, GLfloat y,
                       std::vector<GraphicEngine::CameraInterface*> &cams);
        void draw();
        void draw_to_identify();
        void handle_key(int key);
        void tick();
    };

    //=============================================================================
    //=============================================================================

    class World {
    private:
        std::map<GLuint, GraphicEngine::Hierarchy*> selectable;
        GraphicEngine::Hierarchy *selectedObj;

        std::vector<GraphicEngine::LightSource> lights;

        std::vector<GraphicEngine::CameraInterface*> cameras;
        GraphicEngine::DefaultCameraObject *defaultCamera;
        unsigned char selectedCam;
        GraphicEngine::Camera camera;
        bool rightClick;

        GraphicEngine::Object3DInstance *floor;

        void register_clickable_object(GraphicEngine::Hierarchy *obj);
        void initialize();
    public:
        static World *INSTANCE;
        static World & get_instance();
        World();
        void handle_key(int key);
        void handle_click(int x, int y, bool rightClick, GLuint hits, GLuint *selectB);
        void handle_release_click(bool rightClick);
        void handle_move(int x, int y);
        void handle_wheel(int delta);
        void register_light(GraphicEngine::LightSource &);
        void register_camera(GraphicEngine::CameraInterface *cam);
        void render_world();
        void identify_world();
        void tick_timer();
    };
}

#endif // PARKOBJECTS_H
