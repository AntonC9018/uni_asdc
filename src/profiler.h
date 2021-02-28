#pragma once
#include <chrono>
#include <stdio.h>

#include <mydefines.h>

struct Profiler
{
    u32 num_iters;
    u32 num_comparisons;
    u32 num_experiments;
    std::chrono::system_clock::time_point started_time;
    std::chrono::microseconds time_elapsed;
};

static Profiler _std_profiler;

inline void profiler_start(Profiler* profiler = &_std_profiler)
{
    profiler->num_iters = 0;
    profiler->num_comparisons = 0;
    profiler->num_experiments = 0;
    profiler->started_time = std::chrono::high_resolution_clock::now();
}

inline void profiler_stop(Profiler* profiler = &_std_profiler)
{
    profiler->time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - profiler->started_time);
}

static void profiler_print(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    auto microsecs = (double)profiler->time_elapsed.count();

    fprintf(file, "Time elapsed: %.5f (ms)\n", microsecs / (double)1000);

    if (profiler->num_iters != 0)
    {
        fprintf(file, "Number of iterations: %u\n", profiler->num_iters);
    }

    if (profiler->num_comparisons != 0)
    {
        fprintf(file, "Number of comparisons: %u\n", profiler->num_comparisons);
    }

    if (profiler->num_experiments != 0)
    {
        fprintf(file, "Number of experiments: %u\n", profiler->num_experiments);
        fprintf(file, "Time per experiment: %.5f (microsec)\n", microsecs / profiler->num_experiments);
        
        if (profiler->num_iters != 0)
        {
            fprintf(file, "Iterations per experiment: %f\n", 
                (float)profiler->num_iters / (float)profiler->num_experiments);
        }

        if (profiler->num_comparisons != 0)
        {
            fprintf(file, "Comparisons per experiment: %f\n", 
                (float)profiler->num_comparisons / (float)profiler->num_experiments);
        }
    }
}

static void profiler_report_nicely(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    profiler_stop(profiler);
    printf("--------------------------------------\n");
    profiler_print(file, profiler);
    printf("--------------------------------------\n\n");
}

template<typename Experiment_Function>
static void profiler_perform_experiments(
    const char* description, Experiment_Function func, u32 num_experiments, FILE* file = stdout)
{
    fprintf(file, "%s\n", description);

    Profiler profiler;
    profiler_start(&profiler);

    for (u32 i = 0; i < num_experiments; i++)
    {
        func(&profiler);
        profiler.num_experiments++;
    }

    profiler_report_nicely(file, &profiler);
}