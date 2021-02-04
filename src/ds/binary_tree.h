#include <stdio.h>
#include "../profiler.h"
#include <mydefines.h>
#include "../data.h"

namespace DS
{
    struct Binary_Tree
    {
        Binary_Tree* left;
        Binary_Tree* right;
        Record* value;
    };

    void insert(Binary_Tree** node, Record* value, s32(compare)(Record*, Record*))
    {
        while (true)
        {
            if (!*node)
            {
                Binary_Tree* new_node = (Binary_Tree*) malloc(sizeof(Binary_Tree));
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

    Record* find(Binary_Tree* node, s32(compare)(Record*))
    {
        while(true)
        {
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

    void print(Binary_Tree* node) 
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