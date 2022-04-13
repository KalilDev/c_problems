#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "io_wrapper.h"
#include <stdbool.h>
#include <math.h>
#include "fallible.h"

#define DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(type, name, arguments, block, ...) \
FALLIBLE_FUNCTION(type, name, arguments, block);\
FALLIBLE_FUNCTION_OR_FAIL(type, name, arguments, __VA_ARGS__);

#define DEFINE_FALLIBLE_VOID_FUNCTION_AND_FUNCTION_OR_FAIL(name, arguments, block, ...) \
FALLIBLE_VOID_FUNCTION(name, arguments, block);\
FALLIBLE_VOID_FUNCTION_OR_FAIL(name, arguments, __VA_ARGS__);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(float, read_float, (), {
    float result;
    if (scanf("%f", &result) != 1) {
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(result);
});
DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(float, prompt_float, (), {
    puts("Digite um numero real:");
    RETURN_OTHER(read_float());
});

DEFINE_FALLIBLE_VOID_FUNCTION_AND_FUNCTION_OR_FAIL(prompt_floats, (float *out, size_t count), {
    const char *suffix;
    const char *real;
    if (count>1) {
        suffix = "s";
        real = "reais";
    } else {
        suffix = "";
        real = "real";
    }
    printf("Digite %li numero%s %s:\n", count, suffix, real);
    for (size_t i=0; i < count; i++) {
        UNWRAP_ASSIGN(float, ith_value, read_float());
        out[i] = ith_value;
    }
}, out, count);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(int, read_int, (read_int_format format), {
    const char* format_string;
    switch (format) {
        case int_decimal_octal_or_hexadecimal: format_string="%i";break;
        case int_decimal: format_string="%d";break;
        default:
            RETURN_FAILURE();
    }
    int value;
    if (scanf(format_string, &value) != 1) {
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(value);
}, format);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(uint, read_uint, (read_uint_format format), {
    const char* format_string;
    switch (format) {
        case uint_hexadecimal: format_string = "%x"; break;
        case uint_decimal: format_string = "%u"; break;
        case uint_octal: format_string = "%o"; break;
        default:
            RETURN_FAILURE();
    }
    uint value;
    if (scanf(format_string, &value) != 1) {
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(value);
}, format);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(int, prompt_int, (read_int_format format), {
    puts("Digite um numero inteiro:");
    RETURN_OTHER(read_int(format));
}, format);


DEFINE_FALLIBLE_VOID_FUNCTION_AND_FUNCTION_OR_FAIL(prompt_ints, (int *out, size_t count, read_int_format format), {
    const char *suffix;
    if (count>1) {
        suffix = "s";
    } else {
        suffix = "";
    }
    printf("Digite %li numero%s inteiro%s:\n", count, suffix, suffix);
    for (size_t i=0; i < count; i++) {
        UNWRAP_ASSIGN(int, ith_value, read_int(format));
        out[i] = ith_value;
    }
}, out, count, format);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(uint, prompt_uint, (read_uint_format format), {
    puts("Digite um numero inteiro não negativo:");
    RETURN_OTHER(read_uint(format));
}, format);

DEFINE_FALLIBLE_VOID_FUNCTION_AND_FUNCTION_OR_FAIL(prompt_uints, (uint *out, size_t count, read_uint_format format), {
    const char *suffix;
    if (count>1) {
        suffix = "s";
    } else {
        suffix = "";
    }
    printf("Digite %li numero%s inteiro%s não negativo%s:\n", count, suffix, suffix, suffix);
    for (size_t i=0; i < count; i++) {
        UNWRAP_ASSIGN(uint, ith_value, read_uint(format));
        out[i] = ith_value;
    }
}, out, count, format);

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(int, read_dint, (), {
    RETURN_OTHER(read_int(int_decimal));
});

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(uint, read_duint, (), {
    RETURN_OTHER(read_uint(uint_decimal));
});

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(int, prompt_dint, (), {
    RETURN_OTHER(prompt_int(int_decimal));
});

DEFINE_FALLIBLE_FUNCTION_AND_FUNCTION_OR_FAIL(uint, prompt_duint, (), {
    RETURN_OTHER(prompt_uint(uint_decimal));
});

DEFINE_FALLIBLE_VOID_FUNCTION_AND_FUNCTION_OR_FAIL(prompt_duints, (uint *out, size_t count), {
    UNWRAP_OR_FAIL(prompt_uints(out, count, uint_decimal));
}, out, count);

DEFINE_FALLIBLE_VOID_FUNCTION_AND_FUNCTION_OR_FAIL(prompt_dints, (int *out, size_t count), {
    UNWRAP_OR_FAIL(prompt_ints(out, count, int_decimal));
}, out, count);
