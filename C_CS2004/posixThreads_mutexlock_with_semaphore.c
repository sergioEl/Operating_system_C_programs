#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h> // for linux
#include <fcntl.h> // for linux

/*
    COMP2004 assignment6 Q1
        - Two threads for input and sum
        - Using Mutex lock that prevents from race condition.
        - Using semaphores to avoid busy wait.
    * Compile on linux: 
        gcc posema -o posixThreadsMidterm.c -pthread
    * Compile on MacOS:
        gcc posema -o posixThreadsMidterm.c

    Seokho 201761541 Han
    seokhoh@mun.ca
*/

int sharedmemorynumber = 0;

sem_t *mutex; //semaphore
sem_t *in;
sem_t *out;
pthread_mutex_t lock; // mutex lock

void *getinput(void *param);
void *calculatesum(void *param);

int main()
{
    // mutex = sem_open("SEM", O_CREAT, 0666, 1); // initiate semaphore for avoiding race condition
    // in = (sem_t * )malloc(sizeof(sem_t));
    in = sem_open("IN", O_CREAT, 0666, 1); // initiate semaphore for input
    out = sem_open("OUT", O_CREAT, 0666, 0); // initiate semaphore for sum
    
    // create a mutex lock
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    pthread_t tid[2];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid[0], &attr, getinput, NULL);
    pthread_create(&tid[1], &attr, calculatesum, NULL);

    for (int i = 0; i < 2; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    pthread_mutex_destroy(&lock);// destroy the mutex lock
    // destroy semaphores
    // sem_close(mutex);
    sem_close(out);
    sem_close(in);
    // sem_unlink("SEM");
    sem_unlink("IN");
    sem_unlink("OUT");
    pthread_exit(NULL);
}

void *getinput(void *param)
{
    int myinputnumber = 0;

    while (myinputnumber >= 0)
    {
        sem_wait(in); // decrement "in" semaphore 

        printf("input a number: ");
        scanf("%d", &myinputnumber);
        pthread_mutex_lock(&lock);// blocked 
        sharedmemorynumber = myinputnumber;
        pthread_mutex_unlock(&lock);// unlock

        sem_post(out); // wake up the pthread for sum
    }
    pthread_exit(NULL);
    return NULL;
}
void *calculatesum(void *param)
{
    int sum = 0;
    while (sharedmemorynumber >= 0)
    {
        sem_wait(out); // wait since out semaphore is initially 0


        pthread_mutex_lock(&lock);// lock 
        if (sharedmemorynumber >= 0)
        {
            sum += sharedmemorynumber;
        }
        pthread_mutex_unlock(&lock);// unlock

        if (sharedmemorynumber < 0)
        {
            printf("The sum is: %d\n", sum);
            return NULL;
        }

        sem_post(in); // wake up the pthread for getting the next input from the user
    }
    pthread_exit(NULL);
    return NULL;
}
