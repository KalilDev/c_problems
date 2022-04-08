#include "arr_math.h"
#define SQUARE(x) (x*x)

int sum_of_ints(int *ints, size_t count) {
    int acc = 0;
    for (size_t i=0;i < count; i++) {
        acc+=ints[i];
    }
    return acc;
}

float sum_of_floats(float *vals, size_t count) {
    float acc = 0.0;
    for (size_t i=0;i < count; i++) {
        acc+=vals[i];
    }
    return acc;
}

float avg_of_floats(float *vals, size_t count) {
    float sum = sum_of_floats(vals, count);
    return sum/(float)count;
}

float avg_of_ints(int *ints, size_t count) {
    int sum = sum_of_ints(ints, count);
    return sum/(float)count;
}

float sum_of_squares_of_ints(int *vals, size_t count) {
    int sum_of_squares = 0;
    for (size_t i = 0; i < count; i++) {
        int value = vals[i];
        sum_of_squares += SQUARE(value);
    }
    return sum_of_squares;
}

float sum_of_squares_of_floats(float *vals, size_t count) {
    float sum_of_squares = 0.0;
    for (size_t i = 0; i < count; i++) {
        float value = vals[i];
        sum_of_squares += SQUARE(value);
    }
    return sum_of_squares;
}

void divide_floats_to(float divisor, float *input, float* output, size_t count) {
    for (size_t i = 0; i < count; i++) {
        float result = input[i]/divisor;
        output[i] = result;
    }
}

void divide_floats_to_restrict(float divisor,  float *restrict input, float *restrict output, size_t count)  {
    for (size_t i = 0; i < count; i++) {
        float result = input[i]/divisor;
        output[i] = result;
    }
}

void multiply_floats_to(float multiplier, float *input, float* output, size_t count) {
    for (size_t i = 0; i < count; i++) {
        float result = input[i]*multiplier;
        output[i] = result;
    }
}

void multiply_floats_to_restrict(float multiplier,  float *restrict input, float* restrict output, size_t count)  {
    for (size_t i = 0; i < count; i++) {
        float result = input[i]*multiplier;
        output[i] = result;
    }
}
