#include <stdio.h>
#include <math.h>

int radixCharToInt(char c)
{
    int result = 0;
    switch (c)
    {
        case 'x':
            result = 16;
            break;
        case 'b':
            result = 2;
            break;
        case 'd':
            result = 10;
            break;
    }
    return result;
}

int charToInt(char c)
{
    int result;
    if (c >= '0' && c <= '9')
        result = c - '0';
    /* hex a - f */
    if (c >= 'a' && c <= 'f')
        result = c - 87;

    return result;
}

char intToChar(int num) 
{
    char result;
    if (num >= 0 && num <= 9)
        result = num + 48;
    /* no need to check the base if num is > 9
    we know we need to return a hex value */
    if (num >= 10 && num <= 15)
        result = num + 87;

    return result;
}

void printValue(int value, char base)
{
    int radix = radixCharToInt(base);
    int i = 1;
    int largerPower = 1;
    int divOut = 0;
    int commaSpaces = 3;

    if (base == 'b')
        commaSpaces = 4;
    else if (base == 'x') 
        commaSpaces = 2;
    else if (base == 'd') 
        commaSpaces = 3;

    /* get the larger power of base */
    while (largerPower <= value)
    {
        largerPower = pow(radix, i);
        i++;
    }
    
    /* down a power */
    largerPower = largerPower/radix;
    i = i - 2;

    while (largerPower >= 1)
    {
        divOut = value / largerPower;
        printf("%c", intToChar(divOut));
        
        if (i % commaSpaces  == 0 && i > 1)
            printf(",");

        value = value % largerPower;
        largerPower = largerPower / radix;
        i--;
    }
    printf("\n");
}

int main (void) 
{
    char c;
    char cmdType = 'e', base = 'e';
    int value = 0;

    while (c != 'q')
    {
        scanf("%c", &c);
        switch (c)
        {
            case 'i':
                value = 0;
                /* new input value means new base */
                base = 'e';
            case 'o':
                cmdType = c;
                break;
            case 'b':
                if (base == 'x' && cmdType == 'i')
                {
                    value *= radixCharToInt(base);
                    value += charToInt(c);
                    break; /* b is a hex value not a base*/
                }
            case 'd':
            case 'x':
                base = c;
                break;
            /* characters to ignore */    
            case ' ':
            case ',':
                break;
            case '\n':
                if (cmdType == 'o')
                {
                    printValue(value, base);
                }
                break;
            /* processable char */
            default:
                value *= radixCharToInt(base);
                value += charToInt(c);
                break;
        }
    }
    return 0;
}
