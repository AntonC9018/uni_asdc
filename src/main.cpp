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


int main()
{
    // std::map<str_view_t, int> p;
    // p.insert({str_lit("HELLO"), 1});
    // p.insert({str_lit("WORLD"), 2});

    // printf("%i", p[str_lit("HELLO")]);
    // stuff();
    // profile();
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
    _chdir("assets");
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");

    Record* record = binary_search(records_ordered, 79); 
    serialize_record(record);

    Record* record2 = exponential_search(records_ordered, 79); 
    serialize_record(record2);

    using namespace DS;
    Binary_Tree* t = NULL;

    for (auto& record : records)
    {
        insert(&t, &record, [](auto a, auto b) { return (s32)a->id - (s32)b->id; });
    }
    print(t);

    destroy_records(records);
    destroy_records(records_ordered);
}

void profile()
{
    auto records =          read_records_from_csv("data_unordered.csv");
    auto records_ordered =  read_records_from_csv("data.csv");
    
    const int num_experiments = 10000;
    u64 search_id = 699;

    {
        printf("Linear Search: NORMAL.\n");
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            linear_search(records, search_id);
        
        profiler_report_nicely();


        printf("Linear Search: STATIC PREDICATE.\n");
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            linear_search_predicate(records, [](const Record* r) { return r->id == 699; });

        profiler_report_nicely();


        printf("Linear Search: FUNCTOR WITH THE VALUES COPIED.\n");
        profiler_start();

        
        for (int i = 0; i < num_experiments; i++)
            linear_search_stdfunction(records, [=](const Record* r) { return r->id == search_id; });

        profiler_report_nicely();


        printf("Linear Search: FUNCTOR WITH THE VALUES AS REFERENCE.\n");
        profiler_start();
        
        for (int i = 0; i < num_experiments; i++)
            linear_search_stdfunction(records, [&](const Record* r) { return r->id == search_id; });

        profiler_report_nicely();
    }

    {
        printf("Binary Search: NORMAL.\n");
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            linear_search(records_ordered, search_id);
        
        profiler_report_nicely();
    }

    {
        printf("Exponential Search: NORMAL.\n");
        profiler_start();

        for (int i = 0; i < num_experiments; i++)
            linear_search(records_ordered, search_id);
        
        profiler_report_nicely();
    }
}
