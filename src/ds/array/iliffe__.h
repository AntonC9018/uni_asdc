#include <mydefines>
#include "../shared.h"

t_ 
struct Iliffe_Vector
{
    union
    {
        Iliffe_Vector<T>* next_vectors;
        T* elements;
        void* data;
    };

    Iliffe_Vector<T>& operator[](size_t index);
    T& operator[](size_t index);
};

t_
void _iliffe_vector_make_recursive_logic(Iliffe_Vector<T>* v, size_t num_dimensions, size_t* sizes);

t_
Iliffe_Vector<T> iliffe_vector_make(size_t num_dimensions, size_t* sizes)
{
    Iliffe_Vector<T> result;
    result.num_dimensions = num_dimensions;
    _iliffe_vector_make_recursive_logic(result, num_dimensions, sizes);
    return result;
}

t_
Iliffe_Vector<T>* _iliffe_vector_make_recursive(size_t num_dimensions, size_t* sizes)
{
    auto* result = (Iliffe_Vector<T>*) malloc(sizeof(Iliffe_Vector<T>));
    result->num_dimensions = num_dimensions;
    _iliffe_vector_make_recursive_logic(result, num_dimensions, sizes);
    return result;
}

t_
void _iliffe_vector_make_recursive_logic(Iliffe_Vector<T>* v, size_t num_dimensions, size_t* sizes)
{
    if (num_dimensions > 1)
    {
        v->next_vectors = (Iliffe_Vector<T>*) malloc(sizeof(void*) * sizes[0]);
        for (int i = 0; i < sizes[0]; i++)
        {
            v->next_vectors[i] = _iliffe_vector_make_recursive(num_dimensions - 1, &sizes[1]);
        }
    }
    else
    {
        v->elements = (T*) malloc(sizeof(T) * sizes[0]);
    }
}


t_
const Iliffe_Vector<T>* array_unwrap(const Iliffe_Vector<T>* v, size_t index)
{
    if (v->num_dimensions > 1)
    {
        return v->next_vectors;
    }
    return NULL;
}

t_
T array_at(const Iliffe_Vector<T>* v, size_t index)
{
    assert(v->num_dimensions == 1);
    return v->elements[index];
}
