#include "lib/fallible_main.h"
#include <string.h>

FALLIBLE_FUNCTION(int, fallible_main, (int argc, char** argv), {
    puts("Hello world!");
    puts("I will fail if the second arg is boom");
    if (argc>1 && strcmp(argv[1], "boom") == 0) {
        fputs("Kaboom!!\n", stderr);
        RETURN_FAILURE();
    }
    RETURN_SUCCESS(0);
});