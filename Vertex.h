#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>
#include <GL/gl.h>
#include <iostream>

namespace GraphicEngine {
    template<class T>
    class Vertex {
    private:
        T x;
        T y;
        T z;
    public:
        Vertex(T a, T b, T c) {
            x = a;
            y = b;
            z = c;
        }

        T get_x() {
            return x;
        }

        T get_y() {
            return y;
        }

        T get_z() {
            return z;
        }

        // vectorial product
        Vertex<T> operator * (const Vertex<T> &other) {
            T newX = ((y * other.z) - (z * other.y));
            T newY = ((z * other.x) - (x * other.z));
            T newZ = ((x * other.y) - (y * other.x));
            Vertex<T> p (newX, newY, newZ);
            return p;
        }

        double module() {
            return sqrt((x*x)+(y*y)+(z*z));
        }

        void normailize() {
            double mod = module();
            x /= mod;
            y /= mod;
            z /= mod;
        }
    };

    typedef Vertex<double> vertexf;
    typedef Vertex<unsigned long> vertexi;
}

#endif // VERTEX_H
