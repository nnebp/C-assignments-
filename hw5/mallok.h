#ifndef MALLOK_H_
#define MALLOK_H_

typedef enum { false, true } bool;

void create_pool(int size); 
void *my_malloc(int size); 
void my_free(void *block);
void print_list();

#endif
