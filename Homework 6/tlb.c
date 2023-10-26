// tlb.c
// chapter 19, coding homework
// author: Hank Bao


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <getopt.h>


#ifdef __APPLE__
#include <mach/mach_time.h>
#elif __linux__
#include <time.h>
#include <string.h>
#endif

#ifdef __APPLE__
long double get_tsc_frequency() {
    uint64_t start, end, elapsed;
    long double freq = 0.0;

    // Get the timebase info
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);

    // Get a timestamp before the delay
    start = mach_absolute_time();

    // Introduce a delay (e.g., 1 second)
    sleep(1);

    // Get a timestamp after the delay
    end = mach_absolute_time();

    // Calculate the elapsed ticks
    elapsed = end - start;

    // Convert ticks to seconds
    long double elapsedSeconds = (long double)elapsed * info.numer / info.denom / 1e9;

    // Estimate the frequency
    freq = elapsed / elapsedSeconds;

    return freq;
}
#endif

#ifdef __linux__
long double get_tsc_frequency() {
    FILE *f;
    long double freq = 0.0;
    char line[256];

    f = fopen("/proc/cpuinfo", "r");
    if (!f) {
        perror("Error opening /proc/cpuinfo");
        return 0.0;
    }

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "cpu MHz : %lf", &freq) == 1) {
            freq *= 1.0e6;  // Convert MHz to Hz
            break;
        }
    }

    fclose(f);

    return freq;
}
#endif

typedef struct {
    uint64_t start_ticks;
    uint64_t end_ticks;
    long double frequency;
} Timer;

#ifdef __x86_64__
static inline uint64_t get_ticks(void) {
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x)); // rdtsc: 0F 31 -> EDX:EAX
    return x;
}
#elif defined(__arm64__) || defined(__aarch64__)
static inline uint64_t get_ticks(void) {
    uint64_t val;
    asm volatile("mrs %0, cntpct_el0" : "=r" (val));
    return val;
}
#endif

void timer_start(Timer* timer) {
    timer->start_ticks = get_ticks();
}

void timer_end(Timer* timer) {
    timer->end_ticks = get_ticks();
}

void timer_init(Timer* timer) {
#ifdef __x86_64__
    timer->frequency = get_tsc_frequency();
#elif defined(__arm64__) || defined(__aarch64__)
    uint64_t cntfrq;
    asm volatile("mrs %0, cntfrq_el0" : "=r" (cntfrq));
    timer->frequency = (long double)cntfrq;
#endif
}

long double timer_duration_nanoseconds(Timer* timer) {
    uint64_t elapsed_ticks = timer->end_ticks - timer->start_ticks;
    return (long double)elapsed_ticks / timer->frequency * 1e9;
}

int main(int argc, char *argv[]) {
    Timer timer = {0};
    timer_init(&timer);

    int opt;
    long long num_pages = 0;
    long long num_trials = 0;

    struct option long_options[] = {
        {"pages", required_argument, NULL, 'p'},
        {"trials", required_argument, NULL, 't'},
        {0, 0, 0, 0}
    };

    while (1) {
        int option_index = 0;

        opt = getopt_long(argc, argv, "p:t:", long_options, &option_index);

        if (opt == -1) {
            break;  // End of options
        }

        switch (opt) {
            case 'p':
                num_pages = atoll(optarg);
                break;
            case 't':
                num_trials = atoll(optarg);
                break;
            case '?':
                fprintf(stderr, "Usage: %s --pages=<num_pages> --trials=<num_trials>\n", argv[0]);
                exit(EXIT_FAILURE);
            default:
                abort();  // should not reach here
        }
    }

    if (num_pages <= 0 || num_trials <= 0) {
        fprintf(stderr, "Both --pages and --trials must be provided and greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    printf("Number of pages: %lld\n", num_pages);
    printf("Number of trials: %lld\n", num_trials);

    int pagesize = getpagesize();
    printf("Pagesize: %d\n", pagesize);

    const int jump = pagesize / sizeof(int);
    int* addr = (int*) malloc(num_pages * jump * sizeof(int));

    timer_start(&timer);
    for (unsigned long long i = 0; i < num_trials; ++i) {
        for (unsigned long long j = 0; j < num_pages * jump; j += jump) {
            addr[j] += 1;
        }
    }
    timer_end(&timer);

    long double total = timer_duration_nanoseconds(&timer);
    long double times = num_pages * num_trials;
    printf("Total: %Lf nanoseconds\n", total);
    printf("Avg: %Lf nanoseconds\n", total / times);

    free(addr);
    return EXIT_SUCCESS;
}
