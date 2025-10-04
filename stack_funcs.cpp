#include "stack_funcs.h"

#include <stdio.h>
#include <string.h>

#include "stack_alloc.h"
#include "stack_protection.h"
#include "utils.h"


StackErr_t StackPush(Stack_t* stack, const void* element) {
    STACK_VERIFY(stack);
    
    if (stack->meta.size + 1 == stack->meta.capacity) {
        if (StackUpdate(stack) == STACK_OVERFLOW) 
            return StackDump(stack, STACK_OVERFLOW);
    }

    size_t offset = stack->meta.size * stack->meta.element_size;
    void* dest = (char*)stack->data + offset;
    memcpy(dest, element, stack->meta.element_size);

    stack->meta.size++;
    stack->meta.last_modified = time(NULL);

    stack->meta.data_hash = calculateDataHash(stack);

    STACK_VERIFY(stack);
    return STACK_OK;
}

StackErr_t StackPop(Stack_t* stack, void* element) {
    STACK_VERIFY(stack);

    if (stack->meta.size == 0) {
        return StackDump(stack, STACK_UNDERFLOW);
    }
    
    size_t offset = (stack->meta.size - 1) * stack->meta.element_size;
    void* src = (char*)stack->data + offset;
    
    if (element != NULL) {
        memcpy(element, src, stack->meta.element_size);
    }
    
    fillPoison(src, stack->meta.element_size);
    
    stack->meta.size--;
    stack->meta.last_modified = time(NULL);
    
    stack->meta.data_hash = calculateDataHash(stack);
    
    STACK_VERIFY(stack);
    return STACK_OK;
}
