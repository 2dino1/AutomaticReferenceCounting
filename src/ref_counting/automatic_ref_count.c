#include <pthread.h>
#include "automatic_ref_count.h"

// instead of using a simple lock, we can use a dictionary to store the lock per each object (or the control block to sync retain/release calls) [void*: pthread_mutex_t]
// init the dictionary when allocate is first called, then each time there is a new object create a new entry and pass it to it :D
// static int *lock = NULL;

void *allocate(size_t bytes_count) 
{
    void *allocated_block = malloc(bytes_count + 1);
    uint8_t *reference_count = (uint8_t *)allocated_block;
    *reference_count = 1;

    // if (lock == NULL)
    // {
    //     lock = (int *) malloc(sizeof(int));
    // }

    return (void*)((char *)allocated_block + 1);
}

void retain(void *address) 
{
    uint8_t *reference_count = (uint8_t *)((char *)(address) - 1);
    // pthread_mutex_lock(&lock);
    reference_count ++;
    // pthread_mutex_unlock(&lock);
}

void release(void *address)
{
    uint8_t *reference_count = (uint8_t *)((char *)(address) - 1);
    // pthread_mutex_lock(&lock);
    reference_count --;
    // pthread_mutex_unlock(&lock);

    if (reference_count == 0)
    {
        free(((char *)(address) - 1));
    }
}