// approximate_counter.c
// chapter 29, code
// author: Hank Bao

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"

const int COUNT = 1000000;
const int THREAD_NUM = 10;
const int THRESHOLD = 64;

typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int local[THREAD_NUM];
    pthread_mutex_t llock[THREAD_NUM];
    int threshold;
} counter_t;

void counter_init(counter_t *counter, int threshold) {
    counter->threshold = threshold;
    counter->global = 0;
    pthread_mutex_init(&counter->glock, NULL);
    for (int i = 0; i < THREAD_NUM; i++) {
        counter->local[i] = 0;
        pthread_mutex_init(&counter->llock[i], NULL);
    }
}

void counter_update(counter_t *counter, unsigned long thread_id, int amt) {
    unsigned long cpu = thread_id % THREAD_NUM;
    pthread_mutex_lock(&counter->llock[cpu]);
    counter->local[cpu] += amt;
    if (counter->local[cpu] >= counter->threshold) {
        pthread_mutex_lock(&counter->glock);
        counter->global += counter->local[cpu];
        pthread_mutex_unlock(&counter->glock);
        counter->local[cpu] = 0;
    }
    pthread_mutex_unlock(&counter->llock[cpu]);
}

int counter_get(counter_t *counter) {
    pthread_mutex_lock(&counter->glock);
    int val = counter->global;
    pthread_mutex_unlock(&counter->glock);
    return val;
}

void* counter_thread(void *arg) {
    counter_t *counter = (counter_t *) arg;
    for (int i = 0; i < COUNT; i++) {
        counter_update(counter, (unsigned long) pthread_self(), 1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    Timer timer = {0};
    timer_init(&timer);

    counter_t counter;
    counter_init(&counter, THRESHOLD);

    pthread_t threads[THREAD_NUM] = {0};

    timer_start(&timer);

    for (int i = 0; i < THREAD_NUM; i++) {
        int ret = pthread_create(&threads[i], NULL, counter_thread, &counter);
        if (ret != 0) {
            fprintf(stderr, "pthread_create failed: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    timer_end(&timer);

    long double total = timer_duration_nanoseconds(&timer);
    printf("total time: %Lf\n", total);

    return EXIT_SUCCESS;
}
