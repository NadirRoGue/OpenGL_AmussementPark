#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Texture.h"
#include "Vertex.h"
#include "BMPImage.h"
#include <iostream>

using namespace std;
using namespace GraphicEngine;

Texture::Texture(string file) {

    src = new BMPImage(file);

    for(int i = 0; i < src->get_width() * src->get_height() ; i++)
    {
      int index = i*3;
      unsigned char B,R;
      B = src->get_texture()[index];
      R = src->get_texture()[index+2];

      src->get_texture()[index] = R;
      src->get_texture()[index+2] = B;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D,textureId);

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, src->get_width(), src->get_height(),GL_RGB, GL_UNSIGNED_BYTE, src->get_texture());
}

void Texture::pre_render() {
    glBindTexture(GL_TEXTURE_2D,textureId);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glTexCoordPointer(2,GL_FLOAT,0,texCoords);
}

void Texture::post_render() {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Texture::calc(GLfloat *vertices, DWORD height, DWORD width) {

    GLfloat temp [] = {
        0,0,
        0,1,
        1,1,
        1,0
    };

    texCoords = new GLfloat[8];
    memcpy(texCoords,temp,8*sizeof(GLfloat));
}

GLfloat * Texture::img() {
    return texCoords;
}
