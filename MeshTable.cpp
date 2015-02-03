#include <map>
#include "MeshTable.h"
#include "Object3D.h"

using namespace std;
using namespace GraphicEngine;

MeshTable* MeshTable::INSTANCE = 0;

MeshTable::MeshTable() {
    meshes[string("coche")] = new Object3DPly("meshes/coche.ply");
    meshes[string("caballito")] = new Object3DPly("meshes/caballito_carrusel.ply");
    meshes[string("barra")] = new Object3DPly("meshes/barra_carrusel.ply");
}

void MeshTable::add_mesh(string name, Object3D *o) {
    meshes[name] = o;
}

Object3D * MeshTable::get_mesh(string name) {
    map<string, Object3D*>::iterator it = meshes.find(name);
    if(it == meshes.end())
        return 0;
    return it->second;
}

MeshTable & MeshTable::get_instance() {
    if(MeshTable::INSTANCE == 0)
        MeshTable::INSTANCE = new MeshTable();

    return *(MeshTable::INSTANCE);
}
