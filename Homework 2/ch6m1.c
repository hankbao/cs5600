// ch6m1.c
// chapter 6, measurement homework 1
// author: Hank Bao
//
// Measuring the average cost of a system call

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

#define ITER_TIME 100000

int main() {
    char buf[1];
    struct timeval start, end;

    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    if (gettimeofday(&start, NULL) < 0) {
        perror("gettimeofday");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < ITER_TIME; i++) {
        if (read(fd, buf, 0) != 0) {
            perror("read");
            return EXIT_FAILURE;
        }
    }

    if (gettimeofday(&end, NULL) < 0) {
        perror("gettimeofday");
        return EXIT_FAILURE;
    }

    close(fd);

    long avg_cost = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    printf("Average cost of a system call is: %.4f microseconds\n", (double)avg_cost / ITER_TIME);

    return EXIT_SUCCESS;
}
