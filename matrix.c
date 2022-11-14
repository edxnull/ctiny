#include "matrix.h"

m2x2 identity(void)
{
    m2x2 i = {1, 0, 0, 1};
    return i;
}

m2x2 v2m(v2i v)
{
    m2x2 m;
    m.a11 = v.x;
    m.a12 = 0;
    m.a21 = 0;
    m.a22 = v.y;
    return m;
}

v2i m2v(m2x2 m)
{
    v2i v;
    v.x = m.a11;
    v.y = m.a22;
    return v;
}

int det2x2(m2x2 m)
{
    return ((m.a11 * m.a22) - (m.a12 * m.a21));
}

void transform(m2x2 *m, v2i *v)
{
    v->x = (m->a11 * v->x) + (m->a12 * v->y);
    v->y = (m->a21 * v->x) + (m->a22 * v->y);
}
