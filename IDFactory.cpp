#include "IDFactory.h"
#include <queue>

using namespace std;
using namespace GraphicEngine;

IdFactory * IdFactory::INSTANCE = 0;

IdFactory::IdFactory() {
    nextId = 0;
}

GLuint IdFactory::get_next_id() {
    if(freeIds.empty()) {
        nextId = nextId + 1;
        return nextId;
    } else {
        GLuint id = freeIds.front();
        freeIds.pop();
        return id;
    }
}

void IdFactory::release_id(GLuint id) {
    freeIds.push(id);
}

IdFactory & IdFactory::get_instance() {
    if(INSTANCE == 0)
        INSTANCE = new IdFactory();

    return *INSTANCE;
}
