#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tga_image.h"

static struct tga_header *init_header(const int type)
{
    struct tga_header *h;

    assert(TGA_HEADER_SIZE == sizeof(*h));

    h = malloc(sizeof(struct tga_header));

    assert(h != NULL);

    memset(h, 0x0, sizeof(*h));

    h->datatypecode = type;
    h->width = IMG_W;
    h->height = IMG_H;
    h->bitsperpixel = 24;

    return h;
}

unsigned char *init_data(void)
{
    unsigned char *data;
    data = malloc(sizeof(unsigned char)*IMG_SIZE);
    
    assert(data != NULL);

    memset(data, 0x0, sizeof(unsigned char)*IMG_SIZE);
    return data;
}

/* refactor such that we don't hit the disk with fwrite */
void rle_compress(unsigned char *data, FILE *file)
{
    unsigned int run;
    unsigned int byte;

    run = 1;
    byte = 0;

    const unsigned int maxrun = 128;
    
    while (byte < IMG_SIZE) {
        while (byte < IMG_SIZE && run < maxrun && (memcmp(&data[byte], &data[byte+3], 3) == 0)) {
            byte += 3;
            if (memcmp(&data[byte], &data[byte+3], 3) == 0)
                run++;
            if (memcmp(&data[byte], &data[byte+3], 3) != 0) {
                byte += 3;
                run ^= 1 << 7;
                fwrite(&run, 1, 1, file);
                fwrite(&data[byte-3], 1, 3, file);
                run = 1;
                break;
            }
        }

        if (memcmp(&data[byte], &data[byte+3], 3) == 0) {
            if (run == maxrun) {
                run -= 1;
                run ^= 1 << 7;
                fwrite(&run, 1, 1, file);
                fwrite(&data[byte-3], 1, 3, file);
                run = 0;
            }
        }

        while (byte < IMG_SIZE && run < maxrun && (memcmp(&data[byte], &data[byte+3], 3) != 0)) {
            byte += 3;
            if (memcmp(&data[byte], &data[byte+3], 3) != 0) {
                run++;
            }
            if (memcmp(&data[byte], &data[byte+3], 3) == 0) {
                if (run == 1) {
                    run = 0;
                    fwrite(&run, 1, 1, file);
                    fwrite(&data[byte-3], 1, 3, file);
                    run = 1;
                    break;
                } else {
                    run -= 1;
                    fwrite(&run, 1, 1, file);
                    fwrite(&data[byte-((run+1)*3)], 1, (run+1)*3, file);
                    run = 1;
                    break;
                }
            }
        }

        if (memcmp(&data[byte], &data[byte+3], 3) != 0) {
            if (run == maxrun) {
                run -= 1;
                fwrite(&run, 1, 1, file);
                fwrite(&data[byte-((run+1)*3)], 1, 3, file);
                run = 1;
            }
        }
    }
}

void write_tga_image(unsigned char *data, enum tga_data_type type, FILE *file)
{
    struct tga_header *header;
    header = init_header(type);

    assert(header != NULL);

    fwrite(header, sizeof(*header), 1, file);
    free(header);

    switch(type) {
        /* no case TGA_ERR_T on purpose */
        case TGA_RGB_T:
            fwrite(data, sizeof(*data), IMG_SIZE, file);
            break;

        case TGA_RLE_T:
            rle_compress(data, file);
            break;

        default:
            fprintf(stderr, "ERROR: please specify -RLE or -RGB\n");
            break;
    }

    unsigned char developer_area_ref[4] = {0, 0, 0, 0};
    unsigned char extension_area_ref[4] = {0, 0, 0, 0};
    unsigned char footer[18] = {'T','R','U','E','V','I','S','I','O','N','-',
                                                'X','F','I','L','E','.','\0'};

    fwrite(&developer_area_ref, sizeof(developer_area_ref), 1, file);
    fwrite(&extension_area_ref, sizeof(extension_area_ref), 1, file);
    fwrite(&footer, sizeof(footer), 1, file);
}

unsigned char *file_read_diffuse(char *filename)
{
    FILE *file;
    unsigned char *result = 0;

    file = fopen(filename, "rb");

    if (!file) {
        fprintf(stderr, "ERROR: fopen failed in unsigned char *file_read_diffuse(char *filename)\n");
        return result;
    }

    fseek(file, 18, SEEK_END);
    size_t filesize = ftell(file);
    fseek(file, 18, SEEK_SET);

    result = malloc(sizeof(unsigned char) * filesize + 1);

    assert(result != NULL);

    fread(result, filesize, 1, file);
    result[filesize] = '\0';

    fclose(file);

    return result;
}
