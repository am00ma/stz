#pragma once

#include "str.h"

typedef struct StrMapTrie StrMapTrie;
typedef struct StrMapTrie {
    StrMapTrie* child[4];
    Str         key;
    void*       value;
} StrMapTrie;

void* strmap_trie_lookup(StrMapTrie** t, Str key, Arena* a);
