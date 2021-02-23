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
#include "sort/util.h"

void profile();
void stuff();
void hash_map();
void sorts();
void search_tests();


int main()
{
    _chdir("assets");
    search_tests();
    // profile();
    // hash_map();
    // sorts();
}

s32 compare_func(Record* a, Record* b)
{ 
    return str_compare(str_view(a->first_name), str_view(b->first_name)); 
};


void sorts()
{
    auto records = read_records_from_csv("data_unordered.csv");
    Sort::merge_sort(records.begin(), records.end(), compare_func);
    assert(Sort::is_sorted(records.begin(), records.end(), compare_func));
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

void stuff()
{
    auto records         = read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");

    Record* record = binary_search(records_ordered, 79); 
    serialize_record(record);

    Record* record2 = exponential_search(records_ordered, 79); 
    serialize_record(record2);

    
    // print(t);


    destroy_records(records);
    destroy_records(records_ordered);
}

DS::Binary_Tree<Record*>* binary_tree_from_records(std::vector<Record>& records)
{
    DS::Binary_Tree<Record*>* t = NULL;
    for (auto& record : records)
    {
        DS::insert(&t, &record, [](auto a, auto b) { return (s32)a->id - (s32)b->id; });
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
        assert(DS::find(t, [&](auto rec) { return (s32)rec->id - (s32)(i); })->id == i);
}

void profile()
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
                destroy(t);
            },

            num_experiments
        );

        Binary_Tree<Record*>* t = binary_tree_from_records(records);

        profiler_perform_experiments(
            "Binary tree searches (constructed beforehand)",
            [&](auto p) { find(t, 
                [&](auto rec) { return (s32)rec->id - (s32)id(p); }, p); },
            num_experiments * 100
        );
    }
}
