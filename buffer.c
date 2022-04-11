#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#include "lib/quest.h"
#include "lib/conversion.h"
#include "lib/conversion_quest.h"
#include "lib/duration.h"
#include "lib/vec_math.h"
#include "lib/arr_math.h"
#include "lib/io_wrapper.h"

#ifndef NDEBUG
    #define BUFF_T_USE_BUFF_PTR_T
    #define BUFF_T_UNINITIALIZED_PATTERN
    #define BUFF_T_STORE_VERSION
    #define BUFF_BUFF_PTR_T_CHECK_ELEMENT_SIZE
#endif


typedef unsigned char byte;
const unsigned char bits_in_byte = 8;

#ifdef BUFF_T_USE_BUFF_PTR_T
    #ifndef BUFF_T_STORE_VERSION
    static_assert(0, "To use buff_ptr_t, BUFF_T_STORE_VERSION must be defined")
    #endif
#endif

static_assert(sizeof(byte) == 1, "Expected byte to be one byte!");

static_assert(UINT8_MAX > sizeof(size_t)*8, "The bit width of the size_t must fit in an uint8");
size_t size_t_next_pow_two(size_t num) {
    uint8_t num_next_log2 = 0;
    {
        size_t acc = num;
        while (acc != 0) {
            // shift right. Also subtract the mod2, so that we do not underflow
            acc = (acc - acc % 2) >> 1;
            num_next_log2++;
        }   
    }
    // Ensure we do not overflow
    assert(num_next_log2 < sizeof(size_t)*bits_in_byte);
    size_t next_pow_of_two = (size_t)1 << num_next_log2;
    return next_pow_of_two;
}
size_t size_t_num_or_next_pow_two(size_t num) {
    size_t next_pow_of_two = size_t_next_pow_two(num);
    if (next_pow_of_two > 2 && (next_pow_of_two >> 1) == num) {
        return num;
    }
    return next_pow_of_two;
}

typedef struct buff {
    byte *storage;
    size_t length;
    size_t capacity;
#ifdef BUFF_T_STORE_VERSION
    size_t storage_version;
#endif
} buff_t;


byte unused_capacity_pattern[4] = {0xDE,0xAD,0xBE,0xEF};
const size_t unused_capacity_pattern_length = 4;

buff_t buff_create() {
    buff_t result = {
        .storage=NULL,
        .length = 0,
        .capacity = 0,
    #ifdef BUFF_T_STORE_VERSION
        .storage_version = 0,
    #endif
    };
    return result;
}


#ifdef BUFF_T_USE_BUFF_PTR_T
typedef struct {
    buff_t *buff;
    size_t i;
#ifdef BUFF_BUFF_PTR_T_CHECK_ELEMENT_SIZE
    size_t size_of_element;
#endif
} buff_ptr_t;

buff_ptr_t buff_ptr_to_ith_element(buff_t *buff, size_t i, size_t size_of_element) {
    buff_ptr_t result = {
        .buff = buff,
        .i = i,
#ifdef BUFF_BUFF_PTR_T_CHECK_ELEMENT_SIZE
        .size_of_element = size_of_element,
#endif
    };
    return result;
}
buff_ptr_t buff_ptr_to_ith_byte(buff_t *buff, size_t i) {
    return buff_ptr_to_ith_element(buff, i, sizeof(byte));
}
void *buff_ptr_to_ptr_checked(buff_ptr_t ptr, size_t size_of_element) {
    #ifdef BUFF_BUFF_PTR_T_CHECK_ELEMENT_SIZE
    assert(ptr.size_of_element == size_of_element);
    #endif
    return ptr.buff->storage+ptr.i;
}
void *buff_ptr_to_ptr(buff_ptr_t ptr) {
    return ptr.buff->storage+ptr.i;
}
#define buff_ptr_to_type(buff, type, index) buff_ptr_to_ith_element(buff, sizeof(type)*index, sizeof(type))
#ifdef BUFF_BUFF_PTR_T_CHECK_ELEMENT_SIZE
    #define buff_ptr_to_ptr_type(buff_ptr, index) (type *)buff_ptr_to_ptr_checked(buff_ptr, sizeof(type))
#else
    #define buff_ptr_to_ptr_type(buff_ptr, index) (type *)buff_ptr_to_ptr(buff_ptr)
#endif
#define buff_ptr_deref_type(buff_ptr, type) *buff_ptr_to_ptr_type(buff_ptr, type)
#endif

void buff_debug_set_uninitialized_to_pattern(buff_t* buffer) {
    size_t unused_capacity = buffer->capacity - buffer->length;
    byte *storage = buffer->storage;
    for (size_t i = buffer->length; i < unused_capacity;i++) {
        byte pattern_piece = unused_capacity_pattern[i%unused_capacity_pattern_length];
        storage[i] = pattern_piece;
    }
}

bool buff_ensure_capacity(buff_t *buffer, size_t desired_capacity) {
    size_t current_capacity = buffer->capacity;
    if (current_capacity >= desired_capacity) {
        return true;
    }
    size_t next_capacity = size_t_num_or_next_pow_two(desired_capacity);
    byte *previous_storage = buffer->storage;
    // Realloc with NULL has the same behavior as malloc, so we can just always realloc
    byte *next_storage = realloc(previous_storage, sizeof(byte) * next_capacity);
    if (!next_storage) {
        goto allocation_failure;
    }
    buffer->storage = next_storage;
    buffer->capacity = next_capacity;
    
    allocation_success:
    assert(buffer->length < buffer->capacity);
    assert(desired_capacity <= buffer->capacity);
    #ifdef BUFF_T_STORE_VERSION
    if (previous_storage != buffer->storage) {
        buffer->storage_version++;
    }
    #endif
    #ifdef BUFF_T_UNINITIALIZED_PATTERN
    buff_debug_set_uninitialized_to_pattern(buffer);
    #endif
    return true;

    allocation_failure:
    return false;
}

void buff_debug_fprintf(buff_t* buff, FILE * restrict file) {
    fprintf(file, "buffer_t {.length=%lu, .capacity=%lu", buff->length, buff->capacity);
    #ifdef BUFF_T_STORE_VERSION
    fprintf(file, ", .storage_version=%lu", buff->storage_version);
    #endif
    fprintf(file, ", .storage=\n");
    for (size_t i = 0; i < buff->capacity; i++) {
        fprintf(file, "%x", buff->storage[i]);
    }
    fprintf(file, "\n}");
}

typedef struct {
    bool failure;
    buff_t buff;
} maybe_buff_t;

maybe_buff_t buff_create_with_capacity(size_t initial_capacity) {
    maybe_buff_t result = {
        .buff = buff_create()
    };
    result.failure = !buff_ensure_capacity(&result.buff, initial_capacity);
    return result;
}

void buff_destroy(buff_t *buffer) {
    if (buffer->storage == NULL) {
        return;
    }
    free(buffer->storage);
    buffer->capacity=0;
    buffer->length=0;
}

bool buff_append_from(buff_t *buffer, void *from, size_t count) {
    size_t curr_length = buffer->length;
    size_t needed_capacity = curr_length+count;
    if (!buff_ensure_capacity(buffer, needed_capacity)) {
        goto failure;
    }
    memcpy(buffer->storage+curr_length, from, count);
    buffer->length+=count;

    return true;

    failure:
    return false;
}

bool buff_set_from(buff_t *buffer, void *from, size_t length) {
    size_t curr_length = buffer->length;
    if (!buff_ensure_capacity(buffer, length)) {
        goto failure;
    }
    memcpy(buffer->storage, from, length);
    buffer->length = length;

    return true;

    failure:
    return false;
}

bool buff_append_from_overlapping(buff_t *buffer, void *from, size_t count) {
    size_t curr_length = buffer->length;
    size_t needed_capacity = curr_length+count;
    if (!buff_ensure_capacity(buffer, needed_capacity)) {
        goto failure;
    }
    memmove(buffer->storage+curr_length, from, count);
    
    return true;

    failure:
    return false;
}

bool buff_set_from_overlapping(buff_t *buffer, void *from, size_t length) {
    size_t curr_length = buffer->length;
    if (!buff_ensure_capacity(buffer, length)) {
        goto failure;
    }
    memmove(buffer->storage, from, length);
    buffer->length = length;

    return true;

    failure:
    return false;
}

START_QUESTS

// Hello world basico
QUEST(1, {
    size_t num = 256;
    size_t next_pow = size_t_next_pow_two(num);
    printf("num: %lu, next_pow_two: %lu\n", num, next_pow);
})

// Criar buffer que cabe `Hello world!\0`, inserir `Hello world!\0` e liberar buffer
QUEST(2, {
    const char hello_world[] = "Hello world!";
    buff_t buffer = buff_create();
    bool succeeded = buff_append_from(&buffer, (void *)hello_world, sizeof(hello_world)/sizeof(char));
    assert(succeeded);
    buff_destroy(&buffer);
    puts("Deu certo");
})
// Criar buffer que cabe `Hello world!\0`, inserir `Hello world!\0` imprimir o buffer inteiro e liberar buffer
QUEST(3, {
    const char hello_world[] = "Hello world!";
    buff_t buffer = buff_create();
    bool succeeded = buff_append_from(&buffer, (void *)hello_world, sizeof(hello_world)/sizeof(char));
    buff_debug_fprintf(&buffer, stdout);
    assert(succeeded);
    buff_destroy(&buffer);
    puts("Deu certo");
})
// Criar buffer que cabe `Hello world!Hello world!\0`, inserir `Hello world!` e `Hello world!\0` imprimir o buffer inteiro e liberar buffer
QUEST(4, {
    const char hello_world[] = "Hello world!";
    buff_t buffer = buff_create();
    {
        bool succeeded = buff_append_from(&buffer, (void *)hello_world, sizeof(hello_world)/sizeof(char) - 1 /* dont add \0 */);
        assert(succeeded);
        buff_debug_fprintf(&buffer, stdout);
    }
    {
        bool succeeded = buff_append_from(&buffer, (void *)hello_world, sizeof(hello_world)/sizeof(char));
        assert(succeeded);
        buff_debug_fprintf(&buffer, stdout);
    }
    buff_debug_fprintf(&buffer, stdout);
    buff_debug_set_uninitialized_to_pattern(&buffer);
    buff_debug_fprintf(&buffer, stdout);
    buff_debug_set_uninitialized_to_pattern(&buffer);
    printf("%s", buffer.storage);
    buff_destroy(&buffer);
    puts("Deu certo");
})
// Criar buffer que cabe exatamente 128 elementos
QUEST(5, {
    maybe_buff_t maybe_buffer = buff_create_with_capacity(99999999999999999);
    assert(!maybe_buffer.failure);
    buff_t buffer = maybe_buffer.buff;
    buff_debug_fprintf(&buffer, stdout);
    buff_destroy(&buffer);
    puts("Deu certo");
})

END_QUESTS

void run_questions(unsigned int* questions, size_t count) {
    for (size_t i = 0; i < count; i++) {
        unsigned int question = questions[i];
        RUN_QUEST(question);
    }
}

typedef enum {
    ps_success,
    ps_e_zero,
    ps_e_negative,
    ps_e_not_a_number,
} parse_strings_to_non_zero_uints_result;

parse_strings_to_non_zero_uints_result parse_strings_to_non_zero_uints(unsigned int *out, char** strings, size_t count) {
    for (size_t i = 0; i < count; i++) {
        char *string = strings[i];
        int arg_int;
        if (sscanf(string, "%d", &arg_int) != 1) {
            return ps_e_not_a_number;
        }
        if (arg_int < 0) {
            return ps_e_negative;
        }
        if (arg_int == 0) {
            return ps_e_zero;
        }
        unsigned int unsigned_arg = arg_int;
        out[i] = unsigned_arg;
    }
    return ps_success;
}

int all_tests_runner() {
    puts("Todas as questões serão rodadas!");
    RUN_ALL_QUESTS();
    return 0;
}

int print_usage() {
    puts("Uso:");
    puts("Nenhum argumento rodará todos os testes.");
    puts("Uma lista de numeros não negativos pode ser usada para rodar os respectivos testes");
    return 0;
}

int question_list_runner(int argc, char** argv) {
    int question_count = argc - 1;
    char** start_of_questions = argv+1;

    if (question_count == 0) {
        puts("Nenhuma questão será rodada");
        return 0;
    }

    unsigned int* questoes_a_serem_rodadas = malloc(question_count*sizeof(unsigned int));
    if (!questoes_a_serem_rodadas) {
        fputs("Não há memória!\n", stderr);
        goto cleanup_and_return_failure;
    }
    
    switch (parse_strings_to_non_zero_uints(questoes_a_serem_rodadas, start_of_questions, question_count)) {
        case ps_e_zero:
        case ps_e_negative:
            fputs("Os numeros de questão são não negativos e comecam em 1\n", stderr);
            goto cleanup_and_return_failure;
        case ps_e_not_a_number:
            fputs("Os argumentos devem ser numeros não negativos ou -h ou --h, para o menu de ajuda!\n", stderr);
            goto cleanup_and_return_failure;
        case ps_success: break;
        default:
            fputs("Erro desconhecido!\n", stderr);
            goto cleanup_and_return_failure;
    }

    if (question_count > 1) {
        printf("%i questões serão rodadas!\n", question_count);
    }
    run_questions(questoes_a_serem_rodadas, question_count);

    free(questoes_a_serem_rodadas);
    return 0;

    cleanup_and_return_failure:
    free(questoes_a_serem_rodadas);
    return 1;
}

bool is_help_argument(char* arg) {
    return (strcmp(arg, "-h") == 0) || (strcmp(arg, "--h") == 0);
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return all_tests_runner();
    }

    if (is_help_argument(argv[1])) {
        return print_usage();
    }

    return question_list_runner(argc, argv);
}
