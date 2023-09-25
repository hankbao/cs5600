// ch5c1.c
// chapter 5, coding homework 1
// author: Hank Bao
//
// Write a program that calls fork(). Before calling fork(),
// have the main process access a variable (e.g., x)
// and set its value to something (e.g., 100).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int x = 100;

    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // child (new process)
        printf("Value x in child (pid:%d) is %d.\n", (int) getpid(), x);

        x = 50;
        printf("Value x in child (pid:%d) is %d after changing.\n", (int) getpid(), x);
    } else {
        // parent goes down this path (original process)
        printf("Value x in parent (pid:%d) is %d.\n", (int) getpid(), x);

        x = 30;
        printf("Value x in parent (pid:%d) is %d after changing.\n", (int) getpid(), x);
    }

    return EXIT_SUCCESS;
}
