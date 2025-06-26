#include "range.h"
#include "utils/df.h"

constexpr isize MB = 1024 * 1024;

int main()
{

    Arena mem  = arena_new(16 * MB);
    Arena strs = arena_new(16 * MB);

    isize cap       = 1024;
    isize num_cols  = 3;
    Str   columns[] = {Str("idx"), Str("path"), Str("ext")};
    Dtype dtypes[]  = {UINT32, STR, STR};

    DataFrame df = df_new(num_cols, columns, dtypes, cap, mem, strs);

    u32 idxs[]  = {1, 2};
    Str paths[] = {Str("include/arena.h"), Str("src/arena.c")};
    Str exts[]  = {Str("h"), Str("c")};

    isize len = 0;
    RANGE(i, countof(idxs))
    {
        ((u32*)df.cols[0])[i] = idxs[i];
        ((Str*)df.cols[1])[i] = paths[i];
        ((Str*)df.cols[2])[i] = exts[i];
        len++;
    }
    df.len = len;

    df_print(&df);

    return 0;
}
