#pragma once
#include <mydefines.h>
#include "../data.h"
#include "../profiler.h"

#define T Record

namespace Sort
{
    template<typename Comparator>
    void merge_sort_internal(T* begin, size_t length, Comparator compare_func, T* buffer, Profiler* profiler)
    {
        if (length <= 1)  return;

        // Past this point counts as an iteration.
        profiler->num_iters++;

        auto half_length = length / 2;
        auto middle = half_length + begin;

        merge_sort_internal(begin,           half_length, compare_func, buffer, profiler);
        merge_sort_internal(middle, length - half_length, compare_func, buffer, profiler);

        auto next = begin;
        memcpy(buffer, begin, length * sizeof(T));
        
        auto buf_left   = buffer;
        auto buf_middle = buffer + half_length;
        auto buf_right  = buf_middle;
        auto buf_end    = buffer + length;

        while (buf_left < buf_middle && buf_right < buf_end)
        {
            auto comp_result = compare_func(*buf_left, *buf_right);
            profiler->num_comparisons++;

            // Since we've done a memcpy before this, it still kind of counts as a swap
            profiler->num_swaps++;
            
            // buf_left is greater than right, insert the right one
            if (comp_result > 0)
            {
                *next = *buf_right;
                buf_right++;
            }
            else
            {
                *next = *buf_left;
                buf_left++;
            }

            next++;
        }

        // Fill in the rest.
        if (buf_left != buf_middle)
        {
            memcpy(next, buf_left, (buf_middle - buf_left) * sizeof(T));
            profiler->num_swaps += (buf_middle - buf_left);
        }
        else
        {
            memcpy(next, buf_right, (buf_end - buf_right) * sizeof(T));
            profiler->num_swaps += (buf_end - buf_right);
        }

    }

    // `end` is one past the last element, like in STL algorithms
    template<typename Iterator, typename Comparator>
    void merge_sort(Iterator begin, Iterator end, Comparator compare_func, Profiler* profiler = &_std_profiler)
    {
        if (begin + 1 < end)
        {
            auto length = end - begin;

            auto buffer = (T*) malloc(length * sizeof(T));

            merge_sort_internal(&*begin, length, compare_func, buffer, profiler);

            free(buffer);
        }
    }
}

#undef T