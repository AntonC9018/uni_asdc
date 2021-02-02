#include <stdio.h>
#include <stdlib.h>
#include <mydefines.h>
#include <inttypes.h>
#include "data.h"
#include "string_util.h"
#include "errors.h"

void serialize_deserialize()
{
    Record record;
    record.id = 1;
    record.first_name = "Hello";
    record.last_name = "World";
    record.email = "John";
    record.gender = MALE;
    record.ip_address = deserialize_ip_address("192.168.0.1");

    // Sadly, I cannot write to an in-memory buffer
    serialize_record(stdout, record);
    
    Record record2 = deserialize_record(string_copy_with_malloc("1,Hello,World,John,M,192.168.0.1"));
    if (!compare_record(record, record2))
    {
        report_error("Record one and record two were not equal.");
    }
}

int main()
{
    serialize_deserialize();
}