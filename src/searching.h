#include <vector>
#include <functional>

#include <time.h>
#include <mydefines.h>
#include "data.h"

struct Profiler
{
    u32 num_iters;
    clock_t started_time;
    clock_t time_elapsed;
} _std_profiler;

void profiler_start(Profiler* profiler = &_std_profiler)
{
    profiler->num_iters = 0;
    profiler->started_time = clock();
}

void profiler_stop(Profiler* profiler = &_std_profiler)
{
    profiler->time_elapsed = clock() - profiler->started_time;
}

void profiler_print(FILE* file = stdout, Profiler* profiler = &_std_profiler)
{
    fprintf(file, "Number of iterations: %u\nTime elapsed: %u (ms)\n", 
        profiler->num_iters, profiler->time_elapsed / (CLOCKS_PER_SEC / 1000));
}


Record* find_record_linear(
    std::vector<Record>& records, int search_id, Profiler* profiler = &_std_profiler)
{
    for (auto& record : records)
    {
        profiler->num_iters++;
        if (record.id == search_id)
        {
            return &record;
        }
    }
    return NULL;
}

Record* find_record_linear_predicate(
    std::vector<Record>& records, bool(*predicate)(Record*), Profiler* profiler = &_std_profiler)
{
    for (auto& record : records)
    {
        profiler->num_iters++;
        if (predicate(&record))
        {
            return &record;
        }
    }
    return NULL;
}

Record* find_record_linear_stdfunction(
    std::vector<Record>& records, std::function<bool(Record*)> predicate, Profiler* profiler = &_std_profiler)
{
    for (auto& record : records)
    {
        profiler->num_iters++;
        if (predicate(&record))
        {
            return &record;
        }
    }
    return NULL;
}