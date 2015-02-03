#ifndef PLYFILE_H
#define PLYFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Debug.h"

namespace GraphicEngine {

    using namespace std;

    template<class V, class F>
    class PlyReader {
    private:
        V *vertices;
        F *faces;
        unsigned long verticesSize;
        unsigned long facesSize;
    public:
        PlyReader(string &file) {
            read(file);
        }

        ~PlyReader() {
            free(vertices);
            free(faces);
        }

        void read(string &file){
            ifstream is (file.c_str(), ifstream::in | ifstream::binary);
            if(!is) {
                cerr << "PlyFile::read : Could not open file " << file << endl;
                return;
            }

            char datablock[128];
            char *tok;

            do {
                is.getline(datablock, 128);

                tok = strtok(datablock, " ");
                if(tok == NULL)
                    continue;
                if(strcmp("element", tok) == 0) {
                    tok = strtok(NULL, " ");
                    if(strcmp("vertex", tok) == 0) {
                        tok = strtok(NULL, " ");
                        verticesSize = strtoul(tok, NULL, 0);
                    } else if(strcmp("face", tok) == 0) {
                        tok = strtok(NULL, " ");
                        facesSize = strtoul(tok, NULL, 0);
                    }
                }
            } while(strcmp(tok, "end_header") != 0);

            unsigned long i = 0;
            V vert;
            vector<V> temp_vertices;
            while(i < verticesSize) {
                is.getline(datablock, 128);
                tok = strtok(datablock, " ");
                for(int j = 0; j < 3; j++) {
                   vert = atof(tok);
                   temp_vertices.push_back(vert);
                   tok = strtok(NULL, " ");
                }
                i++;
            }

            #ifdef DEBUG
                assert(temp_vertices.size() / 3 == verticesSize);
            #endif

            verticesSize *= 3;
            vertices = new V[verticesSize];
            memcpy(vertices, &temp_vertices[0], verticesSize * sizeof(V));

            i = 0;
            F fac;
            vector<F> temp_faces;
            int faceSize;
            while(i < facesSize) {
                is.getline(datablock, 128);
                tok = strtok(datablock, " ");
                faceSize = atoi(tok);
                tok = strtok(NULL, " ");
                for(int j = 0; j < faceSize; j++) {
                    fac = strtoul(tok, NULL, 0);
                    temp_faces.push_back(fac);
                    tok = strtok(NULL, " ");
                }
                i++;
            }

            #ifdef DEBUG
                assert(temp_faces.size() / 3 == facesSize);
            #endif

            facesSize *= 3;
            faces = new F[facesSize];
            memcpy(faces, &temp_faces[0], facesSize * sizeof(F));

            is.close();
        }

        unsigned long get_vertices_size() {
            return verticesSize;
        }

        unsigned long get_face_size() {
            return facesSize;
        }

        void dump_vertices(V *dst) {
            memcpy(dst,vertices,verticesSize * sizeof(V));
        }

        void dump_faces(F *dst) {
            memcpy(dst,faces,facesSize * sizeof(F));
        }
    };
}

#endif // PLYFILE_H
