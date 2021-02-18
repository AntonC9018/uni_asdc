#pragma once
#include <chrono>
#include <stdio.h>

#include <mydefines.h>

struct Profiler
{
    u32 num_iters;
    std::chrono::system_clock::time_point started_time;
    std::chrono::milliseconds time_elapsed;
};

static Profiler _std_profiler;

inline void profiler_start(Profiler* profiler = &_std_profiler)
{
    profiler->num_iters = 0;
    profiler->started_time = std::chrono::high_resolution_clock::now();
}

inline void profiler_stop(Profiler* profiler = &_std_profiler)
{
    profiler->time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - profiler->started_time);
}

inline void profiler_print(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    fprintf(file, "Number of iterations: %u\nTime elapsed: %i (ms)\n", 
        profiler->num_iters, (int)profiler->time_elapsed.count());
}

inline void profiler_report_nicely(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    profiler_stop(profiler);
    printf("--------------------------------------\n");
    profiler_print(file, profiler);
    printf("--------------------------------------\n\n");
}