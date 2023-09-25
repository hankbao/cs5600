// ch5c3.c
// chapter 5, coding homework 3
// author: Hank Bao
//
// Write another program using `fork()`. The child process should
// print “hello”; the parent process should print “goodbye”. You should
// try to ensure that the child process always prints first;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
        // child (new process)
        puts("hello");
    } else {
        // parent goes down this path (original process)
        int ret = wait(NULL);
        assert(ret == rc);

        puts("goodbye");
    }

    return EXIT_SUCCESS;
}
