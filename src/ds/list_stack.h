// "t_" is a shortcut for "template<typename T>", imported from here
#include "shared.h"

namespace DS
{
    t_ struct List_Stack
    {
        Singly_Linked_Node<T>* top;
    };

    t_ inline List_Stack<T> list_stack_make()
    {
        return { NULL };
    }

    t_ inline bool stack_is_empty(const List_Stack<T>* stack)
    {
        return stack->top == NULL;
    }

    t_ inline T stack_pop(List_Stack<T>* stack)
    {
        auto top = stack->top;
        auto item = top->item;
        stack->top = top->next;
        free(top);
        return item;
    }

    t_ inline void stack_push(List_Stack<T>* stack, const T item)
    {
        auto node = singly_linked_node_alloc<T>();
        node->item = item;
        node->next = stack->top;
        stack->top = node;
    }

    t_ inline const T* stack_peek(const List_Stack<T>* stack)
    {
        return &stack->top->item;
    }

    // This logically should not make a copy of that value, but rather let you modify it.
    t_ inline T* stack_peek(List_Stack<T>* stack)
    {
        auto const_stack = static_cast<const List_Stack<T>*>(stack);
        auto const_item  = stack_peek(const_stack);
        return const_cast<T*>(const_item);
    }
}