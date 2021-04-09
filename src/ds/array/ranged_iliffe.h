#pragma once
#include <mydefines.h>
#include "range.h"
#include "../shared.h"

namespace DS
{
    template<typename T, size_t N_DIM>
    struct Ranged_Iliffe_Vector
    {
        Index_Range range;
        Ranged_Iliffe_Vector<T, N_DIM - 1>* vectors;
        
        size_t size();

        Ranged_Iliffe_Vector<T, N_DIM - 1>& operator[](ssize_t index);
        const Ranged_Iliffe_Vector<T, N_DIM - 1>& operator[](ssize_t index) const;

        static Ranged_Iliffe_Vector<T, N_DIM> make(Index_Range range);

        void free();
    };

    template<typename T, size_t N_DIM>
    Ranged_Iliffe_Vector<T, N_DIM> Ranged_Iliffe_Vector<T, N_DIM>::make(Index_Range range)
    {
        assert(range.end >= range.start);

        Ranged_Iliffe_Vector<T, N_DIM> result;
        result.range = range;
        result.vectors = (Ranged_Iliffe_Vector<T, N_DIM - 1>*) calloc(
            sizeof(Ranged_Iliffe_Vector<T, N_DIM - 1>), result.size());

        return result;
    }

    template<typename T, size_t N_DIM>
    size_t Ranged_Iliffe_Vector<T, N_DIM>::size()
    {
        return (size_t)(range.end - range.start) + 1;
    }

    template<typename T, size_t N_DIM>
    Ranged_Iliffe_Vector<T, N_DIM - 1>& Ranged_Iliffe_Vector<T, N_DIM>::operator[](ssize_t index)
    {
        assert(index >= range.start && index <= range.end);
        return vectors[index - range.start];
    }

    template<typename T, size_t N_DIM>
    const Ranged_Iliffe_Vector<T, N_DIM - 1>& Ranged_Iliffe_Vector<T, N_DIM>::operator[](ssize_t index) const
    {
        assert(index >= range.start && index <= range.end);
        return vectors[index - range.start];
    }

    template<typename T, size_t N_DIM>
    void Ranged_Iliffe_Vector<T, N_DIM>::free()
    {
        for (size_t i = 0; i < size; i++)
        {
            vectors[i].free();
        }
        free(vectors);
    }

    template<typename T>
    struct Ranged_Iliffe_Vector<T, 1>
    {
        Index_Range range;
        T* elements;

        size_t size();

        T& operator[](ssize_t index);
        const T& operator[](ssize_t index) const;

        static Ranged_Iliffe_Vector<T, 1> make(Index_Range range);

        void free();
    };

    template<typename T>
    Ranged_Iliffe_Vector<T, 1> Ranged_Iliffe_Vector<T, 1>::make(Index_Range range)
    {
        assert(range.end >= range.start);

        Ranged_Iliffe_Vector<T, 1> result;
        result.range = range;
        result.elements = (T*) malloc(sizeof(T) * result.size());

        return result;
    }

    template<typename T>
    size_t Ranged_Iliffe_Vector<T, 1>::size()
    {
        return (size_t)(range.end - range.start) + 1;
    }

    template<typename T>
    T& Ranged_Iliffe_Vector<T, 1>::operator[](ssize_t index)
    {
        assert(index >= range.start && index <= range.end);
        return elements[index - range.start];
    }

    template<typename T>
    const T& Ranged_Iliffe_Vector<T, 1>::operator[](ssize_t index) const
    {
        assert(index >= range.start && index <= range.end);
        return elements[index - range.start];
    }

    template<typename T>
    void Ranged_Iliffe_Vector<T, 1>::free()
    {
        free(elements);
    }
}