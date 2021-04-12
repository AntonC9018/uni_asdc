#pragma once
#include <mydefines.h>
#include "shared.h"
#include "../shared.h"

namespace DS
{
    template<size_t N_DIM>
    struct Dope_Vector
    {
        Index_Range ranges[N_DIM];
        size_t total_length;
        size_t index_costs[N_DIM];
        size_t index_cost_constant_term;

        void init_cache_row_major();
        void init_cache_col_major();
        void init_constants();
        size_t map_indices_to_physical_index(const ssize_t indices[N_DIM]) const;
    };
    
    template<size_t N_DIM>
    void Dope_Vector<N_DIM>::init_constants()
    {
        index_cost_constant_term = 0;
        total_length = 1;
        for (size_t i = 0; i < N_DIM; i++)
        {
            index_cost_constant_term += index_costs[i] * ranges[i].start; 
            total_length *= ranges[i].size();
        }
    }

    template<size_t N_DIM>
    void Dope_Vector<N_DIM>::init_cache_row_major()
    {
        init_costs_row_major(index_costs, ranges, N_DIM);
        init_constants();
    }

    template<size_t N_DIM>
    void Dope_Vector<N_DIM>::init_cache_col_major()
    {
        init_costs_col_major(index_costs, ranges, N_DIM);
        init_constants();
    }

    template<size_t N_DIM>
    size_t Dope_Vector<N_DIM>::map_indices_to_physical_index(const ssize_t indices[N_DIM]) const
    {
        size_t index = 0;
        for (size_t i = 0; i < N_DIM; i++)
        {
            index += indices[i] * index_costs[i];
        }
        return index - index_cost_constant_term;
    }
}