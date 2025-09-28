#include <stdio.h>
#include <stdlib.h>

#define INIT VarInfo{__func__, __LINE__, __FILE__}

typedef int type;

enum StackErr_t {
    OK,
    NULL_PTR,
    STACK_OVERFLOW,
    NULL_PTR_WARNING
};

struct Stack_t {
    void* arr;
    size_t size;
    size_t capacity;
    const int* LeftBorder;
    const int* RightBorder;
    static const int canary = 249359379;
};


void* MoveArrPtr(void* ptr, size_t offset);
const int* create_const_int(void* ptr, int value);
StackErr_t StackInit(Stack_t* stack, size_t capacity);
StackErr_t StackUpdate(Stack_t* stack);
StackErr_t StackPush(Stack_t* stack, type value);
StackErr_t StackPrint(Stack_t* stack);
StackErr_t StackVerify(Stack_t* stack);
StackErr_t StackDump(Stack_t* stack);
StackErr_t StackFree(Stack_t* stack);


int main() {
    Stack_t stk;
    StackErr_t flag = OK;
    const int first_capacity = 5;

    flag = StackInit(&stk, first_capacity);
    if (flag != OK) fprintf(stderr, "ERROR: %d\n", flag);

    StackPush(&stk, 1000);
    StackPush(&stk, 2000);
    StackPush(&stk, 3000);
    StackPush(&stk, 4000);
    StackPush(&stk, 5000);
    StackPush(&stk, 6000);
    StackPush(&stk, 7000);
    StackPush(&stk, 8000);
    StackPush(&stk, 9000);
    StackPush(&stk, 10000);
    StackPrint(&stk);

    flag = StackFree(&stk);
    if (flag != OK) fprintf(stderr, "ERROR: %d\n", flag);

    return 0;
}


void* MoveArrPtr(void* ptr, size_t offset) {
    return (void*)((char*)ptr + offset*sizeof(type));
}

const int* create_const_int(void* ptr, int value) {
    int* temp = (int*)ptr;
    *temp = value;

    return (const int*)temp;
}

StackErr_t StackInit(Stack_t* stack, size_t capacity) {
    if (stack == NULL) return NULL_PTR;

    stack->size = 0;
    stack->capacity = capacity;
    stack->arr = calloc(capacity, sizeof(type));
    if (stack->arr == NULL) return STACK_OVERFLOW;

    stack->LeftBorder = create_const_int(stack->arr, stack->canary);
    stack->RightBorder = create_const_int(MoveArrPtr(stack->arr, capacity-1), stack->canary);

    stack->arr = MoveArrPtr(stack->arr, 1);

    return OK;
}

StackErr_t StackUpdate(Stack_t* stack){
    printf("Stack size: %d\n", stack->size);
    
    const size_t exp_multiplier = 2;

    *(int*)stack->RightBorder = 0;

    stack->capacity *= exp_multiplier;
    stack->arr = (void*)((char*)stack->arr - 1*sizeof(type));
    stack->arr = realloc(stack->arr, stack->capacity*sizeof(type));
    if (stack->arr == NULL) return STACK_OVERFLOW;

    stack->LeftBorder = (const int*)stack->arr;
    stack->RightBorder = create_const_int(MoveArrPtr(stack->arr, stack->capacity-1), stack->canary);
    
    stack->arr = MoveArrPtr(stack->arr, 1);

    printf("LEFT %d\n", *(const int*)stack->LeftBorder);
    printf("ARR %d\n", *(type*)stack->arr);
    printf("RIGHT %d\n", *(const int*)stack->RightBorder);

    return OK;
    
}

StackErr_t StackPush(Stack_t* stack, type value) {
    if (stack == NULL) return NULL_PTR;

    StackVerify(stack);

    *(type*)MoveArrPtr(stack->arr, stack->size) = value;
    ++stack->size;

    return OK;
}

StackErr_t StackPrint(Stack_t* stack) {
    for (size_t i = 0; i < stack->size; i++) {
        printf("%d\n", *(type*)MoveArrPtr(stack->arr, i));
    }
    return OK;
}

// Проверяет состояние стека: канарейки, память
StackErr_t StackVerify(Stack_t* stack) {
    if (stack == NULL) return NULL_PTR;
    if (stack->arr == NULL) return NULL_PTR_WARNING;

    if (stack->size + 1 == stack->capacity - 2) StackUpdate(stack);

    return OK;
}

// Записывает состояние программы в отладочный файл в данный момент
StackErr_t StackDump(Stack_t* stack) {
    return OK;
}

StackErr_t StackFree(Stack_t* stack) {
    if (stack == NULL) return NULL_PTR;
    if (stack->arr == NULL) return NULL_PTR_WARNING;

    free((void*)stack->LeftBorder);

    stack->arr = NULL;
    stack->LeftBorder = NULL;
    stack->RightBorder = NULL;
    stack->size = 0;
    stack->capacity = 0;

    return OK;
}






// static const int canary = 249359379;


// struct CanaryDefence {
//     void* LeftBorder;
//     void* RightBorder;
//     const int canary = 249359379;
// };
