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

// funcao fibonacci:
// fib: A -> B
// A = Z+
// B = Z+
//
// fib:
// { 1, x e {0, 1}
// { fib(x - 1) + fib(x - 2), x > 1
unsigned int fib(unsigned int x) {
    // 1, x e {0, 1}
    if (x == 0 || x == 1) {
        return 1;
    }
    // { fib(x - 1) + fib(x - 2), x > 1
    return fib(x - 1) + fib(x - 2);
}

int main() {
    int numero_n = promt_dint_or_fail();
    if (numero_n <= 2) {
        fputs("O numero não é maior que dois!\n", stderr);
        return 1;
    }
    for (int i = 0; i < numero_n; i++) {
        int indice_a_partir_de_1 = i + 1;
        printf("n: %i, fib(n): %i\n", indice_a_partir_de_1, fib(i));
    }
}
