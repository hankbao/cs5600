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
    int pipefd[2];

    // Create the pipe
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        // Redirect the standard output to the pipe's write end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        char *myargs[2];
        myargs[0] = strdup("/bin/ls");
        myargs[1] = NULL;
        if (execvp(myargs[0], myargs) < 0) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        wait(NULL);

        // Redirect the standard input from the pipe's read end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        char *myargs[3];
        myargs[0] = strdup("grep");
        myargs[1] = strdup("c8");
        myargs[2] = NULL;
        if (execvp(myargs[0], myargs) < 0) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
