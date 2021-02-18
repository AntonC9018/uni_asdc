#include "data.h"


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

void serialize_gender(Gender gender, FILE* file)
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

void serialize_ip_address(Ip_Address ip, FILE* file)
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
    record.first_name = str_copy(token);
    token = strtok(NULL, ",");
    record.last_name = str_copy(token);
    token = strtok(NULL, ",");
    record.email = str_copy(token);
    token = strtok(NULL, ",");
    record.gender = deserialize_gender(token);
    token = strtok(NULL, ",");
    record.ip_address = deserialize_ip_address(token);

    return record;
}

void serialize_record(const Record* record, FILE* file)
{
    fprintf(file, "%u", record->id);
    fputc(',', file);
    fputs(record->first_name.chars, file); 
    fputc(',', file);
    fputs(record->last_name.chars, file); 
    fputc(',', file);
    fputs(record->email.chars, file);
    fputc(',', file);
    serialize_gender(record->gender, file);
    fputc(',', file);
    serialize_ip_address(record->ip_address, file);
    fprintf(file, "\n");
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
    
    fclose(file);

    return result;
}

void destroy_record(Record* record)
{
    str_free(record->first_name);
    str_free(record->last_name);
    str_free(record->email);
}

void destroy_records(std::vector<Record>& records)
{
    for (auto& record : records)
    {
        destroy_record(&record);
    }
}