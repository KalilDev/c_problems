#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include "lib/io_wrapper.h"

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

int weighted_sum_of_values(const int* values, const int *weigths, size_t count) {
    int total = 0;
    for (size_t i = 0; i < count; i++) {
        total+=values[i]*weigths[i];
    }
    return total;
}

const int cpf_digits_weigths[3] = {2,3,4};
const int cpf_second_digits_weigths[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

typedef struct {
    unsigned int as_uint;
    unsigned int verifier_as_uint;
    unsigned int digits[11];
    unsigned int verifier_digits[2];
    
} cpf_t;

bool number_to_uint_digits(unsigned int number, unsigned int* digits, size_t count) {
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

cpf_t do_with_cpf(unsigned int cpf) {
    printf("cpf %i\n", cpf);
    assert(number_width(cpf) == 9);
    cpf_t resulting_cpf = {.verifier_digits = {0xDEADBEEF,0xDEADBEEF}};
    unsigned int *digits = resulting_cpf.digits;
    bool succeeded = number_to_uint_digits(cpf, digits, 9);
    assert(succeeded);

    for (size_t i =0; i < 11; i++) {
        printf("digit: %i\n", digits[i]);
    }
    unsigned int *result_digits = resulting_cpf.verifier_digits;

    // 1.
    int sum_of_sums = 0;
    sum_of_sums += weighted_sum_of_values(digits+0, cpf_digits_weigths, 3);
    sum_of_sums += weighted_sum_of_values(digits+3, cpf_digits_weigths, 3);
    sum_of_sums += weighted_sum_of_values(digits+6, cpf_digits_weigths, 3);

    // 2.
    {
        int mod_11 = sum_of_sums % 11;
        if (mod_11 == 0 || mod_11 == 1) {
            result_digits[0] = 0;
        } else { 
            result_digits[0] = 11-mod_11;
        }
    }
    assert(result_digits[0] != 0xDEADBEEF);
    digits[9]=result_digits[0];

    // 3.
    int next_sum = weighted_sum_of_values(digits, cpf_second_digits_weigths, 10);

    // 4.
    {
        int mod_11 = next_sum % 11;
        if (mod_11 == 0 || mod_11 == 1) {
            result_digits[1] = 0;
        } else { 
            result_digits[1] = 11-mod_11;
        }
    }
    assert(result_digits[1] != 0xDEADBEEF);
    digits[10]=result_digits[1];

    for (size_t i =0; i < 11; i++) {
        printf("digit: %i\n", digits[i]);
    }

    resulting_cpf.as_uint = 0;
    for (size_t i = 0; i < 11; i++) {
        unsigned int current = resulting_cpf.as_uint;
        resulting_cpf.as_uint = current*10 + digits[i];
    }
    
    resulting_cpf.verifier_as_uint = 0;
    for (size_t i = 0; i < 2; i++) {
        unsigned int current = resulting_cpf.verifier_as_uint;
        resulting_cpf.verifier_as_uint = current*10 + result_digits[i];
    }

    return resulting_cpf;
}

int main() {
  puts("Digite o CPF sem os digitos verificadores:");
  int cpf_start_or_cpf = read_int_or_fail(decimal);
  if (cpf_start_or_cpf <= 0) {
      goto invalid_cpf;
  }

  switch (number_width(cpf_start_or_cpf)) {
    case 9:
        cpf_t result = do_with_cpf(cpf_start_or_cpf);
        printf("CPF é %i\n", result.as_uint);
        break;
    case 3:
    default:
      goto invalid_cpf;
  }
  
  return 0;

invalid_cpf:
  fputs("CPF invalido!\n", stderr);
  return 1;
}
