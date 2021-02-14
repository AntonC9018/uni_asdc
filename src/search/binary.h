#include <vector>

#include <mydefines.h>
#include "../data.h"
#include "../profiler.h"

Record* binary_search(
    std::vector<Record>& records, size_t search_id, Profiler* profiler = &_std_profiler)
{
    size_t start = 0;
    size_t end = records.size();

    while (true)
    {    
        profiler->num_iters++;
        size_t current = (end + start) / 2;

        if (records[current].id == search_id)
        {
            return &records[current];
        }
        else if (start >= end)
        {
            return NULL;
        }
        else if (records[current].id < search_id)
        {
            start = current;
        }
        else
        {
            end = current;
        }
    }
    return NULL;
}