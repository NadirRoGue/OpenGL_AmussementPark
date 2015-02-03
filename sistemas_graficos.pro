# -------------------------------------------------
# Project created by QtCreator 2014-03-02T13:23:20
# -------------------------------------------------
QT += core \
    gui \
    opengl
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
TARGET = sistemas_graficos
TEMPLATE = app
SOURCES += main.cpp \
    BMPImage.cpp \
    Light.cpp \
    Material.cpp \
    Object3D.cpp \
    OGLWindow.cpp \
    IDFactory.cpp \
    Carrousel.cpp \
    Threadmil.cpp \
    World.cpp \
    MeshTable.cpp \
    Texture.cpp \
    BumperCars.cpp \
    Collisions.cpp \
    Camera.cpp
HEADERS += Object3D.h \
    Material.h \
    Light.h \
    PlyFile.h \
    BMPImage.h \
    Vertex.h \
    ParkObjects.h \
    Debug.h \
    Callbacks.h \
    OGLWindow.h \
    IDFactory.h \
    MeshTable.h \
    Texture.h \
    Collisions.h \
    Camera.h
FORMS += main.ui
