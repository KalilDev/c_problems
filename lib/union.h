#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define UNION_TAG_ENUM_NAME(typename) typename ## _tag
#define UNION_TAG_ENUM_TYPENAME(typename) typename ## _tag ## _t
#define UNION_TAG_ENUM_CASE_NAME(name, casename) name ## _tag  ## _ ## casename
#define UNION_TAG_ENUM_MEMBER(name, casename) UNION_TAG_ENUM_CASE_NAME(name, casename),

#define UNION_TAG_ENUM(name, ...)\
typedef enum UNION_TAG_ENUM_NAME(name) {\
    UNION_TAG_ENUM_CASE_NAME(name, null) = 0,\
    /* Cases go here: */\
    __VA_ARGS__\
    /* The length+1 so that the length can be the calculated: */\
    UNION_TAG_ENUM_CASE_NAME(name, LENGTH_PLUS_ONE),\
    /* The limit so that the tag is the size and alignment of a pointer: */\
    UNION_TAG_ENUM_CASE_NAME(name, LIMIT) = INTPTR_MAX,\
} UNION_TAG_ENUM_TYPENAME(name);\
\
static_assert(_Alignof(enum UNION_TAG_ENUM_NAME(name)) == _Alignof(void *), "The " #name " union tag type must have the same alignment as an pointer");\
static_assert(sizeof(enum UNION_TAG_ENUM_NAME(name)) == sizeof(void *), "The " #name " union tag type must have the same size as an pointer");

#define UNION_POINTER_STRUCT_NAME(typename) typename ## _ptr
#define UNION_POINTER_STRUCT_TYPENAME(typename) typename ## _ptr ## _t

#define UNION_POINTER_DEF(name)\
typedef struct UNION_POINTER_STRUCT_NAME(name) {\
    enum UNION_TAG_ENUM_NAME(name) tag;\
    void *ptr;\
} UNION_POINTER_STRUCT_TYPENAME(name);

#define UNION_POINTER_STRUCT_NULLPTR_NAME(name) null_ ## name ## _ptr

#define UNION_NULL_POINTER(name)\
const UNION_POINTER_STRUCT_TYPENAME(name) UNION_POINTER_STRUCT_NULLPTR_NAME(name) = {.tag = UNION_TAG_ENUM_CASE_NAME(name, null), .ptr = NULL};

#define UNION_CASE_STRUCT_NAME(union_name, casename) union_name ## casename
#define UNION_CASE_STRUCT_TYPENAME(union_name, casename) union_name ## _ ##  casename ## _t

#define UNION_CASE_STRUCT(union_name, casename, ...)\
typedef struct UNION_CASE_STRUCT_NAME(union_name, casename) {\
    enum UNION_TAG_ENUM_NAME(union_name) tag;\
    /* body goes here */\
    __VA_ARGS__\
} UNION_CASE_STRUCT_TYPENAME(union_name, casename);

#define CREATE_FUNCTION_NAME_FROM(union_name, casename) create_ ## union_name  ## _ ## casename

#define UNION_CREATE_CASE_FUNCTION_SIGNATURE(union_name, casename, arguments) UNION_CASE_STRUCT_TYPENAME(union_name, casename) CREATE_FUNCTION_NAME_FROM(union_name, casename) arguments

#define UNION_CREATE_CASE_FUNCTION(union_name, casename, arguments, ...)\
UNION_CREATE_CASE_FUNCTION_SIGNATURE(union_name, casename, arguments) {\
    UNION_CASE_STRUCT_TYPENAME(union_name, casename) result = {\
        .tag = UNION_TAG_ENUM_CASE_NAME(union_name, casename),\
        /* body initialization goes here */\
        __VA_ARGS__\
    };\
    return result;\
}

#define NEW_FUNCTION_NAME_FROM(union_name, casename) new_ ## union_name  ## _ ## casename

#define UNION_NEW_CASE_FUNCTION_SIGNATURE(union_name, casename, arguments) UNION_POINTER_STRUCT_TYPENAME(union_name) NEW_FUNCTION_NAME_FROM(union_name, casename) arguments

#define UNION_NEW_CASE_FUNCTION(union_name, casename, allocator, arguments, ...)\
UNION_NEW_CASE_FUNCTION_SIGNATURE(union_name, casename, arguments) {\
    UNION_CASE_STRUCT_TYPENAME(union_name, casename) *allocated_memory = allocator(sizeof(UNION_CASE_STRUCT_TYPENAME(union_name, casename)));\
    if (!allocated_memory) {\
        return UNION_POINTER_STRUCT_NULLPTR_NAME(union_name);\
    }\
    UNION_CASE_STRUCT_TYPENAME(union_name, casename) result_from_create = CREATE_FUNCTION_NAME_FROM(union_name, casename)(__VA_ARGS__);\
    assert(result_from_create.tag == UNION_TAG_ENUM_CASE_NAME(union_name, casename));\
    *allocated_memory = result_from_create;\
    UNION_POINTER_STRUCT_TYPENAME(union_name) result = {\
        .tag = result_from_create.tag,\
        .ptr = allocated_memory,\
    };\
    return result;\
}

#define DESTROY_CASE_FUNCTION_NAME_FROM(union_name, casename) destroy_ ## union_name  ## _ ## casename
#define UNION_DESTROY_CASE_FUNCTION_SIGNATURE(union_name, casename) void DESTROY_CASE_FUNCTION_NAME_FROM(union_name, casename)(UNION_CASE_STRUCT_TYPENAME(union_name, casename) *ptr)
#define UNION_DESTROY_CASE_FUNCTION(union_name, casename, body) void DESTROY_CASE_FUNCTION_NAME_FROM(union_name, casename)(UNION_CASE_STRUCT_TYPENAME(union_name, casename) *ptr) {body; }

#define DESTRUCTOR_ARRAY_NAME_FROM(union_name) destructors_for_ ## union_name
#define DESTROY_FUNCTION_NAME_FROM(union_name) destroy_ ## union_name
#define UNION_DESTROY_FUNCTION_SIGNATURE(union_name) void DESTROY_FUNCTION_NAME_FROM(union_name)(UNION_POINTER_STRUCT_TYPENAME(union_name) ptr)
#define CASE_DESTRUCTOR_TYPEDEF_NAME_FROM(union_name) union_name ## _destructor

#define UNION_CASE_DESTRUCTOR_TYPEDEF(union_name)\
typedef void (*CASE_DESTRUCTOR_TYPEDEF_NAME_FROM(union_name))(void*);


#define CASE_DESTRUCTORS_ARRAY_NAME_FROM(union_name) union_name ## _destructors
#define UNION_CASE_DESTRUCTORS_ARRAY(union_name, ...)\
const CASE_DESTRUCTOR_TYPEDEF_NAME_FROM(union_name) CASE_DESTRUCTORS_ARRAY_NAME_FROM(union_name)[] = {\
  __VA_ARGS__\
};\
static_assert(sizeof(CASE_DESTRUCTORS_ARRAY_NAME_FROM(union_name))/sizeof(CASE_DESTRUCTOR_TYPEDEF_NAME_FROM(union_name)) == UNION_TAG_ENUM_CASE_NAME(union_name, LENGTH_PLUS_ONE) - 1, "The destructor array must contain exactly the same amount of destructors as elements in the union");

#define UNION_DESTROY_FUNCTION(union_name)\
UNION_DESTROY_FUNCTION_SIGNATURE(union_name) {\
    size_t length = UNION_TAG_ENUM_CASE_NAME(union_name, LENGTH_PLUS_ONE) - 1;\
    size_t tag = ptr.tag;\
    if (tag <= expression_node_tag_null || tag >= length) {\
        return;\
    }\
    if (!ptr.ptr) {\
        return;\
    }\
    expression_node_destructor destructor = CASE_DESTRUCTORS_ARRAY_NAME_FROM(union_name)[tag - 1];\
    if (!destructor) {\
        destructor = free;\
    }\
    destructor(ptr.ptr);\
}

#define UNION_SWITCH_TAG(ptr_expr) switch (ptr_expr.tag)
#define UNION_INVALID_CASES(union_name)\
        case UNION_TAG_ENUM_CASE_NAME(union_name, null):\
        case UNION_TAG_ENUM_CASE_NAME(union_name, LENGTH_PLUS_ONE): \
        case UNION_TAG_ENUM_CASE_NAME(union_name, LIMIT): 

#define UNION_BREAK_INVALID_CASES(union_name)\
        UNION_INVALID_CASES(union_name) assert(0); break;
#define UNION_CASE(union_name, case_name, target_ptr_name, pointer_expr, body)\
        case UNION_TAG_ENUM_CASE_NAME(union_name, case_name): {\
            UNION_CASE_STRUCT_TYPENAME(union_name, case_name) *target_ptr_name = pointer_expr.ptr;\
            { body; }\
        } break;
