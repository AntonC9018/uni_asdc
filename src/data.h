#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include <vector>

#include <mydefines.h>
#include "string_util.h"
#include "errors.h"

enum Gender
{
    MALE, FEMALE
};

typedef u32 Ip_Address; 
// {
//     u32 address:0;
//     u8 bytes[4]:0;
// };

struct Record
{
    u32 id;
    const char* first_name;
    const char* last_name;
    const char* email;
    Gender gender;
    Ip_Address ip_address;
};

inline bool compare_record(Record a, Record b)
{
    return a.id == b.id
        && strcmp(a.first_name, b.first_name) == 0
        && strcmp(a.last_name, b.last_name) == 0
        && strcmp(a.email, b.email) == 0
        && a.gender == b.gender
        && a.ip_address == b.ip_address;
}

void destroy_record(Record* record);

Gender deserialize_gender(const char* gender_token);
void serialize_gender(Gender gender, FILE* file = stdout);
void serialize_ip_address(Ip_Address ip, FILE* file = stdout);
Ip_Address deserialize_ip_address(const char* ip_token);
Record deserialize_record(char* buffer);
void serialize_record(const Record* record, FILE* file = stdout);
std::vector<Record> read_records_from_csv(const char* fname);
void destroy_record(Record* record);
void destroy_records(std::vector<Record>& records);