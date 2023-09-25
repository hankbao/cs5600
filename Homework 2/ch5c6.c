// ch5c6.c
// chapter 5, coding homework 6
// author: Hank Bao
//
// Write a slight modification of the previous program,
// this time using waitpid() instead of wait().

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
        int ret = waitpid(getppid(), NULL, 0);
        printf("Child (%d): waitpid() returned %d\n", (int) getpid(), ret);
        if (ret < 0) {
            perror("waitpid");
        }
    } else {
        // parent goes down this path (original process)
        int ret = waitpid(rc, NULL, 0);
        printf("Parent (%d): waitpid() returned %d\n", (int) getpid(), ret);
        if (ret < 0) {
            perror("waitpid");
        }
    }

    return EXIT_SUCCESS;
}
