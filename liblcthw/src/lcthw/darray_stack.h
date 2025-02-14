#ifndef lcthw_DArrayStack_h
#define lcthw_DArrayStack_h

#include <stdlib.h>

typedef struct {
    void *value;
    int idx;
} StackEle;

typedef struct {
    int top;
    size_t sz;
    size_t cap;
    StackEle *eles;
} Stack;

Stack *Stack_create(){
    Stack *ret = (Stack*)calloc(1, sizeof(Stack));
    ret->eles = (StackEle*)malloc(2 * sizeof(StackEle));
    ret->top = -1;
    ret->cap = 2;
    ret->sz = 0;
    return ret;
}

void Stack_push(Stack* stack, void *value){
    if(stack->sz + 1 > stack->cap) {
        stack->eles = (StackEle*)realloc(stack->eles, (stack->cap * 2) * sizeof(StackEle));
        stack->cap <<= 1;
    }
    stack->eles[stack->sz].value = value;
    stack->eles[stack->sz].idx = stack->sz;

    ++stack->sz;
    ++stack->top;
}

void *Stack_peek(Stack* stack){
    return stack->top >= 0 ? stack->eles[stack->top].value : NULL; 
}

void *Stack_pop(Stack *stack){
    if(stack->top < 0) return NULL;
    void *ret = stack->eles[stack->top].value;
    --stack->top;
    --stack->sz;
    return ret;
}

size_t Stack_count(Stack *stack){
    return stack->sz;
}

void Stack_destroy(Stack *stack){
    free(stack->eles);
    free(stack);
}

#define STACK_FOREACH(stack, cur) \
    for(StackEle *cur = stack->top >= 0 ? &stack->eles[stack->top] : NULL; cur && cur->idx >= 0;cur = (cur->idx - 1 >= 0 ? &stack->eles[cur->idx - 1] : NULL))
#endif