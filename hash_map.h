#ifndef __HASH_MAP_H
#define __HASH_MAP_H

#include <stdio.h>
#include <stdbool.h>

#include "utils/memory.h"

#include "list.h"
#include "pair.h"

struct hash_map
{
    struct list** arr;
    size_t capacity;
    size_t size;
    size_t of_size_key;
    size_t of_size_value;

    size_t (*hash)(const void* p);
    int (*comp)(const void* p, const void* q);
};

struct hash_map* hash_map_create(size_t of_size_key, size_t of_size_value, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q))
{
    struct hash_map* ths = (struct hash_map*)malloc_s(sizeof(struct hash_map));

    ths->arr = (struct list**)malloc_s(21 * sizeof(struct list*));
    for (int i = 0; i < 21; i++)
        ths->arr[i] = list_create(sizeof(struct pair*));

    ths->capacity = 21;
    ths->size = 0;
    ths->of_size_key = of_size_key;
    ths->of_size_value = of_size_value;

    ths->hash = hash;
    ths->comp = comp;

    return ths;
}

void hash_map_delete(struct hash_map* ths)
{
    for (size_t i = 0; i < ths->capacity; i++)
    {
        for (size_t j = 0; j < ths->arr[i]->size; j++)
            pair_delete(((struct pair**)ths->arr[i]->arr)[j]);
        list_delete(ths->arr[i]);
    }
    free_s(ths->arr);
    free_s(ths);
}

void hash_map_push(struct hash_map* ths, void* key, void* value)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            pair_set_second(pair, value);
            return;
        }
    }

    struct pair* pair = pair_make(key, ths->of_size_key, value, ths->of_size_value);
    list_push(ths->arr[index], &pair);

    ths->size++;
}

void hash_map_pop(struct hash_map* ths, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
            list_remove(ths->arr[index], i);
    }

    ths->size--;
}

bool hash_map_get(struct hash_map* ths, void* dest, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];

        if (ths->comp(key, pair->first) == 0)
        {
            memcpy(dest, pair->second, ths->of_size_value);
            return true;
        }
    }

    return false;
}

#endif