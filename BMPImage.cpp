#include "BMPImage.h"
#include "Debug.h"

#include <stdio.h>
#include <iostream>

using namespace GraphicEngine;
using namespace std;

BMPImage::BMPImage() {
    width = height = 0;
}

BMPImage::BMPImage(string & file) {
    read(file);
}

void BMPImage::read(string & file) {
    FILE *data = fopen(file.c_str(), "rb");
    if(data == 0) {
        cerr << "BMPImage::read(): could not load " << file << endl;
        return;
    }

    unsigned char header[54];
    if(fread(header,1,54,data) < 54) {
        cerr << "BMPImage::read(): could not read texture header" << endl;
        return;
    }

    width = *(int*)(header + 18);
    height = *(int*)(header + 22);
    int size = *(int*)(header + 34);

    if(size <= 0) {
        cerr << "BMPImage::read() image header corrupted. Assuming image size" << endl;
        size = width * height * 3 /* * sizeof(unsigned char)*/;
    }

    texture = new unsigned char[size];

    #ifdef DEBUG
        int readSize = fread(texture,1,size,data);
        int err;
        if(feof(data) != 0 && readSize < size)
            cerr << "eof set before read the whole texture" << endl;
        if((err = ferror(data)) != 0)
            cerr << "err set: " << err << endl;
        assert(readSize == size);
    #else
        fread(texture,1,size,data);
    #endif

    fclose(data);
}

int BMPImage::get_height() {
    return height;
}

int BMPImage::get_width() {
    return width;
}

unsigned char * BMPImage::get_texture() {
    return texture;
}
