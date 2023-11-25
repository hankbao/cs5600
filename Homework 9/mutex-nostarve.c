#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"
#include "sem.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    sem_t inner;
    sem_t pv;
    volatile int count;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    sem_init(&m->inner, 0, 1);
    sem_init(&m->pv, 0, 0);
    m->count = 0;
}

void ns_mutex_acquire(ns_mutex_t *m) {
    sem_wait(&m->inner);
    m->count++;
    if (m->count > 1) {
        sem_post(&m->inner);
        sem_wait(&m->pv);
    } else {
        sem_post(&m->inner);
    }
}

void ns_mutex_release(ns_mutex_t *m) {
    sem_wait(&m->inner);
    m->count--;
    if (m->count > 0) {
        sem_post(&m->inner);
        sem_post(&m->pv);
    } else {
        sem_post(&m->inner);
    }
}

ns_mutex_t m;

typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *worker(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("worker %d: before\n", t->thread_id);
    ns_mutex_acquire(&m);
    sleep(1);
    ns_mutex_release(&m);
    printf("worker %d: after\n", t->thread_id);
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");

    ns_mutex_init(&m);

    for (int i = 0; i < num_threads; i++) {
        t[i].thread_id = i;
        Pthread_create(&p[i], NULL, worker, &t[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        Pthread_join(p[i], NULL);
    }

    printf("parent: end\n");
    return 0;
}
