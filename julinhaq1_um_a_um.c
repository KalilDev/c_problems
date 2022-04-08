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

const int FLAG = 9999;

typedef struct {
    int numero_lido;
    bool era_flag;
} valor_ou_flag_t;

valor_ou_flag_t ler_numero_e_virgula_ou_ler_flag() {
    valor_ou_flag_t resultado = {.era_flag = false};
    resultado.numero_lido = read_int_or_fail(decimal);
    if (resultado.numero_lido == FLAG) {
        resultado.era_flag = true;
    } else {
        scanf(",");
    }
    return resultado;
}

typedef struct {
    size_t numeros_pares;
    size_t numeros_impares;
} pares_ou_impares_result;

bool e_par(int numero) {
    return numero % 2 == 0;
}

pares_ou_impares_result ler_numeros_e_ver_pares_ou_impares() {
    pares_ou_impares_result result = {0, 0};
    while (true) {
        valor_ou_flag_t resultado_da_leitura = ler_numero_e_virgula_ou_ler_flag();
        if (resultado_da_leitura.era_flag) {
            break;
        }

        if (e_par(resultado_da_leitura.numero_lido)) {
            result.numeros_pares++;
        } else {
            result.numeros_impares++;
        }
    }
    return result;
}

int main() {
    pares_ou_impares_result result = ler_numeros_e_ver_pares_ou_impares();
    printf("Há %li numeros pares e %li numeros impares\n", result.numeros_pares, result.numeros_impares);
}
