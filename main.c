#include <stdio.h>

#include "include/stack.h"

const int INITIAL_CAPACITY = 8;

int main() {
    Stack_t* stack = StackInit(INITIAL_CAPACITY, sizeof(int), "stack.txt");

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t arr_size = sizeof(arr)/sizeof(arr[0]);

    for (size_t i = 0; i < arr_size; i++) {
        StackPush(stack, arr + i);
    }

    int* data = (int*)stack->data;
    data[2] = 235215321;

    for (size_t i = 0; i < arr_size; i++) {
        printf("%d ", data[i]);
    }

    StackDestroy(&stack);

    return 0;
}