#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // go through each string in argv
#if 0
    int i = 0;

    while(i < argc) {
        printf("arg %d: %s\n", i, argv[i]);
        i++;
    }
#endif
    int i = argc - 1;
    while(i >= 0){
        printf("arg %d: %s\n", i, argv[i]);
        i--;
    }
    // let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;
#if 1
    /* 复制操作 */
    i = 0;
    while(i < argc && i < num_states){
        states[i] = argv[i]; // 拷贝指针
        //memcpy(states[i], argv[i], strlen(argv[i]));//源字符串申请空间大于目的串，出错
        i++;
    }
#endif
#if 0
    i = 0;  // watch for this
    while(i < num_states) {
        printf("state %d: %s\n", i, states[i]);
        i++;
    }
#endif
    i = num_states - 1;
    while(i >= 0){
        printf("state %d: %s\n", i, states[i]);
        i--;        
    }
    return 0;
}