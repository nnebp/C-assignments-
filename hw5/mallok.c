#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mallok.h"


typedef struct Node Node;

struct Node
{
    void *address;
    int size;
    bool free;
    Node *next;
    Node *prev;
};

typedef struct List List;

struct List
{
    Node *head;
    Node *tail;
};

//global list
List list;

void print_list()
{
    Node* front = list.head;

    while (front != NULL)
    {
        printf("size:%d free: %d \t address:%lu \n", front->size, front->free,
                                                    (unsigned long)front->address);
        front = front->next;
    }
    printf("\n");
}

//add a node in front of the current node
void *split_add_to_list (Node *oldnode, List *list ,int size)
{
    if (list->head != NULL && 
        oldnode != NULL && 
        oldnode->free != false &&
        oldnode->address != NULL)
    {
        //if we want to assign an node the same size as old node
        if (size == oldnode->size)
        {
            oldnode->free = false;
            return oldnode->address;
        }

        Node *newnode = malloc(sizeof(Node));

        //node we are splitting on is front of the list
        if (oldnode->prev == NULL)
        {
            newnode->address = oldnode->address;
            oldnode->address = oldnode->address + size;
            oldnode->size = oldnode->size - size;
            oldnode->prev = newnode;

            list->head = newnode;

            newnode->size = size;
            newnode->free = false;
            newnode->next = oldnode;
            newnode->prev = NULL;
        }
        //insert in front of a node that is not in the front
        else if (oldnode->prev != NULL)
        {
            newnode->prev = oldnode->prev;
            oldnode->prev->next = newnode;

            newnode->address = oldnode->address;
            oldnode->address = oldnode->address + size;
            oldnode->size = oldnode->size - size;
            newnode->free = false;

            newnode->size = size;
            newnode->next = oldnode;
            oldnode->prev = newnode;
        }
        return newnode->address;
    }
    return NULL;
}

Node *mark_node_free(Node *node, void *address)
{
    while (node != NULL)
    {
        if (node->address == address)
        {
            node->free = true;
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void free_and_combine_node(Node *node, List *list)
{
    if (node != NULL && node->free == true)
    {
        //dont need to do this free(node->address);

        //combine with prev node if free
        if (node->prev != NULL &&node->prev->free == true)
        {
            //printf("combine with first\n");
            node->address = node->prev->address;
            node->size = node->size + node->prev->size;
            //deal with previous previous node
            if (node->prev->prev != NULL)
                node->prev->prev->next = node;
            //if our prev combining node was the head of the list
            //printf("head:%lu prev:%lu \n", list->head, node->prev);
            if (list->head == node->prev)
                list->head = node;

            //free and link up to prev node
            Node* prevprev = node->prev->prev;
            free (node->prev);
            node->prev = prevprev;
        }
        //combine with next node
        if (node->next != NULL && node->next->free == true)
        {
            //printf("combine with next\n");
            //we can keep the address where it is
            node->size = node->size + node->next->size;
            if (node->next->next != NULL)
                node->next->next->prev = node;
           //free and link up to next node
           Node *nextnext = node->next->next;
           free(node->next);
           node->next = nextnext;
        }
    }
}

void* my_malloc(int size)
{
    //walk a list and look for a free node. split add a node of size
    //return pointer  node->address
    Node *node = list.head;
    
    while (node != NULL)
    {
        if (node->size >= size && node->free == true)
        {
            /*we have space for the call. split add new node
            and return the node->address*/
            return split_add_to_list(node, &list, size);
        }
        node = node->next;
    }
    return NULL;
}

void create_pool(int size)
{
    void *pool = malloc(size);

    Node *first = malloc(sizeof(Node));
    first->address = pool;
    first->size = size;
    first->free = true;
    first->next = NULL;
    first->prev = NULL;

    list.head = first;
}

void my_free(void * block)
{
    Node *node = mark_node_free(list.head, block);
    free_and_combine_node(node, &list);
}
