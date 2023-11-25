// timer.c
// author: Hank Bao

#include <unistd.h>

#ifdef __APPLE__
#include <mach/mach_time.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#elif __linux__
#include <time.h>
#include <string.h>
#endif

#include "timer.h"

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
