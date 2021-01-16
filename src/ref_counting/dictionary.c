#include <stdlib.h>
#include <pthread.h>
#include "dictionary.h"

#include <stdio.h>

#define DICTIONARY_SIZE 100

typedef struct Entry {
    unsigned long key;
    pthread_mutex_t mutex;
    struct Entry *next;
} entry_t;

typedef struct Dictionary {
    entry_t **entries;
} dictionary_t;

static uint8_t calculate_hash(const void *key);
static inline entry_t *create_entry(void *key, const pthread_mutex_t mutex, entry_t *next_element);

dictionary_t *init_dictionary()
{
    dictionary_t *dictionary = (dictionary_t *)malloc(sizeof(dictionary_t));
    if (dictionary == NULL) { return NULL; }
    dictionary->entries = (entry_t **)malloc(sizeof(entry_t *) * DICTIONARY_SIZE);

    for(size_t i=0; i<DICTIONARY_SIZE; i++)
    {
        dictionary->entries[i] = NULL;
    }

    return dictionary;
}

void insert_value(dictionary_t *dictionary, void *key, const pthread_mutex_t mutex)
{
    uint8_t hash = calculate_hash(key);

    if (dictionary->entries[hash] == NULL)
    {
        dictionary->entries[hash] = create_entry(key, mutex, NULL);
        return;
    } 
}

void release_dictionary(dictionary_t *dictionary)
{

}

static uint8_t calculate_hash(const void *key) 
{
    unsigned long value = (unsigned long) key;
    uint8_t hash = (uint8_t)value / DICTIONARY_SIZE;
    return hash;
}

static inline entry_t *create_entry(void *key, const pthread_mutex_t mutex, entry_t *next_element)
{
    entry_t *entry = (entry_t *)malloc(sizeof(entry_t));
    if (entry == NULL) { return NULL; }
    entry->key = (unsigned long) key;
    entry->mutex = mutex;
    entry->next = next_element;
    return entry;
}