/* Allocating scratch from perm, to avoid excessive refactoring */

#include "arena.h" // Arena, new, arena_new, arena_print
#include "range.h"
#include <stdio.h>

constexpr isize len   = 10;
constexpr isize niter = 5;

void random_data(f32* x)
{
    RANGE(i, len) { x[i] = i; }
}

f32* level2(Arena* a)
{
    // Aggregate over some data
    f32* data[len] = {0};

    // Declare temp scratch arena from perm
    f32* tmp = new (a, f32, len);

    // Declare return array
    f32* sum = new (a, f32, len);

    // Iterate over some data
    RANGE(j, niter)
    {
        // Using the scratch arena
        data[j] = tmp;
        random_data(data[j]);
        RANGE(i, len) { sum[i] += data[j][i]; }
    }

    return sum;
}

f32* level1(Arena* a) { return level2(a); }

f32* base(Arena* a) { return level1(a); }

int main()
{
    Arena perm = arena_new(128);
    arena_print("Init", &perm);

    f32* sum = base(&perm);
    RANGE(i, len) printf("%d: %.2f\n", i, sum[i]);
}
