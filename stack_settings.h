#ifndef STACK_SETTINGS_H
#define STACK_SETTINGS_H

#include <stddef.h>
#include <time.h>

#define CANARY_VALUE 0xDEADBEEF
#define POISON_VALUE 0xBADBAD
#define MAGIC_VALUE  0xD0D0CACA
#define HASH_SEED    0x811C9DC5

#define INIT VarInfo{__FILE__, __func__, __LINE__}
#define STACK_VERIFY(stack)                                        \
    if (stack == NULL) {                                           \
        fprintf(                                                   \
            stderr,                                                \
            "Error: Stack pointer[%p] is NULL from %s at %s:%d\n", \
            &stack, __func__, __FILE__, __LINE__);                 \
        return STACK_NULL_PTR;                                     \
    }                                                              \
    stack->meta.debugMemory = INIT;                                \
    if (StackVerify(stack) != STACK_OK) {                          \
        return StackDump(stack, StackVerify(stack));               \
    }


struct VarInfo{
    const char* file;
    const char* func;
    int line;
};

struct StackMeta{
    size_t capacity;
    size_t size;
    size_t element_size;
    size_t data_hash;
    size_t magic_number;
    const char* stack_name;
    time_t created_time;
    time_t last_modified;
    VarInfo debugMemory;
};

struct Stack_t{
    unsigned int LeftBorder;

    void* data;
    StackMeta meta;

    unsigned int RightBorder;
};

enum StackErr_t{
    STACK_OK = 0,
    STACK_UNDERFLOW,
    STACK_OVERFLOW,
    STACK_CANARY_CORRUPTED,
    STACK_DATA_CORRUPTED,
    STACK_NULL_PTR,
    STACK_INVALID_SIZE,
    STACK_INVALID_CAPACITY,
    STACK_MEMORY_ERROR,
    STACK_USE_AFTER_FREE
};


#endif
