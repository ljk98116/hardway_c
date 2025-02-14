#include <stdio.h>

/* Warning: This program is wrong on purpose. */

int main()
{
    int age = 10;
    int height;

    printf("I am %d years old.\n");//占位符没有值对应，运行时取随机值
    printf("I am %d inches tall.\n", height);

    return 0;
}