#pragma once
#include <strlib.h>
#include <mydefines.h>
#include "../data.h"

#define T Record

namespace Sort
{
    inline void swap(T* a, T* b)
    {
        T t;
        memcpy(&t, a, sizeof(T));
        memcpy(a, b,  sizeof(T));
        memcpy(b, &t, sizeof(T));
    }


    // `end` is one past the last element, like in STL algorithms
    template<typename Comparator>
    void merge_sort_internal(T* begin, T* end, Comparator compare_func, T* buffer, Profiler* profiler)
    {
        if (begin == end || begin == end - 1)  return;

        auto length = end - begin;
        auto middle = length / 2 + begin;

        auto buf_left   = buffer;
        auto buf_middle = buffer + length / 2;
        auto buf_right  = buf_middle;
        auto buf_end    = buffer + length;


        merge_sort_internal(begin, middle, compare_func, buf_left, profiler);
        merge_sort_internal(middle, end, compare_func, buf_right, profiler);

        auto next = begin;
        memcpy(buffer, begin, length * sizeof(T));

        while (buf_left < buf_middle && buf_right < buf_end)
        {
            auto comp_result = compare_func(buf_left, buf_right);
            
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
        }
        else
        {
            memcpy(next, buf_right, (buf_end - buf_right) * sizeof(T));
        }

    }

    template<typename Iterator, typename Comparator>
    void merge_sort(Iterator begin, Iterator end, Comparator compare_func, Profiler* profiler = &_std_profiler)
    {
        if (begin + 1 < end)
        {
            auto begin_ptr = &*begin;
            auto end_ptr = &*end;
            auto length = end_ptr - begin_ptr;

            auto buffer = (T*) malloc(length * sizeof(T));

            merge_sort_internal(begin_ptr, end_ptr, compare_func, buffer, profiler);

            free(buffer);
        }
    }
}

#undef T