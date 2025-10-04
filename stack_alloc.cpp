#include "stack_alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stack_protection.h"
#include "utils.h"

#define CAPACITY_UPPER_LIMIT 1000
#define CAPACITY_LOWER_LIMIT 1


StackErr_t StackInit(Stack_t** stack_ptr, size_t element_size, size_t capacity, const char* name) {
    if (!(CAPACITY_LOWER_LIMIT <= capacity && capacity <= CAPACITY_UPPER_LIMIT)) return STACK_INVALID_CAPACITY;
    if (element_size == 0) return STACK_INVALID_SIZE;

    *stack_ptr = (Stack_t*)calloc(1, sizeof(Stack_t));

    Stack_t* stack = *stack_ptr;

    stack->meta.capacity = capacity;
    stack->meta.size = 0;
    stack->meta.element_size = element_size;
    stack->meta.magic_number = MAGIC_VALUE;
    stack->meta.stack_name = (name != NULL) ? name : "unnamed";
    stack->meta.created_time = time(NULL);
    stack->meta.last_modified = time(NULL);
    stack->meta.debugMemory = INIT;

    stack->data = calloc(stack->meta.capacity, element_size);
    if (stack->data == NULL) return STACK_NULL_PTR;

    stack->LeftBorder = CANARY_VALUE;
    stack->RightBorder = CANARY_VALUE;

    fillPoison(stack->data, stack->meta.capacity * stack->meta.element_size);

    stack->meta.data_hash = calculateDataHash(stack);

    return STACK_OK;
}


StackErr_t StackUpdate(Stack_t* stack){
    STACK_VERIFY(stack);
    
    const size_t exp_multiplier = 2;

    stack->meta.capacity *= exp_multiplier;
    stack->data = realloc(stack->data, stack->meta.capacity * stack->meta.element_size);
    if (stack->data == NULL) return STACK_OVERFLOW;

    stack->meta.last_modified = time(NULL);

    STACK_VERIFY(stack);
    return STACK_OK;
}

StackErr_t StackDestroy(Stack_t* stack) {
    STACK_VERIFY(stack);

    if (stack->data != NULL) {
        size_t total_size = stack->meta.capacity * stack->meta.element_size;
        fillPoison(stack->data, total_size);
        free(stack->data);
        stack->data = NULL;

        stack->meta.capacity = 0;
        stack->meta.size = 0;
        stack->meta.element_size = 0;
        stack->meta.stack_name = "unnamed";
        stack->meta.last_modified = time(NULL);
    }

    free(stack);

    return STACK_OK;
}
