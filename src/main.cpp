#include <stdio.h>
#include <stdlib.h>
#include <mydefines.h>
#include <direct.h>
#include <inttypes.h>
#include "data.h"
#include "string_util.h"

int main()
{
    _chdir("assets");
    auto records = read_records_from_csv("data.csv");
    for (const auto& record : records)
    {
        serialize_record(stdout, record);
        destroy_record(record);
    }
}
