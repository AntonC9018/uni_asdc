#pragma once
#include "../data.h"
#include "../profiler.h"

namespace Sort
{
    template<typename Iterator, typename Comparator>
    Iterator quick_partition(Iterator begin, Iterator last, 
        Comparator compare_func, Profiler* profiler)
    {
        auto pivot_value = *last;
        auto pivot_iter  = begin;

        while (begin != last)
        {
            profiler->num_comparisons++;

            if (compare_func(*begin, pivot_value) < 0)
            {   
                Sort::swap(&*pivot_iter, &*begin);
                pivot_iter++;
            }
            begin++;
        }
        Sort::swap(&*last, &*pivot_iter);
        return pivot_iter;
    }

    template<typename Iterator, typename Comparator>
    void quick_sort(Iterator begin, Iterator end, 
        Comparator compare_func, Profiler* profiler = &_std_profiler)
    {
        if (begin == end || begin + 1 == end) return;

        profiler->num_iters++;

        auto partition_iter = quick_partition(begin, end - 1, compare_func, profiler);
        quick_sort(begin, partition_iter,   compare_func, profiler);
        quick_sort(partition_iter + 1, end, compare_func, profiler);
    }
}