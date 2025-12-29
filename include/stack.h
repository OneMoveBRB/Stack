#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#ifdef DEBUG
#include <time.h>

#define INIT VarInfo{__FILE__, __func__, __LINE__}
#define STACK_DUMP(stack, error)                                    \
    StackDump(stack, error)

#define CHECK_STACK_PTR(stack, ret_val)                             \
    if (stack == NULL) {                                            \
        fprintf(                                                    \
            stderr,                                                 \
            "Error: Stack pointer[%p] is NULL from %s at %s:%d\n",  \
            &stack, __func__, __FILE__, __LINE__);                  \
        return ret_val;                                             \
    }

#define HARD_STACK_VERIFY(stack, ret_val)                           \
    CHECK_STACK_PTR(stack, ret_val)                                 \
    stack->meta.debugMemory = INIT;                                 \
    if (StackVerify(stack) != STACK_OK) {                           \
        return STACK_DUMP(stack, StackVerify(stack));               \
    }

#define SOFT_STACK_VERIFY(stack, ret_val)                           \
    CHECK_STACK_PTR(stack, ret_val)                                 \
    stack->meta.debugMemory = INIT;                                 \
    if (StackVerify(stack) != STACK_OK) {                           \
        STACK_DUMP(stack, StackVerify(stack));                      \
    }

const unsigned int CANARY_VALUE = 0xDEADBEEF;
const unsigned int POISON_VALUE = 0xBADBAD;
const unsigned int MAGIC_VALUE  = 0xD0D0CACA;
const unsigned int HASH_SEED    = 0x811C9DC5;

typedef struct VarInfo{
    const char* file;
    const char* func;
    int line;
} VarInfo;

#else
#define RELEASE
#define STACK_DUMP(stack, error)          (error)
#define CHECK_STACK_PTR(stack, ret_val)   ((void)0)
#define HARD_STACK_VERIFY(stack, ret_val) ((void)0)
#define SOFT_STACK_VERIFY(stack, ret_val) ((void)0)

#endif /* DEBUG */

#define FREE(ptr) free(ptr); ptr = NULL;

typedef struct StackMeta{
    size_t capacity;
    size_t size;
    size_t element_size;
#ifdef DEBUG
    size_t data_hash;
    size_t magic_number;
    const char* stack_name;
    time_t created_time;
    time_t last_modified;
    VarInfo debugMemory;
#endif /* DEBUG */
} StackMeta;

typedef struct Stack_t{
#ifdef DEBUG
    unsigned int LeftBorder;
#endif /* DEBUG */

    void* data;
    StackMeta meta;

#ifdef DEBUG
    unsigned int RightBorder;
#endif /* DEBUG */
} Stack_t;

typedef enum StackErr_t{
    STACK_OK                    = 0,
    STACK_UNDERFLOW             = 1,
    STACK_OVERFLOW              = 2,
    STACK_CANARY_CORRUPTED      = 3,
    STACK_DATA_CORRUPTED        = 4,
    STACK_NULL_PTR              = 5,
    STACK_INVALID_SIZE          = 6,
    STACK_INVALID_CAPACITY      = 7,
    STACK_MEMORY_ERROR          = 8,
    STACK_USE_AFTER_FREE        = 9,
    STACK_DEST_CALLOC_FAILED    = 10
} StackErr_t;

Stack_t* StackInit(size_t capacity, size_t element_size, const char* name);
StackErr_t StackDestroy(Stack_t** stack_ptr);

StackErr_t StackPush(Stack_t* stack, const void* element);
void* StackPop(Stack_t* stack);

void* StackTop(Stack_t* stack);

size_t StackSize(Stack_t* stack);
int StackEmpty(Stack_t* stack);

#endif /* STACK_H */
