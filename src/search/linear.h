#include <vector>
#include <functional>

#include <mydefines.h>
#include "../data.h"
#include "../profiler.h"

Record* linear_search(
    std::vector<Record>& records, u64 search_id, Profiler* profiler = &_std_profiler)
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

Record* linear_search_predicate(
    std::vector<Record>& records, bool(*predicate)(const Record*), Profiler* profiler = &_std_profiler)
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

Record* linear_search_stdfunction(
    std::vector<Record>& records, std::function<bool(const Record*)> predicate, Profiler* profiler = &_std_profiler)
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