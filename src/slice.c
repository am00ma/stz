#include "slice.h"
#include "abort.h"
#include "macros.h"
#include <assert.h>

Si32 si32_new(isize cap, Arena* perm) { return (Si32){.len = 0, .cap = cap, .data = new (perm, i32, cap)}; }
void si32_push(Si32* s, i32 x)
{
    if (s->len < s->cap)
    {
        s->data[s->len] = x;
        s->len++;
    }
    else { oom(); }
}

i32 si32_pop(Si32* s)
{
    if (s->len > 0)
    {
        s->len--;
        return s->data[s->len];
    }
    else { die(-1, "Popping empty slice"); }
}
