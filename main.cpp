#include <stdio.h>

#include "stack.h"

int main() {
    Stack_t* IntegerStack = NULL;
    if (StackInit(&IntegerStack, sizeof(int), 5, "IntegerStack") != STACK_OK) {
        fprintf(stderr, "No allocation of IntegerStack\n");
        return 1;
    }

    // StackPush(&IntegerStack, (void*)5);
    int a = 5;
    StackPush(IntegerStack, &a);
    StackPush(IntegerStack, &a);
    StackPush(IntegerStack, &a);
    StackPush(IntegerStack, &a);

    int b = 0;
    StackPop(IntegerStack, &b);

    StackPush(IntegerStack, &a);
    StackPush(IntegerStack, &a);
    StackPush(IntegerStack, &a);

    for (size_t i=0; i<IntegerStack->meta.size; i++)
        printf("%d\n", *((char*)IntegerStack->data + i*sizeof(int)));

    printf("DELETED: %d\n", b);

    

    StackDestroy(IntegerStack);
    return 0;
}
