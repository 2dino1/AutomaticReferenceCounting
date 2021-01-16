#include <stdio.h>
#include "ref_counting/automatic_ref_count.h"

int main(void)
{
    int *element = allocate(sizeof(int));
    retain(element);
    printf("Element pointer is %p \n", element);
}
