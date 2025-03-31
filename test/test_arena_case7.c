/* Using threads to load files async */

#include "arena.h"   // Arena, new, arena_new, arena_print
#include "range.h"   // RANGE
#include <pthread.h> // pthread_create
#include <stdio.h>   // printf
#include <string.h>

constexpr int NUM = 3;
constexpr int CAP = 512;

typedef struct {
    int    i;
    Arena* a;
    char*  buf;
} ThreadData;

void* read_file(void* arg)
{
    ThreadData* data = (ThreadData*)arg;

    // 'Create' file
    char file[500] = {0};
    memset(file, 'A' + data->i, sizeof(file));

    // 'Read' file
    data->buf = new (data->a, char, sizeof(file));
    memcpy(data->buf, file, sizeof(file));

    return NULL;
}

int main()
{
    pthread_t  threads[NUM];
    ThreadData data[NUM];
    Arena      arenas[NUM];

    // Partition arena equally ( Just to keep it down to 1 malloc )
    Arena perm = arena_new(CAP * NUM);
    RANGE(i, NUM)
    {
        arenas[i] = (Arena){
            .beg = perm.beg + (i * CAP),
            .end = perm.beg + (i * CAP) + CAP,
            .cap = CAP,
        };
        data[i] = (ThreadData){.i = i, .a = &arenas[i]};
    }

    RANGE(i, NUM)
    {
        printf("%d:\n", i);
        arena_print("Before create", data[i].a);
    }
    RANGE(i, NUM) pthread_create(&threads[i], NULL, read_file, &data[i]);

    RANGE(i, NUM) pthread_join(threads[i], NULL);
    RANGE(i, NUM)
    {
        printf("%d: %.*s\n", i, 10, data[i].buf);
        arena_print("After join", data[i].a);
    }

    return 0;
}
