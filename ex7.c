#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    int bugs = 100;
    double bug_rate = 1.2;

    printf("You have %d bugs at the imaginary rate of %f.\n",
            bugs, bug_rate);

    unsigned long universe_of_defects = 1024UL * 1024UL * 1024UL * 1024UL * 1024UL * 1024UL;
    //long, unsigned long, long int, unsigned long int, long long unsigned long long 大小为8
    //LONG_MAX:2^63-1 LONG_MIN:-2^63
    //ULONG_MAX: 2^64-1
    //LLONG_MAX:2^63-1 LLONG_MIN:-2^63
    //ULLONG_MAX: 2^64-1
    printf("%llu\n", ULLONG_MAX);
    printf("The entire universe has %ld bugs.\n",
            universe_of_defects);

    double expected_bugs = bugs * bug_rate;
    printf("You are expected to have %f bugs.\n",
            expected_bugs);

    double part_of_universe = expected_bugs / universe_of_defects;
    printf("That is only a %e portion of the universe.\n",
            part_of_universe);//%e为科学计数法形式

    // this makes no sense, just a demo of something weird
    char nul_byte = '\0'; //'\0'数值上也为0
    int care_percentage = bugs * nul_byte;
    printf("Which means you should care %d%%.\n",
            care_percentage);

    return 0;
}