#pragma once
#include <mydefines.h>
#include "../data.h"
#include "../profiler.h"
#include "util.h"

#define T Record

namespace Sort
{
    template<typename Comparator>
    void heapify(T* arr, size_t length, size_t current_index, 
        Comparator compare_func, Profiler* profiler)
    {
        profiler->num_iters++;

        size_t index_left  = current_index * 2 + 1;
        size_t index_right = current_index * 2 + 2;

        size_t index_largest = current_index;

        if (index_left < length)
        {
            profiler->num_comparisons++;
            if (compare_func(arr[index_left], arr[index_largest]) > 0)
                index_largest = index_left;
        }

        if (index_right < length)
        {
            profiler->num_comparisons++;
            if (compare_func(arr[index_right], arr[index_largest]) > 0)
                index_largest = index_right;
        }

        if (index_largest != current_index)
        {
            Sort::swap(arr[current_index], arr[index_largest]);
            profiler->num_swaps++;

            heapify(arr, length, index_largest, compare_func, profiler);
        }
    }

    template<typename Comparator>
    void heap_sort_internal(T* arr, size_t length, 
        Comparator compare_func, Profiler* profiler)
    {
        for (size_t i = length / 2; i > 0; i--)
        {
            heapify(arr, length, i - 1, compare_func, profiler);
        }

        for (size_t i = length - 1; i > 0; i--)
        {
            Sort::swap(arr[0], arr[i]);
            profiler->num_swaps++;

            heapify(arr, i, 0, compare_func, profiler);
        }
    }

    template<typename Iterator, typename Comparator>
    void heap_sort(Iterator begin, Iterator end, 
        Comparator compare_func, Profiler* profiler = &_std_profiler)
    {
        heap_sort_internal(&*begin, &*end - &*begin, compare_func, profiler); 
    }
}

#undef T
