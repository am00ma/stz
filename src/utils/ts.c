#include "utils/ts.h"
#include "macros.h"

#include <dlfcn.h>
#include <stdio.h>

static char* get_substring(char* line, int start, int end)
{
    int   len = end - start;
    char* s   = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len; i++) { s[i] = line[start + i]; }
    s[len] = '\0';
    return s;
}

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

void ts_parse(Ts* ts, Str text)
{
    ts->text = text;
    if (!ts->parser || (text.len == 0)) return;

    ts->tree = ts_parser_parse_string(ts->parser, NULL, text.buf, text.len);
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

    if (ts_node_is_null(n))
    {
        error("Null node");
        return;
    }

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
        } else {
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

TsQ tsq_new(Str query) { return (TsQ){.query = query}; }

TsError tsq_init(TsQ* tsq, Ts* ts)
{
    uint32_t     eo;
    TSQueryError et;
    tsq->q = ts_query_new(ts->lang(), tsq->query.buf, tsq->query.len, &eo, &et);
    if (et != TSQueryErrorNone)
    {
        error("Error in `ts_query_new`: %lu\n", (uint64_t)et);
        return TS_FAIL_QUERY;
    }

    // Total number of query patterns, unique capture names and strings
    tsq->n_patterns = ts_query_pattern_count(tsq->q);
    tsq->n_captures = ts_query_capture_count(tsq->q);
    tsq->n_strings  = ts_query_string_count(tsq->q);

    return TS_SUCCESS;
}

TsError tsq_parse(TsQ* tsq, Ts* ts)
{
    // Execute query on code
    TSQueryMatch   match;
    TSQueryCursor* cur = ts_query_cursor_new();
    ts_query_cursor_exec(cur, tsq->q, ts->root);

    while (ts_query_cursor_next_match(cur, &match))
    {
        printf("  pattern: %d\n", match.pattern_index);
        for (int ci = 0; ci < match.capture_count; ci++)
        {
            TSQueryCapture cp    = match.captures[ci];
            TSNode         n     = cp.node;
            TSPoint        start = ts_node_start_point(n);
            TSPoint        end   = ts_node_end_point(n);

            uint32_t length;
            // TODO: Not sure how to free, get discards qualifiers
            const char* capture_name = ts_query_capture_name_for_id(tsq->q, cp.index, &length);

            uint32_t sb   = ts_node_start_byte(n);
            uint32_t eb   = ts_node_end_byte(n);
            char*    text = get_substring(ts->text.buf, sb, eb);

            printf("%d: %s\n", cp.index, capture_name);
            printf("  start: %d, %d\n", start.row, start.column);
            printf("    end: %d, %d\n", end.row, end.column);
            printf("   text: %.*s\n", eb - sb, text);
        }
    }
    return TS_SUCCESS;
}

void tsq_print(TsQ* tsq)
{
    printf("query: %.*s\n", pstr(tsq->query));
    printf("  n_patterns: %d\n", tsq->n_patterns);
    printf("  n_captures: %d\n", tsq->n_captures);
    printf("  n_strings:  %d\n", tsq->n_strings);
}
