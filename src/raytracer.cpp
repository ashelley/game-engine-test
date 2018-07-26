
#include "global_defines.h"
#include "random.h"
#include "camera.h"
#include "sphere.h"
#include "hitable_list.h"

#if 0
#define drand48() (rand() / (RAND_MAX + 1.0))
#endif
#if 1
#define drand48() (real32)RandomNextUInt32(&GlobalRandomSeries)/(1.0+(real32)MaxRandomNumber)
#endif

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

float HitSphere(vec3 center, float radius, ray& r) {
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

vec3 RandomPointInUnitSphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    } while(p.squared_length() > 1.0);
    return p;
}

vec3 SampleSkyBox(ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

vec3 SampleFlatSingleSphere(ray& r) {
    float t = HitSphere(vec3(0,0,-1), 0.5, r);
    if(t > 0.0) {
        return vec3(1.0,0.0,0.0);
        // vec3 normal = unit_vector(r.point_at(t) - vec3(0,0,-1));
        // return 0.5*vec3(normal.x()+ 1, normal.y() + 1, normal.z() + 1.0);
        //return vec3(1.0,0.0,0.0);
    }
    return SampleSkyBox(r);
}

vec3 SampleNormals(ray& r, hitable *world) {
    hit_record rec;
    if(world->hit(r, 0.0, FLT_MAX, rec)) {
        return 0.5*vec3(rec.normal.x()+1,rec.normal.y()+1,rec.normal.z()+1);
    }
    return SampleSkyBox(r);
}

vec3 SampleLitSurface(ray r, hitable *world) {
    hit_record rec;
    if(world->hit(r, 0.0, FLT_MAX, rec)) {
        vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
        return 0.5*SampleLitSurface(ray(rec.p, target - rec.p), world);
    }
    return SampleSkyBox(r);
}

vec3 LinearToSRGB(vec3 color) {
    return vec3(sqrt(color.e[0]), sqrt(color.e[1]), sqrt(color.e[2]));
}

int main() {
    int nx = 800;
    int ny = 400;
    int ns = 100;
    
    // vec3 bottomLeft(-2.0, -1.0, -1.0);
    // vec3 horizontal(4.0, 0.0, 0.0);
    // vec3 vertical(0.0, 2.0, 0.0);
    // vec3 origin(0.0, 0.0, 0.0);
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), (float)0.5);
    list[1] = new sphere(vec3(0,-100.5,-1), (float)100.0);

    hitable *world = new hitable_list(list, 2);
    camera cam;

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 color(0,0,0);
            for(int s = 0; s < ns; s++) {
                // float u = (float)i / (float)nx;                
                // float v = (float)j / (float)ny;
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);

                //ray r(origin, bottomLeft + u*horizontal + v*vertical);
                //vec3 color = Sample(r);
                ray r = cam.get_ray(u, v);
                #if 0
                    
                    color += SampleFlatSingleSphere(r);
                    color += SampleNormals(r, world);
                #else
                    color += SampleLitSurface(r, world);
                #endif
            }

            color /= float(ns);

            color = LinearToSRGB(color);            

            int ir = (int)255.99*color[0];
            int ig = (int)255.99*color[1];
            int ib = (int)255.99*color[2];
            
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}

