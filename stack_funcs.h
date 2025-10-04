#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H


#include "stack_settings.h"


StackErr_t StackPush(Stack_t* stack, const void* element);
StackErr_t StackPop(Stack_t* stack, void* element);


#endif