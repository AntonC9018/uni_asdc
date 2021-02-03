#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <inttypes.h>

#include <mydefines.h>
#include "data.h"
#include "string_util.h"
#include "search/linear.h"
#include "search/binary.h"
#include "search/exponential.h"

void profile();

int main()
{
    _chdir("assets");
    auto records =         read_records_from_csv("data_unordered.csv");
    auto records_ordered = read_records_from_csv("data.csv");

    Record* record = binary_search(records_ordered, 79); 
    serialize_record(record);

    Record* record2 = exponential_search(records_ordered, 79); 
    serialize_record(record);

    destroy_records(records);
    destroy_records(records_ordered);
    
    // profile();
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
