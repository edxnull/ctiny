#include <stdio.h>
#include "testing.h"
#include "dtypes.h"

void test_machine_dtypes(void) 
{
    assert(1 == sizeof(char));
    assert(2 == sizeof(short int));
    assert(4 == sizeof(int));

    assert(1 == sizeof(unsigned char));
    assert(2 == sizeof(unsigned short));
    assert(4 == sizeof(unsigned int));

    assert(4 == sizeof(float));
    assert(8 == sizeof(double));
}

void test_typedefed_dtypes(void) 
{
    assert(1 == sizeof(s8));
    assert(2 == sizeof(s16));
    assert(4 == sizeof(s32));

    assert(1 == sizeof(u8));
    assert(2 == sizeof(u16));
    assert(4 == sizeof(u32));

    assert(4 == sizeof(f32));
    assert(8 == sizeof(f64));

    assert(4 == sizeof(rune));
    assert(1 == sizeof(byte));
}

void run_dtypes_tests(void) 
{
    printf("-----\n");
    run_test(test_machine_dtypes);
    run_test(test_typedefed_dtypes);
    printf("...OK\n");
    printf("-----\n");
}
