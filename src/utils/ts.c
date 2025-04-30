#include "utils/ts.h"
#include "macros.h"

#include <dlfcn.h>
#include <stdio.h>

TsOK ts_open(char* name, char* path)
{
    char* err;
    Ts    ts = {0};

    // Open dll
    ts.handle = dlopen(path, RTLD_LAZY);
    err       = dlerror();
    if (err)
    {
        error("%s", err);
        return (TsOK){.err = TS_FAIL_DLOPEN};
    }

    // Check valid handle
    if (!ts.handle)
    {
        error("Handle is null");
        return (TsOK){.err = TS_FAIL_HANDLE};
    }

    // Bind language function
    ts.lang = (lang_fn)dlsym(ts.handle, name);
    err     = dlerror();
    if (err)
    {
        error("%s", err);
        return (TsOK){.err = TS_FAIL_DLSYM};
    }

    // Create parser and set language
    ts.parser = ts_parser_new();
    bool bok  = ts_parser_set_language(ts.parser, ts.lang());
    if (!bok)
    {
        error("Could not set language");
        return (TsOK){.err = TS_FAIL_LANG};
    }

    // Return successfully loaded treesitter parser
    return (TsOK){.data = ts, .err = TS_SUCCESS};
}

void ts_close(Ts* ts) { dlclose(ts->handle); }

void ts_parse(Ts* ts, Str content)
{
    if (!ts->parser || (content.len == 0)) return;

    ts->tree = ts_parser_parse_string(ts->parser, NULL, content.buf, content.len);
    ts->root = ts_tree_root_node(ts->tree);
}

static char* get_indent_str(int indent, char* buf)
{
    for (int i = 0; i < indent; i++) { buf[i] = ' '; }
    buf[indent] = '\0';
    return buf;
}

void ts_print_named_node(TSNode n, char* content, int indent, const char* field)
{
    char buf[128] = {0};

    // Only print named nodes
    if (ts_node_is_named(n))
    {
        TSPoint sp         = ts_node_start_point(n);
        TSPoint ep         = ts_node_end_point(n);
        char*   indent_str = get_indent_str(indent, buf);
        if (!field)
        {
            printf("%s (%s [%d, %d] - [%d, %d]", //
                   indent_str, ts_node_type(n), sp.row, sp.column, ep.row, ep.column);
        }
        else
        {
            printf("%s %s : (%s [%d, %d] - [%d, %d]", //
                   indent_str, field, ts_node_type(n), sp.row, sp.column, ep.row, ep.column);
        }
    }

    // Iterate through all or we miss fields
    int n_children = ts_node_child_count(n);
    for (int i = 0; i < n_children; i++)
    {
        TSNode child = ts_node_child(n, i);
        if (ts_node_is_named(child)) { printf("\n"); }
        const char* field_name = ts_node_field_name_for_child(n, i);
        ts_print_named_node(child, content, indent + 2, field_name);
    }

    // End of named node
    if (ts_node_is_named(n)) { printf(")"); }
}
