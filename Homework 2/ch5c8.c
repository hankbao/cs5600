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

void
call_ls(int pipefd[2])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        close(pipefd[0]);  // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect the standard output to the pipe's write end

        char *myargs[2];
        myargs[0] = strdup("/bin/ls");   // program: "ls" (word count)
        myargs[1] = NULL;
        execvp(myargs[0], myargs);  // runs word count
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

void
call_grep(int pipefd[2])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        close(pipefd[1]);  // Close the write end of the pipe
        dup2(pipefd[0], STDIN_FILENO);  // Redirect the standard input from the pipe's read end

        char *myargs[4];
        myargs[0] = strdup("/bin/grep");   // program: "ls"
        //myargs[1] = strdup("-v");
        myargs[1] = strdup("md");
        myargs[2] = NULL;
        execvp(myargs[0], myargs);  // runs word count
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

int
main(int argc, char *argv[])
{
    int pipefd[2];

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    call_ls(pipefd);
    call_grep(pipefd);

    return EXIT_SUCCESS;
}
