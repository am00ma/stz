#include "array.h"
#include "buf.h"
#include "range.h"
#include "str.h"

Ai32 ai32_new(isize len, Arena* perm) { return (Ai32){.len = len, .data = new (perm, i32, len)}; }

Str ai32_print(Ai32* a, Arena* perm)
{
    isize ndigits = 32;
    Buf   out     = buf_new((a->len * ndigits) + 4, perm); // Account for `[ ... , ]` as well

    buf_join(&out, Str("["));
    RANGE(i, a->len) { buf_join(&out, str_fmtn(perm, ndigits, "%d, ", a->data[i])); }

    // Remove trailing `, `
    out.len -= 2;
    buf_join(&out, Str("]"));

    return buf_final(&out, perm);
}
