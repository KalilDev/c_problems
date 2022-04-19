#include "string_view.h"
#include "fallible.h"
#include <assert.h>

size_t actual_string_view_length(actual_string_view_t sv) {
    // * the flag is length & 0xff
    // * the flag must be equal to ACTUAL_STRING_VIEW_FLAG
    assert((sv.length & 0xff) == ACTUAL_STRING_VIEW_FLAG);
    // * the actual length is (length ^ 0xff) >> 8
    return (sv.length ^ 0xFF) >> 8;
}

string_view_flag_t actual_string_view_flag(actual_string_view_t *sv) {
    return (string_view_flag_t)(sv->length & 0xFF);
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

string_view_flag_t small_string_flag(small_string_t *st) {
    return st->storage.char_buff[SMALL_STRING_MAX_LENGTH];
}

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

string_view_flag_t string_view_flag(string_view_t *sv) {
    string_view_flag_t result_from_actual = actual_string_view_flag(&sv->detail.actual);
    string_view_flag_t result_from_small = small_string_flag(&sv->detail.small_string);
    assert(result_from_actual == result_from_small);
    return result_from_small;
}


string_view_flag_kind_t string_view_flag_kind_from_flag(string_view_flag_t flag) {
    if (flag == '\0' || flag < SMALL_STRING_MAX_LENGTH) {
        return small_string_view_flag_kind;
    }
    if (flag == ACTUAL_STRING_VIEW_FLAG) {
        return actual_string_view_flag_kind;
    }
    return invalid_string_view_flag_kind;
}

string_view_flag_kind_t string_view_flag_kind_sv(string_view_t *sv) {
    return string_view_flag_kind_from_flag(string_view_flag(sv));
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
        .length = ((size - 1) << 8) | ACTUAL_STRING_VIEW_FLAG,
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

size_t string_view_length(string_view_t *view) {
    switch (string_view_flag_kind_sv(view)) {
        case small_string_view_flag_kind: return small_string_length(&view->detail.small_string);
        case invalid_string_view_flag_kind: failf("Invalid flag on the string view at %p. The flag was %x.", view, string_view_flag(view));
        case actual_string_view_flag_kind: return actual_string_view_length(view->detail.actual);
    }
}

void free_actual_string_c_string(char *buff) {
    // TODO: check that this came from an actual_string_view_to_c_string
    free(buff);
}

FALLIBLE_FUNCTION(string_view_as_cstring_t, string_view_to_cstring, (string_view_t *view), {
    string_view_as_cstring_t result;
    switch (string_view_flag_kind_sv(view)) {
        case small_string_view_flag_kind: {
            result.as_cstring = small_string_as_cstring(&view->detail.small_string);
            result.destructor = NULL;
            RETURN_SUCCESS(result);
        }
        case invalid_string_view_flag_kind: failf("Invalid flag on the string view at %p. The flag was %x.", view, string_view_flag(view));
        case actual_string_view_flag_kind: {
            UNWRAP_ASSIGN(heap_string, string_buff, actual_string_view_to_c_string(view->detail.actual));
            result.as_cstring = string_buff;
            result.destructor = free_actual_string_c_string;
            RETURN_SUCCESS(result);
        }
    }
})
