#pragma once
#include <mydefines.h>
#include "range.h"
#include "dope.h"
#include "../shared.h"

namespace DS
{

    enum Major_Order { ROW_MAJOR, COL_MAJOR };

    template<typename T, size_t N_DIM>
    struct Dope_Wrapper
    {
        T* items;
        Dope_Vector<N_DIM> dope;

        T& operator[](ssize_t[N_DIM]);
        void set_range(size_t index, Index_Range range);
        void init(Major_Order major_order);
        size_t size();
        size_t size(size_t index_depth);
        ssize_t start(size_t index_depth);
        ssize_t end(size_t index_depth);
    };

    template<typename T, size_t N_DIM>
    void Dope_Wrapper<T, N_DIM>::init(Major_Order major_order)
    {
        if (major_order == ROW_MAJOR) dope.init_cache_row_major();
        else                          dope.init_cache_col_major();
        items = (T*) malloc(size() * sizeof(T));
    }
    
    template<typename T, size_t N_DIM>
    void Dope_Wrapper<T, N_DIM>::set_range(size_t index, Index_Range range)
    {
        dope.ranges[index] = range;
    }

    template<typename T, size_t N_DIM>
    T& Dope_Wrapper<T, N_DIM>::operator[](ssize_t indices[N_DIM])
    {
        auto physical_index = dope.map_indices_to_physical_index(indices);
        return items[physical_index];
    }

    template<typename T, size_t N_DIM>
    size_t Dope_Wrapper<T, N_DIM>::size()
    {
        return dope.total_length;
    }

    template<typename T, size_t N_DIM>
    size_t Dope_Wrapper<T, N_DIM>::size(size_t index_depth)
    {
        return dope.ranges[index_depth].length();
    }

    template<typename T, size_t N_DIM>
    ssize_t Dope_Wrapper<T, N_DIM>::start(size_t index_depth)
    {
        return dope.ranges[index_depth].start;
    }

    template<typename T, size_t N_DIM>
    ssize_t Dope_Wrapper<T, N_DIM>::end(size_t index_depth)
    {
        return dope.ranges[index_depth].end;
    }

    // template<typename T, size_t N_DIM>
    // Dope_Wrapper<T, N_DIM> make(Index_Range[N_DIM] ranges)
    // {
    //     Dope_Wrapper<T, N_DIM> wrapper;
    //     memcpy(wrapper.dope.ranges, ranges, N_DIM * sizeof(Index_Range));
    //     wrapper.dope.init_cache_col_major();
    //     wrapper.items = malloc(wrapper.size() * sizeof(T));
    // }
}