// ch5c4.c
// chapter 5, coding homework 4
// author: Hank Bao
//
// Write a program that calls `fork()` and then calls some form of
// `exec()` to run the program `/bin/ls`. See if you can try all of the
// variants of `exec()`, including (on Linux) `execl()`, `execle()`,
// `execlp()`, `execv()`, `execvp()`, and `execvpe()`.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>

void
call_execl()
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Child (%d) calling execl()\n", getpid());

        char *ls = "/bin/ls";
        if (execl(ls, ls, NULL) < 0) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

void
call_execlp()
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Child (%d) calling execlp()\n", getpid());

        char *ls = "/bin/ls";
        if (execlp(ls, ls, NULL) < 0) {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

void
call_execle()
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Child (%d) calling execle()\n", getpid());

        char *ls = "/bin/ls";
        char *envp[1];
        envp[0] = NULL;
        if (execle(ls, ls, NULL, envp) < 0) {
            perror("execle");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

void
call_execv()
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Child (%d) calling execv()\n", getpid());

        char *myargs[2];
        myargs[0] = strdup("/bin/ls");
        myargs[1] = NULL;
        if (execv(myargs[0], myargs) < 0) {
            perror("execv");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

void
call_execvp()
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Child (%d) calling execvp()\n", getpid());

        char *myargs[2];
        myargs[0] = strdup("/bin/ls");
        myargs[1] = NULL;
        if (execvp(myargs[0], myargs) < 0) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

void
call_execvpe()
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Child (%d) calling execvpe()\n", getpid());

        char *myargs[2];
        myargs[0] = strdup("/bin/ls");
        myargs[1] = NULL;

        char *envp[1];
        envp[0] = NULL;

        if (execvpe(myargs[0], myargs, envp) < 0) {
            perror("execvpe");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent goes down this path (original process)
        int ls = wait(NULL);
        assert(ls == rc);
    }
}

int
main(int argc, char *argv[])
{
    call_execl();
    call_execlp();
    call_execle();
    call_execv();
    call_execvp();
    call_execvpe();

    return EXIT_SUCCESS;
}

