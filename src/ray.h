#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:        
        ray() {}
        ray(const vec3& a, const vec3& b) {
            A = a;
            B = b;
        }
        vec3 origin() const {
            return A;
        }
        vec3 direction() const {
            return B;
        }
        vec3 point_at(float distance) const {
            return A + distance*B;
        }

        vec3 A;
        vec3 B;        
};

#endif