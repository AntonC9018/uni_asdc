#pragma once
#include <mydefines.h>
#include "../shared.h"

namespace DS
{
    template<typename T, size_t N_DIM>
    struct Iliffe_Vector
    {
        size_t size;
        Iliffe_Vector<T, N_DIM - 1>* vectors;

        Iliffe_Vector<T, N_DIM - 1>& operator[](size_t index);
        const Iliffe_Vector<T, N_DIM - 1>& operator[](size_t index) const;

        static Iliffe_Vector<T, N_DIM> make(size_t size);

        void free();
    };

    template<typename T, size_t N_DIM>
    Iliffe_Vector<T, N_DIM> Iliffe_Vector<T, N_DIM>::make(size_t size)
    {
        Iliffe_Vector<T, N_DIM> result;
        result.vectors = (Iliffe_Vector<T, N_DIM - 1>*) calloc(
            sizeof(Iliffe_Vector<T, N_DIM - 1>), size);
        result.size = size;
        return result;
    }

    template<typename T, size_t N_DIM>
    Iliffe_Vector<T, N_DIM - 1>& Iliffe_Vector<T, N_DIM>::operator[](size_t index)
    {
        assert(index < size);
        return vectors[index];
    }

    template<typename T, size_t N_DIM>
    const Iliffe_Vector<T, N_DIM - 1>& Iliffe_Vector<T, N_DIM>::operator[](size_t index) const
    {
        assert(index < size);
        return vectors[index];
    }

    template<typename T, size_t N_DIM>
    void Iliffe_Vector<T, N_DIM>::free()
    {
        for (size_t i = 0; i < size; i++)
        {
            vectors[i].free();
        }
        free(vectors);
    }

    template<typename T>
    struct Iliffe_Vector<T, 1>
    {
        size_t size;
        T* elements;

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        static Iliffe_Vector<T, 1> make(size_t size);

        void free();
    };

    template<typename T>
    Iliffe_Vector<T, 1> Iliffe_Vector<T, 1>::make(size_t size)
    {
        Iliffe_Vector<T, 1> result;
        result.elements = (T*) malloc(sizeof(T) * size);
        result.size = size;
        return result;
    }

    template<typename T>
    T& Iliffe_Vector<T, 1>::operator[](size_t index)
    {
        assert(index < size);
        return elements[index];
    }

    template<typename T>
    const T& Iliffe_Vector<T, 1>::operator[](size_t index) const
    {
        assert(index < size);
        return elements[index];
    }

    template<typename T>
    void Iliffe_Vector<T, 1>::free()
    {
        free(elements);
    }
}