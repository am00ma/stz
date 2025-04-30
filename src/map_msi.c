#include "map_msi.h"
#include <stdio.h> // printf

StrMapMSI strmap_msi_new(isize cap2, Arena* a)
{
    StrMapMSI env = {0};
    env.cap2      = cap2;
    env.keys      = new (a, Str, 1 << cap2);
    env.vals      = new (a, Str, 1 << cap2);
    return env;
}

// Now we leverage equals and hash64 for a double-hashed, open address search on
// the keys array.
// By returning a pointer to the unmodified value slot, this function covers
// both lookup and insertion. So that’s the entire hash table implementation. To
// insert, the caller assigns the slot. For mere lookup, check the slot for a
// null pointer.
Str* strmap_msi_lookup(StrMapMSI* env, Str key)
{
    u64 hash = str_hash64(key);
    u64 mask = (1 << env->cap2) - 1;
    u64 step = (hash >> (64 - env->cap2)) | 1;
    for (i64 i = hash;;)
    {
        i = (i + step) & mask;
        if (!env->keys[i].buf)
        {
            env->keys[i] = key;
            return env->vals + i;
        }
        else if (str_equal(env->keys[i], key)) { return env->vals + i; }
    }
}

void strmap_msi_print(StrMapMSI* env, Str key)
{
    Str* val = 0;
    val      = strmap_msi_lookup(env, key);
    printf("%p\n", (void*)val);
    if (val->len) { printf("%10.*s: %.*s\n", (int)key.len, key.buf, (int)val->len, val->buf); }
    else { printf("!! %10.*s: not found\n", (int)key.len, key.buf); }
}
