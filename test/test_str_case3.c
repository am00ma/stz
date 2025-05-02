/* Split text into lines on '\n'
 *
 *   Strs strs_lines(Str text, bool ignore_empty, bool substitute_null, Arena* a);
 *
 * */

#include "macros.h"
#include "str.h"

int main()
{
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
    lines   = strs_lines(literal, false, false, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("1", &scratch);

    // BUG: cannot alter `const char *` !!
    // substitute_null -> can use as char**, alters `text`
    scratch = perm;
    lines   = strs_lines(text, false, true, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("1 (ignore)", &scratch);

    // Reset `text` with trailing '\n'
    literal = Str("Hello\nhi\nhow\nare\n\nnow\n");
    text    = str_copy(literal, &perm);
    dbg("%.*s", pstr(literal));

    // 'deault'
    scratch = perm;
    lines   = strs_lines(literal, false, false, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("2", &scratch);

    // substitute_null
    scratch = perm;
    lines   = strs_lines(text, false, true, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("2 (ignore)", &scratch);

    literal = Str("\n\n");
    text    = str_copy(literal, &perm);
    dbg("%.*s", pstr(literal));

    // 'deault'
    scratch = perm;
    lines   = strs_lines(literal, false, false, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("3", &scratch);

    // substitute_null
    scratch = perm;
    lines   = strs_lines(text, false, true, &scratch);
    for (int i = 0; i < lines.len; i++) dbg("%d: %.*s", i, pstr(lines.data[i]));
    arena_print("3 (ignore)", &scratch);

    return 0;
}
