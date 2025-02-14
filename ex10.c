#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;

    // go through each string in argv
    // why am I skipping argv[0]?
    // ans: argv[0] is the filename

    for(i = 1; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    // let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        NULL, "Texas"
    };
    int num_states = 4;
    /* 赋值可以生效 */ 
    argv[1] = states[1];
    for(i = 0; i < num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
    }
    for(i = 1; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }
    //对于值为NULL的char指针，打印字符串输出(null)
    return 0;
}