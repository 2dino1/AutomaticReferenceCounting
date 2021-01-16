#include <stdlib.h>

#ifndef AUTOMATIC_REF_COUNT
#define AUTOMATIC_REF_COUNT

// define here types if needed

#endif

void *allocate(size_t bytes_count);
void retain(void *address);
void release(void *address);