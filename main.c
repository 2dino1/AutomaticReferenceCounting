#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// instead of using a simple lock, we can use a dictionary to store the lock per each object (or the control block to sync retain/release calls) [void*: pthread_mutex_t]
static pthread_mutex_t lock;

// allocator
// reference_counter
void *allocate(size_t bytes_count) 
{
    void *allocated_block = malloc(bytes_count + 1);
    uint8_t *reference_count = (uint8_t *)allocated_block;
    *reference_count = 1;

    return (void*)((char *)allocated_block + 1);
}

void create_lock()
{
    pthread_mutex_init(&lock, NULL);
}

void destroy_lock() 
{
    pthread_mutex_destroy(&lock);
}

void retain(void *address) 
{
    uint8_t *reference_count = (uint8_t *)((char *)(address) - 1);
    pthread_mutex_lock(&lock);
    reference_count ++;
    pthread_mutex_unlock(&lock);
}

void release(void *address)
{
    uint8_t *reference_count = (uint8_t *)((char *)(address) - 1);
    pthread_mutex_lock(&lock);
    reference_count --;
    pthread_mutex_unlock(&lock);

    if (reference_count == 0)
    {
        free(((char *)(address) - 1));
    }
}

int main(void)
{
    int *element = allocate(sizeof(int));
    retain(element);
    printf("Element pointer is %p \n", element);
}
