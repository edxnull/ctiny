#ifndef TESTING_H
#define TESTING_H

#include <assert.h>

#define run_test(fn_name) \
    printf("%s\n", #fn_name); \
    fn_name()

void run_dtypes_tests(void);
void run_util_tests(void);

#endif /* TESTING_H */
