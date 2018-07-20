#include <iostream>
#include "ray.h"

void TestPattern() {
    int nx = 200;
    int ny = 200;
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 color((float)i / (float)nx,(float)j / (float)ny,0.2);

            int ir = (int)255.99*color[0];
            int ig = (int)255.99*color[1];
            int ib = (int)255.99*color[2];
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }    
}

vec3 Sample(ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 200;
    int ny = 100;
    
    vec3 bottomLeft(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = (float)i / (float)nx;
            float v = (float)j / (float)ny;

            ray r(origin, bottomLeft + u*horizontal + v*vertical);
            vec3 color = Sample(r);

            int ir = (int)255.99*color[0];
            int ig = (int)255.99*color[1];
            int ib = (int)255.99*color[2];
            
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

