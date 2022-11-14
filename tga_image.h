#ifndef TGA_IMAGE_H
#define TGA_IMAGE_H

#include "stdio.h"

#define TGA_HEADER_SIZE 18

#define TGA_DATA_TYPE_ERR -1
#define TGA_DATA_TYPE_RGB 2 
#define TGA_DATA_TYPE_RLE 10

enum tga_data_type {
    TGA_ERR_T = TGA_DATA_TYPE_ERR,
    TGA_RGB_T = TGA_DATA_TYPE_RGB,
    TGA_RLE_T = TGA_DATA_TYPE_RLE 
};

#define IMG_W 800
#define IMG_H 800

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define IMG_BPP 3
#define IMG_SIZE (IMG_W * IMG_H * IMG_BPP)

#define WHITE  "\xff\xff\xff"
#define RED    "\x00\x00\xff"
#define GREEN  "\x00\xff\x00"
#define BLUE   "\xff\x00\x00"
#define ORANGE "\x33\x57\xff"
#define YELLOW "\x33\xeb\xff"

#pragma pack(push,1)
struct tga_header {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)

unsigned char *init_data(void);

void rle_compress(unsigned char *data, FILE *file);
void write_tga_image(unsigned char *data, const int type, FILE *file);

unsigned char *file_read_diffuse(char *filename);

#endif /* TGA_IMAGE_H */
