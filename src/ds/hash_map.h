#pragma once
#include <stdio.h>
#include <functional>

#include "../profiler.h"
#include "../data.h"

#include <mydefines.h>
#include <strlib.h>
#include <algorithm>

namespace DS
{
    struct Hash_Map_Bucket
    {
        str_view_t key;
        Record value;
        Hash_Map_Bucket* next_bucket;
    };

    struct Hash_Map
    {
        #define HASH_MAP_BUCKET_COUNT 199
        Hash_Map_Bucket* buckets[HASH_MAP_BUCKET_COUNT];
    };

    inline Hash_Map hmap_create()
    {
        return {0};
    }

    size_t hmap_hash(Hash_Map* hm, str_view_t key)
    {
        return str_hash(key) % HASH_MAP_BUCKET_COUNT;
    }

    Hash_Map_Bucket* hmap_get_bucket(Hash_Map* hm, str_view_t key)
    {
        size_t index = hmap_hash(hm, key);
        auto bucket  = hm->buckets[index];
        while (bucket != NULL)
        {
            if (str_compare(key, bucket->key) == 0)
            {
                return bucket;
            }
            else
            {
                bucket = bucket->next_bucket;
            }
        }
        return NULL;
    }

    bool hmap_has_key(Hash_Map* hm, str_view_t key)
    {
        return hmap_get_bucket(hm, key) != NULL;
    }

    Record hmap_get(Hash_Map* hm, str_view_t key)
    {
        auto bucket = hmap_get_bucket(hm, key);
        return bucket ? bucket->value : Record{0};
    }

    bool hmap_has_value(Hash_Map* hm, Record value)
    {
        for (size_t i = 0; i < HASH_MAP_BUCKET_COUNT; i++)
        {
            auto bucket = hm->buckets[i];

            while (bucket != NULL)
            {
                if (compare_record(&bucket->value, &value))
                {
                    return true;
                }
                bucket = bucket->next_bucket;
            }
        }
        return false;
    }

    Hash_Map_Bucket* hmap_make_bucket(str_view_t key, Record value)
    {
        auto new_bucket   = (Hash_Map_Bucket*) malloc(sizeof(Hash_Map_Bucket));
        new_bucket->key   = key;
        new_bucket->value = value;
        new_bucket->next_bucket = NULL;
        return new_bucket;
    }

    // Returns false if an element with that key already existed and was replaced with the new value
    // Returns true if the new element has been successfully added
    bool hmap_put(Hash_Map* hm, str_view_t key, Record value)
    {
        size_t index = hmap_hash(hm, key);
        auto bucket  = hm->buckets[index];

        if (bucket == NULL)
        {
            hm->buckets[index] = hmap_make_bucket(key, value);
            return true;
        }

        auto parent_bucket = bucket;

        while (bucket != NULL)
        {
            // Found a duplicate key.
            // In this case, replace the old value.
            if (str_compare(bucket->key, key) == 0)
            {
                bucket->value = value;
                return false;
            }

            parent_bucket = bucket;
            bucket = bucket->next_bucket;
        }

        // The last bucket in chain, no duplicate keys.
        parent_bucket->next_bucket = hmap_make_bucket(key, value);
        return true;
    }

    void hmap_free(Hash_Map* hm)
    {
        for (size_t i = 0; i < HASH_MAP_BUCKET_COUNT; i++)
        {
            auto bucket = hm->buckets[i];

            while (bucket != NULL)
            {
                auto t = bucket->next_bucket;
                free(bucket);
                bucket = t;
            }
        }
    }
}
