#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "lib/union.h"

UNION_TAG_ENUM(expression_node, UNION_TAG_ENUM_CASE_NAME(expression_node, binary_op), UNION_TAG_ENUM_CASE_NAME(expression_node, int_literal),)

UNION_POINTER_DEF(expression_node);

UNION_NULL_POINTER(expression_node);

UNION_CASE_STRUCT(expression_node, binary_op,
    expression_node_ptr_t left;
    // TODO
    int operation;
    expression_node_ptr_t right;
)

UNION_CASE_STRUCT(expression_node, int_literal,
    // TODO
    int value;
)
UNION_CREATE_CASE_FUNCTION_SIGNATURE(expression_node, binary_op, (expression_node_ptr_t left,expression_node_ptr_t right, int operation));

UNION_CREATE_CASE_FUNCTION(expression_node, binary_op, (expression_node_ptr_t left,expression_node_ptr_t right, int operation), .left = left, .right = right, .operation = operation);

UNION_CREATE_CASE_FUNCTION_SIGNATURE(expression_node, int_literal, (int value));
UNION_CREATE_CASE_FUNCTION(expression_node, int_literal, (int value), .value = value);


UNION_NEW_CASE_FUNCTION_SIGNATURE(expression_node, binary_op, (expression_node_ptr_t left,expression_node_ptr_t right, int operation));
UNION_NEW_CASE_FUNCTION(expression_node, binary_op, malloc, (expression_node_ptr_t left,expression_node_ptr_t right, int operation), left, right, operation);

UNION_NEW_CASE_FUNCTION_SIGNATURE(expression_node, int_literal, (int value));
UNION_NEW_CASE_FUNCTION(expression_node, int_literal, malloc, (int value), value);

UNION_DESTROY_FUNCTION_SIGNATURE(expression_node);

UNION_DESTROY_CASE_FUNCTION_SIGNATURE(expression_node, binary_op);
UNION_DESTROY_CASE_FUNCTION(expression_node, binary_op, {
    printf("Destroying binary op!");
    destroy_expression_node(ptr->left);
    destroy_expression_node(ptr->right);
})

UNION_CASE_DESTRUCTOR_TYPEDEF(expression_node);

UNION_CASE_DESTRUCTORS_ARRAY(expression_node,
  destroy_expression_node_binary_op,
  NULL,
)

UNION_DESTROY_FUNCTION(expression_node)

// top level statement: function decl (type: int), name (main), args ([]) body:
// statement: variable_declaration: type (expression_node_ptr_t), name (pointer)
// statement: variable_declaration: type (int), name (choice)
// statement: call function: function_pointer scanf, args: const char** ("%d"), addressing_operator(operator: '&', expression: variable: choice)
// if statement


int main() {
    UNION_POINTER_STRUCT_TYPENAME(expression_node) pointer;

    int choice;
    scanf("%d", &choice);
    if (choice) {
        pointer = new_expression_node_int_literal(choice);
    } else {
        pointer = new_expression_node_binary_op(null_expression_node_ptr, null_expression_node_ptr, 0);
    }

    
    UNION_SWITCH_TAG(pointer) {
        UNION_BREAK_INVALID_CASES(expression_node)
        UNION_CASE(expression_node, binary_op, value, pointer, {
            printf("We have an binary op at %p", value);
        })
        UNION_CASE(expression_node, int_literal, value, pointer, {
            printf("We have an int literal at %p with the value, %i", value, value->value);
        })
    }
    printf("Gonna destroy the expr node\n");
    destroy_expression_node(pointer);
    printf("Destroyed!\n");
}