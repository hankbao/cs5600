// hash_table.c
// chapter 29, code
// author: Hank Bao

#include <pthread.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"

const int COUNT = 1000000;
const int THREAD_NUM = 10;
const int BUCKETS = 101;

typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

void list_init(list_t *list) {
    list->head = NULL;
    pthread_mutex_init(&list->lock, NULL);
}

int list_insert(list_t *list, int key) {
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("malloc");
        return -1;
    }
    new_node->key = key;

    pthread_mutex_lock(&list->lock);
    new_node->next = list->head;
    list->head = new_node;
    pthread_mutex_unlock(&list->lock);

    return 0;
}

int list_lookup(list_t *list, int key) {
    int rv = -1;
    pthread_mutex_lock(&list->lock);
    node_t *current = list->head;
    while (current) {
        if (current->key == key) {
            rv = 0;
            break;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&list->lock);
    return rv;
}

typedef struct __hash_t {
    list_t lists[BUCKETS];
} hash_t;

void hash_init(hash_t *hash) {
    int i;
    for (i = 0; i < BUCKETS; i++) {
        list_init(&hash->lists[i]);
    }
}

int hash_insert(hash_t *hash, int key) {
    const int bucket = key % BUCKETS;
    return list_insert(&hash->lists[bucket], key);
}

int hash_lookup(hash_t *hash, int key) {
    const int bucket = key % BUCKETS;
    return list_lookup(&hash->lists[bucket], key);
}

void* hash_thread(void *arg) {
    hash_t *hash = (hash_t *)arg;
    for (int i = 0; i < COUNT; i++) {
        hash_insert(hash, i);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    Timer timer = {0};
    timer_init(&timer);

    hash_t hash = {0};
    hash_init(&hash);

    pthread_t threads[THREAD_NUM] = {0};

    timer_start(&timer);

    for (int i = 0; i < THREAD_NUM; i++) {
        int ret = pthread_create(&threads[i], NULL, hash_thread, &hash);
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
