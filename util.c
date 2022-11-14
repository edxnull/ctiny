#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "tga_image.h"

ssize_t getline(char **buffer, size_t *n, FILE *file) {
    char *c;
    size_t i;
    ssize_t curr_len;

    i = 0;
    curr_len = 0;

    c = *buffer;

    for (i = 0; i < *n; i++) {
        if (((*c) = (char)fgetc(file)) == EOF) {
            break;
        }
        if ((*c) == '\n') {
            break;
        }
        curr_len++;
        c++;
    }

    if ((*c) == EOF) {
        return -1;
    }

    return curr_len;
}

enum tga_data_type handle_cmdline_args(int argc, char *argv[]) {
    if (argc > 1) {
        int i, j, eq;
        char *c;

        const char *usage = "error: parsing flags. Please use -RGB or -RLE\n";

        struct _img_type { char *cval; enum tga_data_type type; };

        struct _img_type _types[] = {
            {"RGB", TGA_RGB_T},
            {"RLE", TGA_RLE_T}
        };

        for (i = 1; i < argc; i++) {
            c = argv[i];
            if (('-' == *c) && ('\0' != *++c)) {
                for (j = ARRAY_LEN(_types)-1; j >= 0; j--) {
                    eq = memcmp(_types[j].cval, c, sizeof(char)*3);
                    if (0 == eq) {
                        return _types[j].type;
                    }
                }
                if (eq != 0) {
                    fprintf(stderr, "error: unknown flag: %s\n", argv[i]);
                    fprintf(stderr, usage);
                    return TGA_ERR_T;
                }
            } else {
                fprintf(stderr, usage);
                return TGA_ERR_T;
            }
        }
    }
    return TGA_ERR_T;
}
