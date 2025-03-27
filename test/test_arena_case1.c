#include "arena.h"
#include "range.h"
#include <stdio.h>

typedef struct {
    char* buf;
    isize len;
} str;

typedef struct {
    f32*  x;
    isize n;
} vf32;

str str_new(i32 num, Arena* perm)
{
    char* chars = new (perm, char, num);

    RANGE(i, num) chars[i] = 65 + i;

    return (str){.buf = chars, .len = num};
}

void str_print(str x) { printf("%.*s\n", (int)x.len, x.buf); }

vf32 vf32_new(i32 num, Arena* perm)
{
    f32* x = new (perm, f32, num);

    RANGE(i, num) x[i] = i;

    return (vf32){.x = x, .n = num};
}

void vf32_print(vf32 x)
{

    printf("%4d ", 0);
    RANGE(i, 1, x.n) printf(" %4d ", i);
    printf("\n");
    printf("%1.2f ", x.x[0]);
    RANGE(i, 1, x.n) printf(" %1.2f ", x.x[i]);
    printf("\n");
}

int main()
{
    // Usual case, we want to allocate objects on heap inside function and
    // return the reference within the permanent arena

    // Create arena
    Arena perm = arena_new(256);

    // Pass it to functions by reference to get valid objects
    i32 num = 10;

    vf32 xrange = vf32_new(num, &perm);
    vf32_print(xrange);
    arena_print("vf32 * 10", &perm);

    str word = str_new(num, &perm);
    str_print(word);
    arena_print("vf32 * 10", &perm);
    return 0;
}
