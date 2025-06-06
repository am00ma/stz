#include "tree.h"
#include "range.h"
#include "slice.h"
#include <assert.h>

Tree tree_from_children(i32 len, Ai32* children, Arena* perm)
{
    Tree t = {.len = len, .children = children};
    return t;
}

Tree tree_from_levels(i32 len, i32* levels, Arena* perm)
{
    Tree t = {
        .len      = len,
        .order    = new (perm, i32, len),
        .levels   = new (perm, i32, len),
        .parents  = new (perm, i32, len),
        .children = new (perm, Ai32, len),
    };

    // First element is root
    i32  level_root = levels[0];
    Si32 stack      = si32_new(len, perm);
    si32_push(&stack, -1);

    RANGE(i, len)
    {
        t.order[i]  = i;
        t.levels[i] = levels[i] - level_root;

        if (i == 0) {} // Skip the following else statements

        else if (t.levels[i] > t.levels[i - 1])
        {
            // Make sure not malformed
            assert((t.levels[i] - t.levels[i - 1]) == 1);
            si32_push(&stack, i - 1);
        }
        else if (t.levels[i] < t.levels[i - 1])
        {
            // Pop as many parents as jump
            RANGE(j, t.levels[i - 1] - t.levels[i]) { si32_pop(&stack); }
        }

        // Parent is at top of stack
        t.parents[i] = stack.data[stack.len - 1];
    }

    return t;
}
