#ifndef K_CONVERSION_QUEST_H
#define K_CONVERSION_QUEST_H

#include "quest.h"

#define BASE_CONVERSION_QUESTION(what, the_what, num, from, to) QUEST(num, {\
    printf("Digite %s em %s:", what, #from);\
    float value = read_float_or_fail();\
    \
    printf("%s em %s é %f\n", the_what, #to, from ## _from_ ## to(value));\
})
#define BASE_CONVERSION_PAIR_QUESTION(what, the_what, num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_QUESTION(what, the_what, num_from, first_from, first_to) \
    BASE_CONVERSION_QUESTION(what, the_what, num_to, first_to, first_from) 

#define CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("um valor", "O valor", num, from, to)
#define CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("um valor", "O valor", num_from, num_to, first_from, first_to)

#define MASS_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("uma massa", "A massa", num, from, to)
#define MASS_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("uma massa", "A massa", num_from, num_to, first_from, first_to)

#define TEMPERATURE_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("uma temperatura", "A temperatura", num, from, to)
#define TEMPERATURE_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("uma temperatura", "A temperatura", num_from, num_to, first_from, first_to)

#define VELOCITY_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("uma velocidade", "A velocidade", num, from, to)
#define VELOCITY_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("uma velocidade", "A velocidade", num_from, num_to, first_from, first_to)

#define DISTANCE_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("uma distancia", "A distancia", num, from, to)
#define DISTANCE_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("uma distancia", "A distancia", num_from, num_to, first_from, first_to)

#define LENGTH_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("um comprimento", "O comprimento", num, from, to)
#define LENGTH_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("um comprimento", "O comprimento", num_from, num_to, first_from, first_to)

#define AREA_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("uma área", "A área", num, from, to)
#define AREA_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("uma área", "A área", num_from, num_to, first_from, first_to)

#define VOLUME_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("um volume", "O volume", num, from, to)
#define VOLUME_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("um volume", "O volume", num_from, num_to, first_from, first_to)

#define ANGLE_CONVERSION_QUESTION(num, from, to) \
    BASE_CONVERSION_QUESTION("um ângulo", "O ângulo", num, from, to)
#define ANGLE_CONVERSION_PAIR_QUESTION(num_from, num_to, first_from, first_to) \
    BASE_CONVERSION_PAIR_QUESTION("um ângulo", "O ângulo", num_from, num_to, first_from, first_to)

#endif
