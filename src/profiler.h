#pragma once
#include <time.h>
#include <stdio.h>

#include <mydefines.h>

struct Profiler
{
    u32 num_iters;
    clock_t started_time;
    clock_t time_elapsed;
} _std_profiler;

inline void profiler_start(Profiler* profiler = &_std_profiler)
{
    profiler->num_iters = 0;
    profiler->started_time = clock();
}

inline void profiler_stop(Profiler* profiler = &_std_profiler)
{
    profiler->time_elapsed = clock() - profiler->started_time;
}

inline void profiler_print(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    fprintf(file, "Number of iterations: %u\nTime elapsed: %u (ms)\n", 
        profiler->num_iters, profiler->time_elapsed / (CLOCKS_PER_SEC / 1000));
}

inline void profiler_report_nicely(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    profiler_stop(profiler);
    printf("--------------------------------------\n");
    profiler_print(file, profiler);
    printf("--------------------------------------\n\n");
}
