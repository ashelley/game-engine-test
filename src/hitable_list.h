#include "hitable.h"

class hitable_list: public hitable {
    public: 
        hitable_list() {

        };

        hitable_list(hitable **l, int listCount) {
            List = l;
            ListCount = listCount;
        }

        virtual bool hit(ray& r, float t_min, float t_max, hit_record& rec);

        hitable **List;
        int ListCount;
};

bool hitable_list::hit(ray& r, float tmin, float tmax, hit_record& rec) {
    hit_record temp_rec;
    bool hit_anything = false;
    double closesthit = tmax;
    for(int i = 0; i < ListCount; i++) {
        if(List[i]->hit(r, tmin, closesthit, temp_rec)) {
            hit_anything = true;
            closesthit = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
};