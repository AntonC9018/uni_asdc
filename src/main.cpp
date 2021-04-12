#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <inttypes.h>
#include <assert.h>

#include <mydefines.h>
#include <strlib.h>

#include "data.h"
#include "search/linear.h"
#include "search/binary.h"
#include "search/exponential.h"

#include "ds/binary_search_tree.h"
#include "ds/hash_map.h"

#include "sort/merge.h"
#include "sort/heap.h"
#include "sort/quick.h"
#include "sort/selection.h"
#include "sort/util.h"

#include "ds/singly_linked_list.h"
#include "ds/doubly_linked_list.h"
#include "ds/array_stack.h"
#include "ds/list_stack.h"
#include "ds/cyclic_buffer.h"
#include "ds/queue.h"

#include "ds/array/iliffe.h"
#include "ds/array/ranged_iliffe.h"
#include "ds/array/dope_wrapper.h"
#include "ds/array/raw_wrapper.h"

void search_tests();
void search_profile();
void hash_map();
DS::Binary_Tree<Record*>* binary_tree_from_records(std::vector<Record>& records);
void bst_print();
void sort_tests();
void sort_profile();
void list_tests();
void stack_tests();
void queue_tests();
void bst_removal();
void bst_pretty();
void vectors_test();
template<typename Wrapper_Type>
void dope_vector_test_mode(DS::Major_Order mode);

int main()
{
    _chdir("assets");

    
    {
        // Lab. 1
        // search_tests();
        // bst_print();
        // search_profile();
        // hash_map();
    }
    
    /*
    {
        // Lab. 2
        sort_tests();
        sort_profile();
    }
    */
    {
        // Lab. 3
        // list_tests();
        // stack_tests();
        // queue_tests();
        // bst_removal();
        // bst_pretty();
    }
    {
        // Lab 4.
        vectors_test();
    }
}

void vectors_test()
{
    using namespace DS;

    {
        auto iliffe = Iliffe_Vector<int, 2>::make(3);
        for (size_t i = 0; i < iliffe.size; i++)
        {
            iliffe[i] = Iliffe_Vector<int, 1>::make(2);
            iliffe[i][0] = i;
            iliffe[i][1] = i + 1;
        }

        for (size_t i = 0; i < iliffe.size; i++)
        {
            for (size_t j = 0; j < iliffe[i].size; j++)
            {
                printf("[%zu][%zu] = %i\n", i, j, iliffe[i][j]);
            }
        }
    }
    {
        auto iliffe = Ranged_Iliffe_Vector<int, 2>::make({-1, 1});
        for (ssize_t i = iliffe.range.start; i <= iliffe.range.end; i++)
        {
            iliffe[i] = Ranged_Iliffe_Vector<int, 1>::make({-1, 0});
            iliffe[i][-1] = i;
            iliffe[i][ 0] = i + 1;
        }

        for (ssize_t i = iliffe.range.start; i <= iliffe.range.end; i++)
        {
            for (ssize_t j = iliffe[i].range.start; j <= iliffe[i].range.end; j++)
            {
                printf("[%2zi][%2zi] = %i\n", i, j, iliffe[i][j]);
            }
        }
    }
    {
        dope_vector_test_mode<Dope_Wrapper<int, 4>>(ROW_MAJOR);
        dope_vector_test_mode<Dope_Wrapper<int, 4>>(COL_MAJOR);

        dope_vector_test_mode<Raw_Array_Wrapper<int, 4>>(ROW_MAJOR);
        dope_vector_test_mode<Raw_Array_Wrapper<int, 4>>(COL_MAJOR);
    }
}

template<typename Wrapper_Type>
void dope_vector_test_mode(DS::Major_Order mode)
{
    Wrapper_Type arr;
    arr.set_range(0, { 3,  6}); 
    arr.set_range(1, { 1,  3}); 
    arr.set_range(2, {-3, -1});
    arr.set_range(3, {-5, -3});
    arr.init(mode); 

    ssize_t indices[4];
    #define i indices[0]
    #define j indices[1]
    #define k indices[2]
    #define w indices[3]
    
    #define ITERATE_ROWS \
        for (i = arr.start(0); i <= arr.end(0); i++) \
        for (j = arr.start(1); j <= arr.end(1); j++) \
        for (k = arr.start(2); k <= arr.end(2); k++) \
        for (w = arr.start(3); w <= arr.end(3); w++) 
    
    #define ITERATE_COLS \
        for (w = arr.start(3); w <= arr.end(3); w++) \
        for (k = arr.start(2); k <= arr.end(2); k++) \
        for (j = arr.start(1); j <= arr.end(1); j++) \
        for (i = arr.start(0); i <= arr.end(0); i++) 

    size_t el_count = 0;

    if (mode == DS::ROW_MAJOR)
        ITERATE_ROWS { arr[indices] = el_count++; }
    else
        ITERATE_COLS { arr[indices] = el_count++; }


    assert(el_count == arr.size());
    printf("\n");

    ITERATE_ROWS { printf("arr[%2zi][%2zi][%2zi][%2zi] = %i\n", i, j, k, w, arr[indices]); }
    printf("\n");

    ITERATE_COLS { printf("arr[%2zi][%2zi][%2zi][%2zi] = %i\n", i, j, k, w, arr[indices]); }
    printf("\n");

    #undef i
    #undef j
    #undef k

    // should always print secventially
    for (size_t i = 0; i < arr.size(); i++)
    {
        printf("arr[%zu] = %i\n", i, arr.items[i]);
    }   
}

void bst_pretty()
{
    using namespace DS;
    auto f = [](char a, char b) { return a - b; };
    Binary_Tree<char>* bst = NULL;
    bst_insert(&bst, 'l', f);
    bst_insert(&bst, 'f', f);
    bst_insert(&bst, 'q', f);
    bst_insert(&bst, 'c', f);
    bst_insert(&bst, 'h', f);
    bst_insert(&bst, 'n', f);
    bst_insert(&bst, 'u', f);
    bst_insert(&bst, 'b', f);
    bst_insert(&bst, 'a', f);
    bst_insert(&bst, 'd', f);
    bst_insert(&bst, 'e', f);
    bst_insert(&bst, 'j', f);
    bst_insert(&bst, 'i', f);
    bst_insert(&bst, 'm', f);
    bst_insert(&bst, 'p', f);
    bst_insert(&bst, 't', f);
    bst_insert(&bst, 'x', f);
    bst_insert(&bst, 'w', f);
    bst_insert(&bst, 'z', f);
    bst_graph(bst);
}

void bst_removal()
{
    using namespace DS;
    auto cmp_ints = [](int a, int b) { return a - b; };
    auto find_int = [](int b){ return [=](int a) { return a - b; }; };

    Binary_Tree<int>* bst = NULL;

    //          3              //
    //         / \             //
    //        /   \            //
    //       1     5           //
    //      / \   / \          //
    //     0   2 4   6         //
    bst_insert(&bst, 3, cmp_ints);
    bst_insert(&bst, 1, cmp_ints);
    bst_insert(&bst, 5, cmp_ints);
    bst_insert(&bst, 0, cmp_ints);
    bst_insert(&bst, 2, cmp_ints);
    bst_insert(&bst, 4, cmp_ints);
    bst_insert(&bst, 6, cmp_ints);
    bst_print_inorder(bst); printf("\n");

    // -------------------------------------------------------- //
    // Simplest case, leaf node                                 //
    // -------------------------------------------------------- //
    //         3                  3                   3         //
    //        / \                / \                 / \        //
    //       /   \              /   \               /   \       //
    //      1     5     ->     1     5      ->     1     5      //
    //     / \   / \          / \   / \             \   / \     //
    //    0   2 4   6        x   2 4   6             2 4   6    //
    // -------------------------------------------------------- //
    bst = bst_remove(bst, find_int(0));
    bst_print_inorder(bst); printf("\n");

    // -------------------------------------------------------- //
    // One child                                                //
    // -------------------------------------------------------- //
    //       3                    3                   3         //
    //      / \                  / \                 / \        //
    //     /   \                /   \               /   \       //
    //    1     5      ->      x     5      ->     2     5      //
    //     \   / \              \   / \                 / \     //
    //      2 4   6              2 4   6               4   6    //
    // -------------------------------------------------------- //
    bst = bst_remove(bst, find_int(1));
    bst_print_inorder(bst); printf("\n");

    // -------------------------------------------------------- //
    // Two children                                             //
    // -------------------------------------------------------- //
    //       3                    x                   4         //
    //      / \                  / \                 / \        //
    //     /   \                /   \               /   \       //
    //    2     5      ->      2     5      ->     2     5      //
    //         / \                  / \                   \     //
    //        4   6                4   6                   6    //
    // -------------------------------------------------------- //
    bst = bst_remove(bst, find_int(3)); 
    bst_print_inorder(bst); printf("\n");

    // Make sure they are in this order
    assert(              bst->item == 4);
    assert(        bst->left->item == 2);
    assert(       bst->right->item == 5);
    assert(       bst->right->left == NULL);
    assert(bst->right->right->item == 6);

    bst_free(bst);
}
     
void queue_tests()
{
    using namespace DS;
    {
        auto cycbuf = cycbuf_make<int>(3);

        assert(cycbuf_is_empty(&cycbuf));

        cycbuf_enqueue(&cycbuf, 1);
        cycbuf_enqueue(&cycbuf, 2);
        cycbuf_enqueue(&cycbuf, 3);

        assert(cycbuf_is_full(&cycbuf));

        auto item_1 = cycbuf_dequeue(&cycbuf);

        assert(!cycbuf_is_full(&cycbuf));
        assert(cycbuf.count == 2);
        assert(item_1 == 1);

        cycbuf_enqueue(&cycbuf, 4);

        auto item_2 = *cycbuf_peek(&cycbuf);

        assert(item_2 == 2);

        cycbuf_dequeue(&cycbuf);
        cycbuf_dequeue(&cycbuf);
        cycbuf_dequeue(&cycbuf);

        assert(cycbuf_is_empty(&cycbuf));

        cycbuf_free(&cycbuf);
    }
    {
        auto queue = q_make<int>();

        assert(q_is_empty(&queue));

        q_enqueue(&queue, 1);
        q_enqueue(&queue, 2);
        q_enqueue(&queue, 3);

        assert(*q_peek(&queue) == 1);

        assert(q_dequeue(&queue) == 1);
        assert(q_dequeue(&queue) == 2);
        assert(q_dequeue(&queue) == 3);

        assert(q_is_empty(&queue));

        q_free(&queue);
    }
}

template<typename Stack>
void stack_test(Stack* stack)
{
    // initally empty
    assert(stack_is_empty(stack));

    // add some elements
    stack_push(stack, 3);
    stack_push(stack, 4);
    stack_push(stack, 5);

    // peek an element
    assert(*stack_peek(stack) == 5);

    // pop some back
    assert(stack_pop(stack) == 5);
    assert(stack_pop(stack) == 4);
    assert(stack_pop(stack) == 3);

    // emptied back
    assert(stack_is_empty(stack));

    stack_free(stack);
}

void stack_tests()
{
    using namespace DS;
    {
        auto stack = array_stack_make<int>(10);
        stack_test(&stack);
    }
    {
        auto stack = list_stack_make<int>();
        stack_test(&stack);
    }
}

void list_tests()
{
    using namespace DS;

    {
        auto list = singly_linked_list_make<int>();

        assert(list.head == NULL);
        assert(list.tail == NULL);

        // 5
        auto node_5 = list_insert_back(&list, 5);

        assert(node_5->next == NULL);
        assert(list.head && list.head == node_5);
        assert(list.tail && list.tail == node_5);
        assert(node_5->item == 5);

        // 3 -> 5
        auto node_3 = list_insert_front(&list, 3);
        
        assert(list.head == node_3);
        assert(list.tail == node_5);

        // 3 -> 1 -> 5
        auto node_1 = list_insert_after(&list, node_3, 1);

        assert(list.head->next == node_1);
        assert(node_1->next    == node_5);

        // 3 -> 1 -> 5 -> 2
        auto node_2 = list_insert_after(&list, list.tail, 2);

        assert(list.tail == node_2);

        // 3 -> 1 -> 5
        auto removed_2 = list_remove(&list, 2);

        assert(list.tail == node_5);
        free(removed_2);

        // 3 -> 1
        auto removed_5 = list_remove_after(&list, node_1);

        assert(list.tail == node_1);
        free(removed_5);

        printf("Printing the list: "); 
        list_print(&list);

        list_free(&list);
    }
    {
        auto list = doubly_linked_list_make<int>();

        assert(list.head == NULL);
        assert(list.tail == NULL);

        // 1
        auto node_1 = list_insert_front(&list, 1);

        assert(node_1->item == 1);
        assert(node_1->next == NULL);
        assert(node_1->prev == NULL);
        assert(list.head == node_1);
        assert(list.tail == node_1);

        // 1 -> 2
        auto node_2 = list_insert_back(&list, 2);

        assert(node_1->prev == NULL);
        assert(node_1->next == node_2);
        assert(node_2->prev == node_1);
        assert(node_2->next == NULL);
        assert(list.head == node_1);
        assert(list.tail == node_2);

        // 3 -> 1 -> 2
        auto node_3 = list_insert_before(&list, node_1, 3);

        assert(node_1->prev == node_3);
        assert(node_3->next == node_1);
        assert(node_3->prev == NULL);
        assert(list.head == node_3);

        // 3 -> 1 -> 2 -> 4
        auto node_4 = list_insert_after(&list, node_2, 4);

        assert(node_2->next == node_4);
        assert(node_4->prev == node_2);
        assert(node_4->next == NULL);
        assert(list.tail == node_4);

        // 3 -> 1 -> 2 -> 5 -> 4 
        auto node_5 = list_insert_after(&list, node_2, 5); 

        assert(node_2->next == node_5);
        assert(node_4->prev == node_5);

        // 3 -> 1 -> 2 -> 4
        auto removed_5 = list_remove(&list, 5);

        assert(removed_5 == node_5);
        free(removed_5);

        // 3 -> 1 -> 2
        list_remove_node(&list, node_4);

        assert(node_2->next == NULL);
        assert(node_2 == list.tail);

        // 1 -> 2
        auto removed_3 = list_remove(&list, 3);

        assert(removed_3 == node_3);
        free(removed_3);

        // 1 -> 2
        auto removed_null = list_remove(&list, 3);
        assert(removed_null == NULL);

        printf("Printing the list forwards: "); 
        list_print(&list);

        printf("Printing the list backwards: "); 
        list_print_backwards(&list);

        list_free(&list);
    }
}

// Use references here, since it plays out better with templates
s32 compare_func(const Record& a, const Record& b)
{
    return str_compare(str_view(a.first_name), str_view(b.first_name)); 
};

void sort_tests()
{
    auto records = read_records_from_csv("data.csv");
    auto begin = records.begin();
    auto end   = records.end();

#define SHUFFLE()       std::random_shuffle(begin, end)
#define ASSERT_SORTED() assert(Sort::is_sorted(begin, end, compare_func))
#define SORT_TEST(sort) \
    SHUFFLE(); \
    (sort)(begin, end, compare_func); \
    ASSERT_SORTED();

    SORT_TEST(Sort::merge_sort);
    SORT_TEST(Sort::heap_sort);
    SORT_TEST(Sort::quick_sort);
    SORT_TEST(Sort::selection_sort);

    destroy_records(records);
}

void sort_profile()
{
    auto records = read_records_from_csv("data.csv");
    auto begin = records.begin();
    auto end   = records.end();

    const size_t num_experiments = 1000;

#define PERFORM_EXPERIMENTS(text, sort) \
    profiler_perform_experiments( \
        (text), \
        [&](auto p) \
        { \
            std::random_shuffle(begin, end);\
            (sort)(begin, end, compare_func, p); \
        },\
        num_experiments\
    )\

    PERFORM_EXPERIMENTS("Merge Sort.",     Sort::merge_sort);
    PERFORM_EXPERIMENTS("Heap Sort.",      Sort::heap_sort);
    PERFORM_EXPERIMENTS("Quick Sort.",     Sort::quick_sort);
    PERFORM_EXPERIMENTS("Selection Sort.", Sort::selection_sort);

    destroy_records(records);
}

void hash_map()
{
    using namespace DS;
    auto hash_map = hmap_create();
    auto hm       = &hash_map;
    
    Record john;
    john.id         = 7;
    john.gender     = FEMALE;
    john.ip_address = deserialize_ip_address("123.123.123.123");
    john.email      = str_copy_lit("@mail.ru");
    john.first_name = str_copy_lit("John");
    john.email      = str_copy_lit("Titor");

    Record okabe;
    okabe.id         = 1;
    okabe.gender     = MALE;
    okabe.ip_address = deserialize_ip_address("231.231.231.231");
    okabe.email      = str_copy_lit("@dmail.com");
    okabe.first_name = str_copy_lit("Okabe");
    okabe.email      = str_copy_lit("Rintarou");


    // First time adding these records
    assert(hmap_put(hm, str_lit("John"),  john));

    // Print the hashed bucket index 
    // Strings are hashed using the FNVa hashing algorithm
    printf("%zu\n", hmap_hash(hm, str_lit("John")));
    printf("%zu\n", hmap_hash(hm, str_lit("Okabe")));
    // Might as well show the hashes themselves
    printf("%zu\n", str_hash(str_lit("John")));
    printf("%zu\n", str_hash(str_lit("Okabe")));

    assert(hmap_put(hm, str_lit("Okabe"), okabe));

    // Okabe has already been added, so he is replaced here
    assert(!hmap_put(hm, str_lit("Okabe"), okabe));

    assert( hmap_has_key(hm, str_lit("Okabe")));
    assert( hmap_has_key(hm, str_lit("John")));
    assert(!hmap_has_key(hm, str_lit("Mayuri")));
    assert(hmap_has_value(hm, okabe));

    auto value = hmap_get(hm, str_lit("Okabe"));

    assert(compare_record( &value, &okabe ));
}

DS::Binary_Tree<Record*>* binary_tree_from_records(std::vector<Record>& records)
{
    DS::Binary_Tree<Record*>* t = NULL;
    for (auto& record : records)
    {
        DS::bst_insert(&t, &record, [](auto a, auto b) { return (s32)a->id - (s32)b->id; });
        // or insert(&t, record.id, ...);
    }
    return t;
}

void search_tests()
{
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");

    for (u32 i = 1; i <= records.size(); i++)
        assert(linear_search(records, i)->id == i); 

    for (u32 i = 1; i <= records_ordered.size(); i++)
        assert(binary_search(records_ordered, i)->id == i);

    for (u32 i = 1; i <= records_ordered.size(); i++)
        assert(exponential_search(records_ordered, i)->id == i); 

    auto t = binary_tree_from_records(records); 

    for (u32 i = 1; i <= records_ordered.size(); i++)
        assert((*DS::bst_find(t, [&](auto rec) { return (s32)rec->id - (s32)(i); }))->id == i);

    destroy_records(records);
    destroy_records(records_ordered);
}

// Should be printed in ascending order
void bst_print()
{
    auto records = read_records_from_csv("data_unordered.csv");
    auto bt      = binary_tree_from_records(records);

    printf("Printing INORDER: \n\n");
    DS::bst_print_inorder(bt);
    printf("\n\n");

    printf("Printing OUTORDER: \n\n");
    DS::bst_print_outorder(bt);
    printf("\n\n");

    printf("Printing NOORDER: \n\n");
    DS::bst_print_noorder(bt);
    printf("\n\n");

    destroy_records(records);
    bst_free(bt);
}

void search_profile()
{
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");
    
    const int num_experiments = 10000;

    // generate random id's for each of the experiments
    std::vector<u64> ids(num_experiments);
    for (int i = 0; i < num_experiments; i++)
    {
        ids[i] = rand() % records.size() + 1;
    }

    // retrieve the random id for the current iteration
    auto id = [&](Profiler* p) { return ids[p->num_experiments % num_experiments]; };


    profiler_perform_experiments(
        "Linear Search: NORMAL.",
        [&](auto p) { linear_search(records, id(p), p); },
        num_experiments
    );

    profiler_perform_experiments(
        "Linear Search: STD::FUNCTION.",
        [&](auto p) { linear_search_stdfunction(records, 
            [=](const Record* r) { return r->id == id(p); }, p); 
        },
        num_experiments
    );

    profiler_perform_experiments(
        "Binary Search: NORMAL.",
        [&](auto p) { binary_search(records_ordered, id(p), p); },
        num_experiments * 100
    );

    profiler_perform_experiments(
        "Exponential Search: NORMAL.",
        [&](auto p) { exponential_search(records_ordered, id(p), p); },
        num_experiments * 100
    );

    {
        using namespace DS;

        profiler_perform_experiments(
            "Binary tree construction.",

            [&](auto p) { 
                Binary_Tree<Record*>* t = binary_tree_from_records(records);
                bst_free(t);
            },

            num_experiments
        );

        Binary_Tree<Record*>* t = binary_tree_from_records(records);

        profiler_perform_experiments(
            "Binary tree searches (constructed beforehand)",
            [&](auto p) { bst_find(t, 
                [&](auto rec) { return (s32)rec->id - (s32)id(p); }, p); },
            num_experiments * 100
        );
    }
    
    destroy_records(records);
    destroy_records(records_ordered);
}
