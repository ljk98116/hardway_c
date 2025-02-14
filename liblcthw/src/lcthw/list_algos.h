#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

typedef int (*List_compare)(const void *a, const void *b, const size_t sz_a, const size_t sz_b);

int List_bubble_sort(List *list, List_compare cmp);

/* 归并排序改进点：
1.适合的长度终止递归，对小数组使用插入排序
2.避免不必要的merge操作，判断左边最后一个和右边第一个的大小
3.
*/
List *List_merge_sort(List *list, List_compare cmp);

/*原地自底向上归并排序*/
List *List_merge_sort_bottom(List *list, List_compare cmp);
List *List_insert_sort(List *list, List_compare cmp);
List *List_insert_sorted(List *list, List_compare cmp, const void *value, const size_t sz);

#endif