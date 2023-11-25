// sem.h
// Semaphore implementation on macOS
// copy of Zemaphore from OSTEP

#ifdef __APPLE__

#include <pthread.h>

typedef struct __sem_t {
    unsigned int value;
    pthread_cond_t cond;
    pthread_mutex_t lock;
} sem_t;

void sem_init(sem_t *sem, int _dummy, unsigned int value);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);

#elif defined(__linux__)

#include <semaphore.h>

#else

#error "Unknown platform"

#endif
