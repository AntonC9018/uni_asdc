#pragma once
#include "../data.h"
#include "../profiler.h"

namespace Sort
{
    template<typename Iterator, typename Comparator>
    void selection_sort(Iterator start, Iterator end, 
        Comparator compare_func, Profiler* profiler = &_std_profiler)
    {
        while (start != end)
        {
            profiler->num_iters++;

            // find the minimum
            Iterator min_iter = start;
            for (Iterator i = start + 1; i != end; i++)
            {
                profiler->num_comparisons++;
                if (compare_func(*i, *min_iter) < 0)
                {
                    min_iter = i;
                }
            }

            Sort::swap(&*min_iter, &*start);
            profiler->num_swaps++;

            start++;
        }
    }
}