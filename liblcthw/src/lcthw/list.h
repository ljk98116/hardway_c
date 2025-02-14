#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>
/*
双向链表可以访问前驱，但删除需要额外的空间
单向链表删除简单，但无法回退
*/
struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
    size_t sz;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void List_push(List *list, void *value, size_t sz);
void *List_pop(List *list);

void List_unshift(List *list, void *value, size_t sz);
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);
List *List_dup(List *list);
List *List_join(List *l1, List *l2);

//返回以value开头的链表
List *List_split(List *l, void *value, size_t sz);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif