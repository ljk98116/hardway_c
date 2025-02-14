#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}


char *test_destroy()
{
    List_clear_destroy(list);
    return NULL;
}


char *test_push_pop()
{
    List_push(list, test1, strlen(test1) + 1);
    mu_assert(List_last(list) && test1 && strcmp(List_last(list), test1) == 0, "Wrong last value.");

    List_push(list, test2, strlen(test2) + 1);
    mu_assert(List_last(list) && test2 && strcmp(List_last(list), test2) == 0, "Wrong last value");

    List_push(list, test3, strlen(test3) + 1);
    mu_assert(List_last(list) && test3 && strcmp(List_last(list), test3) == 0, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val && test3 && strcmp(val, test3) == 0, "Wrong value on pop.");
    free(val);

    val = List_pop(list);
    mu_assert(val && test2 && strcmp(val, test2) == 0, "Wrong value on pop.");
    free(val);

    val = List_pop(list);
    mu_assert(val && test1 && strcmp(val, test1) == 0, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");
    free(val);

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1, strlen(test1) + 1);
    mu_assert(List_first(list) && test1 && strcmp(List_first(list), test1) == 0, "Wrong first value.");

    List_unshift(list, test2, strlen(test2) + 1);
    mu_assert(List_first(list) && test2 && strcmp(List_first(list), test2) == 0, "Wrong first value");

    List_unshift(list, test3, strlen(test3) + 1);
    mu_assert(List_first(list) && test3 && strcmp(List_first(list), test3) == 0, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    // we only need to test the middle remove case since push/shift
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val && test2 && strcmp(val, test2) == 0, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) && test3 && strcmp(List_first(list), test3) == 0, "Wrong first after remove.");
    mu_assert(List_last(list) && test1 && strcmp(List_last(list), test1) == 0, "Wrong last after remove.");
    free(val);
    return NULL;
}


char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val && test3 && strcmp(val, test3) == 0, "Wrong value on shift.");
    free(val);

    val = List_shift(list);
    mu_assert(val && test1 && strcmp(val, test1) == 0, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");
    free(val);

    return NULL;
}

char *test_copy(){
    mu_assert(List_count(list) == 0, "Wrong count before copy.");
    List *copyed_list = NULL;
    List_push(list, test1, strlen(test1) + 1);
    List_push(list, test2, strlen(test2) + 1);
    List_push(list, test3, strlen(test3) + 1);
    copyed_list = List_dup(list);

    char *val = List_pop(copyed_list);
    mu_assert(val && test3 && strcmp(val, test3) == 0, "Wrong value on shift.");
    free(val);    
    val = List_pop(copyed_list);
    mu_assert(val && test2 && strcmp(val, test2) == 0, "Wrong value on shift.");
    free(val);
    val = List_pop(copyed_list);
    mu_assert(val && test1 && strcmp(val, test1) == 0, "Wrong value on shift.");
    free(val);
    List_clear_destroy(copyed_list);
    return NULL;
}

char *test_split(){
    list = List_create();
    List_push(list, test1, strlen(test1) + 1);
    List_push(list, test2, strlen(test2) + 1);
    List_push(list, test3, strlen(test3) + 1);
    List *l2 = List_split(list, test2, strlen(test2));
    mu_assert(list->count == 1 && l2->count == 2, "split error at count");
    char *val = List_pop(l2);
    mu_assert(val && test3 && strcmp(val, test3) == 0, "Wrong value on split.");
    free(val);    
    val = List_pop(l2);
    mu_assert(val && test2 && strcmp(val, test2) == 0, "Wrong value on split.");
    free(val);
    val = List_pop(list);
    mu_assert(val && test1 && strcmp(val, test1) == 0, "Wrong value on split.");
    free(val);
    List_clear_destroy(l2);
    List_clear_destroy(list);
    return NULL;
}

char *all_tests() {
    mu_suite_start();
    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_copy);
    mu_run_test(test_destroy);
    mu_run_test(test_split);
    return NULL;
}

RUN_TESTS(all_tests);