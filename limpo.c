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

START_QUESTS

// Hello world basico
QUEST(1, {
    puts("Olá mundo!");
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
