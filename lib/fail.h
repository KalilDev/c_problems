#ifndef K_FAIL_H
#define K_FAIL_H

#include <stdlib.h>
#include <stdio.h>

#define failf(fmt, ...) \
do {\
    fprintf(stderr, fmt, __VA_ARGS__);\
    exit(EXIT_FAILURE);\
} while(0)

#endif