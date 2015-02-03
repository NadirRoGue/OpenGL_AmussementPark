#ifndef MESHTABLE_H
#define MESHTABLE_H

#include <map>
#include <Object3D.h>
#include <string>

namespace GraphicEngine {
    class MeshTable {
    private:
        std::map<std::string, GraphicEngine::Object3D*> meshes;

    public:
        static MeshTable *INSTANCE;
        MeshTable();
        void add_mesh(std::string name, Object3D *);
        Object3D * get_mesh(std::string name);
        static MeshTable & get_instance();
    };
}

#endif // MESHTABLE_H
