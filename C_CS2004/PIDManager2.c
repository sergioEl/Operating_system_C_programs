#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//2004 assignment 2 q)4
//Seokho Han 201761541 seokhoh@mun.ca

#define MIN_PID 300
#define MAX_PID 5000
#define CB CHAR_BIT

int sz = MAX_PID - MIN_PID + 1;

unsigned char *unsignedChar;

// creates and initializes a data structure.
int allocate_map()
{
    unsignedChar = (unsigned char *)malloc((sz + CB - 1) / CB * sizeof(char));
    if (unsignedChar)
        return 1;
    return -1;
}

// allocates and returns a pid
int allocate_pid()
{
    int i = 0;
    int pid = unsignedChar[i / CB] & (1 << (i & (CB - 1)));
    while (pid != 0)
    {
        i++;
        pid = unsignedChar[i / CB] & (1 << (i & (CB - 1)));
    }

    if (i + MIN_PID > MAX_PID)
        return -1;
    unsignedChar[i / CB] |= 1 << (i & (CB - 1));
    return i + MIN_PID;
}

// releases a pid
void release_pid(int pid)
{
    int i = pid - MIN_PID;
    unsignedChar[i / CB] &= ~(1 << (i & (CB - 1)));
}

int main()
{
    int map = allocate_map();
    if (map == 1)
    {
        printf("\tData Structure created.\n");
        int id = 0, i = 0;

        //create 30 processes
        while (i < 30)
        {
            int val = allocate_pid();
            printf("\nProcess %d: pid = %d", i + 1, val);
            i++;
        }
        //release a few processes
        release_pid(300);
        printf("\nRelease pid 300.");
        release_pid(310);
        printf("\nProcess pid 310.");
        release_pid(311);
        printf("\nRelease pid 311.");

        //allocate a few more processes after this release
        int val = allocate_pid();
        printf("\nProcess %d : pid = %d", ++i, val); //should be 300
        val = allocate_pid();
        printf("\nProcess %d : pid = %d\n", ++i, val); //should be 310
    }
    else
        printf("\nFailed to initialize data structure.\n");
}
