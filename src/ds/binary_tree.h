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
    void insert(Binary_Tree<T>** node, T value, Comparator compare)
    {
        while (true)
        {
            if (!*node)
            {
                Binary_Tree<T>* new_node = (Binary_Tree<T>*) malloc(sizeof(Binary_Tree<T>));
                new_node->value = value;
                new_node->left = NULL;
                new_node->right = NULL;
                *node = new_node;
                return;
            }

            s32 comparison_result = compare((*node)->value, value);
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
    T find(Binary_Tree<T>* node, Comparator compare, Profiler* profiler = &_std_profiler)
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
                return node->value;
            }
            if (comparison_result > 0)
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }
    }

    template<typename T>
    void destroy(Binary_Tree<T>* tree)
    {
        if (tree != NULL)
        {
            destroy(tree->left);
            destroy(tree->right);
            free(tree);
        }
    }

    template<typename T>
    void print(Binary_Tree<T>* node) 
    {
        if (!node)
        {
            return;
        }
        serialize_record(node->value);
        print(node->left);
        print(node->right);
    }
}