/* Split text into lines on '\n'
 *
 *   Strs str_split(Str text, Str delimiter, bool ignore_empty, bool substitute_null, Arena* a);
 *
 *   Currently supports only single char delimiter
 *   When using `substitute_null = true`, crashes if text.buf is string literal
 *
 * */

#include "macros.h"
#include "str.h"

int main()
{
    Str   delimiter = Str("\n");
    Strs  lines;
    Str   literal;
    Str   text;    // Modifiable for `substitute_null`
    Arena scratch; // To reset arena for each example

    Arena perm = arena_new(CAPACITY);

    literal = Str("Hello\nhi\nhow\nare\n\n\nyou");
    text    = str_copy(literal, &perm);
    dbg("%.*s", pstr(literal));

    // 'deault'
    scratch = perm;
    lines   = str_split(literal, delimiter, false, false, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("1", &scratch);

    // BUG: cannot alter `const char *` !!
    // substitute_null -> can use as char**, alters `text`
    scratch = perm;
    lines   = str_split(text, delimiter, false, true, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("1 (ignore)", &scratch);

    // Reset `text` with trailing '\n'
    literal = Str("Hello\nhi\nhow\nare\n\nnow\n");
    text    = str_copy(literal, &perm);
    dbg("%.*s", pstr(literal));

    // 'deault'
    scratch = perm;
    lines   = str_split(literal, delimiter, true, false, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("2 (ignore_empty)", &scratch);

    // substitute_null
    scratch = perm;
    lines   = str_split(text, delimiter, false, true, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("2 (substitute_null)", &scratch);

    literal = Str("\n\n");
    text    = str_copy(literal, &perm);
    dbg("%.*s", pstr(literal));

    // 'deault'
    scratch = perm;
    lines   = str_split(literal, delimiter, false, false, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("3", &scratch);

    // substitute_null
    scratch = perm;
    lines   = str_split(text, delimiter, true, true, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("3 (ignore, substitute_null)", &scratch);

    return 0;
}
