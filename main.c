#define TESTING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tga_image.h"
#include "model.h"
#include "geometry.h"
#include "testing.h"
#include "util.h"
#include "dtypes.h"

int main(int argc, char *argv[]) {
    FILE *file;
    float *zbuffer;
    unsigned char *data;
    unsigned char *diffuse_information;

// #ifdef TESTING
//     run_dtypes_tests();
//     run_util_tests();
// #endif
    
    zbuffer = malloc(sizeof(float)*(800*800));
    memset(zbuffer, -1.0, sizeof(*zbuffer));
    
    file = fopen("image.tga", "wb");
    diffuse_information = file_read_diffuse("obj/face.obj");

    if (0 == diffuse_information) {
        free(zbuffer);
        fclose(file);
        return 1;
    }

    data = init_data();

    if (data == NULL) {
        free(zbuffer);
        fclose(file);
        return 1;
    }

    construct_model(data, zbuffer, diffuse_information);

    v2i t0[3] = {{10,  70},  {50,  160}, {70,   80}};
    v2i t1[3] = {{180, 50},  {150,   1}, {70,  180}};
    v2i t2[3] = {{180, 150}, {120, 160}, {130, 180}};
    v2i t3[3] = {{200, 200}, {250, 300}, {300, 200}};
    v2i t4[3] = {{220, 220}, {270, 320}, {320, 240}};
    v2i t5[3] = {{210, 190}, {290, 120}, {320, 240}};

    triangle(t0[0], t0[1], t0[2], data, GREEN, zbuffer);
    triangle(t1[0], t1[1], t1[2], data, BLUE, zbuffer);
    triangle(t2[0], t2[1], t2[2], data, YELLOW, zbuffer);
    triangle(t3[0], t3[1], t3[2], data, GREEN, zbuffer);
    triangle(t4[0], t4[1], t4[2], data, ORANGE, zbuffer);
    triangle(t5[0], t5[1], t5[2], data, BLUE, zbuffer);

    write_tga_image(data, handle_cmdline_args(argc, argv), file);

    free(data);
    free(zbuffer);
    fclose(file);
    return 0;
}
