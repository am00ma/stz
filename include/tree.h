#pragma once

#include "array.h"

typedef struct {
    i32   len;      // len - defines idx
    i32*  order;    // Order of traversal
    i32*  levels;   // From root (min: 0, i.e. root has level -1)
    i32*  parents;  // Parents   (nil: -1)
    Ai32* children; // Children  (nil: len == 0)
} Tree;

Tree tree_from_children(i32 len, Ai32* children, Arena* perm);
Tree tree_from_levels(i32 len, i32* levels, Arena* perm);
