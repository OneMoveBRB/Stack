#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H


#include "stack_settings.h"


StackErr_t StackInit(Stack_t** stack, size_t element_size, size_t capacity, const char* name);
StackErr_t StackUpdate(Stack_t* stack);
StackErr_t StackDestroy(Stack_t* stack);


#endif