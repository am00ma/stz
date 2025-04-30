#pragma once

#include "str.h"

/* MSI - dynamically allocated, 32 bit hash */
typedef struct {
    Str*  keys;
    Str*  vals;
    isize cap2; // As power of 2, as we need number of bits in step of lookup
} StrMapMSI;

// Initialize capacity as power of 2
StrMapMSI strmap_msi_new(isize cap2, Arena* a);

// Lookup and store
Str* strmap_msi_lookup(StrMapMSI* env, Str key);

// Lookup and print
void strmap_msi_print(StrMapMSI* env, Str key);
