#pragma once
#include <assert.h>

// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"

namespace DS
{
    t_ struct Cyclic_Buffer
    {
        T* items;
        size_t item_index;
        size_t count;
        size_t capacity;
    };

    t_ inline Cyclic_Buffer<T> cycbuf_make(size_t size)
    {
        Cyclic_Buffer<T> result;
        
        result.items      = (T*) malloc(size * sizeof(T));
        result.item_index = 0;
        result.count      = 0;
        result.capacity   = size;

        return result;
    }

    t_ inline void cycbuf_free(Cyclic_Buffer<T>* cycbuf)
    {
        free(cycbuf->items);
    }

    t_ inline bool cycbuf_is_empty(const Cyclic_Buffer<T>* cycbuf)
    {
        return cycbuf->count == 0;
    }

    t_ inline bool cycbuf_is_full(const Cyclic_Buffer<T>* cycbuf)
    {
        return cycbuf->count == cycbuf->capacity;
    }

    t_ inline size_t _cycbuf_next_index(const Cyclic_Buffer<T>* cycbuf)
    {
        return (cycbuf->count + cycbuf->item_index) % cycbuf->capacity;
    }

    t_ inline void cycbuf_enqueue(Cyclic_Buffer<T>* cycbuf, const T item)
    {
        // assert(!cycbuf_is_full(cycbuf));
        auto index = _cycbuf_next_index(cycbuf);
        cycbuf->items[index] = item;
        cycbuf->count++;
    }

    t_ inline const T* cycbuf_peek(const Cyclic_Buffer<T>* cycbuf)
    {
        // assert(!cycbuf_is_empty(cycbuf));
        return &cycbuf->items[cycbuf->item_index];
    }

    t_ inline T* cycbuf_peek(Cyclic_Buffer<T>* cycbuf)
    {
        auto const_cycbuf = static_cast<const Cyclic_Buffer<T>*>(cycbuf);
        auto const_item   = cycbuf_peek(const_cycbuf);
        return const_cast<T*>(const_item);
    }

    t_ inline T cycbuf_dequeue(Cyclic_Buffer<T>* cycbuf)
    {
        // assert(!cycbuf_is_empty(cycbuf));
        auto index = cycbuf->item_index;
        cycbuf->count--;
        cycbuf->item_index = (cycbuf->item_index + 1) % cycbuf->capacity;
        return cycbuf->items[index];
    }
}