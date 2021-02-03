#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <inttypes.h>

#include <mydefines.h>
#include "data.h"
#include "string_util.h"
#include "searching.h"

int main()
{
    _chdir("assets");
    auto records = read_records_from_csv("data_unordered.csv");
    
    {
        printf("Started profiling NORMAL.\n");
        profiler_start();

        for (int i = 0; i < 100000; i++)
            find_record_linear(records, 69);
        
        profiler_stop();
        profiler_print();


        printf("Started profiling STATIC PREDICATE.\n");
        profiler_start();

        for (int i = 0; i < 100000; i++)
            find_record_linear_predicate(records, [](Record* r) { return r->id == 69; });

        profiler_stop();
        profiler_print();


        printf("Started profiling FUNCTOR.\n");
        profiler_start();

        int search_id = 69;
        
        for (int i = 0; i < 100000; i++)
            find_record_linear_stdfunction(records, [=](Record* r) { return r->id == search_id; });

        profiler_stop();
        profiler_print();
    }

    for (auto& record : records)
    {
        destroy_record(&record);
    }
}
