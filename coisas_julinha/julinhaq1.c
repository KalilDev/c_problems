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

const size_t TAMANHO_DE_NUMEROS = 99999;
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

// em sucesso: retorna numero de coisas que leu
// em falha: retorna -1
int ler_numeros(int numeros[], size_t size_of_numeros) {
    for (size_t i = 0; i < size_of_numeros; i++){
        valor_ou_flag_t resultado = ler_numero_e_virgula_ou_ler_flag();
        if (!resultado.era_flag) {
            numeros[i] = resultado.numero_lido;
            continue;
        }
        if (resultado.era_flag) {
            return i;
        }
    }
    // Aqui, nos nao chegamos na FLAG e ja chegamos no fim de numeros
    // Checar se o proximo numero é a flag ou nao, caso seja a flag, retornar o tamanho de numeros,
    // se nao, retornar -1;
    valor_ou_flag_t numero_lido;
    if (numero_lido.era_flag) {
        return size_of_numeros;
    }
    return -1;
}

typedef struct {
    size_t numeros_pares;
    size_t numeros_impares;
} pares_ou_impares_result;

bool e_par(int numero) {
    return numero % 2 == 0;
}

pares_ou_impares_result pares_ou_impares(int numeros[], size_t count) {
    pares_ou_impares_result result = {0, 0};
    for (int i = 0; i < count; i++) {
        int numero = numeros[i];
        if (e_par(numero)) {
            result.numeros_pares++;
        } else {
            result.numeros_impares++;
        }
    }
    return result;
}


int main() {
    int numeros[TAMANHO_DE_NUMEROS];
    int numeros_lidos = ler_numeros(numeros, TAMANHO_DE_NUMEROS);
    assert(numeros_lidos != -1);
    pares_ou_impares_result result = pares_ou_impares(numeros, numeros_lidos);
    printf("Há %li numeros pares e %li numeros impares\n", result.numeros_pares, result.numeros_impares);
}
