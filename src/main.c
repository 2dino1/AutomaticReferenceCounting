#include <stdio.h>
#include<pthread.h>
#include "ref_counting/automatic_ref_count.h"
#include "ref_counting/dictionary.h"

int main(void)
{
    void *intAddr0 = allocate(sizeof(int));
    retain(intAddr0);
    // release(intAddr0);
    // release(intAddr0);
}
