#pragma once
#include <mydefines.h>
#include "shared.h"
#include "../shared.h"

namespace DS
{
    template<typename T, size_t N_DIM>
    struct Raw_Array_Wrapper
    {
        T* items;
        Index_Range ranges[N_DIM];
        Major_Order order;

        T& operator[](const ssize_t[N_DIM]);
        void set_range(size_t index, Index_Range range);
        void init(Major_Order major_order);
        size_t size() const;
        size_t size(size_t index_depth) const;
        ssize_t start(size_t index_depth) const;
        ssize_t end(size_t index_depth) const;

        void free();
    };

    template<typename T, size_t N_DIM>
    void Raw_Array_Wrapper<T, N_DIM>::init(Major_Order major_order)
    {
        items = (T*) malloc(size() * sizeof(T));
        order = major_order;
    }
    
    template<typename T, size_t N_DIM>
    T& Raw_Array_Wrapper<T, N_DIM>::operator[](const ssize_t indices[N_DIM])
    {
        size_t index_costs[N_DIM];
        size_t physical_index = 0;

        init_costs(index_costs, ranges, N_DIM, order);
        
        for (size_t i = 0; i < N_DIM; i++)
        {
            physical_index += (indices[i] - ranges[i].start) * index_costs[i];
        }

        return items[physical_index];
    }

    template<typename T, size_t N_DIM>
    void Raw_Array_Wrapper<T, N_DIM>::set_range(size_t index, Index_Range range)
    {
        assert(index < N_DIM);
        ranges[index] = range;
    }

    template<typename T, size_t N_DIM>
    size_t Raw_Array_Wrapper<T, N_DIM>::size() const
    {
        size_t result = 1;
        for (size_t i = 0; i < N_DIM; i++)
        {
            result *= size(i);
        }
        return result;
    }

    template<typename T, size_t N_DIM>
    size_t Raw_Array_Wrapper<T, N_DIM>::size(size_t index_depth) const
    { 
        assert(index_depth < N_DIM);
        return ranges[index_depth].size(); 
    }

    template<typename T, size_t N_DIM>
    ssize_t Raw_Array_Wrapper<T, N_DIM>::start(size_t index_depth) const
    { 
        assert(index_depth < N_DIM);
        return ranges[index_depth].start; 
    }

    template<typename T, size_t N_DIM>
    ssize_t Raw_Array_Wrapper<T, N_DIM>::end(size_t index_depth) const
    { 
        assert(index_depth < N_DIM);
        return ranges[index_depth].end; 
    }

    template<typename T, size_t N_DIM>
    void Raw_Array_Wrapper<T, N_DIM>::free()
    {
        std::free(items);
    }
}