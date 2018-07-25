#include <iostream>
#include <float.h>

#include "sphere.h"
#include "hitable_list.h"

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

float HitSphere(vec3& center, float radius, ray& r) {
    vec3 distance = r.origin() - center;

    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(distance, r.direction());
    float c = dot(distance,distance) - radius*radius;
    float discriminant = b*b - 4*a*c;

    if(discriminant < -1.0) {
        return -1.0;
    }
    return (-b - sqrt(discriminant)) / (2.0*a);
}

vec3 Sample(ray& r) {
    float t = HitSphere(vec3(0,0,-1), 0.5, r);
    if(t > 0.0) {
        vec3 normal = unit_vector(r.point_at(t) - vec3(0,0,-1));
        return 0.5*vec3(normal.x()+ 1, normal.y() + 1, normal.z() + 1.0);
        //return vec3(1.0,0.0,0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

vec3 Sample(ray&r, hitable *world) {
    hit_record rec;
    if(world->hit(r, 0.0, FLT_MAX, rec)) {
        return 0.5*vec3(rec.normal.x()+1,rec.normal.y()+1,rec.normal.z()+1);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    
    vec3 bottomLeft(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), (float)0.5);
    list[1] = new sphere(vec3(0,-100.5,-1), (float)100.0);

    hitable *world = new hitable_list(list, 2);

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = (float)i / (float)nx;
            float v = (float)j / (float)ny;

            ray r(origin, bottomLeft + u*horizontal + v*vertical);

            //vec3 color = Sample(r);
            vec3 color = Sample(r, world);

            int ir = (int)255.99*color[0];
            int ig = (int)255.99*color[1];
            int ib = (int)255.99*color[2];
            
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

