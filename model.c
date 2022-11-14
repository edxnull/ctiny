#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "model.h"
#include "tga_image.h"
#include "geometry.h"
#include "util.h"

static void parse_obj(void)
{
    FILE *obj_file;
    char *buff, *token;

    size_t size;
    int i, j, k, t;

    const char delim[] = " /";

    size = 255;
    obj_file = fopen("obj/face.obj", "r");
    buff = malloc(sizeof(char)*size);

    i = j = k = t = 0;
    while ((getline(&buff, &size, obj_file)) != EOF) {
        if (buff[0] == 'v' && buff[1] == ' ') {
            t = 0;
            token = strtok(buff, delim);
            while (token != NULL) {
                token = strtok(NULL, delim);
                if (token == NULL) break;
                if (isdigit(token[0]) || token[0] == '-' || token[0] == '.') {
                    if (t == 0) vert[i].x = atof(token);
                    if (t == 1) vert[i].y = atof(token);
                    if (t == 2) vert[i].z = atof(token);
                    t++;
                }
            }
            i++;
        } else if (buff[0] == 'v' && buff[1] == 't' && buff[2] == ' ') {
            t = 0;
            token = strtok(buff, delim);
            while ((token = strtok(NULL, delim)) != NULL) {
                if (isdigit(token[0]) || token[0] == '-' || token[0] == '.') {
                    if (t == 0) vt[j].x = atof(token);
                    if (t == 1) vt[j].y = atof(token);
                    if (t == 2) vt[j].z = atof(token);
                    t++;
                }
            }
            j++;
        } else if (buff[0] == 'f' && buff[1] == ' ') {
            t = 0;
            token = strtok(buff, delim);
            while ((token = strtok(NULL, delim)) != NULL) {
                if (isdigit(token[0])) {
                    if (t == 0) {
                        face[k].x = atoi(token);
                        token = strtok(NULL, delim);
                        face_vt[k].x = atoi(token);
                        token = strtok(NULL, delim);
                        face_n[k].x = atoi(token);
                    }
                    if (t == 1) {
                        face[k].y = atoi(token);
                        token = strtok(NULL, delim);
                        face_vt[k].y = atoi(token);
                        token = strtok(NULL, delim);
                        face_n[k].y = atoi(token);

                    }
                    if (t == 2) {
                        face[k].z = atoi(token);
                        token = strtok(NULL, delim);
                        face_vt[k].z = atoi(token);
                        token = strtok(NULL, delim);
                        face_n[k].z = atoi(token);
                    }
                    t++;
                }
            }
            k++;
        }
    }
    free(buff);
    fclose(obj_file);
}

void construct_model(unsigned char *data, float *zbuffer, unsigned char *diffuse_information)
{
    int i;
    int r, g, b;
    v2i p0, p1, p2;
    v3f v0, v1, v2;
    /* v3f vt0, vt1, vt2; */
    v3f n, d1, d2;
    float dist;

    char c[3] = {0, 0, 0};

    parse_obj();

    for (i = 0; i < N_FACES; i++) {
        v0 = vert[face[i].x-1];
        v1 = vert[face[i].y-1];
        v2 = vert[face[i].z-1];

        p0.x = (v0.x + 1.0) * IMG_W/2.0;
        p0.y = (v0.y + 1.0) * IMG_H/2.0;
        p1.x = (v1.x + 1.0) * IMG_W/2.0;
        p1.y = (v1.y + 1.0) * IMG_H/2.0;
        p2.x = (v2.x + 1.0) * IMG_W/2.0;
        p2.y = (v2.y + 1.0) * IMG_H/2.0;

        /* 
         * wireframe
         * line(p0.x, p0.y, p1.x, p1.y, data, WHITE);
         * line(p1.x, p1.y, p2.x, p2.y, data, WHITE);
         * line(p2.x, p2.y, p0.x, p0.y, data, WHITE);
         */

        d1.x = v1.x - v0.x;
        d1.y = v1.y - v0.y;
        d1.z = v1.z - v0.z;

        d2.x = v2.x - v0.x;
        d2.y = v2.y - v0.y;
        d2.z = v2.z - v0.z;

        n = fcross(d1, d2);
        dist =  sqrt(n.x * n.x + n.y * n.y + n.z * n.z);

        n.x = n.x / dist;
        n.y = n.y / dist;
        n.z = n.z / dist;

        float dir = (n.x * 0) + (n.y * 0) + (n.z * 1);

        if (dir > 0) {

            r = dir * 255;
            g = dir * 255;
            b = dir * 255;

            memcpy(&c[0], &b, 1);
            memcpy(&c[1], &g, 1);
            memcpy(&c[2], &r, 1);

            int minx = MIN(p0.x, MIN(p1.x, p2.x));
            int miny = MIN(p0.y, MIN(p1.y, p2.y));
            int maxx = MAX(p0.x, MAX(p1.x, p2.x));
            int maxy = MAX(p0.y, MAX(p1.y, p2.y));

            /*
             * minx = MAX(minx, 0);
             * miny = MAX(miny, 0);
             * maxx = MIN(maxx, 800-1);
             * maxy = MIN(maxy, 800-1);
             */

            v2i p;
            for (p.y = miny; p.y < maxy; p.y++) {
                for (p.x = minx; p.x < maxx; p.x++) {

                    int w0 = edge(p1, p2, p);
                    int w1 = edge(p2, p0, p);
                    int w2 = edge(p0, p1, p);

                    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                        float pz = 0;
                        pz += v0.z * p.x + p.y * 3;
                        pz += v1.z * p.x + p.y * 3;
                        pz += v2.z * p.x + p.y * 3;

                        if (zbuffer[p.x + p.y * 800] < pz) {
                            zbuffer[p.x + p.y * 800] = pz;

                            /*
                             * vt0 = vt[face_vt[i].x-1];
                             * vt1 = vt[face_vt[i].y-1];
                             * vt2 = vt[face_vt[i].z-1];
                             */

                            setpixel(p.x, p.y, data, c);
                        }
                    }
                }
            }
        }
    }
}
