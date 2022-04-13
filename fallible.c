#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME _$$success
#define FALLIBLE_FUNCTION_FAILURE_LABEL_NAME _$$failure

#define MAYBE_STRUCT_OF_NAME(type) maybe_ ## type
#define MAYBE_STRUCT_OF_TYPE(type) maybe_ ## type ## _t

#define FALLIBLE_FUNCTION_RESULT_TYPE(type) MAYBE_STRUCT_OF_TYPE(type)
#define FALLIBLE_FUNCTION_RESULT_NAME _$$result

#define RETURN_SUCCESS(value)\
    FALLIBLE_FUNCTION_RESULT_NAME.unwrap = value;\
\
    goto FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME;

#define RETURN_FAILURE() goto FALLIBLE_FUNCTION_FAILURE_LABEL_NAME;

#define UNWRAP_OTHER(type, name, fallible_expression)\
    type name;\
    {\
        FALLIBLE_FUNCTION_RESULT_TYPE(type) temp_fallible_ ## name = fallible_expression;\
        if (!temp_fallible_ ## name.has_value)\
        {\
            RETURN_FAILURE();\
        }\
        name = temp_fallible_ ## name.unwrap;\
    }
#define UNWRAP_VOID(fallible_expression)\
    {\
        FALLIBLE_FUNCTION_RESULT_TYPE(void) temp_fallible_void = fallible_expression;\
        if (!temp_fallible_void.has_value)\
        {\
            RETURN_FAILURE();\
        }\
    }

#define FALLIBLE_FUNCTION(type, name, arguments, block) \
FALLIBLE_FUNCTION_RESULT_TYPE(type) name arguments {\
    FALLIBLE_FUNCTION_RESULT_TYPE(type) FALLIBLE_FUNCTION_RESULT_NAME;\
    \
    {\
        block\
    }\
    \
    FALLIBLE_FUNCTION_FAILURE_LABEL_NAME:\
    FALLIBLE_FUNCTION_RESULT_NAME.has_value = false;\
    return FALLIBLE_FUNCTION_RESULT_NAME;\
    \
    FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME:\
    FALLIBLE_FUNCTION_RESULT_NAME.has_value = true;\
    return FALLIBLE_FUNCTION_RESULT_NAME;\
\
}


#define DEFINE_MAYBE_STRUCT_OF(type) \
typedef struct MAYBE_STRUCT_OF_NAME(type) {\
    bool has_value;\
    type unwrap;\
} MAYBE_STRUCT_OF_TYPE(type);\

typedef size_t value;
typedef int other_value;
DEFINE_MAYBE_STRUCT_OF(other_value)

FALLIBLE_FUNCTION(other_value, other_fallible_function, (int input), {
    if (input != 0) {
        RETURN_SUCCESS(input);
    }
    RETURN_FAILURE();
});

DEFINE_MAYBE_STRUCT_OF(value)

FALLIBLE_FUNCTION(value, fallible_function, (int input), {
    UNWRAP_OTHER(other_value, result, other_fallible_function(input))
    if (result >= 0) {
        RETURN_SUCCESS((size_t)input);
    }
    RETURN_FAILURE();
});


// How this representation works:
// * the length & 0xff must be equal to 0xff
// * the length must be less than size_t >> 8
// * the actual length is length >> 8
// * there are no guarantees whether it has or not an NULL terminator.
typedef struct actual_string_view {
    char* start;
    size_t length;
} actual_string_view_t;

typedef char *heap_string;

DEFINE_MAYBE_STRUCT_OF(heap_string)

typedef void *heap_memory;

DEFINE_MAYBE_STRUCT_OF(heap_memory)

FALLIBLE_FUNCTION(heap_memory, fallible_malloc, (size_t size), {
    void *result = malloc(size);
    if (result == NULL) {
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(result);
})

FALLIBLE_FUNCTION(heap_memory, fallible_calloc, (size_t nmemb, size_t size), {
    void *result = calloc(nmemb, size);
    if (result == NULL) {
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(result);
})

FALLIBLE_FUNCTION(heap_memory, fallible_realloc, (heap_memory ptr, size_t size), {
    void *result = realloc(ptr, size);
    if (result == NULL) {
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(result);
})

size_t actual_string_view_length(actual_string_view_t sv) {
    // * the length must be less than size_t >> 8
    assert((sv.length & 0xff) == 0xFF);
    // * the actual length is (length ^ 0xFF) >> 8
    return (sv.length ^ 0xFF) >> 8;
}

typedef struct MAYBE_STRUCT_OF_NAME(void) {
    bool has_value;
} MAYBE_STRUCT_OF_TYPE(void);

#define VOID_RETURN_SUCCESS() \
    goto FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME;

FALLIBLE_FUNCTION(void, do_stuff, (int input), {
    VOID_RETURN_SUCCESS();
})
typedef void (*string_view_as_cstring_destructor)(char *memory);

typedef struct string_view_as_cstring {
    char *as_cstring;
    string_view_as_cstring_destructor destructor;
} string_view_as_cstring_t;

DEFINE_MAYBE_STRUCT_OF(string_view_as_cstring_t)

void string_view_as_cstring_destroy(string_view_as_cstring_t *value) {
    string_view_as_cstring_destructor destructor = value->destructor;
    if (destructor == NULL) {
        return;
    }
    destructor(value->as_cstring);
}


FALLIBLE_FUNCTION(heap_string, actual_string_view_to_c_string, (actual_string_view_t sv), {
    size_t length = actual_string_view_length(sv);
    size_t heap_length = length + 1;
    
    UNWRAP_OTHER(heap_memory, resulting_memory, fallible_malloc(heap_length * sizeof(unsigned char)));

    heap_string result = resulting_memory;
    // * there are no guarantees whether it has or not an NULL terminator.
    result[length] = '\0';
    // TODO: Copy string from the buff

    RETURN_SUCCESS(result);
})


// How this representation works:
// * if the length is 15, the last character MUST be \0
// * if the length is less than 15, the last character is the length, and the string contains an null terminator at char_buff[length]
typedef struct small_string {
    union {
        intptr_t __ptr_buff[2];
        char char_buff[16];
    } storage;
} small_string_t;

char *small_string_as_cstring(small_string_t *small_string) {
    return small_string->storage.char_buff;
}
unsigned char small_string_length(small_string_t *small_string) {
    unsigned char last_char = small_string->storage.char_buff[15];
    // * if the length is 15, the last character MUST be \0
    if (last_char == '\0') {
        return 15;
    }
    // * if the length is less than 15, the last character is the length,
    //   and the string contains an null terminator at char_buff[length]
    assert(last_char < 15);
    assert(small_string->storage.char_buff[last_char] == '\0');
    return last_char;
}

// Since char* and size_t are the same size, we can treat it as an 
typedef struct string_view {
    union {
        actual_string_view_t actual;
        small_string_t small_string;
    } detail;
} string_view_t;

static_assert(sizeof(actual_string_view_t) == sizeof(small_string_t), "For string_view_t to work, actual_string_view_t and small_string_t must have the same size");
static_assert(_Alignof(actual_string_view_t) == _Alignof(small_string_t), "For string_view_t to work, actual_string_view_t and small_string_t must have the same alignment");

const size_t small_string_max_length = 15;


small_string_t small_string_from_cstring(char *buff, size_t size) {
    // The null char does not count to the length of the string view 
    size_t length = size - 1;
    assert(length <= small_string_max_length);

    small_string_t result;
    char *tgt_buff = result.storage.char_buff;

    unsigned char last_char;
    if (length == small_string_max_length) {
        // * if the length is 15, the last character MUST be \0
        last_char = '\0';
    } else {
        // * if the length is less than 15, the last character is the length,
        //   and the string contains an null terminator at char_buff[length]
        last_char = (unsigned char)length;
    }
    
    tgt_buff[15] = last_char;
    
    // TODO: Copy to buff from the cstring;

    return result;
}

actual_string_view_t actual_string_view_from_cstring(char *buff, size_t size) {
    actual_string_view_t result = {
        .start = buff,
        // The null char does not count to the length of the string view 
        .length = ((size - 1) << 8) | 0xFF,
    };
    return result;
}

string_view_t string_view_from_cstring(char *buff, size_t size) {
    string_view_t result;
    // The null char does not count to the length of the string view 
    size_t view_length = size - 1;
    if (view_length <= small_string_max_length) {
        result.detail.small_string = small_string_from_cstring(buff, size);
        return result;
    }
    result.detail.actual = actual_string_view_from_cstring(buff, size);
    return result;
}

bool string_view_is_small_string(string_view_t *view) {
    size_t as_actual_length = view->detail.actual.length;
    if ((as_actual_length & 0xFF) == 0xFF) {
        return false;
    }
    return true;
}

size_t string_view_length(string_view_t *view) {
    if (string_view_is_small_string(view)) {
        return small_string_length(&view->detail.small_string);
    }
    return actual_string_view_length(view->detail.actual);
}

void free_actual_string_c_string(char *buff) {
    // TODO: check that this came from an actual_string_view_to_c_string
    free(buff);
}

FALLIBLE_FUNCTION(string_view_as_cstring_t, string_view_to_cstring, (string_view_t *view), {
    string_view_as_cstring_t result;
    if (string_view_is_small_string(view)) {
        result.as_cstring = small_string_as_cstring(&view->detail.small_string);
        result.destructor = NULL;
        RETURN_SUCCESS(result);
    }
    UNWRAP_OTHER(heap_string, string_buff, actual_string_view_to_c_string(view->detail.actual));
    result.as_cstring = string_buff;
    result.destructor = free_actual_string_c_string;
    RETURN_SUCCESS(result);
})

DEFINE_MAYBE_STRUCT_OF(int)

FALLIBLE_FUNCTION(int, fallible_main, (int argc, char** argv), {
    puts("Hello world!");
    puts("I will fail if the second arg is boom");
    if (argc>1 && strcmp(argv[1], "boom") == 0) {
        fputs("Kaboom!!\n", stderr);
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(0);
});

int main(int argc, char** argv) {
    FALLIBLE_FUNCTION_RESULT_TYPE(int) maybe_result = fallible_main(argc, argv);
    if (maybe_result.has_value) {
        return maybe_result.unwrap;
    }
    return 1;
}

