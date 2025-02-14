#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend die from ex17. */
void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);

typedef int *(*sort_algo)(int *numbers, int count, compare_cb cmp);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}

//以左边元素为基准
//基准点归位
static int partition(int *numbers, int left, int right, compare_cb cmp){
    int i = left;
    int j = right;
    while(i < j){
        while(i < j && cmp(numbers[j], numbers[left]) >= 0){
            --j;
        }
        while(i < j && cmp(numbers[i], numbers[left]) <= 0){
            ++i;
        }
        int tmp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = tmp;
    }
    int tmp = numbers[i];
    numbers[i] = numbers[left];
    numbers[left] = tmp;
    return i;
}

static void _quick_sort(int *numbers, int left, int right, compare_cb cmp){
    if(right <= left) return;
    int piv_point = partition(numbers, left, right, cmp);
    _quick_sort(numbers, left, piv_point - 1, cmp);
    _quick_sort(numbers, piv_point + 1, right, cmp);
}


int *quick_sort(int *numbers, int count, compare_cb cmp){
    int *target = malloc(count * sizeof(int));
    memcpy(target, numbers, count * sizeof(int));
    _quick_sort(target, 0, count - 1, cmp);
    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, compare_cb cmp)
{
    int i = 0;
    int *sorted = bubble_sort(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.");

    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);

    /* 练习中添加 */
    unsigned char *data = (unsigned char *)cmp;

    for(i = 0; i < 25; i++) {
        printf("%02x:", data[i]);
    }

    printf("\n");
}

void test_sorting_algo(sort_algo algo, int *numbers, int count, compare_cb cmp)
{
    int i = 0;
    int *sorted = algo(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.");

    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);

    /* 练习中添加 */
    unsigned char *data = (unsigned char *)cmp;

    for(i = 0; i < 25; i++) {
        printf("%02x:", data[i]);
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1;
    int i = 0;
    char **inputs = argv + 1;

    int *numbers = malloc(count * sizeof(int));
    if(!numbers) die("Memory error.");

    for(i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

#define USE_ALGO
#ifndef USE_ALGO
    test_sorting(numbers, count, sorted_order);
    test_sorting(numbers, count, reverse_order);
    test_sorting(numbers, count, strange_order);
    //test_sorting(numbers, count, NULL);//会导致段错误
#else
    test_sorting_algo(bubble_sort, numbers, count, sorted_order);
    test_sorting_algo(bubble_sort, numbers, count, reverse_order);
    test_sorting_algo(bubble_sort, numbers, count, strange_order);   

    test_sorting_algo(quick_sort, numbers, count, sorted_order);
    test_sorting_algo(quick_sort, numbers, count, reverse_order);
    test_sorting_algo(quick_sort, numbers, count, strange_order);  
#endif
    free(numbers);

    return 0;
}