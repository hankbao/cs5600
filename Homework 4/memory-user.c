// memory-user.c
// chapter 13, coding homework
// author: Hank Bao
//
// create a little program that uses a certain amount of memory,
// called memory-user.c. This program should take one commandline argument:
// the number of megabytes of memory it will use.
// When run, it should allocate an array, and constantly stream through
// the array, touching each entry. The program should do this indefinitely,
// or, perhaps, for a certain amount of time also specified at the command line.

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage() {
    puts("Usage: memory-user -a --amount <mem_amount> [-t --time <iter_count>]");
    puts("\t-a --amount <mem_amount>\tThe number of megabytes of memory the program will use.");
    puts("\t-t --time <iter_count>\t\tThe number of iterations on the allocated array the program will perform. If omitted, the program runs indefinitely.");
    puts("\t-h --help\t\t\tPrint this usage.");
}

int main(int argc, char *argv[]) {
    int opt;
    int mem_amount = 0, iter_count = -1;

    struct option long_options[] = {
        {"amount", required_argument, 0, 'a'},
        {"time", required_argument, 0, 't'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "a:t:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'a':
                mem_amount = atoi(optarg);
                break;
            case 't':
                iter_count = atoi(optarg);
                break;
            case 'h':
            default:
                print_usage();
                return EXIT_SUCCESS;
        }
    }

    if (mem_amount <= 0) {
        puts("Memory amount must be greater than zero.");
        print_usage();
        return EXIT_FAILURE;
    }

    // print out pid
    printf("memory-user: pid is %d\n", getpid());

    // allocate memory
    char *mem = (char *) malloc(mem_amount * 1024 * 1024);
    if (!mem) {
        puts("Failed to allocate memory.");
        return EXIT_FAILURE;
    }

    // touch each entry
    while (iter_count != 0) {
        for (int i = 0; i < mem_amount * 1024 * 1024; ++i) {
            mem[i] = 0;
        }

        // decrement iter_count if it's not -1
        if (iter_count > 0) {
            iter_count--;
        }
    }

    return EXIT_SUCCESS;
}
