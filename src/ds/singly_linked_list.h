#pragma once

// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"

namespace DS
{
    t_ struct Singly_Linked_Node
    {
        Singly_Linked_Node<T>* next;
        T item;
    };

    t_ inline Singly_Linked_Node<T>* singly_linked_node_alloc()
    {
        return (Singly_Linked_Node<T>*) malloc(sizeof(Singly_Linked_Node<T>));
    }

    // In a valid list, these two are either both null, or both not null
    t_ struct Singly_Linked_List
    {
        union { 
            Singly_Linked_Node<T>* head;
            Singly_Linked_Node<T>* front;
            Singly_Linked_Node<T>* first;
        };
        union {
            Singly_Linked_Node<T>* tail;
            Singly_Linked_Node<T>* back;
            Singly_Linked_Node<T>* last;
        };
    };

    t_ Singly_Linked_List<T> singly_linked_list_make()
    {
        return { NULL, NULL };   
    }

    t_ void node_cascade_free(T* node)
    {
        while (node)
        {
            auto next_node = node->next;
            free(node);
            node = next_node;
        }
    }

    t_ void list_free(Singly_Linked_List<T>* list)
    {
        node_cascade_free(list->head);
    }

    t_ inline void list_clear(Singly_Linked_List<T>* list)
    {
        list_free(list);
        list->head = NULL;
        list->tail = NULL;
    }

    t_ inline bool list_is_empty(const Singly_Linked_List<T>* list)
    {
        return list->head == NULL;
    }

    t_ Singly_Linked_Node<T>* list_insert_front(Singly_Linked_List<T>* list, const T item)
    {
        auto prev_head = list->head;

        auto node = singly_linked_node_alloc<T>();
        node->item = item;

        list->head = node;
        node->next = prev_head;

        // This is equivalent to checking 
        // if (!prev_head)
        // Since the list must be always valid
        if (!list->tail)
        {
            list->tail = node;
        }

        return node;
    }

    t_ Singly_Linked_Node<T>* list_insert_back(Singly_Linked_List<T>* list, const T item)
    {
        auto prev_tail = list->tail;

        auto node = singly_linked_node_alloc<T>();
        node->item = item;
        node->next = NULL;

        list->tail = node;

        if (prev_tail)
        {
            prev_tail->next = node;
        }
        // If there is no tail, there must be no head too
        else
        {
            list->head = node;
        }
        
        return node;
    }

    t_ Singly_Linked_Node<T>* list_insert_after(
        Singly_Linked_List<T>* list, 
        Singly_Linked_Node<T>* node, 
        const T item)
    {
        auto new_node = singly_linked_node_alloc<T>();
        new_node->item = item;

        // (node) -> (next_node)
        // Changes into
        // (node) -> (new_node) -> (next_node)
        new_node->next = node->next;
        node->next = new_node;

        // Need to adjust the list, in case the new node becomes the last
        if (node == list->tail)
        {
            list->tail = new_node;
        }

        return new_node;
    }

    t_ Singly_Linked_Node<T>* list_remove_after(
        Singly_Linked_List<T>* list, 
        Singly_Linked_Node<T>* node)
    {
        if (node->next)
        {
            // (node) -> (node_next) -> (new_next)
            // Deleting node_next, leaves
            // (node) -> (new_next)
            auto new_next = node->next->next;
            auto removed_node = node->next;
            node->next = new_next;

            // The removed node was the tail
            if (!new_next)
            {
                list->tail = node;
            }

            return removed_node;
        }

        return NULL;
    }

    t_ const Singly_Linked_Node<T>* list_find_node(
        const Singly_Linked_List<T>* list, 
        const T item)
    {
        auto node = list->head;
        
        while (node)
        {
            if (node->item == item)
                return node;

            node = node->next;
        }

        return NULL;
    }

    // https://stackoverflow.com/a/123995/9731532
    t_ Singly_Linked_Node<T>* list_find_node(
        Singly_Linked_List<T>* list, 
        const T item)
    {
        auto const_list = static_cast<const Singly_Linked_List<T>*>(list);
        auto const_node = list_find_node(const_list, item);
        return const_cast<Singly_Linked_Node<T>*>(const_node);
    }

    t_ const Singly_Linked_Node<T>* list_find_node_before(
        const Singly_Linked_List<T>* list, 
        const T item)
    {
        auto parent = list->head;
        if (!parent) return NULL;
        auto node = parent->next; 

        while (node)
        {
            if (node->item == item)
                return parent;

            parent = node;
            node = node->next;
        }

        return NULL;
    }

    t_ Singly_Linked_Node<T>* list_find_node_before(
        Singly_Linked_List<T>* list, 
        const T item)
    {
        auto const_list = static_cast<const Singly_Linked_List<T>*>(list);
        auto const_node = list_find_node_before(const_list, item);
        return const_cast<Singly_Linked_Node<T>*>(const_node);
    }

    t_ Singly_Linked_Node<T>* list_remove_head(Singly_Linked_List<T>* list)
    {
        auto removed_node = list->head;
        list->head = removed_node->next;

        // If it's both head and tail, the result is going to be an empty list
        if (list->tail == removed_node)
            list->tail = NULL;

        return removed_node;
    }

    t_ Singly_Linked_Node<T>* list_remove(Singly_Linked_List<T>* list, const T item)
    {
        if (list->head)
        {
            if (list->head->item == item)
            {
                return list_remove_head(list);
            }

            auto node_before = list_find_node_before(list, item);
            if (node_before)
            {
                return list_remove_after(list, node_before);
            }
        }
        return NULL;
    }

    // template<typename T, typename Comparator>
    // void list_remove(

    template<typename T, typename Print_Function>
    void list_print(
        const Singly_Linked_List<T>* list, 
        Print_Function& print_func)
    {
        auto node = list->head;
        while (node)
        {
            print_func(node->item);
            node = node->next; 
        }
    }

    void list_print(const Singly_Linked_List<int>* list)
    {
        auto node = list->head;
        while (node)
        {
            printf("%i", node->item);
            node = node->next;
            if (node) printf(" -> ");
        }
        printf("\n");
    }
}