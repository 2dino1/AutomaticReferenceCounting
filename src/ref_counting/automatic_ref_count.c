#include <pthread.h>
#include "automatic_ref_count.h"
#include "dictionary.h"

#include <stdio.h>

static dictionary_t *dictionary = NULL;
static void create_entry();

void *allocate(size_t bytes_count) 
{
    void *allocated_block = malloc(bytes_count + 1);
    uint8_t *reference_count = (uint8_t *)allocated_block;
    *reference_count = 1;

    if (dictionary == NULL)
    {
        dictionary = init_dictionary();
    }

    create_entry(allocated_block);
    return (void*)((char *)allocated_block + 1);
}

void retain(void *address) 
{
    uint8_t *reference_count = (uint8_t *)((char *)(address) - 1);
    printf("Magic dictionary %p \n", dictionary);
    pthread_mutex_t *lock = get_value(dictionary, address);
    printf("Magic lock value %p \n", lock);

    pthread_mutex_lock(lock);
    reference_count ++;
    pthread_mutex_unlock(lock);
}

void release(void *address)
{
    uint8_t *reference_count = (uint8_t *)((char *)(address) - 1);
    pthread_mutex_t *lock = get_value(dictionary, address);

    pthread_mutex_lock(lock);
    reference_count --;
    pthread_mutex_unlock(lock);

    if (reference_count == 0)
    {
        delete_value(dictionary, address);
        free(((char *)(address) - 1));
    }
}

static void create_entry(void *allocated_block)
{
    // Copying the value from the stack is not such a good ideea :D
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    insert_value(dictionary, allocated_block, mutex);
}