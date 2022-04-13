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
#include "lib/fallible.h"

#define SQUARE(num) (num*num)

float area_de_circulo(float raio) {
    return M_PI*SQUARE(raio);
}

float hipotenusa_de_catetos(float a, float b) {
    float a2 = SQUARE(a);
    float b2 = SQUARE(b);
    return sqrtf(a2+b2);
}

unsigned char ascii_to_uppercase(unsigned char input_char) {
    const unsigned char a_lowercase = 'a';
    const unsigned char a_uppercase = 'A';
    const unsigned char z_lowercase = 'z';
    if (input_char < a_lowercase || input_char > z_lowercase) {
        return input_char;
    }
    
    unsigned char alphabet_i = input_char-a_lowercase;

    return a_uppercase + alphabet_i;
}

unsigned int invert_number(unsigned int number) {
    unsigned int result = 0;
    unsigned int acc = number;
    while (acc != 0) {
        unsigned int rightmost_digit = acc % 10;
        acc/=10;
        result*=10;
        result+=rightmost_digit;
    }
    return result;
}

size_t number_width(unsigned int number) {
    size_t width = 0;
    while (number != 0) {
        number/=10;
        width++;
    }
    return width;
}

bool number_to_digits(unsigned int number, unsigned char *digits, size_t count) {
    size_t width = number_width(number);
    if (width > count) {
        return false;
    }
    for (size_t i = width; i >0; i--) {
        unsigned int rightmost_digit = number % 10;
        number /= 10;
        digits[i - 1] = rightmost_digit;
    }
    for (size_t i=width; i < count; i++) {
        digits[i] = 0;
    }
    return true;
}

START_QUESTS

QUEST(1, {
    int number = prompt_dint_or_fail();
    printf("%d\n", number);
})

QUEST(2, {
    float number = prompt_float_or_fail();
    printf("%f\n", number);
})

QUEST(3, {
    int nums[3];
    prompt_dints_or_fail(nums, 3);
    int sum = sum_of_ints(nums, 3);
    printf("A soma deles é %i\n", sum);
})

QUEST(4, {
    float numero = prompt_float_or_fail();

    printf("O quadrado dele é %f\n", SQUARE(numero));
})

QUEST(5, {
    float numero = prompt_float_or_fail();

    printf("O quinto dele é %f\n", numero/5.0);
})

TEMPERATURE_CONVERSION_PAIR_QUESTION(6, 7, fahrenheit, celsius);

TEMPERATURE_CONVERSION_PAIR_QUESTION(8, 9, kelvin, celsius);

VELOCITY_CONVERSION_PAIR_QUESTION(10, 11, m_s, km_h);

DISTANCE_CONVERSION_PAIR_QUESTION(12, 13, mile, km);

ANGLE_CONVERSION_PAIR_QUESTION(14, 15, rad, deg);

LENGTH_CONVERSION_PAIR_QUESTION(16, 17, cm, pol);

VOLUME_CONVERSION_PAIR_QUESTION(18, 19, lt, m_3);

MASS_CONVERSION_PAIR_QUESTION(20, 21, kg, lb);

LENGTH_CONVERSION_PAIR_QUESTION(22, 23, m, yd);

AREA_CONVERSION_PAIR_QUESTION(24, 25, m_2, ac);

AREA_CONVERSION_PAIR_QUESTION(26, 27, m_2, hec);

QUEST(28, {
    float nums[3];
    prompt_floats_or_fail(nums, 3);
    float sum_of_squares = sum_of_squares_of_floats(nums, 3);
    printf("A soma do quadrado deles é %f\n", sum_of_squares);
})

QUEST(29, {
    float nums[4];
    prompt_floats_or_fail(nums, 4);
    float avg = avg_of_floats(nums, 4);
    printf("A média das notas é %f\n", avg);
})

QUEST(30, {
    puts("Digite um valor em reais e a cotacao do dólar");
    float reais = read_float_or_fail();
    float cotacao_dolar = read_float_or_fail();

    float em_dolares=reais/cotacao_dolar;

    printf("O valor em dólares é %f\n", em_dolares);
})

QUEST(31, {
    int num = prompt_dint_or_fail();
    int antecessor = num-1;
    int sucessor = num+1;

    printf("O antecessor é %i e o sucessor é %i\n", antecessor, sucessor);
})

QUEST(32, {
    int num = prompt_dint_or_fail();
    int antecessor_do_dobro = (num*2)-1;
    int sucessor_do_triplo = (num*3)+1;

    int result = sucessor_do_triplo + antecessor_do_dobro;

    printf("O resultado é %i\n", result);
})


QUEST(33, {
    float lado = prompt_float_or_fail();

    printf("A área do quadrado é %f\n", SQUARE(lado));
})

QUEST(34, {
    float raio = prompt_float_or_fail();

    printf("A área do círculo é %f\n", area_de_circulo(raio));
})

QUEST(35, {
    float catetos[2];
    prompt_floats_or_fail(catetos, 2);


    printf("A hipotenusa é %f\n", hipotenusa_de_catetos(catetos[0], catetos[1]));
})

QUEST(36, {
    puts("Digite a altura e o raio");
    float altura = read_float_or_fail();
    float raio = read_float_or_fail();
    
    float area_base =  area_de_circulo(raio);
    float volume = area_base*altura;

    printf("A volume é %f\n", volume);
})

QUEST(37, {
    float valor = prompt_float_or_fail();
    const float desconto = 0.12;
    float valor_final = valor - (valor*desconto);
    
    printf("A valor com desconto é %f\n", valor_final);
})

QUEST(38, {
    float salario = prompt_float_or_fail();
    const float aumento = 0.25;

    float salario_final = salario + (salario*aumento);
    
    printf("O salário com o aumento é %f\n", salario_final);
})

QUEST(39, {
    const float quantia = 780000.0;
    const float fator_primeiro = 0.46;
    const float fator_segundo = 0.32;
    const float fator_terceiro = 1.0-fator_primeiro-fator_segundo;

    float primeiro = quantia*fator_primeiro;
    float segundo = quantia*fator_segundo;
    float terceiro = quantia*fator_terceiro;
    
    printf("O valor recebido pelo primeiro, segundo e terceiro foi, respectivamente, %f; %f; %f\n", primeiro, segundo, terceiro);
})

QUEST(40, {
    const float diaria = 30.0;
    const float fator_com_imposto = 1.08;
    int dias = prompt_dint_or_fail();

    float diarias = diaria * dias;
    float diarias_com_imposto = diarias/fator_com_imposto;
    
    printf("O valor a ser pago com o imposto é %f\n", diarias_com_imposto);
})

QUEST(41, {
    float valor_hora = prompt_float_or_fail();
    int horas = prompt_dint_or_fail();
    
    const float fator = 1.1;

    float final = horas*valor_hora*fator;

    printf("O valor a ser pago é %f\n", final);
})

QUEST(42, {
    float salario_base = prompt_float_or_fail();
    
    const float fator_gratificado = 1.05;
    const float fator_depois_imposto = 0.93;

    float final = salario_base*fator_gratificado*fator_depois_imposto;

    printf("O valor do salário é %f\n", final);
})

QUEST(43, {
    float valor_base = prompt_float_or_fail();
    
    const float fator_com_10_desconto = 0.9;
    const float um_terco = 1.0/3.0;
    const float fator_comissao = 0.05;

    float valor_desconto = valor_base*fator_com_10_desconto;
    float valor_parcela = valor_base*um_terco;
    float comissao_vendedor_vista = valor_desconto*fator_comissao;
    float comissao_vendedor_parcelado = valor_base*fator_comissao;

    printf("O valor com desconto é %f, o de cada parcela é %f, o da comissao à vista é %f e o da comissao parcelada é %f\n", valor_desconto, valor_parcela, comissao_vendedor_vista, comissao_vendedor_parcelado);
})

QUEST(44, {
    puts("Digite a altura do degrau e da escada");
    float altura_degrau = read_float_or_fail();
    float altura_escada = read_float_or_fail();
    
    float total_degraus = altura_escada/altura_degrau;
    int degraus = ceilf(total_degraus);
    printf("Deverá subir %i degraus\n", degraus);
})

QUEST(45, {
    int character = prompt_dint_or_fail();
    if (character > 0xff) {
        puts("Caractere n é ascii!");
    } else {
        unsigned char uppercase = ascii_to_uppercase(character);
        printf("O caractere %c em uppercase é %c\n", character, uppercase);
    }
})

QUEST(46, {
    int number = prompt_dint_or_fail();
    if (number<100 || number > 999) {
        puts("Numero não tem 3 digitos!");
    } else {
        int inverted_number = invert_number(number);
        printf("O numero invertido é %i\n", inverted_number);
    }
})

QUEST(47, {
    int number = prompt_dint_or_fail();
    if (number<1000 || number > 9999) {
        puts("Numero não tem 4 digitos!");
    } else {
        unsigned char digits[4];
        bool had_enough_space = number_to_digits(number, digits, 4);
        assert(had_enough_space);
        for (size_t i = 0;i < 4; i++) {
            printf("%i\n", digits[i]);
        }
    }
})

QUEST(48, {
    int segundos = prompt_dint_or_fail();
    duration_t duration = duration_from_seconds(segundos);
    unsigned int minutos = duration_as_minutes(duration);
    unsigned int horas =  duration_as_hours(duration);
    printf("%is ou %im ou %ih\n", segundos, minutos, horas);
})

QUEST(49, {
    puts("Digite o inicio em h m s e a duracao em s");
    int horas = read_dint_or_fail();
    int minutos = read_dint_or_fail();
    int segundos = read_dint_or_fail();
    
    duration_t experiment_start = duration_from_components(segundos, minutos, horas);
    int duration_seconds = read_dint_or_fail();

    duration_t experiment_duration = duration_from_seconds(duration_seconds);

    duration_t experiment_end = experiment_start+experiment_duration;

    int end_s;
    int end_m;
    int end_h;

    duration_to_components(experiment_end, &end_s, &end_m, &end_h);
    printf("O final do experimento é às %ih %im %is\n", end_h, end_m, end_s);
})

QUEST(50, {
    puts("Digite ano de nascimento e o ano atual.");
    int idade = read_dint_or_fail();
    int ano_atual = read_dint_or_fail();

    int ano_nascimento = ano_atual - idade;

    printf("O ano de nascimento é %i\n", ano_nascimento);
})

QUEST(51, {
    float coords[2];
    prompt_floats_or_fail(coords, 2);
    vec2f point = vec2f_from_coords(coords);
    float distance = vec2f_distance(point);

    printf("A distancia de (%f, %f) da origem é %f\n", point.x, point.y, distance);
})

QUEST(52, {
    // Poderia ser 3 arrays e usar a versão restrict de divide_floats_to e multiply_floats_to.
    float invested[3];
    prompt_floats_or_fail(invested, 3);
    float valor_loteria = prompt_float_or_fail();

    float invested_total = sum_of_floats(invested, 3);
    // Pegar a fracao de cada investidor e salvar em invested
    divide_floats_to(invested_total, invested, invested, 3);
    // Multiplicar a fracao de cada investidor pelo valor da loteria e salvar em invested
    multiply_floats_to(valor_loteria, invested, invested, 3);


    printf("O primeiro ganhará %f, o segundo ganhará %f e o terceiro ganhará %f\n", invested[0], invested[1], invested[2]);
})

QUEST(53, {
    float dimensions[2];
    prompt_floats_or_fail(dimensions, 2);
    float preco_metro = prompt_float_or_fail();
    
    float area = dimensions[0]*dimensions[1];
    float preco = area/preco_metro;


    printf("O preco pra cercar o terreno é %f\n", preco);
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
