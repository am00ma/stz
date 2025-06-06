#include "arena.h"
#include "array.h"
#include "range.h"
#include "str.h"
#include "tree.h"
#include <stdio.h>

int main()
{

    Arena perm = arena_new(CAPACITY);

    i32 len       = 8;
    i32 levels[8] = {0, 1, 1, 2, 1, 0, 1, 0};

    i32 order[8]   = {0, 1, 2, 3, 4, 5, 6, 7};
    i32 parents[8] = {-1, 0, 0, 2, 0, -1, 5, -1};
    printf("i: order[i], parents[i]\n");
    RANGE(i, 8) { printf("%d: %d, %2d\n", i, order[i], parents[i]); }

    i32 c_0[3] = {1, 2, 4};
    i32 c_2[1] = {3};
    i32 c_5[1] = {6};

    Ai32 children[8] = {};
    children[0]      = (Ai32){.len = 3, .data = c_0};
    children[2]      = (Ai32){.len = 1, .data = c_2};
    children[5]      = (Ai32){.len = 1, .data = c_5};

    Tree t     = tree_from_levels(len, levels, &perm);
    t.children = children;
    printf("Tree: %d\n", t.len);
    printf("i: order[i], parents[i], children[i]\n");

    RANGE(i, 8)
    {
        Str c = ai32_print(&t.children[i], &perm);
        printf("%d: %d, %2d, %.*s\n", i, t.order[i], t.parents[i], pstr(c));
    }
}
