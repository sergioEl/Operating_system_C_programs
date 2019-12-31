#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

/*
    COMP2004 assignment3 Q7
        - threads in multi-process
    Seokho Han 201761541
    seokhoh@mun.ca
*/

#define MIN_PID 300
#define MAX_PID 10000
#define CB CHAR_BIT
#define NUM_THREADS 5

pthread_t threads[NUM_THREADS];


void * thread_work(void * param);
int allocate_map();
int allocate_pid();
void release_pid(int pid);


int sz = MAX_PID - MIN_PID + 1;

unsigned char *unsignedChar;

//thread function
void * thread_work(void * param)
{
    int index = *((int *)param);
    int sleepTime =1 + rand() % NUM_THREADS; 
    printf("Thread %d started\n", index);
    int pidVal = allocate_pid();
    printf("pid %d created from thread %d\n", pidVal, index);
    sleep(sleepTime);
    release_pid(pidVal);
    printf("pid %d released from thread %d\n", pidVal, index);

    
    pthread_exit(NULL);
}

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
    int rc;
    int i;

    // pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    if (map == 1)
    {
        for (i = 0; i < NUM_THREADS; i++)
        {
            printf("creating thread %d\n", i);
            thread_args[i] = i;
            rc = pthread_create(&threads[i], NULL, thread_work, &thread_args[i]);
        }

        printf("All threads are created\n");
        // wait for the threads finishing its job
        for (i = 0; i < NUM_THREADS; i++)
        {
            rc = pthread_join(threads[i], NULL);
            printf("Thread %d ended\n", i);
        }
    }
    else
        printf("\nFailed to initialize data structure.\n");

    printf("\n program terminated \n");
    pthread_exit(NULL);
    return 0;

}
