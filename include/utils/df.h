#pragma once

#include "arena.h"
#include "str.h"

typedef enum {

    NIL = 0,
    INT32,
    INT64,
    UINT32,
    UINT64,
    FLOAT32,
    FLOAT64,
    STR,

} Dtype;

typedef struct {

    isize  num_cols;
    Str*   columns;
    Dtype* dtypes;

} Schema;

typedef struct {

    isize  idx;
    void** cells;

} Row;

typedef struct {

    Row*  data;
    isize len;

} Rows;

typedef struct {

    isize  num_cols;
    Str*   columns;
    Dtype* dtypes;

    isize len;
    isize cap;

    void** cols;
    Arena  mem;  // Memory for other datatypes
    Arena  strs; // Memory to correctly point to strings

} DataFrame;

DataFrame df_new(isize num_cols, Str* columns, Dtype* dtypes, isize cap, Arena mem, Arena strs);

void df_print(DataFrame* df);
