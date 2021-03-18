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

#include "ds/binary_tree.h"
#include "ds/hash_map.h"

#include "sort/merge.h"
#include "sort/heap.h"
#include "sort/quick.h"
#include "sort/selection.h"
#include "sort/util.h"

#include "ds/singly_linked_list.h"
#include "ds/doubly_linked_list.h"


void search_tests();
void search_profile();
void hash_map();
DS::Binary_Tree<Record*>* binary_tree_from_records(std::vector<Record>& records);
void bt_print();
void sort_tests();
void sort_profile();
void list_tests();

int main()
{
    _chdir("assets");

    /*
    {
        // Lab. 1
        search_tests();
        bt_print();
        search_profile();
        hash_map();
    }
    */
    /*
    {
        // Lab. 2
        sort_tests();
        sort_profile();
    }
    */
    {
        // Lab. 3
        list_tests();
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

        list_destroy(&list);
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

        list_destroy(&list);
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
        DS::bt_insert(&t, &record, [](auto a, auto b) { return (s32)a->id - (s32)b->id; });
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
        assert(DS::bt_find(t, [&](auto rec) { return (s32)rec->id - (s32)(i); })->id == i);

    destroy_records(records);
    destroy_records(records_ordered);
}

// Should be printed in ascending order
void bt_print()
{
    auto records = read_records_from_csv("data_unordered.csv");
    auto bt      = binary_tree_from_records(records);
    DS::bt_print(bt);
    destroy_records(records);
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
                bt_destroy(t);
            },

            num_experiments
        );

        Binary_Tree<Record*>* t = binary_tree_from_records(records);

        profiler_perform_experiments(
            "Binary tree searches (constructed beforehand)",
            [&](auto p) { bt_find(t, 
                [&](auto rec) { return (s32)rec->id - (s32)id(p); }, p); },
            num_experiments * 100
        );
    }
    
    destroy_records(records);
    destroy_records(records_ordered);
}
