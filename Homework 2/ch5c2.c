// ch5c2.c
// chapter 5, coding homework 2
// author: Hank Bao
//
// Write a program that opens a file (with the `open()` system call)
// and then calls `fork()` to create a new process.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/uio.h>

int
main(int argc, char *argv[])
{
    int fd = open("./ch5c2.output", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        ssize_t wrote = write(fd, "child\n", strlen("child\n"));
        if (wrote < 0) {
            printf("Child (%d): cannot access fd %d opened in parent.\n", (int) getpid(), fd);
            perror("write");

            exit(EXIT_FAILURE);
        } else {
            printf("Chld (%d): wrote %zd bytes to fd %d.\n", (int)getpid(), wrote, fd);
        }
    } else {
        // parent
        int wc = wait(NULL);
        assert(wc >= 0);

        ssize_t wrote = write(fd, "parent\n", strlen("parent\n"));
        if (wrote < 0) {
            printf("Parent (%d): cannot access fd %d.\n", (int) getpid(), fd);
            perror("write");

            exit(EXIT_FAILURE);
        } else {
            printf("Parent (%d): wrote %zd bytes to fd %d.\n", (int)getpid(), wrote, fd);
        }
    }

    return EXIT_SUCCESS;
}
