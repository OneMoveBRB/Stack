#ifndef UTILS_H
#define UTILS_H


#include "stack_settings.h"


size_t calculateDataHash(const Stack_t* stack);
void fillPoison(void* data, size_t size);
const char* getErrorMessage(StackErr_t error);


#endif
