#ifndef K_STRING_VIEW_H
#define K_STRING_VIEW_H

#include <assert.h>
#include <stdint.h>
#include "fallible.h"
#include "fallible_main.h"

typedef enum string_view_flag_kind {
    small_string_view_flag_kind,
    invalid_string_view_flag_kind,
    actual_string_view_flag_kind,
} string_view_flag_kind_t;

typedef unsigned char string_view_flag_t;

#define ACTUAL_STRING_VIEW_FLAG 0xFF

// How this representation works:
// * the flag is length & 0xff
// * the flag must be equal to ACTUAL_STRING_VIEW_FLAG
// * the length must be less than SIZE_MAX >> 8
// * the actual length is (length ^ 0xff) >> 8
// * there are no guarantees whether it has or not an NULL terminator.
typedef struct actual_string_view {
    char* start;
    size_t length;
} actual_string_view_t;

typedef char *heap_string;

DEFINE_MAYBE_STRUCT_OF(heap_string)

string_view_flag_t actual_string_view_flag(actual_string_view_t *sv);

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
// * the flag is the last character
// * if the length is SMALL_STRING_MAX_LENGTH, the flag must be be \0
// * the length must be less than or equal to SMALL_STRING_MAX_LENGTH
// * if the length is less than SMALL_STRING_MAX_LENGTH, the flag must
//   be the length, and the string contains an null terminator at `char_buff[length]`
typedef struct small_string {
    union {
        intptr_t __ptr_buff[2];
        char char_buff[SMALL_STRING_MAX_LENGTH+1];
    } storage;
} small_string_t;

string_view_flag_t small_string_flag(small_string_t *st);

char *small_string_as_cstring(small_string_t *small_string);
unsigned char small_string_length(small_string_t *small_string);

// How this representation works:
// * The flag, when fetched from each impl in the union, must be identical on every one of them.
// * The flag determines which implementation will be used
typedef struct string_view {
    union {
        actual_string_view_t actual;
        small_string_t small_string;
    } detail;
} string_view_t;

static_assert(sizeof(actual_string_view_t) == sizeof(small_string_t), "For string_view_t to work, actual_string_view_t and small_string_t must have the same size");
static_assert(_Alignof(actual_string_view_t) == _Alignof(small_string_t), "For string_view_t to work, actual_string_view_t and small_string_t must have the same alignment");

string_view_flag_t string_view_flag(string_view_t *sv);

string_view_flag_kind_t string_view_flag_kind_from_flag(string_view_flag_t flag);

string_view_flag_kind_t string_view_flag_kind_sv(string_view_t *sv);

small_string_t small_string_from_cstring(char *buff, size_t size);

actual_string_view_t actual_string_view_from_cstring(char *buff, size_t size);

string_view_t string_view_from_cstring(char *buff, size_t size);

size_t string_view_length(string_view_t *view);

void free_actual_string_c_string(char *buff);

FALLIBLE_FUNCTION_SIGNATURE(string_view_as_cstring_t, string_view_to_cstring, (string_view_t *view));

#define typedef_fn(return_type, typedef_name, args)\
typedef return_type (*typedef_name)args;

#endif
