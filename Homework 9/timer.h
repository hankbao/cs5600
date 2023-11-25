// timer.h
// author: Hank Bao

#pragma once

#include <unistd.h>

typedef struct {
    uint64_t start_ticks;
    uint64_t end_ticks;
    long double frequency;
} Timer;

void timer_start(Timer* timer);

void timer_end(Timer* timer);

void timer_init(Timer* timer);

long double timer_duration_nanoseconds(Timer* timer);
