#ifndef K_FALLIBLE_MAIN_H
#define K_FALLIBLE_MAIN_H

#include "fallible.h"

DEFINE_MAYBE_STRUCT_OF(int)

FALLIBLE_FUNCTION_SIGNATURE(int, fallible_main, (int argc, char** argv));

int main(int argc, char** argv);

#endif