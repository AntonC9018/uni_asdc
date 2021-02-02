#pragma once
#include <string.h>
#include <stdlib.h>
#include <initializer_list>

// Copies the given string into a fresh malloc-ed buffer
inline char* string_copy_with_malloc(const char* src)
{
    size_t size = strlen(src) + 1;
    char* buff = (char*)malloc(size);
    memcpy(buff, src, size);
    return buff;
}

// Increments the pointer until the first occurence of a character which isn't whitespace
inline char* trim_front(char* str, char ch = ' ')
{
    while (*str == ch)
    {
        str++;
    }
    return str;
}

inline const char* max(std::initializer_list<const char*> args)
{
    const char* result = 0;
    for (auto ptr : args)
    {
        if (result < ptr)
        {
            result = ptr;
        }
    }
    return result;
}