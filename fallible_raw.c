#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef size_t value;

typedef struct {
    bool has_value;
    value unwrap;
} maybe_value;

typedef int other_value;

typedef struct {
    bool has_value;
    value unwrap;
} maybe_other_value;

maybe_other_value other_fallible_function(int input) {
    maybe_other_value other_fallible_function_result;
    
    if (input == 0) {
        goto other_fallible_function_return_fail;
    }

    other_value result = -input;
    other_fallible_function_result.unwrap = result;

    goto other_fallible_function_return_success;

    other_fallible_function_return_success:
    other_fallible_function_result.has_value = true;
    return other_fallible_function_result;

    other_fallible_function_return_fail:
    other_fallible_function_result.has_value = false;
    return other_fallible_function_result;
}

maybe_value fallible_function(int input) {
    maybe_value fallible_function_result;

    maybe_other_value maybe_other_result = other_fallible_function(input);
    if (!maybe_other_result.has_value) {
        goto fallible_function_return_fail;
    }
    other_value other_result = maybe_other_result.unwrap;
    if (other_result < 0) {
        goto fallible_function_return_fail;
    }
    fallible_function_result.unwrap = (value)other_result;
    goto fallible_function_return_success;

    fallible_function_return_success:
    fallible_function_result.has_value = true;
    return fallible_function_result;

    fallible_function_return_fail:
    fallible_function_result.has_value = false;
    return fallible_function_result;
}