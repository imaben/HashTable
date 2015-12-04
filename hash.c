/**
 * Author : maben
 * Date   : 2014-12-23
 */

#include "stdio.h"
#include "hash.h"
#include "assert.h"

#define HASH_SIZE 100

void hash_init(HashTable** hash)
{
    (*hash) = (HashTable*)malloc(sizeof(HashTable));
    if ((*hash) == NULL) {
        printf("Hash table initialization failure!\n");
        exit(0);
    }
    (*hash)->buckets = (Bucket*)malloc(sizeof(Bucket) * HASH_SIZE);
    memset((*hash)->buckets, 0, sizeof(Bucket) * HASH_SIZE);
}

unsigned int get_hash_index(char* str)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    while (*str) {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
    return (hash & 0x7FFFFFFF) % HASH_SIZE;
}


int hash_insert(HashTable* hash, char* key, char* value)
{
    assert(hash != NULL);
    assert(key != NULL);
    int index = get_hash_index(key);
    Bucket* bucket;
    bucket = &hash->buckets[index];
    while (NULL != (bucket->next)) {
        bucket = bucket->next;
    }
    int key_len = strlen(key);
    int value_len = strlen(value);
    bucket->key = (char*)malloc(sizeof(char) * (key_len + 1));
    bucket->value = (char*)malloc(sizeof(char) * (value_len + 1));
    strcpy(bucket->key, key);
    bucket->key[key_len] = '\0';
    strcpy(bucket->value, value);
    bucket->value[value_len] = '\0';
    bucket->next = (Bucket*)malloc(sizeof(Bucket));
    memset(bucket->next, 0, sizeof(Bucket));
    bucket->next->next = NULL;
    return 0;
}

int hash_find(HashTable* hash, char* key, Bucket** result)
{
    assert(hash != NULL);
    assert(key != NULL);
    int index = get_hash_index(key);
    Bucket* bucket = &hash->buckets[index];
    while (bucket) {
        if (bucket->key && strcmp(key, bucket->key) == 0) {
            (*result) = bucket;
            return 1;
        }
        bucket = bucket->next;
    }
    return 0;
}

int hash_remove(HashTable* hash, char* key)
{
    assert(hash != NULL);
    assert(key != NULL);
    int index = get_hash_index(key);
    Bucket* bucket = &hash->buckets[index];
    int is_sub = 0;
    Bucket* prev = NULL;
    while (bucket) {
        if (strcmp(key, bucket->key) != 0)
            continue;
        if (bucket->key)
            free(bucket->key);
        if (bucket->value)
            free(bucket->value);
        if (is_sub) {
            prev->next = bucket->next;
            free(bucket);
            return 1;
        } else {
            if (bucket->next)
                memcpy(bucket, bucket->next, sizeof(Bucket));
            return 1;
        }
        prev = bucket;
        bucket = bucket->next;
        is_sub++;
    }
    return 0;
}

void hash_free(HashTable* hash)
{
    int i = 0;
    Bucket* bucket;
    for (i = HASH_SIZE - 1; i > -1; i--) {
        bucket = &hash->buckets[i];
        if (!bucket)
            continue;
        int is_sub = 0;
        do {
            if (bucket->key)
                free(bucket->key);
            if (bucket->value)
                free(bucket->value);
            if (is_sub) {
                Bucket* tmp = bucket;
                bucket = bucket->next;
                free(tmp);
            } else {
                bucket = bucket->next;
            }
            is_sub++;
        } while(bucket);
    }
    free(hash->buckets);
    free(hash);
    hash = NULL;
}
