#include "arena.h"
#include "range.h"
#include "vec.h"
#include <stdio.h>

int main(void)
{
    Arena temp;
    Arena perm = arena_new(128);

    i32 num = 10;

    temp = perm;
    vec_i32 vi32 = vec_i32_range(num, &temp);
    RANGE(i, vi32.n) printf("%d: %d\n", i, vi32.x[i]);
    arena_print("i32 * 10", &temp);

    temp = perm;
    vec_f32 vf32 = vec_f32_range(num, &temp);
    RANGE(i, vf32.n) printf("%d: %0.2f\n", i, vf32.x[i]);
    arena_print("f32 * 10", &temp);

    temp = perm;
    vec_f64 vf64 = vec_f64_range(num, &temp);
    RANGE(i, vf64.n) printf("%d: %0.2f\n", i, vf64.x[i]);
    arena_print("f64 * 10", &temp);

    return 0;
}
