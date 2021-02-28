#include <stdio.h>
#include "../profiler.h"
#include <mydefines.h>
#include "../data.h"

namespace DS
{
    template<typename T>
    struct Binary_Tree
    {
        Binary_Tree* left;
        Binary_Tree* right;
        T value;
    };

    template<typename T, typename Comparator>
    void bt_insert_node(Binary_Tree<T>** node, Binary_Tree<T>* new_node, Comparator compare)
    {
        while (true)
        {
            if (!*node)
            {
                *node = new_node;
                return;
            }

            s32 comparison_result = compare(new_node->value, (*node)->value);
            if (comparison_result > 0)
            {
                node = &((*node)->right);
            }
            else
            {
                node = &((*node)->left);
            }
        }
    }

    template<typename T, typename Comparator>
    void bt_insert(Binary_Tree<T>** tree, T value, Comparator compare)
    {
        Binary_Tree<T>* new_node = (Binary_Tree<T>*) malloc(sizeof(Binary_Tree<T>));
        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;
        bt_insert_node(tree, new_node, compare);
    }
    

    template<typename T, typename Comparator>
    Binary_Tree<T>* bt_find_node(Binary_Tree<T>* node, Comparator compare, Profiler* profiler = &_std_profiler)
    {
        while(true)
        {
            profiler->num_iters++;

            if (!node)
            {
                return {};
            }

            s32 comparison_result = compare(node->value);

            if (comparison_result == 0)
            {
                return node;
            }
            if (comparison_result < 0)
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }
        return NULL;
    }

    template<typename T, typename Comparator>
    T bt_find(Binary_Tree<T>* tree, Comparator compare, Profiler* profiler = &_std_profiler)
    {
        auto node = bt_find_node(tree, compare, profiler);
        return node ? node->value : T();
    }


    template<typename T>
    void bt_destroy(Binary_Tree<T>* tree)
    {
        if (tree != NULL)
        {
            bt_destroy(tree->left);
            bt_destroy(tree->right);
            free(tree);
        }
    }

    template<typename T>
    void bt_print(Binary_Tree<T>* node) 
    {
        if (!node)
        {
            return;
        }
        bt_print(node->left);
        serialize_record(node->value);
        bt_print(node->right);
    }
}