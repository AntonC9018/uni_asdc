#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <inttypes.h>
#include <map>
#include <assert.h>

#include <mydefines.h>
#include <strlib.h>

#include "data.h"
#include "string_util.h"
#include "search/linear.h"
#include "search/binary.h"
#include "search/exponential.h"

#include "ds/binary_tree.h"
#include "ds/hash_map.h"

void profile();
void stuff();
void hash_map();


int main()
{
    _chdir("assets");
    // profile();
    // stuff();
    hash_map();
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

    assert(compare_record( hmap_get(hm, str_lit("Okabe")), okabe ));
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

void profile()
{
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");
    
    const int num_experiments = 30000;
    constexpr u64 const_search_id = 699;
    u64 search_id = const_search_id;

    {
        printf("Linear Search: NORMAL. (%i samples)\n", num_experiments);
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            assert(linear_search(records, search_id)->id == search_id);
        
        profiler_report_nicely();


        printf("Linear Search: STATIC PREDICATE. (%i samples)\n", num_experiments);
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            assert(
                linear_search_predicate(records, [](const Record* r) { return r->id == const_search_id; })->id == search_id);

        profiler_report_nicely();


        printf("Linear Search: FUNCTOR WITH THE VALUES COPIED. (%i samples)\n", num_experiments);
        profiler_start();
        
        for (int i = 0; i < num_experiments; i++)
            assert(linear_search_stdfunction(records, [=](const Record* r) { return r->id == search_id; })->id == search_id);

        profiler_report_nicely();


        printf("Linear Search: FUNCTOR WITH THE VALUES AS REFERENCE. (%i samples)\n", num_experiments);
        profiler_start();
        
        for (int i = 0; i < num_experiments; i++)
            assert(linear_search_stdfunction(records, [&](const Record* r) { return r->id == search_id; })->id == search_id);

        profiler_report_nicely();
    }

    {
        printf("Binary Search: NORMAL. (%i samples)\n", num_experiments);
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            assert(binary_search(records_ordered, search_id)->id == search_id);
        
        profiler_report_nicely();
    }

    {
        printf("Exponential Search: NORMAL. (%i samples)\n", num_experiments);
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            assert(exponential_search(records_ordered, search_id)->id == search_id);
        
        profiler_report_nicely();
    }

    {
        using namespace DS;
        Binary_Tree<Record*>* t = NULL; // or Binary_Tree<u32>* t = NULL;

        for (auto& record : records)
        {
            insert<Record*>(&t, &record, [](auto a, auto b) { return (s32)a->id - (s32)b->id; });
            // or insert(&t, record.id, ...);
        }

        printf("Binary tree Search: PREDICATE. (%i samples)\n", num_experiments * 100);
        profiler_start();

        for (int i = 0; i < num_experiments * 100; i++)
            assert(find<Record*>(t, [](auto rec) { return (s32)rec->id - (s32)const_search_id; })->id == search_id); 
            // or records_ordered[find(t, ...)]; 

        profiler_report_nicely();
    }
}
