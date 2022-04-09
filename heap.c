#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "lib/quest.h"
#include "lib/conversion.h"
#include "lib/conversion_quest.h"
#include "lib/duration.h"
#include "lib/vec_math.h"
#include "lib/arr_math.h"
#include "lib/io_wrapper.h"

typedef struct {
    void* pointer;
    bool error;
} safe_malloc_ret_t;

safe_malloc_ret_t safe_malloc(size_t size) {
    safe_malloc_ret_t result = {.error=false};
    result.pointer = malloc(size);
    if (result.pointer == NULL) {
        result.error = true;
    }
    return result;
}

void *safe_malloc_or_fail(size_t size)  {
    safe_malloc_ret_t malloc_result = safe_malloc(size);
    if (malloc_result.error) {
        fprintf(stderr, "Não foi possivel alocar %lu bytes!\n", size);
        exit(EXIT_FAILURE);
        // unreachable
        assert(false);
    }
    return malloc_result.pointer;
}

#define __HEAP_ALLOC_CLEANUP_LABEL(name) free_HEAP_ALLOC_BLOCK_ ## name
#define HEAP_ALLOC_BLOCK(type, name, count, block, allocator) \
{ \
    /* Essa label é declarada no comeco do bloco com o atributo
    __label__ para definir que ela só é valida dentro do bloco
    em que estamos */ \
    __label__ __HEAP_ALLOC_CLEANUP_LABEL(name);\
    type *name = allocator(((size_t)count) * sizeof(type));\
    block;\
    __HEAP_ALLOC_CLEANUP_LABEL(name):\
    free(name);\
};

#define HEAP_SAFE_MALLOC_BLOCK(type, name, count, block) HEAP_ALLOC_BLOCK(type, name, count, block, safe_malloc_or_fail)
#define HEAP_MALLOC_BLOCK(type, name, count, block) HEAP_ALLOC_BLOCK(type, name, count, block, malloc)
#define HEAP_CALLOC_BLOCK(type, name, count, block) HEAP_ALLOC_BLOCK(type, name, count, block, calloc)
#define EXIT_HEAP_ALLOC_BLOCK(name) goto __HEAP_ALLOC_CLEANUP_LABEL(name);

START_QUESTS

// Hello world basico
QUEST(1, {
    puts("Olá mundo!");
})

// Stack alloc basico
QUEST(2, {
    int nums[3];

    promt_ints_or_fail(nums, 3, decimal);
    int sum = sum_of_ints(nums, 3);
    
    printf("A soma deles é %i\n", sum);
})

// Heap alloc basico
QUEST(3, {
    int quantos_numeros = promt_dint_or_fail();
    
    int *nums = safe_malloc_or_fail(quantos_numeros * sizeof(int));

    promt_ints_or_fail(nums, quantos_numeros, decimal);
    int sum = sum_of_ints(nums, quantos_numeros);
    
    free(nums);
    // Tente acessar nums agora, vai dar algum erro/comportamento indefinido
    //nums[1]
    
    printf("A soma deles é %i\n", sum);
})

// Heap alloc em bloco separado para evitar problema anterior
QUEST(4, {
    int quantos_numeros = promt_dint_or_fail();
    
    int sum;
    // Bloco em que `nums` existe no heap e somos o dono dele.
    // 
    // Por que separar esse bloco do resto da funcao?
    //
    // Porque assim é impossivel agt tentar acessar `nums`
    // quando ele for NULL causando uma NPE (Caso ele fosse
    // inicializado como NULL antes), e é impossivel agt usar ele
    // após liberar, o que ocasionaria num erro de use after
    // free indetectavel, causando comportamento indefinido no
    // runtime.
    {
        int *nums = safe_malloc_or_fail(quantos_numeros * sizeof(int));

        promt_ints_or_fail(nums, quantos_numeros, decimal);
        // E se eu n quiser retornar a soma caso o primeiro numero seja 10?
        sum = sum_of_ints(nums, quantos_numeros);
        
        free(nums);
    }
    printf("A soma deles é %i\n", sum);
})

// Heap alloc em bloco separado com saída antes do fim do bloco
QUEST(5, {
    int quantos_numeros = promt_dint_or_fail();
    
    int sum;
    {
        int *nums = safe_malloc_or_fail(quantos_numeros * sizeof(int));
        promt_ints_or_fail(nums, quantos_numeros, decimal);

        const int primeiro_numero_proibido = 10;
        if (quantos_numeros > 0 && nums[0] == primeiro_numero_proibido) {
            puts("O primeiro numero não pode ser 10!");
            sum = -1;
            // Para sair agt tem que limpar, entao ao inves de limpar e usar um
            // else pra tudo, agt vai até o final do bloco, que é onde limpamos.
            goto cleanup_nums;
        }
        assert(nums[0] != primeiro_numero_proibido);
        
        sum = sum_of_ints(nums, quantos_numeros);
        
        // a label que indica onde limpamos os nums
        cleanup_nums:
        free(nums);
    }
    printf("A soma deles é %i\n", sum);
})
// Heap alloc em bloco separado com macro
QUEST(6, {
    int quantos_numeros = promt_dint_or_fail();
    
    int sum;
    HEAP_SAFE_MALLOC_BLOCK(int, nums, quantos_numeros, {
        promt_ints_or_fail(nums, quantos_numeros, decimal);
        sum = sum_of_ints(nums, quantos_numeros);
    })
    printf("A soma deles é %i\n", sum);
})

// Heap alloc em bloco separado com macro e saída anterior
QUEST(7, {
    int quantos_numeros = promt_dint_or_fail();
    
    int sum;
    HEAP_SAFE_MALLOC_BLOCK(int, nums, quantos_numeros, {
        promt_ints_or_fail(nums, quantos_numeros, decimal);

        const int primeiro_numero_proibido = 10;
        if (quantos_numeros > 0 && nums[0] == primeiro_numero_proibido) {
            puts("O primeiro numero não pode ser 10!");
            sum = -1;
            // nos nao podemos sair sem limpar, entao para sair usamos esse
            // outro macro que definimos. Ele usa goto poro baixo dos panos.
            EXIT_HEAP_ALLOC_BLOCK(nums);
        }
        assert(nums[0] != primeiro_numero_proibido);

        sum = sum_of_ints(nums, quantos_numeros);
    })
    printf("A soma deles é %i\n", sum);
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
