#ifndef K_IO_WRAPPER_H
#define K_IO_WRAPPER_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    decimal_or_octal,
    decimal,
    octal,
} read_int_format;

typedef struct {
    float value;
    bool error;
} read_float_result_t;

typedef struct {
    int value;
    bool error;
} read_int_result_t;

read_float_result_t read_float();

read_int_result_t read_int(read_int_format format);

float read_float_or_fail();

int read_int_or_fail(read_int_format format);

int promt_int_or_fail(read_int_format format);

void promt_ints_or_fail(int *out, size_t count, read_int_format format);

void promt_floats_or_fail(float *out, size_t count);

int promt_dint_or_fail();

float promt_float_or_fail();

#endif
