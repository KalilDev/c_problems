#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "lib/arr_math.h"
#include "lib/quest.h"
#include "lib/conversion.h"
#include "lib/duration.h"
#include "lib/fallible.h"
#include "lib/io_wrapper.h"
#include "lib/vec_math.h"

typedef intptr_t vm_size_t;
typedef intptr_t vm_ptr_t;
typedef unsigned char vm_instruction_arg_count_t;

typedef struct __attribute__((__packed__)) {
    int64_t _vals[2];
} vm_i128;
typedef int64_t vm_i64;
typedef int32_t vm_i32;
typedef int16_t vm_i16;
typedef int8_t vm_i8;

#define REGISTER_64_32_16_8_8(name_64,name_32,name_16,name_8_1,name_8_0)\
    union {\
        struct {\
            union {\
                struct {\
                    union {\
                        struct {\
                            vm_i8 name_8_0;\
                            vm_i8 name_8_1;\
                        };\
                        vm_i16 name_16;\
                    };\
                    vm_i16 __PADDING_FROM_ ## name_32 ## _to_ ## name_16;\
                };\
                vm_i32 name_32;\
            };\
            vm_i32 __PADDING_FROM_ ## name_64 ## _to_ ## name_32;\
        };\
        vm_i64 name_64;\
    };
#define REGISTER_64_32_16_8(name_64,name_32,name_16,name_8) REGISTER_64_32_16_8_8(name_64,name_32,name_16,__PADDING_FROM_ ## name_16 ## _to_ ## name_8,name_8)

typedef struct __attribute__((__packed__)) x86_64_general_purpose_registers {
    REGISTER_64_32_16_8_8(rax, eax, ax, ah, al);
    REGISTER_64_32_16_8_8(rbx, ebx, bx, bh, bl);
    REGISTER_64_32_16_8_8(rcx, ecx, cx, ch, cl);
    REGISTER_64_32_16_8_8(rdx, edx, dx, dh, dl);
    REGISTER_64_32_16_8(rbp, ebp, bp, bpl);
    REGISTER_64_32_16_8(rsl, esl, si, sll);
    REGISTER_64_32_16_8(rdl, edl, di, dll);
    REGISTER_64_32_16_8(rsp, esp, sp, spl);
    REGISTER_64_32_16_8(r8, r8d, r8w, r8b);
    REGISTER_64_32_16_8(r9, r9d, r9w, r9b);
    REGISTER_64_32_16_8(r10, r10d, r10w, r10b);
    REGISTER_64_32_16_8(r11, r11d, r11w, r11b);
    REGISTER_64_32_16_8(r12, r12d, r12w, r12b);
    REGISTER_64_32_16_8(r13, r13d, r13w, r13b);
    REGISTER_64_32_16_8(r14, r14d, r14w, r14b);
    REGISTER_64_32_16_8(r15, r15d, r15w, r15b);
} x86_64_general_purpose_registers_t;

typedef struct {
    vm_ptr_t stack_pointer;

} vm_cpu_state_t;

typedef enum {
    push,
    pop,
} opcode_t;

const vm_instruction_arg_count_t opcode_arg_count[] = {
    1,
    0,
};



typedef struct {
    opcode_t opcode;
    vm_instruction_arg_count_t arg_count;
    
} instruction_kind_t;