#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5

List *create_words()
{
    int i = 0;
    List *words = List_create();

    for(i = 0; i < NUM_VALUES; i++) {
        List_push(words, values[i], strlen(values[i]) + 1);
    }

    return words;
}

/*链表长度   bubble耗时     merge耗时       merge bottom耗时*/
/*10        0.039ms          0.383ms         0.167ms */
/*100       1.019ms          2.483ms         0.648ms */
/*1000      100.54ms         37.69ms         6.558ms */
/*10000     11052.695ms      567.165ms       147.627ms*/
//可见:短链表处理时,merge sort不如bubble sort，长链表merge sort更优
//链表排序不能使用索引，找到某个值的代价较高，导致归并排序分裂时速度较慢
List *create_random_words_list(){
    int len = 100;
    List *words = List_create();
    static char word[6];
    for(int i=0;i<len;++i){
        int wordlen = 1 + rand() % 5;
        memset(word, 0, sizeof(word));
        for(int j=0;j<wordlen;++j) word[j] = 'a' + rand() % 26;
        //printf("word: %s\n", word);
        List_push(words, word, wordlen + 1);
    }
    return words;
}

int is_sorted(List *words)
{
    LIST_FOREACH(words, first, next, cur) {
        //printf("%s\n", (char*)cur->value);
        if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

int list_compare(const void *a, const void *b, const size_t sz_a, const size_t sz_b){
    return strcmp(a, b);
}

char *test_bubble_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort(words, (List_compare)list_compare);
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

    // should work on an already sorted list
    rc = List_bubble_sort(words, (List_compare)list_compare);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

    List_clear_destroy(words);

    // should work on an empty list
    words = List_create(words);
    rc = List_bubble_sort(words, (List_compare)list_compare);
    mu_assert(rc == 0, "Bubble sort failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");

    List_clear_destroy(words);

    return NULL;
}

char *test_merge_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    List *res = List_merge_sort(words, (List_compare)list_compare);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    List *res2 = List_merge_sort(res, (List_compare)list_compare);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
    List_clear_destroy(res2);
    List_clear_destroy(res);

    List_clear_destroy(words);
    return NULL;
}

char *test_merge_sort_bottom()
{
    List *words = create_words();

    // should work on a list that needs sorting
    List *res = List_merge_sort_bottom(words, (List_compare)list_compare);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    List *res2 = List_merge_sort_bottom(res, (List_compare)list_compare);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
    //List_destroy(res2);
    //List_destroy(res);

    List_clear_destroy(words);
    return NULL;
}

char *test_running_time(){
    int iteration = 1000000;
    clock_t start, end;
    start = clock();
    for(int i=0;i<iteration;++i){
        test_bubble_sort();
    }
    end = clock();
    printf("bubble sort %d iteration used: %f ms\n", iteration, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    start = clock();
    for(int i=0;i<iteration;++i){
        test_merge_sort();
    }
    end = clock();
    printf("merge sort %d iteration used: %f ms\n", iteration, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for(int i=0;i<iteration;++i){
        test_merge_sort_bottom();
    }
    end = clock();
    printf("merge sort bottom %d iteration used: %f ms\n", iteration, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    return NULL;
}

char *test_running_time_random(){
    //初始化测试用例
    List *wordlists[10];
    for(int i=0;i<sizeof(wordlists) / sizeof(List*);++i){
        wordlists[i] = create_random_words_list();
    }
    clock_t start, end;

    start = clock();
    for(int i=0;i<sizeof(wordlists) / sizeof(List*);++i){
        List *words = wordlists[i];

        // should work on a list that needs sorting
        List *res = List_merge_sort(words, (List_compare)list_compare);
        mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

        List *res2 = List_merge_sort(res, (List_compare)list_compare);
        mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
        List_clear_destroy(res2);
        List_clear_destroy(res);
        //List_destroy(words);
    }
    end = clock();
    printf("merge sort random %d iteration used: %f ms\n", 10, (double)(end - start) / CLOCKS_PER_SEC * 1000);   

    start = clock();
    for(int i=0;i<sizeof(wordlists) / sizeof(List*);++i){
        List *words = wordlists[i];

        // should work on a list that needs sorting
        int rc = List_bubble_sort(words, (List_compare)list_compare);
        mu_assert(rc == 0, "Bubble sort failed.");
        mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

        // should work on an already sorted list
        rc = List_bubble_sort(words, (List_compare)list_compare);
        mu_assert(rc == 0, "Bubble sort of already sorted failed.");
        mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

        //List_destroy(words);

        // should work on an empty list
        words = List_create(words);
        rc = List_bubble_sort(words, (List_compare)list_compare);
        mu_assert(rc == 0, "Bubble sort failed on empty list.");
        mu_assert(is_sorted(words), "Words should be sorted if empty.");

        List_clear_destroy(words);
    }
    end = clock();
    printf("bubble sort random %d iteration used: %f ms\n", 10, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for(int i=0;i<sizeof(wordlists) / sizeof(List*);++i){
        List *words = wordlists[i];

        // should work on a list that needs sorting
        List *res = List_merge_sort_bottom(words, (List_compare)list_compare);
        mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

        List *res2 = List_merge_sort_bottom(res, (List_compare)list_compare);
        mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
        //List_destroy(res2);
        //List_destroy(res);
        //List_destroy(words);
    }
    end = clock();
    printf("merge sort bottom random %d iteration used: %f ms\n", 10, (double)(end - start) / CLOCKS_PER_SEC * 1000);   
    for(int i=0;i<10;++i){
        List_clear_destroy(wordlists[i]);
    }
}

/*这个一定比创建链表再排序快*/
char *test_insert_sorted(){
    List *words = create_words();

    // should work on a list that needs sorting
    List *res = List_merge_sort(words, (List_compare)list_compare);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    List *res2 = List_merge_sort(res, (List_compare)list_compare);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");

    List *res3 = List_insert_sorted(res, (List_compare)list_compare, "abc", 4);
    mu_assert(is_sorted(res), "Should still be sorted after insert sorted.");

    List *res4 = List_insert_sorted(res3, (List_compare)list_compare, "zzz", 4);
    mu_assert(is_sorted(res), "Should still be sorted after insert sorted.");

    List_clear_destroy(res2);
    List_clear_destroy(res);

    List_clear_destroy(words);
    return NULL;    
}

char *all_algos_tests()
{
    mu_suite_start();

    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);
    mu_run_test(test_insert_sorted);
    mu_run_test(test_merge_sort_bottom);
    mu_run_test(test_running_time);
    //mu_run_test(test_running_time_random);
    return NULL;
}

RUN_TESTS(all_algos_tests);