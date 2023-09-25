// ch5c7.c
// chapter 5, coding homework 7
// author: Hank Bao
//
// Write a program that creates a child process, and then in the child
// closes standard output (`STDOUT_FILENO`).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
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
	    close(STDOUT_FILENO);
        printf("Child (%d): this shouldn’t print out", (int) getpid());
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
	    assert(wc >= 0);
    }

    return EXIT_SUCCESS;
}
