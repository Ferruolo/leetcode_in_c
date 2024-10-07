//
//  main.c
//  LeetCode_in_C
//
//  Created by Andrew Ferruolo on 10/7/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

typedef struct {
    int key;
    int value;
    u_int8_t set;
} DataItem;

typedef struct {
    DataItem * data;
    int size;
    int reserved;
} HashTable;


unsigned int hash(int key, int size) {
    // Convert to unsigned 32-bit integer to prevent overflow
    // Maybe use a more efficient hash function
    uint32_t k = (uint32_t)key;
    k ^= k >> 16;
    k *= 0x85ebca6b;
    k ^= k >> 13;
    k *= 0xc2b2ae35;
    k ^= k >> 16;
    return (unsigned int)(k % size);
}

HashTable * create_hash_table(int reserved) {
    HashTable * table = (HashTable *)malloc(sizeof(HashTable));
    table->size = 0;
    table->reserved = reserved;
    table->data = (DataItem *)calloc(reserved, sizeof(DataItem));
    // Possible not necessary
    for (int i = 0; i < reserved; ++i) {
        DataItem * item = table->data + i;
        item->key = 0; // Dislike, need to for 0s;
        item->value = 0;
        item->set = 0;
    }
    return table;

}

void freeHashTable(HashTable * table) {
    free(table->data);
    free(table);
}

int * get_item (HashTable * t, int key) {
    int hash_i = hash(key, t->reserved);
    DataItem * selected = t->data + hash_i;
    DataItem * endpoint = t->data + t->reserved;
    while (selected != endpoint && selected->set && selected->key != key) {
        ++selected;
        ++hash_i;
        
        if (selected == endpoint) {
            selected = t->data; // Loop Around to begining
            hash_i = 0;
        }
    }

    if (selected->set) {
        // Return pointer to relavent value
        return &(selected->value);
    } else {
        // Return Nullptr
        return 0;
    }
}

void set_item(HashTable * t, int key, int value) {
    int hash_i = hash(key, t->reserved);
    
    DataItem * selected = t->data + hash_i;
    DataItem * endpoint = t->data + t->reserved;

    while (selected != endpoint && selected->set && selected->key != key) {
        ++selected;
        ++hash_i;
        if (selected == endpoint) {
            selected = t->data; // Loop Around to begining
            hash_i = 0;
        }
    }

    if (selected == endpoint) {
        return;
    }
    selected->key = key;

    selected->value = value;
    selected->set = 1;
    ++(t->size);
}


int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    HashTable * map = create_hash_table(numsSize);
    for (int i = 0; i < numsSize; ++i) {
        int * other_key = get_item(map, nums[i]);

        if (other_key) {
            *returnSize = 2;
            int * result = (int *)malloc(sizeof(int) * 2);
            result[0] = i;
            result[1] = *other_key;
            return result;
        } else {
            set_item(map, target - nums[i], i);
        }
    }
    freeHashTable(map);
    return 0;
}

