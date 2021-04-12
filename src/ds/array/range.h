#pragma once

struct Index_Range
{
    ssize_t start; 
    ssize_t end;

    size_t length();
};

size_t Index_Range::length() { return (size_t)(end - start) + 1; }