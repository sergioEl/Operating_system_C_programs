
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN_PID 300
#define MAX_PID 5000

//2004 assignment 2 q)4
//Seokho Han 201761541 seokhoh@mun.ca
//PID manager using an array.

//An array of pids.
//A value of a PID is stored in the array.
int pids[MAX_PID - MIN_PID];

int allocate_map(void)
{
    for (int i = MIN_PID; i < MAX_PID; i++)
    {
        pids[i] = 0;
    }
    return 1;
    
}

int allocate_pid(void)
{

    for (int i = MIN_PID; i < MAX_PID; i++)
    {
        if (pids[i] == 0) 
        {
            pids[i] = i;
            return i;
        }
    }
    return -1;
}

void release_pid(int pid)
{
    pids[pid] = 0;
}

int main()
{
    int map = allocate_map();
    printf("%d\n\n", map);
    for (int i = 0; i < 20; i++)
    {
        int p = allocate_pid();
        printf("PID: %d, Value: %d\n", p, pids[p]);
    }
    int p = allocate_pid();
    printf("PID: %d, Value: %d\n", p, pids[p]);
    release_pid(300);
    p = allocate_pid();
    printf("PID: %d, Value: %d\n", p, pids[p]);
  
    return 0;
}