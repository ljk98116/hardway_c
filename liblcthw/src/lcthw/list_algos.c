#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

inline void ListNode_swap(ListNode *a, ListNode *b)
{
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;

    size_t tmp = a->sz;
    a->sz = b->sz;
    b->sz = tmp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
    int sorted = 1;

    if(List_count(list) <= 1) {
        return 0;  // already sorted
    }

    do {
        sorted = 1;
        LIST_FOREACH(list, first, next, cur) {
            if(cur->next) {
                if(cmp(cur->value, cur->next->value, cur->sz, cur->next->sz) > 0) {
                    ListNode_swap(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while(!sorted);

    return 0;
}

/* 注意List_push此处会复制指针指向的值*/
inline List *List_merge(List *left, List *right, List_compare cmp)
{
    List *result = List_create();
    void *val = NULL;
    while(List_count(left) > 0 || List_count(right) > 0) {
        size_t sz = 0;
        if(List_count(left) > 0 && List_count(right) > 0) {
            if(cmp(List_first(left), List_first(right), left->first ? left->first->sz : 0, right->first ? right->first->sz : 0) <= 0) {
                sz = left->first == NULL ? sz : left->first->sz;
                val = List_shift(left);
            } else {
                sz = right->first == NULL ? sz : right->first->sz;
                val = List_shift(right);
            }
            List_push(result, val, sz);
        } else if(List_count(left) > 0) {
            sz = left->first == NULL ? sz : left->first->sz;
            val = List_shift(left);
            List_push(result, val, sz);
        } else if(List_count(right) > 0) {
            sz = right->first == NULL ? sz : right->first->sz;
            val = List_shift(right);
            List_push(result, val, sz);
        }
        if(val) free(val);
    }
    return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if(List_count(list) <= 1) {
        return list;
    }

    List *left = List_create();
    List *right = List_create();
    int middle = List_count(list) / 2;

    LIST_FOREACH(list, first, next, cur) {
        if(middle > 0) {
            List_push(left, cur->value, cur->sz);
        } else {
            List_push(right, cur->value, cur->sz);
        }
        middle--;
    }

    //List_clear_destroy(list);
    List *sort_left = List_merge_sort(left, cmp);
    List *sort_right = List_merge_sort(right, cmp);

    if(sort_left != left) List_clear_destroy(left);
    if(sort_right != right) List_clear_destroy(right);

    List *ret = List_merge(sort_left, sort_right, cmp);
    List_clear_destroy(sort_left);
    List_clear_destroy(sort_right);
    return ret;
}

List *List_insert_sorted(List *list, List_compare cmp, const void *value, const size_t sz){
    LIST_FOREACH(list, first, next, cur){
        if(value && cmp(cur->value, value, cur->sz, sz) > 0){
            ListNode *node = (ListNode*)malloc(sizeof(ListNode));
            node->value = malloc(sz);
            memcpy(node->value, value, sz);
            node->sz = sz;
            cur->prev->next = node;
            node->prev = cur->prev;
            node->next = cur;
            cur->prev = node;
            ++list->count;
            return list;
        }
    }
    List_push(list, value, sz);
    return list;
}

/* 链表原地merge, 以node1为基准*/
// 1245 367
// 1235 467
// 1234 567
// 4567 123
// 1567 423
// 1467 523
// 14
void List_merge_bottom(ListNode **nodes, int i1, int i2, size_t len1, size_t len2, List_compare cmp, int cnt){
    ListNode **target = (ListNode **)malloc(sizeof(ListNode*) * cnt);
    int c1 = 0, c2 = 0;
    int start = i1;
    while(c1 < len1 || c2< len2){
        if(c1 < len1 && c2 < len2){
            if(cmp(nodes[i1 + c1]->value, nodes[i2 + c2]->value, nodes[i1 + c1]->sz, nodes[i2+c2]->sz) <= 0){
                target[start++] = nodes[i1 + c1];
                ++c1;
            }else{
                target[start++] = nodes[i2 + c2];
                ++c2;                
            }
        }
        else if(c1 < len1){
            target[start++] = nodes[i1 + c1];
            ++c1;            
        }
        else if(c2 < len2){
            target[start++] = nodes[i2 + c2];
            ++c2;            
        }
    }
    for(int i=i1;i<start;++i){
        nodes[i] = target[i];
    }
    free(target);
}

List *List_merge_sort_bottom(List *list, List_compare cmp){
    /* 节点指针数组 */
    ListNode **nodes = (ListNode **)malloc(sizeof(ListNode*) * list->count);
    /* 有效归并单元指针数量 */
    int cnt = 0;
    LIST_FOREACH(list, first, next, cur){
        nodes[cnt++] = cur;
    }
    for(int i=0;i<list->count;++i){
        nodes[i]->prev = NULL;
        nodes[i]->next = NULL;
    }
    int len = 1;
    while(len < list->count){
        int next_cnt = 0;
        for(int i=0;i < cnt; i+= (len << 1) ){
            if(i + len < list->count){
                List_merge_bottom(nodes, i, i+len, len, i + len + len - 1 >= list->count ? (list->count - i - len) : len, cmp, list->count);
            }
            next_cnt += 1;
        }
        len <<= 1;
        cnt = next_cnt;
    }
    /*释放节点指针数组的堆内存*/
    //memset(nodes, NULL, sizeof(ListNode*) * list->count);
    list->first = nodes[0];
    list->last = nodes[list->count - 1];

    for(int i=1;i<list->count;++i){
        nodes[i]->prev = nodes[i-1];
        nodes[i-1]->next = nodes[i];
    }
    free(nodes);
    return list;
}