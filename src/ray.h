#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:        
        ray() {}
        ray(vec3& a, vec3& b) {
            A = a;
            B = b;
        }
        vec3 origin() {
            return A;
        }
        vec3 direction() {
            return B;
        }
        vec3 point_at(float distance) {
            return A + distance*B;
        }

        vec3 A;
        vec3 B;        
};

#endif