#include "arena.h"
#include "range.h"
#include <stdio.h>

typedef struct {
    char* buf;
    isize len;
} str;

typedef struct {
    str*  x;
    isize n;
} vstr;

str str_new(i32 num, Arena* perm)
{
    char* chars = new (perm, char, num);

    RANGE(i, num) chars[i] = 65 + i;

    return (str){.buf = chars, .len = num};
}

vstr vstr_new(i32 num, Arena* perm)
{
    str* strs = new (perm, str, num);

    RANGE(j, num) strs[j] = str_new(num, perm);

    RANGE(j, num) RANGE(i, num) strs[j].buf[i] = 65 + i + j;

    return (vstr){.x = strs, .n = num};
}

void vstr_print(vstr x) { RANGE(j, x.n) printf("%d: %.*s\n", j, (int)x.x[j].len, x.x[j].buf); }

int main()
{
    /* Nested structs that need initialization
     *
     * We want to allocate nested objects on heap inside function and
     * return the reference within the permanent arena */

    // Create arena
    Arena perm = arena_new(512);

    // Pass it to functions by reference to get valid objects
    i32  num = 10;
    vstr x   = vstr_new(num, &perm);

    // x itself is a light struct with one pointer and a size,
    // the min args necessary for the print anyway
    vstr_print(x);

    arena_print("vf32 * 10", &perm);
    return 0;
}
