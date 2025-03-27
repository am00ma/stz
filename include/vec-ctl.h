/* From https://github.com/glouw/ctl */
#include "arena.h"
#include "macros.h"
#include "range.h"
#include "types.h"

// Default to i32
#ifndef T
// #error "Template type T undefined for <vec.h>"
#define T i32
#endif

#define A JOIN(vec, T)

/* ---------------------------------------------------------------------------
 * API
 * ------------------------------------------------------------------------- */
typedef struct A {
    T*    x;
    isize n;
} A;

A JOIN(A, new)(isize n, Arena* a);


/* ---------------------------------------------------------------------------
 * Implementation
 * ------------------------------------------------------------------------- */
A JOIN(A, new)(isize n, Arena* a) {
    return (A) {.x = new(a, T, n), .n = n};
}

A JOIN(A, range)(isize n, Arena* a) {
    A x = {.x = new(a, T, n), .n = n};
    RANGE(i, n) x.x[i] = i;
    return x;
}

#undef A
#undef T
