// ch5c5.c
// chapter 5, coding homework 5
// author: Hank Bao
//
// Now write a program that uses wait() to wait for the child process
// to finish in the parent.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        int ret = wait(NULL);
        printf("Child (%d): wait() returned %d\n", (int) getpid(), ret);
        if (ret < 0) {
            perror("wait");
        }
    } else {
        // parent goes down this path (original process)
        int ret = wait(NULL);
        printf("Parent (%d): wait() returned %d\n", (int) getpid(), ret);
        if (ret < 0) {
            perror("wait");
        }
    }

    return EXIT_SUCCESS;
}
