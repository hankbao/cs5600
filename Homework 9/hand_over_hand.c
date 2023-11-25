// hand_over_hand.c
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

typedef struct __node_t {
    int key;
    pthread_mutex_t lock;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
    node_t *tail;
} list_t;

void list_init(list_t *list) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->key = 0;
    node->next = NULL;
    list->head = list->tail = node;
    pthread_mutex_init(&node->lock, NULL);
}

int list_insert(list_t *list, int key) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("malloc");
        return -1;
    } else {
        new_node->key = key;
        pthread_mutex_init(&new_node->lock, NULL);
    }

    node_t *tail = list->tail;
    pthread_mutex_lock(&tail->lock);
    tail->next = new_node;
    list->tail = new_node;
    pthread_mutex_unlock(&tail->lock);

    return 0;
}

int list_lookup(list_t *list, int key) {
    node_t *current = list->head;
    if (!current) {
        return -1;
    } else {
        pthread_mutex_lock(&current->lock);
    }

    while (1) {
        if (current->key == key) {
            pthread_mutex_unlock(&current->lock);
            return 0;
        } else {
            node_t *next = current->next;
            if (!next) {
                pthread_mutex_unlock(&current->lock);
                return -1;
            } else {
                pthread_mutex_lock(&next->lock);
                pthread_mutex_unlock(&current->lock);
                current = next;
            }
        }
    }
}

void* list_thread(void *arg) {
    unsigned long delta = (unsigned long)pthread_self();
    delta %= THREAD_NUM;

    list_t *list = (list_t *)arg;
    int ret = list_lookup(list, COUNT - delta - 1);
    assert(ret == 0);

    return NULL;
}

int main(int argc, char *argv[]) {
    Timer timer = {0};
    timer_init(&timer);

    list_t list = {0};
    list_init(&list);

    pthread_t threads[THREAD_NUM] = {0};

    for (int i = 0; i < COUNT; i++) {
        list_insert(&list, i);
    }

    timer_start(&timer);

    for (int i = 0; i < THREAD_NUM; i++) {
        int ret = pthread_create(&threads[i], NULL, list_thread, &list);
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
