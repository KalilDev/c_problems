#ifndef K_VEC_MATH_H
#define K_VEC_MATH_H

typedef struct {
    float coords[2][2];
} mat2f;

typedef struct {
    float x;
    float y;
} vec2f;

float vec2f_distance_squared(vec2f v);
float vec2f_distance(vec2f v);

vec2f vec2f_sub(vec2f a, vec2f b);

vec2f vec2f_add(vec2f a, vec2f b);

vec2f vec2f_from_coords(float coords[2]);
#endif
