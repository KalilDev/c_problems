#ifndef K_ARR_MATH_H
#define K_ARR_MATH_H

#include <stddef.h>

int sum_of_ints(int *ints, size_t count);

float sum_of_floats(float *vals, size_t count);

float avg_of_floats(float *vals, size_t count);

float avg_of_ints(int *ints, size_t count);

float sum_of_squares_of_ints(int *vals, size_t count);

float sum_of_squares_of_floats(float *vals, size_t count);

void divide_floats_to(float divisor, float *input, float* output, size_t count);

void divide_floats_to_restrict(float divisor,  float *input, float *output, size_t count);

void multiply_floats_to(float multiplier, float *input, float* output, size_t count);

void multiply_floats_to_restrict(float multiplier,  float *input, float *output, size_t count);

#endif
