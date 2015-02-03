#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <string>

namespace GraphicEngine {

    class BMPImage {
    private:
        int width;
        int height;
        unsigned char *texture;
    public:
        BMPImage();
        BMPImage(std::string &file);
        void read(std::string &file);
        unsigned char * get_texture();
        int get_width();
        int get_height();
    };
}

#endif // BMPIMAGE_H
