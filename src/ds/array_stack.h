#pragma once

// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"

namespace DS
{
    /*
        Implements a statically-sized array-based stack without bounds checking.
        A safe version of this is not provided.
        An event simpler implementation would be to just increment/decrement a pointer,
        however with that one would not be able to check if the stack has emptied.
        A more flexible design would keep the capacity and error out if it has been breached.
        An even more flexible design would reallocate a new memory and memcpy elements
        into there, effectively being resized (like an std::vector).
    */
    t_ struct Array_Stack
    {
        T* items;
        // Shows the next free index in which a new value may be inserted
        size_t count;
    };

    t_ inline Array_Stack<T> array_stack_make(size_t size)
    {
        Array_Stack<T> result;
        result.items = (T*) malloc(size * sizeof(T));
        result.count = 0;
        return result;
    }

    t_ inline void stack_free(Array_Stack<T>* stack)
    {
        free(stack->items);
    }

    t_ inline bool stack_is_empty(const Array_Stack<T>* stack)
    {
        return stack->count == 0;
    }

    t_ inline T stack_pop(Array_Stack<T>* stack)
    {
        stack->count--;
        return stack->items[stack->count];
    }

    t_ inline void stack_push(Array_Stack<T>* stack, const T item)
    {
        stack->items[stack->count] = item;
        stack->count++;
    }

    t_ inline const T* stack_peek(const Array_Stack<T>* stack)
    {
        return &stack->items[stack->count - 1];
    }

    // This logically should not make a copy of that value, but rather let you modify it.
    t_ inline T* stack_peek(Array_Stack<T>* stack)
    {
        auto const_stack = static_cast<const Array_Stack<T>*>(stack);
        auto const_item  = stack_peek(const_stack);
        return const_cast<T*>(const_item);
    }
}