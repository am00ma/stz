/*
 * Usual case, we want to allocate objects on heap inside function and
 * return a valid reference to the outer scope
 *
 * Is this true?
 *   As we cannot access the stack of the function, it HAS to either return a
 *   reference on the heap, or return the struct by copy
 *
 * Typical function that needs to return valid ref:
 *
 *      // Definition
 *      type type_new(..., Arena *a);
 *
 *      // Call
 *      type x = type_new(..., a);
 *      assert (x); // Not null
 *
 */

#include "arena.h"
#include "range.h"
#include <stdio.h>

/* ---------------------------------------------------------------------------
 * counted string
 * ------------------------------------------------------------------------- */
typedef struct {
    char* buf;
    isize len;
} str;

str str_new(i32 num, Arena* perm)
{
    char* chars = new (perm, char, num);
    RANGE(i, num) { chars[i] = 65 + i; }
    return (str){.buf = chars, .len = num};
}

void str_print(str x) { printf("%.*s\n", (int)x.len, x.buf); }

/* ---------------------------------------------------------------------------
 * vector of floats (mathematical vector, i.e. n scalars)
 * ------------------------------------------------------------------------- */
typedef struct {
    f32*  x;
    isize n;
} vf32;

vf32 vf32_new(i32 num, Arena* perm)
{
    f32* x = new (perm, f32, num);
    RANGE(i, num) { x[i] = i; }
    return (vf32){.x = x, .n = num};
}

void vf32_print(vf32 x)
{
    printf("%4d ", 0);
    RANGE(i, 1, x.n) { printf(" %4d ", i); }
    printf("\n");
    printf("%1.2f ", x.x[0]);
    RANGE(i, 1, x.n) { printf(" %1.2f ", x.x[i]); }
    printf("\n");
}

/* ---------------------------------------------------------------------------
 * Various styles of memory management
 * ------------------------------------------------------------------------- */

/* malloc free */
void mf(i32 num) {}

/* resource aquisition is initialization */
void raii(i32 num) {}

/* zero initialization */
void zi(i32 num, Arena life_fn)
{
    // Create string
    str word = str_new(num, &life_fn);
    str_print(word);
    arena_print("life_fn: str (char * 10)", &life_fn);

    // Create vector of floats
    vf32 xrange = vf32_new(num, &life_fn);
    vf32_print(xrange);
    arena_print("life_fn: vf32 (f32 * 10)", &life_fn);
}

/* ---------------------------------------------------------------------------
 * Boilerplate
 * ------------------------------------------------------------------------- */
int main()
{
    i32 num = 10;

    mf(num);

    raii(num);

    // Create arena
    Arena life_main = arena_new(256);
    arena_print("life_main: before zi", &life_main);

    zi(num, life_main);

    arena_print("life_main: after zi", &life_main);

    return 0;
}
