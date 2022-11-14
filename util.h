#ifndef UTIL_H
#define UTIL_H

#include "tga_image.h"
#include "stdio.h"

#define ARRAY_LEN(x) ((sizeof(x)) / (sizeof(x[0])))

ssize_t getline(char **buffer, size_t *n, FILE *file);

enum tga_data_type handle_cmdline_args(int argc, char *argv[]);

#endif /* UTIL_H */
