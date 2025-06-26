#include "../str.h"
#include <tree_sitter/api.h>

typedef TSLanguage* (*lang_fn)(void);

typedef enum
{
    TS_SUCCESS = 0,
    TS_FAIL_DLOPEN,
    TS_FAIL_HANDLE,
    TS_FAIL_DLSYM,
    TS_FAIL_LANG,
    TS_FAIL_QUERY,

} TsError;

typedef struct
{
    void*     handle;
    lang_fn   lang;
    TSParser* parser;

    Str     text;
    TSTree* tree;
    TSNode  root;

} Ts;

typedef struct
{
    Ts      data;
    TsError err;

} TsOK;

/* Open and close dll */
TsOK ts_open(char* name, char* path);
void ts_close(Ts* ts);

/* Main parse function (Sets tree and root in Ts) */
void ts_parse(Ts* ts, Str text);

/* Recursive */
void ts_print_named_node(TSNode n, char* content, int indent, const char* field);

typedef struct
{
    Str query;

    TSQuery* q;

    int n_patterns;
    int n_captures;
    int n_strings;

} TsQ;

TsQ     tsq_new(Str query);
TsError tsq_init(TsQ* tsq, Ts* ts);
TsError tsq_parse(TsQ* tsq, Ts* ts);

void tsq_print(TsQ* tsq);
