#include <stdio.h>
#include<pthread.h>
#include "ref_counting/automatic_ref_count.h"
#include "ref_counting/dictionary.h"

int main(void)
{
    int *element = allocate(sizeof(int));
    retain(element);
    int *element1 = allocate(sizeof(int));
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    insert_value(NULL, element, mutex);
    insert_value(NULL, element1, mutex);
    printf("Element pointer is %p \n", element);
}
