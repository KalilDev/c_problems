#include "fallible.h"

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
