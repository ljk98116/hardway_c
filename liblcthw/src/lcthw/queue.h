#ifndef lcthw_Queue_h
#define lcthw_Queue_h

#include <lcthw/list.h>

typedef struct {
    ListNode *head;
    ListNode *tail;
    size_t sz;
}Queue;

Queue *Queue_create(){
    Queue *ret = (Queue *)calloc(1, sizeof(Queue));
    return ret;
}

void Queue_destroy(Queue *queue){
    for(ListNode *cur = queue->head; cur != NULL;){
        ListNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(queue);
}

void Queue_send(Queue *queue, void *value){
    ListNode *node = (ListNode*)calloc(1, sizeof(ListNode));
    node->value = value;
    if(queue->head == NULL){
        queue->head = node;
        queue->tail = node;
    }
    else{
        queue->tail->next = node;
        node->prev = queue->tail;
        queue->tail = node;
    }
    ++queue->sz;
}

void *Queue_peek(Queue *queue){
    return queue->head->value;
}

void *Queue_recv(Queue *queue){
    void *ret = queue->head->value;
    ListNode *tmp = queue->head;
    queue->head = queue->head->next;
    tmp->next = NULL;
    --queue->sz;
    free(tmp);
    return ret;
}

size_t Queue_count(Queue *queue){
    return queue->sz;
}

#define QUEUE_FOREACH(queue, cur) \
    for(ListNode *cur=queue->head;cur != queue->tail;cur = cur->next)
#endif