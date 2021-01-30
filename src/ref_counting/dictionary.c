#include <stdlib.h>
#include <pthread.h>
#include "dictionary.h"

// use this for pretty printed of the dictionary for debug mode
#include <stdio.h>

#define DICTIONARY_SIZE 100

typedef struct Entry {
    unsigned long key;
    pthread_mutex_t value;
    struct Entry *next;
} entry_t;

typedef struct Dictionary {
    entry_t **entries;
} dictionary_t;

static uint8_t calculate_hash(const void *key);
static void update_value_for_key(entry_t *entry, const pthread_mutex_t value);
static inline entry_t *create_entry(void *key, const pthread_mutex_t value);
static void release_entry(entry_t *entry);

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

void insert_value(dictionary_t *dictionary, void *key, const pthread_mutex_t value)
{
    uint8_t hash = calculate_hash(key);
    entry_t *entry = dictionary->entries[hash];

    // if there is not a value at the specific key, insert the value -> key and then return
    if (entry == NULL)
    {
        dictionary->entries[hash] = create_entry(key, value);
        return;
    } 

    // keep a previous copy of the element, so if there is a 
    entry_t *previous_entry;

    while (entry != NULL)
    {
        // if there is a duplicated key, we just need to update the content of the entry for that particular key
        if (entry->key == (unsigned long) key)
        {
            update_value_for_key(entry, value);
            return;
        }

        previous_entry = entry;
        entry = entry->next;
    }

    // a value is found at that particular key, so we have a collision. Now we need to  interate through the linked list and insert the value at the end of it
    entry_t *new_entry = create_entry(key, value);
    previous_entry->next = new_entry;
}

pthread_mutex_t *get_value(dictionary_t *dictionary, void *key)
{
   for (size_t i = 0; i < DICTIONARY_SIZE; i++)
    {
        if (dictionary->entries[i] != NULL)
        {
            
            entry_t *entry = dictionary->entries[i];
            while (entry != NULL)
            {
                if (entry->key == (unsigned long) key)
                {
                    return &(entry->value);
                }
                entry = entry-> next;
            }
        }
    }  

    return NULL;
}

void delete_value(dictionary_t *dictionary, void *key)
{
    uint8_t hash = calculate_hash(key);
    entry_t *entry = dictionary->entries[hash];

    // there is no entry to delete, then return
    if (entry == NULL)
    {
        return;
    }
    
    // first element in the linked list
    if (entry->key == (unsigned long) key)
    {
        dictionary->entries[hash] = entry->next;
        release_entry(entry);
        return;
    }

    entry_t *previous_element;
    // if the entry is not the first element, we need to iterate through the linked list to detect the element
    // this will work even if the entry is the last element in the linked list
    while(entry != NULL)
    {
        if (entry->key == (unsigned long) key)
        {
            previous_element->next = entry->next;
            release_entry(entry);
            return;
        }

        previous_element = entry;
        entry = entry->next;
    }
}

void release_dictionary(dictionary_t *dictionary)
{
    for(size_t i=0; i<DICTIONARY_SIZE; i++)
    {
        entry_t *entry = dictionary->entries[i];
        while (entry != NULL)
        {
            entry_t *current_entry = entry;
            entry = entry->next;
            release_entry(current_entry);
        }
    }

    free(dictionary);
}

static uint8_t calculate_hash(const void *key) 
{
    unsigned long value = (unsigned long) key;
    uint8_t hash = (uint8_t)value / DICTIONARY_SIZE;
    return hash;
}

static void update_value_for_key(entry_t *entry, const pthread_mutex_t value)
{
    pthread_mutex_destroy(&entry->value);
    entry->value = value;
}

static inline entry_t *create_entry(void *key, const pthread_mutex_t value)
{
    entry_t *entry = (entry_t *)malloc(sizeof(entry_t));
    if (entry == NULL) { return NULL; }
    entry->key = (unsigned long) key;
    entry->value = value;
    entry->next = NULL;
    return entry;
}

static void release_entry(entry_t *entry)
{
    pthread_mutex_destroy(&entry->value);
    entry->next = NULL;
    free(entry);
}

// this function is going to be used just for pretty printing
// the format of the print will be [key-value]
void pretty_print(dictionary_t *dictionary)
{
    for (size_t i = 0; i < DICTIONARY_SIZE; i++)
    {
        if (dictionary->entries[i] != NULL)
        {
            printf("Dictionary index is %ld: \n", i);
            entry_t *entry = dictionary->entries[i];
            while (entry != NULL)
            {
                printf("Key is: %ld \n", entry->key);
                printf("Value is: %p \n", &entry->value);
                entry = entry-> next;
            }
            printf("\n");
        }
    }
}