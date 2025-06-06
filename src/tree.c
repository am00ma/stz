#include "tree.h"
#include "arena.h"
#include "array.h"
#include "range.h"
#include "slice.h"
#include <assert.h>
#include <stdio.h>

void indexLevels(Tree* t, i32 i, i32 level)
{
    t->levels[i] = level;
    RANGE(j, t->children[i].len) { indexLevels(t, t->children[i].data[j], level + 1); }
}

i32 indexOrder(Tree* t, i32 i, i32 count)
{
    count++;
    t->order[count] = i;
    RANGE(j, t->children[i].len) { count = indexOrder(t, t->children[i].data[j], count); }
    return count;
}

Tree tree_from_children(i32 len, Ai32* children, Arena* perm)
{
    Tree t = {
        .len      = len,
        .order    = new (perm, i32, len),
        .levels   = new (perm, i32, len),
        .parents  = new (perm, i32, len),
        .children = new (perm, Ai32, len),
    };
    RANGE(i, len) { t.parents[i] = -1; }

    // Store children, Index parents
    RANGE(i, len)
    {
        t.children[i] = ai32_new(children[i].len, perm);
        RANGE(j, children[i].len)
        {
            t.children[i].data[j] = children[i].data[j];

            t.parents[t.children[i].data[j]] = i;
        }
    }

    // Index order and levels
    i32 count = -1;
    RANGE(i, len)
    {
        if (t.parents[i] == -1)
        {
            indexLevels(&t, i, 0);
            count = indexOrder(&t, i, count);
        }
    }

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
    i32 level_root = levels[0];

    Arena temp  = *perm;
    Si32  stack = si32_new(len, &temp);
    si32_push(&stack, -1);

    // Index parents (only one pass needed, no allocations)
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

    // Index children (min 2 passes, 4 passes needed for proper inits, memory management)
    Si32* children  = new (&temp, Si32, len);
    i32*  nchildren = new (&temp, i32, len);
    RANGE(i, len)
    {
        if (t.parents[i] >= 0) { nchildren[t.parents[i]]++; }
    }
    RANGE(i, len) { children[i] = si32_new(nchildren[i], perm); }
    RANGE(i, len)
    {
        if (t.parents[i] >= 0) { si32_push(&children[t.parents[i]], i); }
    }
    RANGE(i, len) { t.children[i] = (Ai32){.len = children[i].len, .data = children[i].data}; }

    return t;
}
