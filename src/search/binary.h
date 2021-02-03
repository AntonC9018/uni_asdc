#include <vector>

#include <mydefines.h>
#include "../data.h"
#include "../profiler.h"

Record* binary_search(
    std::vector<Record>& records, u64 search_id, Profiler* profiler = &_std_profiler)
{
    u64 start = 0;
    u64 end = records.size();
    profiler->num_iters = 0;
    while (true)
    {    
        profiler->num_iters++;
        u64 current = (end + start) / 2;

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