#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "io_wrapper.h"
#include <stdbool.h>
#include <math.h>

read_float_result_t read_float() {
    read_float_result_t result = {.value = NAN};
    result.error = scanf("%f", &result.value) != 1;
    return result;
}

read_int_result_t read_int(read_int_format format) {
    read_int_result_t result = {.value = 0xDEADBEEF};
    const char* format_string;
    switch (format) {
        case decimal_or_octal: format_string="%i";break;
        case decimal: format_string="%d";break;
        case octal: format_string="%o";break;
        default:
            result.error = true;
            return result;
    }
    result.error = scanf("%i", &result.value) != 1;
    return result;
}

float read_float_or_fail() {
    read_float_result_t result = read_float();
    if (result.error) {
        fputs("Não foi possivel ler o float!\n", stderr);
        exit(EXIT_FAILURE);
        // unreachable
        assert(false);
    }
    return result.value;
}

int read_int_or_fail(read_int_format format) {
    read_int_result_t result = read_int(format);
    if (result.error) {
        fputs("Não foi possivel ler o int!\n", stderr);
        exit(EXIT_FAILURE);
        // unreachable
        assert(false);
    }
    return result.value;
}

int promt_int_or_fail(read_int_format format) {
    puts("Digite um numero inteiro:");
    return read_int_or_fail(format);
}

void promt_ints_or_fail(int *out, size_t count, read_int_format format) {
    const char *suffix;
    if (count>1) {
        suffix = "s";
    } else {
        suffix = "";
    }
    printf("Digite %li numero%s inteiro%s:\n", count, suffix, suffix);
    for (size_t i=0; i < count; i++) {
        out[i] = read_int_or_fail(format);
    }
}

void promt_floats_or_fail(float *out, size_t count) {
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
        out[i] = read_float_or_fail();
    }
}

int promt_dint_or_fail() {
    return promt_int_or_fail(decimal);
}
float promt_float_or_fail() {
    puts("Digite um numero real:");
    return read_float_or_fail();
}
