#include "str.h"
#include <tree_sitter/api.h>

typedef TSLanguage* (*lang_fn)(void);

typedef enum {
    TS_SUCCESS = 0,
    TS_FAIL_DLOPEN,
    TS_FAIL_HANDLE,
    TS_FAIL_DLSYM,
    TS_FAIL_LANG,
} TsError;

typedef struct {
    void*     handle;
    lang_fn   lang;
    TSParser* parser;

    TSTree* tree;
    TSNode  root;
} Ts;

typedef struct {
    Ts      data;
    TsError err;
} TsOK;

/* Open and close dll */
TsOK ts_open(char* name, char* path);
void ts_close(Ts* ts);

/* Main parse function (Sets tree and root in Ts) */
void ts_parse(Ts* ts, Str content);

/* Recursive */
void ts_print_named_node(TSNode n, char* content, int indent, const char* field);
