#include "map_trie.h"

void* strmap_trie_lookup(StrMapTrie** env, Str key, Arena* a)
{
    // h<<==2 ?? h >> 62 ?? wtf is going on??
    for (u64 h = str_hash64(key); *env; h <<= 2)
    {

        // Return value if key matches
        if (str_equal(key, (*env)->key)) { return &(*env)->value; }

        // Else, proceed to child? which child?
        // - Based on hash choose out of 4 children
        //   => Next time hash will follow same tree
        env = &(*env)->child[h >> 62];
    }

    // if only lookup, then return that nothing was found
    if (!a) return 0;

    // else insert into arena
    *env        = (StrMapTrie*)arena_alloc(a, sizeof(StrMapTrie), alignof(StrMapTrie), 1, 0);
    (*env)->key = key;
    return &(*env)->value;
}
