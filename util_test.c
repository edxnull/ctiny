#include "util.h"
#include "assert.h"
#include "tga_image.h"
#include "testing.h"
#include "stdio.h"

void it_returns_error_on_argc_leq_1(void) {
        int argc = 1; 
        char *argv[] = {"", ""};
        enum tga_data_type result = handle_cmdline_args(1, argv);
        assert(result == TGA_ERR_T);
}

void run_util_tests(void) {
    printf("-----\n");
    run_test(it_returns_error_on_argc_leq_1);
    printf("...OK\n");
    printf("-----\n");
}
