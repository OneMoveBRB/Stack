#ifndef STACK_PROTECTION
#define STACK_PROTECTION


#include "stack_settings.h"


StackErr_t StackVerify(Stack_t* stack);
StackErr_t StackDump(Stack_t* stack, StackErr_t error);


#endif