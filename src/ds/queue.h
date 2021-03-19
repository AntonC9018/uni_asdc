#pragma once

// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"
#include "singly_linked_list.h"

namespace DS
{
    t_ using Queue = Singly_Linked_List<T>;

    t_ inline Queue<T> q_make()
    {
        return singly_linked_list_make<T>();
    }

    t_ inline void q_free(Queue<T>* queue)
    {
        list_free(queue);
    }

    t_ inline T q_dequeue(Queue<T>* queue)
    {
        auto head = list_remove_head(queue);
        auto item = head->item;
        free(head);
        return item;
    }

    t_ inline T* q_peek(Queue<T>* queue)
    {
        return &(queue->head->item);
    }

    t_ inline bool q_is_empty(const Queue<T>* queue)
    {
        return list_is_empty(queue);
    }

    t_ inline void q_enqueue(Queue<T>* queue, const T item)
    {
        list_insert_back(queue, item);
    }
}