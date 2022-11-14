#ifndef MATRIX_H
#define MATRIX_H

#include "geometry.h"

struct m2x2 {
    int a11, a12;
    int a21, a22;
};

typedef struct m2x2 m2x2;

int det2x2(m2x2 m);
m2x2 identity(void);
m2x2 v2m(v2i v);
v2i m2v(m2x2 m);
void transform(m2x2 *m, v2i *v);

#endif /* MATRIX_H */
