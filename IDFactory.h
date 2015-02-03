#ifndef IDFACTORY_H
#define IDFACTORY_H

#include <queue>
#include <GL/gl.h>

namespace GraphicEngine {

    class IdFactory {
    private:
        GLuint nextId;
        std::queue<GLuint> freeIds;
    public:
        static IdFactory *INSTANCE;
        static IdFactory & get_instance();

        IdFactory();
        GLuint get_next_id();
        void release_id(GLuint id);
    };
}

#endif // IDFACTORY_H
