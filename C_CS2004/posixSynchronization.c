#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include <unistd.h> 


/*
    COMP2004 assignment6 Q2 
        - Farmers.
    Seokho Han 201761541
    seokhoh@mun.ca
*/

#define NUM_THREADS  10

int bridge = 0;
int north = 0;
int south = 0;
int prev = 0;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *northGo(void *t);
void *southGo(void *t);  

int main(int argc, char *argv[])
{
  int i, rc; 

  pthread_t threads[10];
  pthread_attr_t attr;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_threshold_cv, NULL);

  /* For portability, explicitly create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (int i = 0; i < NUM_THREADS; i ++)
  {
      if (i < 4)
      {
          pthread_create(&threads[i], &attr, southGo, NULL);
      }
      else
      {
          pthread_create(&threads[i], &attr, northGo,  NULL);
      }
      
  }

    /* Wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit (NULL);

}

void *northGo(void *t) 
{
 
    pthread_mutex_lock(&count_mutex);

    north ++;
    while (bridge == 1 || (prev ==0 && south > 0))
    {
        printf("south farmer crossing now, north farmer wait\n");
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
    }

    bridge = 1;
    north --;
    printf("north crossing\n");
    prev = 0;

    sleep(rand() % 5);

    bridge = 0;
    pthread_cond_signal(&count_threshold_cv);

    pthread_mutex_unlock(&count_mutex);

    pthread_exit(NULL);
}

void *southGo(void *t) 
{
    pthread_mutex_lock(&count_mutex);

    south ++;
    while (bridge == 1 || (prev ==1 && north > 0))
    {
        printf("north farmer crossing now, south farmer wait\n");
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
    }

    bridge = 1;
    south --;
    printf("south crossing\n");
    prev = 1;

    //exit
    sleep(rand() % 5);
    bridge = 0; 
    pthread_cond_signal(&count_threshold_cv);

    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}
