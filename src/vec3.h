#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
    public:     
        vec3() {}
        vec3(float e0, float e1, float e2) {
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;           
        }

        float e[3];

        inline float x() { return e[0];}
        inline float y() { return e[1];}
        inline float z() { return e[2];}
        inline float r() { return e[0];}
        inline float g() { return e[1];}
        inline float b() { return e[2];}

        inline vec3& operator+() { return *this; }
        inline vec3 operator-() { return vec3(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) { return e[i]; }
        //inline float& operator[](int i) { return e[i]; }
        inline vec3& operator+=(vec3 &v);
        inline vec3& operator-=(vec3 &v);
        inline vec3& operator*=(vec3 &v);
        inline vec3& operator/=(vec3 &v);

        inline vec3& operator*=(float scalar);
        inline vec3& operator/=(float scalar);

        inline float length() {
            return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
        }
        inline float squared_length() {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }        

};
