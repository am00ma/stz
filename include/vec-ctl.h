/* From https://github.com/glouw/ctl */
#include "arena.h"
#include "macros.h"
#include "range.h"
#include "types.h"

/* To use, define the following and #include "vec-ctl.h"
 *
 * #define T                  i32
 * #define VEC_HEADER         1
 * #define VEC_IMPLEMENTATION 1
 *
 * */

// Default to i32
#ifndef T
#error "Template type T undefined for <vec.h>"
#endif

#define A JOIN(vec, T)

#ifdef VEC_HEADER

/* ---------------------------------------------------------------------------
 * API
 * ------------------------------------------------------------------------- */
typedef struct A {
    T*    x;
    isize n;
} A;

A JOIN(A, new)(isize n, Arena* a);
A JOIN(A, range)(isize n, Arena* a);

void JOIN(A, check)(A* a, A b);
void JOIN(A, check_zero)(A* a);

void JOIN(A, sum)(A* a, A b);
void JOIN(A, prod)(A* a, A b);
void JOIN(A, div)(A* a, A b);

#endif // VEC_HEADER

#ifdef VEC_IMPLEMENTATION

/* ---------------------------------------------------------------------------
 * Implementation
 * ------------------------------------------------------------------------- */
A JOIN(A, new)(isize n, Arena* a) { return (A){.x = new (a, T, n), .n = n}; }

A JOIN(A, range)(isize n, Arena* a)
{
    A x                = {.x = new (a, T, n), .n = n};
    RANGE(i, n) x.x[i] = i;
    return x;
}

/* ---------------------------------------------------------------------------
 * Fatal errors
 * ------------------------------------------------------------------------- */
void JOIN(A, check)(A* a, A b)
{
    if (a->n != b.n) die(1, "Unequal lengths: a.n, b.n : %ld, %ld", a->n, b.n);
}

void JOIN(A, check_zero)(A* a) { RANGE(i, a->n) if (a->x[i] == 0) die(1, "Div by zero: a[%d] = %f", i, (f64)a->x[i]); }

/* ---------------------------------------------------------------------------
 * Math
 * ------------------------------------------------------------------------- */
void JOIN(A, sum)(A* a, A b)
{
    JOIN(A, check);
    RANGE(i, a->n) a->x[i] += b.x[i];
}

void JOIN(A, prod)(A* a, A b)
{
    JOIN(A, check);
    RANGE(i, a->n) a->x[i] *= b.x[i];
}

void JOIN(A, div)(A* a, A b)
{
    JOIN(A, check);
    JOIN(A, check_zero)(&b);
    RANGE(i, a->n) a->x[i] /= b.x[i];
}

#endif // VEC_IMPLEMENTATION

#undef A
#undef T
