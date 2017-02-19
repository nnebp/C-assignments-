#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mallok.h"

void test_three()
{
    create_pool(1000);
    void * one = my_malloc(200);
    void * two = my_malloc(200);
    void * three = my_malloc(200);
    void * four = my_malloc(200);
    void * five = my_malloc(200);


    print_list();

    //free stuff
    my_free(three);
    print_list();

    printf("cant get get 250 bytes (1): %d \n", (my_malloc(250) == NULL)); 
    printf("cant get 150 bytes (0): %d \n", (my_malloc(150) == NULL)); 
    print_list();
    printf("cant get 60 bytes (1): %d \n", (my_malloc(60) == NULL)); 
    printf("cant get 50 bytes (0): %d \n", (my_malloc(50) == NULL)); 
    
    print_list();

}

void test_one()
{
    create_pool(1000);
    int howmany = 0;
    while (my_malloc(10) != NULL)
        howmany++;

    printf("how many times can we request 10 bytes from a pool of 1000? %d times.\n", howmany);

}

void test_two()
{
    create_pool(1000);
    void * one ;
    void * two; 
    void * three ;
    void * four ;
    void * five ;

    for (int i = 0; i < 20; i++)
    {

        one = my_malloc(200);
        two = my_malloc(200);
        three = my_malloc(200);
        four = my_malloc(200);
        five = my_malloc(200);

        my_free(one);
        my_free(two);
        my_free(three);
        my_free(four);
        my_free(five);
    }
    one = my_malloc(200);
    two = my_malloc(200);
    three = my_malloc(200);
    four = my_malloc(200);
    five = my_malloc(200);
    print_list();
}

void test_four()
{
    create_pool(1000);
    
    bool correct = true;
    bool correctB= true;
    void * one = my_malloc(200);
    void * two = my_malloc(200);
    void * three = my_malloc(200);
    void * four = my_malloc(200);
    void * five = my_malloc(200);
    
    memset(one, 'A', 200);
    memset(two, 'B', 200);

    //would make the test fail
    //*((char *) two) = '&';

    for (int i = 0; i < 200; i++)
    {
        if (*((char *) (one + i)) != 'A')
        {
            correct = false;
            break;
        }
    }
    for (int i = 0; i < 200; i++)
    {
        if (*((char *) (two + i)) != 'B')
        {
            correctB = false;
            break;
        }
    }

    printf("200 A's?(1) %d \t 200 B's?(1)%d\n", correct, correctB);
}

int main ()
{
   test_one();
   test_two();
   test_three();
   test_four();
}
