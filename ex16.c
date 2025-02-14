#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define USE_STACK

struct Person {
    char *name;
    int age;
    int height;
    int weight;
};


struct Person *Person_create(char *name, int age, int height, int weight)
{
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    who->name = strdup(name);//开辟堆上空间，复制name过去
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}

void Person_destroy(struct Person *who)
{
    assert(who != NULL);

    free(who->name);
    free(who);
}

void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

struct Person Person_create_stack(char *name, int age, int height, int weight){
    struct Person ret;
    ret.name = strdup(name);
    ret.age = age;
    ret.height = height;
    ret.weight = weight;
    return ret;
}

void Person_destroy_stack(struct Person x){
    free(x.name);
    x.name = NULL;
}

void Person_print_stack(struct Person who)
{
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

int main(int argc, char *argv[])
{
    // make two people structures
#ifndef USE_STACK
    struct Person *joe = Person_create(
            "Joe Alex", 32, 64, 140);

    struct Person *frank = Person_create(
            "Frank Blank", 20, 72, 180);

    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);

    // make everyone age 20 years and print them again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);

    // destroy them both so we clean up
    Person_destroy(joe);
    Person_destroy(frank);
#else
    struct Person joe = Person_create_stack(
            "Joe Alex", 32, 64, 140);

    struct Person frank = Person_create_stack(
            "Frank Blank", 20, 72, 180);
    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", &joe);
    Person_print_stack(joe);

    printf("Frank is at memory location %p:\n", &frank);
    Person_print_stack(frank);
    // make everyone age 20 years and print them again
    joe.age += 20;
    joe.height -= 2;
    joe.weight += 40;    
    Person_print_stack(joe);

    frank.age += 20;
    frank.weight += 20;
    Person_print_stack(frank);

    // destroy them both so we clean up
    Person_destroy_stack(joe);
    Person_destroy_stack(frank);       
#endif
    return 0;
}