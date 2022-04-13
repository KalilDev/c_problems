#include "string_view.h"
#include "fallible.h"
#include <assert.h>

size_t actual_string_view_length(actual_string_view_t sv) {
    // * the length must be less than size_t >> 8
    assert((sv.length & 0xff) == 0xFF);
    // * the actual length is (length ^ 0xFF) >> 8
    return (sv.length ^ 0xFF) >> 8;
}

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
    
    UNWRAP_ASSIGN(heap_memory, resulting_memory, fallible_malloc(heap_length * sizeof(unsigned char)));

    heap_string result = resulting_memory;
    // * there are no guarantees whether it has or not an NULL terminator.
    result[length] = '\0';
    // TODO: Copy string from the buff

    RETURN_SUCCESS(result);
});

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

small_string_t small_string_from_cstring(char *buff, size_t size) {
    // The null char does not count to the length of the string view 
    size_t length = size - 1;
    assert(length <= SMALL_STRING_MAX_LENGTH);

    small_string_t result;
    char *tgt_buff = result.storage.char_buff;

    unsigned char last_char;
    if (length == SMALL_STRING_MAX_LENGTH) {
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
    if (view_length <= SMALL_STRING_MAX_LENGTH) {
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
    UNWRAP_ASSIGN(heap_string, string_buff, actual_string_view_to_c_string(view->detail.actual));
    result.as_cstring = string_buff;
    result.destructor = free_actual_string_c_string;
    RETURN_SUCCESS(result);
})
