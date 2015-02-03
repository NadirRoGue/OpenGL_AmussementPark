#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/gl.h>
#include "BMPImage.h"

namespace GraphicEngine {

    typedef unsigned long DWORD;
    typedef unsigned char UCHAR;

    class Texture {
    private:
        GLfloat *texCoords;
        GLuint textureId;
        BMPImage *src;
    public:
        Texture(std::string file);
        void pre_render();
        void post_render();
        void calc(GLfloat *vertices, DWORD height, DWORD width);
        GLfloat * img();
    };
}

#endif // TEXTURE_H
