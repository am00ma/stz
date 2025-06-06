#include "arena.h"
#include "array.h"
#include "range.h"
#include "str.h"
#include "tree.h"
#include <assert.h>
#include <stdio.h>

int main()
{
    Arena perm = arena_new(CAPACITY);

    i32 len       = 8;
    i32 levels[8] = {0, 1, 1, 2, 1, 0, 1, 0};

    Tree tl = tree_from_levels(len, levels, &perm);
    printf("Tree: %d\n", tl.len);
    printf("i: order  level  parents  children\n");

    Arena temp = perm;
    RANGE(i, 8)
    {
        Str c = ai32_print(&tl.children[i], &temp);
        //    ( "i: order  level  parents  children\n");
        printf("%d: %2d     %2d     %2d       %.*s\n", i, tl.order[i], tl.levels[i], tl.parents[i], pstr(c));
    }

    i32 c_0[3] = {1, 2, 4};
    i32 c_2[1] = {3};
    i32 c_5[1] = {6};

    Ai32 children[8] = {};
    children[0]      = (Ai32){.len = 3, .data = c_0};
    children[2]      = (Ai32){.len = 1, .data = c_2};
    children[5]      = (Ai32){.len = 1, .data = c_5};

    Tree tc = tree_from_children(len, children, &perm);
    printf("Tree: %d\n", tl.len);
    printf("i: order  level  parents  children\n");

    temp = perm;
    RANGE(i, 8)
    {
        Str c = ai32_print(&tc.children[i], &temp);
        //    ( "i: order  level  parents  children\n");
        printf("%d: %2d     %2d     %2d       %.*s\n", i, tl.order[i], tl.levels[i], tl.parents[i], pstr(c));
    }

    // Expected
    i32 order[8]   = {0, 1, 2, 3, 4, 5, 6, 7};
    i32 parents[8] = {-1, 0, 0, 2, 0, -1, 5, -1};
    RANGE(i, 8)
    {
        assert(tc.order[i] == order[i]);
        assert(tc.levels[i] == levels[i]);
        assert(tc.parents[i] == parents[i]);
        assert(tc.children[i].len == children[i].len);
        RANGE(j, tc.children[i].len) assert(tc.children[i].data[j] == children[i].data[j]);

        assert(tl.order[i] == order[i]);
        assert(tl.levels[i] == levels[i]);
        assert(tl.parents[i] == parents[i]);
        assert(tc.children[i].len == children[i].len);
        RANGE(j, tc.children[i].len) assert(tc.children[i].data[j] == children[i].data[j]);
    }
}
