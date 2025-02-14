#include <stdio.h>
#include <ctype.h>
#include "dbg.h"


int print_a_message(const char *msg)
{
    printf("A STRING: %s\n", msg);

    return 0;
}


int uppercase(const char *msg, size_t len)
{
    size_t i = 0;

    // BUG: \0 termination problems
    for(i = 0; i < len; i++) {
        printf("%c", toupper(msg[i]));
    }

    printf("\n");

    return 0;
}

int lowercase(const char *msg, size_t len)
{
    size_t i = 0;

    // BUG: \0 termination problems
    for(i = 0; i < len; i++) {
        printf("%c", tolower(msg[i]));
    }

    printf("\n");

    return 0;
}

int fail_on_purpose(const char *msg)
{
    (void)msg;
    return 1;
}