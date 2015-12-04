/**
 * Author : maben
 * Date   : 2014-12-23
 */
#ifndef __HASH_H__
#define __HASH_H__
typedef struct _bucket {
    char* key;
    char* value;
    struct _bucket* next;
}Bucket;

typedef struct _hash {
    Bucket* buckets;
}HashTable;

void hash_init(HashTable** hash);
unsigned int get_hash_index(char* str);
int hash_insert(HashTable* hash, char* key, char* value);
int hash_find(HashTable* hash, char* key, Bucket** bucket);
int hash_remove(HashTable* hash, char* key);
void hash_free(HashTable* hash);
#endif
