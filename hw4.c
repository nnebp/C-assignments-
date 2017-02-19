#include <stdio.h>
#include <string.h>

#define MAXNAMELEN 30
#define MAXITEMS 10
#define MAXCUSTOMERS 20

typedef struct item
{
    char name[MAXNAMELEN];
    int numOrdered;
    double price;
} item;


typedef struct customer 
{
    char name[MAXNAMELEN] ;
    int itemCount;
    item items[MAXITEMS];
} customer;

typedef struct customerList
{
    customer customers[MAXCUSTOMERS];
    int count;
} customerList;

double getOrderTotal(item items[])
{
    double result = 0.0;

    for (int i = 0; i < MAXITEMS; i++)
    {
        if (items[i].numOrdered > 0)
            result += items[i].price *  items[i].numOrdered;
    }

    return result;
}

void sortItems(item items[])
{
    item temp;
    //bubble sort items based on price
    for(int i = 0; i < MAXITEMS ; i++)
    {
        for (int j = 0; j + 1 < MAXITEMS; j++)
        {
            if (items[j].numOrdered != 0 &&
                items[j + 1].numOrdered != 0 &&
                
                (items[j].price * items[j].numOrdered) <
               (items[j + 1].price * items[j + 1].numOrdered ))
            {
                temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
}

void sortCustomers(customer custs[])
{
    customer temp;
    //bubble sort customers
    for(int i = 0; i < MAXCUSTOMERS; i++)
    {
        for (int j = 0; j + 1 < MAXCUSTOMERS; j++)
        {
            if (custs[j].itemCount != 0 &&
                custs[j + 1].itemCount!= 0 &&
                
                getOrderTotal(custs[j].items) < getOrderTotal(custs[j + 1].items))
            {
                temp = custs[j];
                custs[j] = custs[j + 1];
                custs[j + 1] = temp;
            }
        }
    }
}

void addItem(customer *theCustomer, int numItems, char* itemName, double itemPrice)
{
    int itemCount = theCustomer->itemCount;

    strcpy(theCustomer->items[itemCount].name, itemName);
    theCustomer->items[itemCount].price = itemPrice;
    theCustomer->items[itemCount].numOrdered = numItems;
    theCustomer->itemCount++;
}


void readToList(customerList *list, char *customerName, int numItems, char *itemName,
                double itemPrice)
{
    //check in our list for this customer
    for(int i = 0; i < MAXCUSTOMERS; i++)
    {
        //customer already in list
        if (strcmp(list->customers[i].name, customerName) == 0)
        {
            addItem(&list->customers[i], numItems, itemName, itemPrice);
            break;
        }
        //new customer
        else if (list->customers[i].name[0] == '\0')
        {
            list->count++;
            strcpy(list->customers[i].name, customerName); 
            addItem(&list->customers[i], numItems, itemName, itemPrice);
            break;
        }
    }
}

void readFile( customerList *list)
{
    FILE *infile = fopen("hw4input.txt", "r");
    double itemPrice;
    int itemsOrdered;
    char customerName[MAXNAMELEN];
    char itemName[MAXNAMELEN];
    

    while (fscanf(infile, "%s %d %s $%lf", customerName, &itemsOrdered
                                        ,itemName, &itemPrice) != EOF) {
        readToList(list, customerName, itemsOrdered, itemName, itemPrice);
    }
}

void printSortedFile(customerList list)
{
    FILE *outfile = fopen("hw4money.txt", "w");

    sortCustomers(list.customers);    
    for(int i = 0; i < MAXCUSTOMERS; i++)
    {
        sortItems(list.customers[i].items);
        
        if (list.customers[i].itemCount != 0)
        {
            fprintf (outfile, "%s, Total Order = $%0.2f \n" ,list.customers[i].name,
                                      getOrderTotal(list.customers[i].items)) ;
            for (int j = 0; j < list.customers[i].itemCount; j++)
                fprintf(outfile, "%s %d $%0.2f, Item Value = $%0.2f \n", list.customers[i].items[j].name
                                        ,list.customers[i].items[j].numOrdered
                                        ,list.customers[i].items[j].price
                                        ,(list.customers[i].items[j].price
                                        * list.customers[i].items[j].numOrdered)
                                        );
            fprintf(outfile, "\n");    
        }

    }
}

void printTimeFile(customerList list)
{
    FILE *outfile = fopen("hw4time.txt", "w");

    for(int i = 0; i < MAXCUSTOMERS; i++)
    {
        
        if (list.customers[i].itemCount != 0)
        {
            fprintf (outfile, "%s\n" ,list.customers[i].name);
            for (int j = 0; j < list.customers[i].itemCount; j++)
                fprintf(outfile, "%s %d $%0.2f\n", list.customers[i].items[j].name
                                        ,list.customers[i].items[j].numOrdered
                                        ,list.customers[i].items[j].price);
            fprintf(outfile, "\n");    
        }

    }
}

int main () 
{
    customerList list;
    customer emptyCustomerArray[MAXCUSTOMERS], emptyCustomer;
    item emptyItemArray[MAXITEMS], emptyItem;
    
    // init an empty data structure
    emptyItem.name[0] = '\0';
    emptyItem.numOrdered = 0;
    emptyItem.price = 0.0;

    emptyCustomer.name[0] = '\0';
    emptyCustomer.itemCount = 0;

    for(int i = 0; i < MAXITEMS; i++)
    {
        emptyCustomer.items[i] = emptyItem;
    }

    list.count = 0;
    for(int i = 0; i < MAXCUSTOMERS; i++)
    {
        list.customers[i] = emptyCustomer;
    }

    //fill the data structure
    readFile(&list);
    //write the files
    printTimeFile(list);
    printSortedFile(list);

    return 0;
}

