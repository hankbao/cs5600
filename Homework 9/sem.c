// sem.c
// Semaphore implementation on macOS
// copy of Zemaphore from OSTEP

#ifdef __APPLE__

#include "sem.h"

void sem_init(sem_t *sem, int _dummy, unsigned int value) {
    sem->value = value;
    pthread_cond_init(&sem->cond, NULL);
    pthread_mutex_init(&sem->lock, NULL);
}

void sem_wait(sem_t *sem) {
    pthread_mutex_lock(&sem->lock);
    while (sem->value <= 0) {
        pthread_cond_wait(&sem->cond, &sem->lock);
    }
    sem->value--;
    pthread_mutex_unlock(&sem->lock);
}

void sem_post(sem_t *sem) {
    pthread_mutex_lock(&sem->lock);
    sem->value++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->lock);
}

#endif
