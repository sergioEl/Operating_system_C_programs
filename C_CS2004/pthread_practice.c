#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum = 0;

void *PrintHello(void *data)
{
  pthread_t tid = (pthread_t)data;

  // pthread_join(tid, NULL);
  // int my_data = (int)data;
  // sleep(1);

  printf("\n Hello from new thread %u - from  %u !\n", pthread_self(), data);

  int in = 0;
  ;
  while (in >= 0)
  {
    printf("input : ");
    scanf("%d", &in);
    if (in > 0)
    {
      sum += in;
    }
  }

  pthread_exit(NULL);
}

int main()
{
  int rc;
  pthread_t thread_id;
  pthread_t tid;

  tid = pthread_self();
  // int t = 11;

  printf("I am thread %u", tid);

  // for (size_t i = 0; i < 10; i++)
  // {
  //     printf("I am thread %u", tid);
  //     rc = pthread_create(&thread_id, NULL, PrintHello, (void*)tid);
  //     printf("\n Created new thread (%u)... \n", thread_id);
  // }

  rc = pthread_create(&thread_id, NULL, PrintHello, (void *)tid);
  if (rc)
  {
    printf("\n ERROR: return code from pthread_create is %d \n", rc);
    exit(1);
  }
  sleep(1);
  printf("\n Created new thread (%u)... \n", thread_id);
  pthread_join(thread_id, NULL);
  printf("sum is %d ", sum);
  pthread_exit(NULL);
}
