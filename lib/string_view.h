#ifndef K_STRING_VIEW_H
#define K_STRING_VIEW_H

#include <assert.h>
#include <stdint.h>
#include "fallible.h"
#include "fallible_main.h"

// How this representation works:
// * the (length & 0xff) must be equal to 0xff
// * the length must be less than SIZE_MAX >> 8
// * the actual length is (length ^ 0xff) >> 8
// * there are no guarantees whether it has or not an NULL terminator.
typedef struct actual_string_view {
    char* start;
    size_t length;
} actual_string_view_t;

typedef char *heap_string;

DEFINE_MAYBE_STRUCT_OF(heap_string)

unsigned char actual_string_view_flag_byte(actual_string_view_t *sv) {
    return (unsigned char)(sv->length & 0xFF);
}

size_t actual_string_view_length(actual_string_view_t sv);

typedef void (*string_view_as_cstring_destructor)(char *memory);

typedef struct string_view_as_cstring {
    char *as_cstring;
    string_view_as_cstring_destructor destructor;
} string_view_as_cstring_t;

DEFINE_MAYBE_STRUCT_OF(string_view_as_cstring_t)

void string_view_as_cstring_destroy(string_view_as_cstring_t *value);


FALLIBLE_FUNCTION_SIGNATURE(heap_string, actual_string_view_to_c_string, (actual_string_view_t sv));


#define SMALL_STRING_MAX_LENGTH 15

// How this representation works:
// * if the length is SMALL_STRING_MAX_LENGTH, the last character MUST be \0
// * if the length is less than SMALL_STRING_MAX_LENGTH, the last character stores the
//   `length`, and the string contains an null terminator at `char_buff[length]`
typedef struct small_string {
    union {
        intptr_t __ptr_buff[2];
        char char_buff[SMALL_STRING_MAX_LENGTH+1];
    } storage;
} small_string_t;

unsigned char small_string_flag_byte(small_string_t *st) {
    return st->storage.char_buff[SMALL_STRING_MAX_LENGTH];
}

char *small_string_as_cstring(small_string_t *small_string);
unsigned char small_string_length(small_string_t *small_string);

// Since char* and size_t are the same size, we can treat it as an 
typedef struct string_view {
    union {
        actual_string_view_t actual;
        small_string_t small_string;
    } detail;
} string_view_t;

static_assert(sizeof(actual_string_view_t) == sizeof(small_string_t), "For string_view_t to work, actual_string_view_t and small_string_t must have the same size");
static_assert(_Alignof(actual_string_view_t) == _Alignof(small_string_t), "For string_view_t to work, actual_string_view_t and small_string_t must have the same alignment");

unsigned char string_view_flag_byte(string_view_t *sv) {
    unsigned char result_from_actual = actual_string_view_flag_byte(&sv->detail.actual);
    unsigned char result_from_small = small_string_flag_byte(&sv->detail.small_string);
    assert(result_from_actual == result_from_small);
    return result_from_small;
}

small_string_t small_string_from_cstring(char *buff, size_t size);

actual_string_view_t actual_string_view_from_cstring(char *buff, size_t size);

string_view_t string_view_from_cstring(char *buff, size_t size);

bool string_view_is_small_string(string_view_t *view);

size_t string_view_length(string_view_t *view);

void free_actual_string_c_string(char *buff);

FALLIBLE_FUNCTION_SIGNATURE(string_view_as_cstring_t, string_view_to_cstring, (string_view_t *view));

#endif
