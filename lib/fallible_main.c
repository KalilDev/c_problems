#include "fallible_main.h"
#include "fallible.h"
 
int main(int argc, char** argv) {
    FALLIBLE_FUNCTION_RESULT_TYPE(int) maybe_result = fallible_main(argc, argv);
    if (maybe_result.has_value) {
        return maybe_result.unwrap;
    }
    return 1;
}
