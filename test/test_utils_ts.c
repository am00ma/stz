#include "arena.h"
#include "macros.h"
#include "str.h"
#include "utils/file.h"
#include "utils/ts.h"

#include <stdio.h>  // printf
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // strlen

constexpr i32 MB = 1024 * 1024;

// For easier concatenation, defining as macro
#define TS_DIR "/home/x/hub/repos/study/c/libs/parsers/nvim-ts-lsp-dap/data/ts"

typedef struct Row
{
    isize len;

} Row;

int main()
{
    // Currently hardcoded, but can be given with args
    Str   file = Str(__FILE__);
    char* name = "tree_sitter_c";
    char* path = TS_DIR "/c.so";

    // Alloc 10MB
    Arena perm = arena_new(10 * MB);

    // Read file (exit on error)
    auto textOK = file_read(file, &perm);
    die(textOK.err, "Could not read %.*s", pstr(file));
    Str text = textOK.data;

    // Open treesitter .so file (exit on error)
    TsOK tsOK = ts_open(name, path);
    die(tsOK.err, "Could not load %s from %s", name, path);
    Ts ts = tsOK.data;

    // Parse given file
    ts_parse(&ts, text);

    // Recursively print the tree
    ts_print_named_node(ts.root, text.buf, 0, NULL);
    printf("\n");

    // Make query
    Str     query = Str("(struct_specifier name: (type_identifier) @local.definition.type)");
    TsQ     tsq   = tsq_new(query);
    TsError err   = tsq_init(&tsq, &ts);
    die(err, "Could not parse query: %.*s", pstr(query));

    tsq_print(&tsq);
    tsq_parse(&tsq, &ts);

    // Close parser
    ts_close(&ts);

    return EXIT_SUCCESS;
}
