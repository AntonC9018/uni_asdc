#pragma once

#include <mydefines.h>

#include "../profiler.h"
#include "shared.h"

namespace DS
{
    t_
    struct Binary_Tree
    {
        Binary_Tree* left;
        Binary_Tree* right;
        T item;
    };
    
    tc_ 
    void bst_insert_node(
        Binary_Tree<T>** node, 
        Binary_Tree<T>* new_node, 
        C compare_func)
    {
        while (true)
        {
            if (!*node)
            {
                *node = new_node;
                return;
            }

            int comparison_result = compare_func(new_node->item, (*node)->item);
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

    tc_
    void bst_insert(Binary_Tree<T>** tree, const T value, C compare_func)
    {
        auto new_node = (Binary_Tree<T>*) malloc(sizeof(Binary_Tree<T>));

        new_node->item = value;
        new_node->left = NULL;
        new_node->right = NULL;

        bst_insert_node(tree, new_node, compare_func);
    }

    tc_
    Binary_Tree<T>* bst_find_node(
        Binary_Tree<T>* node, 
        C compare_func, 
        Profiler* profiler = &_std_profiler)
    {
        while(true)
        {
            profiler->num_iters++;

            if (!node)
            {
                return NULL;
            }

            int comparison_result = compare_func(node->item);

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

    tc_
    inline T* bst_find(
        Binary_Tree<T>* tree, 
        C compare_func, 
        Profiler* profiler = &_std_profiler)
    {
        auto node = bst_find_node(tree, compare_func, profiler);
        return node ? &node->item : NULL;
    }

    t_
    void bst_free(Binary_Tree<T>* tree)
    {
        if (tree != NULL)
        {
            bst_free(tree->left);
            bst_free(tree->right);
            free(tree);
        }
    }

    t_
    void bst_print_inorder(Binary_Tree<T>* node);

    template<>
    void bst_print_inorder(Binary_Tree<Record*>* node)
    {
        if (!node) return;
        bst_print_inorder(node->left);
        serialize_record(node->item);
        bst_print_inorder(node->right);
    }

    t_
    void bst_print_outorder(Binary_Tree<T>* node);

    template<>
    void bst_print_outorder(Binary_Tree<Record*>* node) 
    {
        if (!node) return;
        bst_print_outorder(node->right);
        serialize_record(node->item);
        bst_print_outorder(node->left);
    }

    t_
    void bst_print_noorder(Binary_Tree<T>* node);

    template<>
    void bst_print_noorder(Binary_Tree<Record*>* node)
    {
        if (!node) return;
        serialize_record(node->item);
        bst_print_noorder(node->right);
        bst_print_noorder(node->left);
    }

    template<>
    void bst_print_inorder(Binary_Tree<int>* node)
    {
        if (!node) return;
        bst_print_inorder(node->left);
        printf("%i ", node->item);
        bst_print_inorder(node->right);
    }
    template<>
    void bst_print_outorder(Binary_Tree<int>* node) 
    {
        if (!node) return;
        bst_print_outorder(node->right);
        printf("%i ", node->item);
        bst_print_outorder(node->left);
    }

    template<>
    void bst_print_noorder(Binary_Tree<int>* node)
    {
        if (!node) return;
        printf("%i ", node->item);
        bst_print_noorder(node->right);
        bst_print_noorder(node->left);
    }

    /*
        Returns the parent node and sets the child_node pointer, 
        passed by poiter to double pointer, to the child, that matched on comparison.
        If the parent is NULL, while the child_node is a pointer to a NULL, the child_node must be the root
        (assuming the value passed was a double pointer to the root node).
        If the parent is not NULL, while the child is pointing to NULL, 
        the parent node now actually indicates the potential parent for an item, 
        that would match on comparison, while the child_node points to the slot in the parent
        in which a new node with that value would have to be inserted (i.e. left or right).
    */
    /*
    tc_
    Binary_Tree<T>* bst_find_parent(Binary_Tree<T>*** child_node, C compare_func)
    {
        if (!child_node) return NULL;
        auto parent_node = *child_node;

        {
            int comparison_result = compare_func(parent_node->item);

            if (comparison_result == 0)
            {
                return NULL;
            }
            if (comparison_result < 0)
            {
                *child_node = parent_node->right;
            }
            else
            {
                *child_node = parent_node->left;
            }
        }

        while (*child_node)
        {
            int comparison_result = compare_func((*child_node)->item);

            if (comparison_result == 0)
            {
                return parent_node;
            }
            else
            {
                parent_node = *child_node;
                if (comparison_result < 0)
                {
                    *child_node = parent_node->right;
                }
                else
                {
                    *child_node = parent_node->left;
                }
            }
        }

        return parent_node;
    }
    */

    t_
    Binary_Tree<T>* bst_min_node(Binary_Tree<T>* node)
    {
        while (node)
        {
            node = node->left;
        }

        return node;
    }

    t_
    Binary_Tree<T>** bst_min_ptr_in_parent(Binary_Tree<T>** min_node)
    {
        while ((*min_node)->left)
        {
            min_node = &(*min_node)->left;
        }

        return min_node;
    }

    tc_
    Binary_Tree<T>* bst_remove(
        Binary_Tree<T>* node, 
        C compare_func)
    {
        if (!node) return NULL;

        int comparison_result = compare_func(node->item);
        
        if (comparison_result > 0)
        {
            node->left = bst_remove(node->left, compare_func);
        }
        else if (comparison_result < 0)
        {
            node->right = bst_remove(node->right, compare_func);
        }
        else
        {
            // Right becomes the root
            if (!node->left)
            {
                auto t = node->right;
                free(node);
                return t;
            }
            
            // Left becomes the root
            if (!node->right)
            {
                auto t = node->left;
                free(node);
                return t;
            }

            // Otherwise, the node has 2 children.
            // Find the parent of inorder successor (minimum in the right subtree).
            auto** min_leaf_ptr_in_parent = bst_min_ptr_in_parent(&node->right);
            auto*  min_leaf = *min_leaf_ptr_in_parent;

            // Since the info about the parent of the node the is currently being removed
            // has been lost, copy the value instead.
            node->item = min_leaf->item;

            // We're free to delete this node, since it is terminal.
            free(min_leaf);

            // Remove the dangling pointer from the parent node.
            *min_leaf_ptr_in_parent = NULL;
        }

        return node;
    }
}