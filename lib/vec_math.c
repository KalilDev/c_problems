#include "vec_math.h"
#include <math.h>

const vec2f vec2f_zero = {0.0, 0.0};
const mat2f mat2f_zero = {{{0.0, 0.0}, {0.0, 0.0}}};
const mat2f mat2f_identity = {{{1.0, 0.0}, {0.0, 1.0}}};

float vec2f_distance_squared(vec2f v) {
    float x = v.x;
    float y = v.y;
    return (x*x) + (y*y);
}

float vec2f_distance(vec2f v) {
    float d2 = vec2f_distance_squared(v);
    return sqrtf(d2);
}

vec2f vec2f_sub(vec2f a, vec2f b) {
    vec2f r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

vec2f vec2f_add(vec2f a, vec2f b) {
    vec2f r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

vec2f vec2f_from_coords(float coords[2]) {
    vec2f r = {
        .x = coords[0],
        .y = coords[1]
    };
    return r;
}
