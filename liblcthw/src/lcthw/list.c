#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create()
{
    return calloc(1, sizeof(List));
}

void List_destroy(List *list)
{
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    if(list->last) free(list->last);
    if(list) free(list);
}


void List_clear(List *list)
{
    LIST_FOREACH(list, first, next, cur) {
        if(cur->value) free(cur->value);
    }
}


void List_clear_destroy(List *list)
{
    //List_clear(list);
    //List_destroy(list);
    LIST_FOREACH(list, first, next, cur){
        if(cur->value) free(cur->value);
        if(cur->prev){
            free(cur->prev);
        }
    }
    if(list && list->last) free(list->last);
    if(list) free(list);
}


void List_push(List *list, void *value, size_t sz)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    //node->value = value;//fucking shit code, what if you free the value, what will happen ?
    if(value){
        node->value = malloc(sz);
        memset(node->value, 0, sz);
        memcpy(node->value, value, sz);
        node->sz = sz;
    }
    
    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *List_pop(List *list)
{
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value, size_t sz)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    //node->value = value;
    void *val = malloc(sz);
    memcpy(val, value, sz);
    node->value = val;
    node->sz = sz;
    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}


void *List_remove(List *list, ListNode *node)
{
    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }
    list->count--;
    result = node->value;
    free(node);
error:
    return result;
}

List *List_dup(List *list){
    List *ret = List_create();
    LIST_FOREACH(list, first, next, cur){
        List_push(ret, cur->value, cur->sz);
    }
    return ret;
}

List *List_join(List *l1, List *l2){
    LIST_FOREACH(l2, first, next, cur){
        if(l1->first == NULL){
            l1->first = cur;
            l1->last = cur;
            cur->prev = NULL;
        }
        else{
            l1->last->next = cur;
            cur->prev = l1->last;
            l1->last = cur;
        }
        ++l1->count;
        --l2->count;
    }
    if(l2->count > 0) printf("error\n");
    free(l2);
    return l1;
}

List *List_split(List *l, void *value, size_t sz){
    int cnt = 0;
    LIST_FOREACH(l, first, next, cur){
        if(memcmp(cur->value, value, sz) == 0){
            List *ret = List_create();
            ret->count = l->count - cnt;
            ret->first = cur;
            ret->last = l->last;
            l->last = cur->prev;
            l->count = cnt;
            return ret;
        }
        ++cnt;
    }
    return NULL;
}