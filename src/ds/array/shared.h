#pragma once

namespace DS 
{ 
    enum Major_Order { ROW_MAJOR, COL_MAJOR };

    struct Index_Range
    {
        ssize_t start; 
        ssize_t end;

        size_t size() const;
    };

    size_t Index_Range::size() const { return (size_t)(end - start) + 1; }

    inline void init_costs_row_major(size_t* index_costs, const Index_Range* ranges, const size_t n_dim)
    {
        index_costs[n_dim - 1] = 1;
        for (size_t i = n_dim - 1; i > 0; i--)
        {
            index_costs[i - 1] = index_costs[i] * ranges[i].size();
        }
    }

    inline void init_costs_col_major(size_t* index_costs, const Index_Range* ranges, const size_t n_dim)
    {
        index_costs[0] = 1;
        for (size_t i = 1; i < n_dim; i++)
        {
            index_costs[i] = index_costs[i - 1] * ranges[i - 1].size();
        }
    }

    inline void init_costs(size_t* index_costs, const Index_Range* ranges, const size_t n_dim, const Major_Order order)
    {
        if (order == ROW_MAJOR) init_costs_row_major(index_costs, ranges, n_dim);
        else                    init_costs_col_major(index_costs, ranges, n_dim);
    }
}