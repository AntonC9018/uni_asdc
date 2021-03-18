// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"

namespace DS
{
    t_ struct Array_Queue
    {
        T* items;
        // Shows the next free index in which a new value may be inserted
        size_t top_index;
        // Shows the index of the next element in the cyclic buffer
        size_t bottom_index;
        // Shows the size of the cyclic buffer
        size_t size;
    };

    t_ inline Array_Queue<T> array_queue_make(size_t size)
    {
        Array_Queue<T> result;
        result.items = (T*) malloc(size * sizeof(T));
        result.current_index = 0;
        return result;
    }

    t_ inline bool stack_is_empty(const Array_Queue<T>* stack)
    {
        return stack->current_index == 0;
    }

    t_ inline T stack_pop(Array_Queue<T>* stack)
    {
        stack->current_index--;
        return stack->items[stack->current_index];
    }

    t_ inline void stack_push(Array_Queue<T>* stack, const T item)
    {
        stack->items[stack->current_index] = item;
        stack->current_index++;
    }

    t_ inline const T* stack_peek(const Array_Queue<T>* stack)
    {
        return &stack->items[stack->current_index - 1];
    }

    // This logically should not make a copy of that value, but rather let you modify it.
    t_ inline T* stack_peek(Array_Queue<T>* stack)
    {
        auto const_stack = static_cast<const Array_Queue<T>*>(stack);
        auto const_item  = stack_peek(const_stack);
        return const_cast<T*>(const_item);
    }
}