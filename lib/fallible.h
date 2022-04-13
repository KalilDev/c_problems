#ifndef K_FALLIBLE_H
#define K_FALLIBLE_H

#include <stdbool.h>
#include "fail.h"

#define FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME _$$success
#define FALLIBLE_FUNCTION_FAILURE_LABEL_NAME _$$failure

#define MAYBE_STRUCT_OF_NAME(type) maybe_ ## type
#define MAYBE_STRUCT_OF_TYPE(type) maybe_ ## type ## _t

#define FALLIBLE_FUNCTION_RESULT_TYPE(type) MAYBE_STRUCT_OF_TYPE(type)
#define FALLIBLE_FUNCTION_RESULT_NAME _$$result

#define RETURN_OTHER(maybe_value)\
    return maybe_value;

#define RETURN_SUCCESS(value)\
    FALLIBLE_FUNCTION_RESULT_NAME.unwrap = value;\
\
    goto FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME;

#define RETURN_FAILURE() goto FALLIBLE_FUNCTION_FAILURE_LABEL_NAME;

#define UNWRAP_ASSIGN(type, name, fallible_expression)\
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

#define VOID_RETURN_SUCCESS() \
    goto FALLIBLE_FUNCTION_SUCCESS_LABEL_NAME;

#define FAIL_WITH_INFO(expression_that_caused_failure, file, line, function)\
    do {\
        failf("The expression %s, at %s:%i#%s was supposed to return an value, but instead returned none!\n", expression_that_caused_failure, file, line, function);\
    } while (0)

#define UNWRAP_OR_FAIL_ASSIGN(type, name, fallible_expression)\
    type name;\
    {\
        FALLIBLE_FUNCTION_RESULT_TYPE(type) temp_fallible_ ## name = fallible_expression;\
        UNWRAP_OR_FAIL(temp_fallible_ ## name);\
        name = temp_fallible_ ## name.unwrap;\
    }

#define UNWRAP_OR_FAIL(fallible_expression)\
    {\
        if (!(fallible_expression).has_value)\
        {\
            FAIL_WITH_INFO(#fallible_expression, __FILE__, __LINE__, __FUNCTION__);\
        }\
    }

#define FALLIBLE_FUNCTION_OR_FAIL_SIGNATURE(type, name, arguments)\
type name ## _or_fail arguments

#define FALLIBLE_VOID_FUNCTION_OR_FAIL_SIGNATURE(name, arguments)\
FALLIBLE_FUNCTION_OR_FAIL_SIGNATURE(void, name, arguments)

#define FALLIBLE_FUNCTION_OR_FAIL(type, name, arguments, ...)\
FALLIBLE_FUNCTION_OR_FAIL_SIGNATURE(type, name, arguments) {\
    FALLIBLE_FUNCTION_RESULT_TYPE(type) FALLIBLE_FUNCTION_RESULT_NAME = name(__VA_ARGS__);\
    UNWRAP_OR_FAIL_ASSIGN(type, result, FALLIBLE_FUNCTION_RESULT_NAME);\
    return FALLIBLE_FUNCTION_RESULT_NAME.unwrap;\
}

#define FALLIBLE_VOID_FUNCTION_OR_FAIL(name, arguments, ...)\
FALLIBLE_VOID_FUNCTION_OR_FAIL_SIGNATURE(name, arguments) {\
    UNWRAP_OR_FAIL(name(__VA_ARGS__));\
}

#define ASSERT_UNWRAPS(fallible_expression)\
    {\
        assert((fallible_expression).has_value);\
    }

#define FALLIBLE_FUNCTION_SIGNATURE(type, name, arguments) FALLIBLE_FUNCTION_RESULT_TYPE(type) name arguments

#define FALLIBLE_FUNCTION(type, name, arguments, block) \
FALLIBLE_FUNCTION_SIGNATURE(type, name, arguments) {\
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
#define FALLIBLE_VOID_FUNCTION(name, arguments, block) \
FALLIBLE_FUNCTION(void, name, arguments, {\
    {\
        block\
    }\
    VOID_RETURN_SUCCESS();\
})


#define DEFINE_MAYBE_STRUCT_OF(type) \
typedef struct MAYBE_STRUCT_OF_NAME(type) {\
    bool has_value;\
    type unwrap;\
} MAYBE_STRUCT_OF_TYPE(type);\

typedef void *heap_memory;

DEFINE_MAYBE_STRUCT_OF(heap_memory)

FALLIBLE_FUNCTION_SIGNATURE(heap_memory, fallible_malloc, (size_t size));

FALLIBLE_FUNCTION_SIGNATURE(heap_memory, fallible_calloc, (size_t nmemb, size_t size));

FALLIBLE_FUNCTION_SIGNATURE(heap_memory, fallible_realloc, (heap_memory ptr, size_t size));

typedef struct MAYBE_STRUCT_OF_NAME(void) {
    bool has_value;
} MAYBE_STRUCT_OF_TYPE(void);

#endif