#include <stdio.h>

int main(int argc, char *argv[])
{
    int areas[] = {10, 12, 13, 14, 20};
    char name[] = "Zed";
    char full_name[] = {
        'Z', 'e', 'd',
         ' ', 'A', '.', ' ',
         'S', 'h', 'a', 'w', '\0'
    };
    areas[1] = name[0];
    // WARNING: On some systems you may have to change the
    // %ld in this code to a %u since it will use unsigned ints
    printf("The size of an int: %ld\n", sizeof(int)); // 4 B
    printf("The size of areas (int[]): %ld\n",
            sizeof(areas));// 20 B
    printf("The number of ints in areas: %ld\n",
            sizeof(areas) / sizeof(int)); // 5
    printf("The first area is %d, the 2nd %d.\n",
            areas[0], areas[1]);//10 12

    printf("The size of a char: %ld\n", sizeof(char)); // 1
    printf("The size of name (char[]): %ld\n",
            sizeof(name)); // 4
    printf("The number of chars: %ld\n",
            sizeof(name) / sizeof(char)); // 4

    printf("The size of full_name (char[]): %ld\n",
            sizeof(full_name)); // 12
    printf("The number of chars: %ld\n",
            sizeof(full_name) / sizeof(char)); // 12

    printf("name=\"%s\" and full_name=\"%s\"\n",
            name, full_name);

    return 0;
}