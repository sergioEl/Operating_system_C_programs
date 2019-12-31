#include <stdio.h>
#include <stdlib.h>

/*
    COMP2004 assignment7 Q6
        - 32 - bits virtual address with 4KB page size.
        - Get a virtual address as a command line input, 
        and return its page number and offset.
    Seokho Han 201761541
    seokhoh@mun.ca
*/

int main(int argc, char *argv[])
{
    unsigned long page;
    unsigned long offset;
    unsigned long address;

    // command line input error check
    if (argc < 2)
    {
        printf("Enter a number: \n");
        return -1;
    }

    // convert a char to long int
    address = atoll(argv[1]);

    // As same as address / 4096
    page = address >> 12; 
    // offset also can be obtained by address % 4096
    // 0xfff == (int)4096 -> 4KB
    offset = address & 0xfff;  

    printf("The address %lu contains: \n", address);
    printf("Page number = %lu\n", page);
    printf("Offset = %lu\n", offset);

    return 0;
}