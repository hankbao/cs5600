// ch6m2.c
// chapter 6, measurement homework 2
// author: Hank Bao
//
// Measuring the average cost of a context switch

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sched.h>

#define ITER_TIME 100000

int main(void) {
    int pipe1[2], pipe2[2];
    pid_t child;
    char text = 'x';

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child = fork();
    if (child < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Set CPU affinity
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    if (sched_setaffinity(getpid(), sizeof(set), &set) < 0) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    if (child == 0) {
        // Child process
        close(pipe1[1]);
        close(pipe2[0]);

        for (int i = 0; i < ITER_TIME; i++) {
            if (read(pipe1[0], &text, 1) != 1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (write(pipe2[1], &text, 1) != 1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        // Parent process
        close(pipe1[0]);
        close(pipe2[1]);

        struct timeval start, end;
        if (gettimeofday(&start, NULL) < 0) {
            perror("gettimeofday");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < ITER_TIME; i++) {
            if (write(pipe1[1], &text, 1) != 1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            if (read(pipe2[0], &text, 1) != 1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
        }

        if (gettimeofday(&end, NULL) < 0 ) {
            perror("gettimeofday");
            exit(EXIT_FAILURE);
        }

        long avg_cost = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;

        printf("Average cost of a context switch is: %.4f microseconds\n", (double) avg_cost / 2 / ITER_TIME);

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
