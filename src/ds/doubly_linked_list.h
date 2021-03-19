#pragma once

// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"
#include "singly_linked_list.h"

namespace DS
{
    t_ struct Doubly_Linked_Node
    {
        Doubly_Linked_Node<T>* prev;
        Doubly_Linked_Node<T>* next;
        T item;
    };

    t_ inline Doubly_Linked_Node<T>* doubly_linked_node_alloc()
    {
        return (Doubly_Linked_Node<T>*) malloc(sizeof(Doubly_Linked_Node<T>));
    }

    // In a valid list, these two are either both null, or both not null
    t_ struct Doubly_Linked_List
    {
        union { 
            Doubly_Linked_Node<T>* head;
            Doubly_Linked_Node<T>* front;
            Doubly_Linked_Node<T>* first;
        };
        union {
            Doubly_Linked_Node<T>* tail;
            Doubly_Linked_Node<T>* back;
            Doubly_Linked_Node<T>* last;
        };
    };

    t_ Doubly_Linked_List<T> doubly_linked_list_make()
    {
        return { NULL, NULL };   
    }

    t_ void list_free(Doubly_Linked_List<T>* list)
    {
        node_cascade_free(list->head);
    }

    t_ inline void list_clear(Doubly_Linked_List<T>* list)
    {
        list_free(list);
        list->head = NULL;
        list->tail = NULL;
    }

    t_ inline bool list_is_empty(const Doubly_Linked_List<T>* list)
    {
        return list->head == NULL;
    }

    t_ Doubly_Linked_Node<T>* list_insert_front(Doubly_Linked_List<T>* list, const T item)
    {
        auto head_prev = list->head;

        auto node = doubly_linked_node_alloc<T>();
        node->item = item;
        node->prev = NULL;
        node->next = head_prev;

        list->head = node;
        
        if (head_prev)
        {
            head_prev->prev = node;
        }
        else
        {
            list->tail = node;
        }

        return node;
    }

    t_ Doubly_Linked_Node<T>* list_insert_back(Doubly_Linked_List<T>* list, const T item)
    {
        auto prev_tail = list->tail;

        auto node = doubly_linked_node_alloc<T>();
        node->item = item;
        node->prev = prev_tail;
        node->next = NULL;

        list->tail = node;

        if (prev_tail)
        {
            prev_tail->next = node;
        }
        else
        {
            list->head = node;
        }
        
        return node;
    }

    t_ Doubly_Linked_Node<T>* list_insert_after(
        Doubly_Linked_List<T>* list, 
        Doubly_Linked_Node<T>* node, 
        const T item)
    {
        auto new_node = doubly_linked_node_alloc<T>();
        new_node->item = item;

        // (node) <-> (next_node)
        // Changes into
        // (node) <-> (new_node) <-> (next_node)
        new_node->next = node->next;
        new_node->prev = node;
        if (node->next)  
            node->next->prev = new_node;
        node->next = new_node;

        // Need to adjust the list, in case the new node becomes the last
        if (node == list->tail)
        {
            list->tail = new_node;
        }

        return new_node;
    }

    t_ Doubly_Linked_Node<T>* list_insert_before(
        Doubly_Linked_List<T>* list, 
        Doubly_Linked_Node<T>* node, 
        const T item)
    {
        auto new_node = doubly_linked_node_alloc<T>();
        new_node->item = item;

        // (prev_node) <-> (node)
        // Changes into
        // (prev_node) <-> (new_node) <-> (node)
        new_node->next = node;
        new_node->prev = node->prev;
        if (node->prev)  
            node->prev->next = new_node;
        node->prev = new_node;

        // Need to adjust the list, in case the new node becomes the first
        if (node == list->head)
        {
            list->head = new_node;
        }

        return new_node;
    }

    t_ const Doubly_Linked_Node<T>* list_find_node(
        const Doubly_Linked_List<T>* list, 
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
    t_ Doubly_Linked_Node<T>* list_find_node(
        Doubly_Linked_List<T>* list, 
        const T item)
    {
        auto const_list = static_cast<const Doubly_Linked_List<T>*>(list);
        auto const_node = list_find_node(const_list, item);
        return const_cast<Doubly_Linked_Node<T>*>(const_node);
    }

    t_ const Doubly_Linked_Node<T>* list_find_node_reverse(
        const Doubly_Linked_List<T>* list, 
        const T item)
    {
        auto node = list->tail;
        
        while (node)
        {
            if (node->item == item)
                return node;

            node = node->prev;
        }

        return NULL;
    }

    t_ Doubly_Linked_Node<T>* list_find_node_reverse(
        Doubly_Linked_List<T>* list, 
        const T item)
    {
        auto const_list = static_cast<const Doubly_Linked_List<T>*>(list);
        auto const_node = list_find_node_reverse(const_list, item);
        return const_cast<Doubly_Linked_Node<T>*>(const_node);
    }
    
    // This function produces unexpected behavior if the given node is not in the given list.
    // If the node is in the middle of some other list, it will work fine. 
    // Otherwise, a null pointer will be dereferenced so expect a crash.
    t_ void list_remove_node(
        Doubly_Linked_List<T>* list, 
        Doubly_Linked_Node<T>* node)
    {
        auto prev_node = node->prev;
        auto next_node = node->next;

        if (list->head == node) list->head = next_node;
        // Since the node is not the head node, it must have a node before itself.
        else prev_node->next = next_node;

        if (list->tail == node) list->tail = prev_node;
        // Since the node is not the tail node, it must have a node after itself.
        else next_node->prev = prev_node;
    }

    t_ Doubly_Linked_Node<T>* list_remove(Doubly_Linked_List<T>* list, const T item)
    {
        auto node = list_find_node(list, item);
        if (!node) return NULL;
        list_remove_node(list, node);
        return node;
    }

    t_ Doubly_Linked_Node<T>* list_remove_reverse(Doubly_Linked_List<T>* list, const T item)
    {
        auto node = list_find_node_reverse(list, item);
        if (!node) return NULL;
        list_remove_node(list, node);
        return node;
    }

    t_ inline bool list_is_empty(Doubly_Linked_List<T>* list)
    {
        return list->head == 0;
    }


    template<typename T, typename Print_Function>
    void list_print(
        const Doubly_Linked_List<T>* list, 
        Print_Function& print_func)
    {
        auto node = list->head;
        while (node)
        {
            print_func(node->item);
            node = node->next; 
        }
    }

    template<typename T, typename Print_Function>
    void list_print_backwards(
        const Doubly_Linked_List<T>* list, 
        Print_Function& print_func)
    {
        auto node = list->tail;
        while (node)
        {
            print_func(node->item);
            node = node->prev; 
        }
    }

    void list_print(const Doubly_Linked_List<int>* list)
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

    void list_print_backwards(const Doubly_Linked_List<int>* list)
    {
        auto node = list->tail;
        while (node)
        {
            printf("%i", node->item);
            node = node->prev;
            if (node) printf(" -> ");
        }
        printf("\n");
    }
}