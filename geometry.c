#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "geometry.h"
#include "tga_image.h"

v3f fcross(v3f a, v3f b)
{
    v3f cross;

    cross.x = a.y * b.z - a.z * b.y;
    cross.y = a.z * b.x - a.x * b.z;
    cross.z = a.x * b.y - a.y * b.x;

    return cross;
}

void setpixel(int x, int y, unsigned char *data, const char *color)
{
    memcpy(data+(x+y*IMG_W)*IMG_BPP, color, 3);
}

void line(int x0, int y0, int x1, int y1, unsigned char *data, const char *color)
{
    int dx = abs(x1 - x0); 
    int sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0); 
    int sy = y0 < y1 ? 1 : -1; 
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    for(;;) {
        setpixel(x0, y0, data, color);
        if (x0 == x1 && y0 == y1) 
            break;
        e2 = err;
        if (e2 > -dx) { 
            err -= dy; 
            x0 += sx; 
        }
        if (e2 < dy) { 
            err += dx; 
            y0 += sy; 
        }
    }
}

void swap_v2i(v2i *v1, v2i *v2)
{
    v2i temp;

    temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

void triangle(v2i v0, v2i v1, v2i v2, unsigned char *data, const char *color, float *zbuffer)
{
    int i;
    v2i v4;
    float t, m1, m2;
    float curra, currb;

    if (v0.y > v1.y) swap_v2i(&v0, &v1);
    if (v0.y > v2.y) swap_v2i(&v0, &v2);
    if (v1.y > v2.y) swap_v2i(&v1, &v2);

    if ((v2.y - v0.y) != 0)
        t = (float)(v1.y - v0.y) / (float)(v2.y - v0.y);
    else
        t = (float)(v1.y - v0.y);

    v4.x = v0.x + t * (v2.x - v0.x);
    v4.y = v1.y;

    if ((v2.y - v0.y) != 0)
        m1 = (float)(v2.x - v0.x) / (float)(v2.y - v0.y);
    else
        m1 = (float)(v2.x - v0.x);

    if ((v1.y - v0.y) != 0)
        m2 = (float)(v1.x - v0.x) / (float)(v1.y - v0.y);
    else
        m2 = (float)(v1.x - v0.x);

    curra = v4.x;
    currb = v1.x;

    for (i = v4.y; i > v0.y; i--) {
        line((int)curra, i, (int)currb, i, data, color);
        curra -= m1;
        currb -= m2;
    }

    if ((v2.y - v4.y) != 0)
        m1 = (float)(v2.x - v4.x) / (float)(v2.y - v4.y);
    else
        m1 = (float)(v2.x - v4.x);

    if ((v2.y - v1.y) != 0)
        m2 = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
    else
        m2 = (float)(v2.x - v1.x);

    curra = v4.x;
    currb = v1.x;

    for (i = v4.y; i < v2.y; i++) {
        line((int)curra, i, (int)currb, i, data, color);
        curra += m1;
        currb += m2;
    }
}

int edge(v2i a, v2i b, v2i c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

float lerp(float a, float t, float b)
{
    return (1.0 - t) * a + t * b;
}
