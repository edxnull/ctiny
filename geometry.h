#ifndef GEOMETRY_H
#define GEOMETRY_H

#define N_VERTS 1258
#define N_FACES 2492 
#define N_VT    1339

struct v2i {
    int x;
    int y;
};

struct v2f {
    float x;
    float y;
};

struct v3i {
    int x;
    int y;
    int z;
};

struct v3f {
    double x;
    double y;
    double z;
};

static struct v3f vert[N_VERTS];
static struct v3f vt[N_VT];
static struct v3f vtn[N_VERTS]; /* not used */
static struct v3i face[N_FACES];
static struct v3i face_vt[N_FACES];
static struct v3i face_n[N_FACES];

typedef struct v2i v2i;
typedef struct v2f v2f;
typedef struct v3i v3i;
typedef struct v3f v3f;

v3f fcross(v3f a, v3f b);

void setpixel(int x, int y, unsigned char *data, const char *color);
void line(int x0, int y0, int x1, int y1, unsigned char *data, const char *color);
void swap_v2i(v2i *v1, v2i *v2);
void triangle(v2i v0, v2i v1, v2i v2, unsigned char *data, const char *color, float *zbuffer);

int edge(v2i a, v2i b, v2i c);

float lerp(float a, float t, float b);
float bilinear_interp(void);

#endif /* GEOMETRY_H */
