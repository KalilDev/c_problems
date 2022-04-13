#ifndef K_IO_WRAPPER_H
#define K_IO_WRAPPER_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fallible.h"

typedef enum {
    int_decimal_octal_or_hexadecimal,
    int_decimal,
} read_int_format;

typedef enum {
    uint_hexadecimal,
    uint_decimal,
    uint_octal,
} read_uint_format;

DEFINE_MAYBE_STRUCT_OF(float)

DEFINE_MAYBE_STRUCT_OF(int)

typedef unsigned int uint;
DEFINE_MAYBE_STRUCT_OF(uint)

#define DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(type, name, arguments) \
FALLIBLE_FUNCTION_SIGNATURE(type, name, arguments);\
FALLIBLE_FUNCTION_OR_FAIL_SIGNATURE(type, name, arguments);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(float, read_float, ());
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(float, prompt_float, ());
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(void, prompt_floats, (float *out, size_t count));

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(int, read_int, (read_int_format format));
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(int, prompt_int, (read_int_format format));
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(void, prompt_ints, (int *out, size_t count, read_int_format format));

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(uint, read_uint, (read_uint_format format));
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(uint, prompt_uint, (read_uint_format format));
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(void, prompt_uints, (uint *out, size_t count, read_uint_format format));


DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(int, read_dint, ());
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(int, prompt_dint, ());
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(void, prompt_dints, (int *out, size_t count));

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(uint, read_duint, ());
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(uint, prompt_duint, ());
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL_SIGNATURES(void, prompt_duints, (uint *out, size_t count));

#endif
