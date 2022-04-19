#include <stdint.h>
#include <stdio.h>
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
    jump,
    load,
    add_i8,
    add_i16,
    add_i32,
    add_i64,
    sub_i8,
    sub_i16,
    sub_i32,
    sub_i64,
    div_i8,
    div_i16,
    div_i32,
    div_i64,
    mod_i8,
    mod_i16,
    mod_i32,
    mod_i64,
    cmp_8,
    cmp_16,
    cmp_32,
    cmp_64,
    syscall,
} opcode_t;

/* Write N bytes of BUF to FD.  Return the number written, or -1.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t write (int __fd, const void *__buf, size_t __n) __wur
    __attr_access ((__read_only__, 2, 3));

long int vm_kernel(long int sysnum, ...) {
    asm volatile (
        "int $0x80\n"
        "ret\n");
}


vm_size_t vm_write(int fd, char* chars, vm_size_t n) {
    if (chars == NULL) {
        return 0;
    }
    long int result = vm_kernel(1, fd, chars, n);
    if (result < 0) {
        // error
        return 0;
    }
    return result;
}

vm_size_t vm_strlen(char* chars) {
    if (chars == NULL) {
        return 0;
    }
    vm_size_t len = 0;
    for (char *it, curr = *it; curr != '\0'; it++, curr = *it) {
        len++;
    }
    return len;
}

void vm_fputs(char* chars, FILE *file) {
    int fd = fileno(file);
    vm_size_t len = vm_strlen(chars);
    vm_write(fd, chars, len);
}

void puts_digit_i8(vm_i64 i, FILE *file) {
    //assert(i < 10);
    char chars[2] = {
        '0'+i,
        '\0'
    };
    vm_fputs(chars, file);
}

void vm_puts_i64(vm_i64 i, FILE *fd) {
    if (i < 10) {
        puts_digit_i8(i, fd);
        return;
    }
    puts_digit_i8(i % 10, fd);
    return vm_puts_i64(i / 10, fd);
}

int main(int argc, char** argv) {
    for (vm_i64 i =0; i < 10; i++) {
        vm_fputs("Hello at ", stdout);
        vm_puts_i64(i, stdout);
        vm_fputs("\n", stdout);
    }
    return 0;
}

const vm_instruction_arg_count_t opcode_arg_count[] = {
    1,
    0,
};



typedef struct {
    opcode_t opcode;
    vm_instruction_arg_count_t arg_count;
    
} instruction_t;