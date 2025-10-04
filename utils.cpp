#include "utils.h"

#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 10


struct error2string {
    StackErr_t err;
    const char* str;
};

static error2string TableErr[TABLE_SIZE] = {
    { STACK_OK,                 "STACK_OK"               },
    { STACK_UNDERFLOW,          "STACK_UNDERFLOW"        },
    { STACK_OVERFLOW,           "STACK_OVERFLOW"         },
    { STACK_CANARY_CORRUPTED,   "STACK_CANARY_CORRUPTED" },
    { STACK_DATA_CORRUPTED,     "STACK_DATA_CORRUPTED"   },
    { STACK_NULL_PTR,           "STACK_NULL_PTR"         },
    { STACK_INVALID_SIZE,       "STACK_INVALID_SIZE"     },
    { STACK_INVALID_CAPACITY,   "STACK_INVALID_CAPACITY" },
    { STACK_MEMORY_ERROR,       "STACK_MEMORY_ERROR"     },
    { STACK_USE_AFTER_FREE,     "STACK_USE_AFTER_FREE"   },
};

static size_t fnv1aHash(const void* data, size_t len, size_t seed) {
    const unsigned char* bytes = (const unsigned char*)data;
    size_t hash = seed;
    
    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= 0x01000193;
    }
    
    return hash;
}

size_t calculateDataHash(const Stack_t* stack) {
    if (stack->data == NULL || stack->meta.size == 0)  return 0;
    return fnv1aHash(stack->data, 
                      stack->meta.size * stack->meta.element_size, 
                      HASH_SEED);
}

void fillPoison(void* data, size_t size) {
    memset(data, 0xBAD, size);
}

const char* getErrorMessage(StackErr_t error) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (TableErr[i].err == error) 
            return TableErr[i].str;
    }
    return "NAN";
}
