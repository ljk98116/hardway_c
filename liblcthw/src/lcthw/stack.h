#ifndef lcthw_Stack_h
#define lcthw_Stack_h

#include <lcthw/list.h>

typedef struct {
    ListNode *top;
    size_t sz;
} Stack;

Stack *Stack_create(){
    Stack *ret = (Stack*)calloc(1, sizeof(Stack));
    return ret;
}

void Stack_push(Stack *stack, const void *value){
    ++stack->sz;
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = value;

    if(stack->top){
        stack->top->prev = node;
        node->next = stack->top;
        stack->top = node;
    }
    else{
        stack->top = node;
    }
}

void *Stack_peek(Stack *stack){
    return stack->top->value;
}

void *Stack_pop(Stack *stack){
    void *ret = stack->top->value;
    ListNode *tmp = stack->top;
    stack->top = tmp->next;
    --stack->sz;
    free(tmp);
    return ret;
}

size_t Stack_count(Stack *stack){
    return stack->sz;
}

void Stack_destroy(Stack *stack){
    for(ListNode *cur=stack->top;cur != NULL;){
        ListNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(stack);
}

#define STACK_FOREACH(stack, cur) \
    for(ListNode *cur=stack->top;cur!=NULL;cur = cur->next)

#endif