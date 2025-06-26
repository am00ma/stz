#include "utils/df.h"
#include "arena.h"
#include "range.h"
#include "types.h"
#include <stdio.h>

DataFrame df_new(isize num_cols, Str* columns, Dtype* dtypes, isize cap, Arena mem, Arena strs)
{
    DataFrame df = {
        .num_cols = num_cols,
        .cap      = cap,
        .mem      = mem,
        .strs     = strs,
    };

    // Store column strings to memory
    df.columns = new (&mem, Str, num_cols);
    RANGE(i, num_cols) { df.columns[i] = columns[i]; };

    // Store dtypes to memory
    df.dtypes = new (&mem, Dtype, num_cols);
    RANGE(i, num_cols) { df.dtypes[i] = dtypes[i]; };

    // Create columns
    df.cols = new (&mem, void*, num_cols);
    RANGE(i, num_cols)
    {
        void* col = 0;
        switch (df.dtypes[i])
        {
        case NIL: break;
        case INT32: col = new (&mem, i32, cap); break;
        case INT64: col = new (&mem, i64, cap); break;
        case UINT32: col = new (&mem, u32, cap); break;
        case UINT64: col = new (&mem, u64, cap); break;
        case FLOAT32: col = new (&mem, f32, cap); break;
        case FLOAT64: col = new (&mem, f64, cap); break;
        case STR: col = new (&mem, Str, cap); break;
        }
        df.cols[i] = col;
    }

    return df;
}

void df_print(DataFrame* df)
{
    printf("Columns:\n");
    RANGE(i, df->num_cols) { printf("  %15.*s %d\n", pstr(df->columns[i]), df->dtypes[i]); }

    RANGE(j, df->len)
    {
        printf(" %5d ", j);
        RANGE(i, df->num_cols)
        {
            switch (df->dtypes[i])
            {
            case NIL: break;
            case INT32: printf(" | %-15d ", ((i32*)df->cols[i])[j]); break;
            case INT64: printf(" | %-15ld ", ((i64*)df->cols[i])[j]); break;
            case UINT32: printf(" | %-15d ", ((u32*)df->cols[i])[j]); break;
            case UINT64: printf(" | %-15ld ", ((u64*)df->cols[i])[j]); break;
            case FLOAT32: printf(" | %-15f ", ((f32*)df->cols[i])[j]); break;
            case FLOAT64: printf(" | %-15f ", ((f64*)df->cols[i])[j]); break;
            case STR: printf(" | %-15.*s ", pstr(((Str*)df->cols[i])[j])); break;
            }
        }
        printf("|\n");
    }
}
