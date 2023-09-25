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
        execl(ls, ls, NULL);
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
        execlp(ls, ls, NULL);
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
        execle(ls, ls, NULL, envp);
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
        execv(myargs[0], myargs);
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
        execvp(myargs[0], myargs);
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

        execvpe(myargs[0], myargs, envp);
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

