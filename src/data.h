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

inline void report_token_error(const char* type, const char* token)
{
    report_error("Unexpected token while parsing a %s: %s", type, token);
}

Gender deserialize_gender(const char* gender_token)
{
    if (strcmp(gender_token, "M") == 0)
    {
        return MALE;
    }
    else if (strcmp(gender_token, "F") == 0)
    {
        return FEMALE;
    }
    else
    {
        report_token_error("Gender", gender_token);        
        return MALE;
    }
}

void serialize_gender(FILE* file, Gender gender)
{
    if (gender == MALE)
    {
        fputc('M', file); 
    }
    else if (gender == FEMALE)
    {
        fputc('F', file);
    }
    else
    {
        report_error("The enum Gender does not contain a member \"%i\"", (int)gender);         
    }
}

void serialize_ip_address(FILE* file, Ip_Address ip)
{
    u32 temp = ip;
    for (int i = 0; i < 4; i++)
    {
        u32 t = temp >> (3 - i) * 8;
        temp -= t << (3 - i) * 8;
        fprintf(file, "%u", t);
        if (i < 3)
        {
            fputc('.', file);
        }
    }
    fseek(file, -1, SEEK_CUR);
}

Ip_Address deserialize_ip_address(const char* ip_token)
{
    u32 ip = 0;
    for (int i = 0; i < 4; i++)
    {
        u32 t;
        sscanf(ip_token, "%u", &t);
        ip <<= 8;        
        ip += t;
        ip_token = strchr(ip_token, '.') + 1;
    }
    return ip;
}

Record deserialize_record(char* buffer)
{
    Record record;
    char* token = strtok(buffer, ",");

    sscanf(token, "%u", &record.id);
    token = strtok(NULL, ",");
    record.first_name = string_copy_with_malloc(token);
    token = strtok(NULL, ",");
    record.last_name = string_copy_with_malloc(token);
    token = strtok(NULL, ",");
    record.email = string_copy_with_malloc(token);
    token = strtok(NULL, ",");
    record.gender = deserialize_gender(token);
    token = strtok(NULL, ",");
    record.ip_address = deserialize_ip_address(token);

    return record;
}

void serialize_record(FILE* file, Record record)
{
    fprintf(file, "%u", record.id);
    fputc(',', file);
    fputs(record.first_name, file); 
    fputc(',', file);
    fputs(record.last_name, file); 
    fputc(',', file);
    fputs(record.email, file);
    fputc(',', file);
    serialize_gender(file, record.gender);
    fputc(',', file);
    serialize_ip_address(file, record.ip_address);
    fprintf(file, "\n");
}

inline void destroy_record(Record record)
{
    free((void*)record.first_name);
    free((void*)record.last_name);
    free((void*)record.email);
}

std::vector<Record> read_records_from_csv(const char* fname)
{
    FILE* file = fopen(fname, "r");
    if (!file)
    {
        report_error("Failed to open file %s", fname);
    }
    char buffer[1024];
    fgets(buffer, 1024, file);
    
    std::vector<Record> result;
    
    while (fgets(buffer, 1024, file) != 0)
    {
        result.push_back(deserialize_record(buffer));
    }

    return std::move(result);
}

void destroy_records(const std::vector<Record>& records)
{
    for (auto& record : records)
    {
        destroy_record(record);
    }
}