#include <vector>

#include <mydefines.h>
#include "../data.h"
#include "../profiler.h"

Record* exponential_search(
    std::vector<Record>& records, size_t search_id, Profiler* profiler = &_std_profiler)
{
    if (records[0].id == search_id)
        return &records[0];

    size_t end = records.size();
    size_t c = 0;
    size_t i = 1;

    while (c < end)
    {
        profiler->num_iters++;
        
        if (records[c + i].id == search_id)
        {
            return &records[c + i];
        }
        else if (records[c + i].id < search_id)
        {
            i *= 2;
        }
        else
        {
            c += i / 2;
            i = 1;
        }
    }

    return NULL;
}