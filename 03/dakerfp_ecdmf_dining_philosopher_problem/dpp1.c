/*
 * dpp1.c
 * Copyright (C) 2015 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Simple implementation of Dining Philosopher Problem using a naive
 * approach of philospher take right, left stick to eat. Might cause
 * deadlock when all left|right stick are take by all philosphers at
 * same time.
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "thread.h"

#define N 5

typedef struct context {
    int id;
    MUTEX_T* left_chopstick;
    MUTEX_T* right_chopstick;
} context_t;

THREAD_FUNC(print_hello, param) {
    context_t* ctx = param;

    for (;;) {
        SLEEP(rand() % 5);
        printf("%d is hungry!\n", ctx->id);
        MUTEX_LOCK(*ctx->right_chopstick);
        printf("%d got right chopstick\n", ctx->id);
        MUTEX_LOCK(*ctx->left_chopstick);
        printf("%d got left chopstick\n", ctx->id);

        printf("%d start eating\n", ctx->id);
        SLEEP(1);
        printf("%d finish eating\n", ctx->id);

        MUTEX_UNLOCK(*ctx->right_chopstick);
        printf("%d release right chopstick\n", ctx->id);
        MUTEX_UNLOCK(*ctx->left_chopstick);
        printf("%d release left chopstick\n", ctx->id);
    }

    free(ctx);
    return 0;
}

int main() {
    srand(time(NULL));
    MUTEX_T  chopsticks[N];
    THREAD_T philosophers[N];

    for (int i = 0; i < N; i++) {
        MUTEX_INIT(chopsticks[i]);
    }

    for (int i = 0; i < N; i++) {
        context_t* ctx = (context_t*) malloc(sizeof(context_t));
        ctx->id = i;
        ctx->left_chopstick = &chopsticks[i];
        ctx->right_chopstick = &chopsticks[(i+1) % N];

        THREAD_INIT(philosophers[i], print_hello, ctx);
    }

    for (int i = 0; i < N; i++) {
        THREAD_JOIN(philosophers[i]);
        THREAD_DESTROY(philosophers[i]);
    }

    for (int i = 0; i < N; i++) {
        MUTEX_DESTROY(chopsticks[i]);
    }

    return 0;
}
