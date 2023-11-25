// simple_counter.c
// chapter 29, code
// author: Hank Bao

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"

const int COUNT = 1000000;
const int THREAD_NUM = 4;

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

void counter_init(counter_t *counter) {
    counter->value = 0;
    pthread_mutex_init(&counter->lock, NULL);
}

void counter_increment(counter_t *counter) {
    pthread_mutex_lock(&counter->lock);
    counter->value++;
    pthread_mutex_unlock(&counter->lock);
}

void counter_decrement(counter_t *counter) {
    pthread_mutex_lock(&counter->lock);
    counter->value--;
    pthread_mutex_unlock(&counter->lock);
}

int counter_get(counter_t *counter) {
    pthread_mutex_lock(&counter->lock);
    int rc = counter->value;
    pthread_mutex_unlock(&counter->lock);
    return rc;
}

void* counter_thread(void *arg) {
    counter_t *counter = (counter_t *) arg;
    for (int i = 0; i < COUNT; i++) {
        counter_increment(counter);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    Timer timer = {0};
    timer_init(&timer);

    counter_t counter;
    counter_init(&counter);

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
