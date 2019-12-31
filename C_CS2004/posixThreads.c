#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
    COMP2004 assignment3 Q6
        - multithreading calc of statistical values
        - get command line arguments
        - creating 3 threads(workers) that calculate
        - parent thread will only print the result when
          the workers finish the calc.
    Seokho Han 201761541
    seokhoh@mun.ca
*/

int sum; /* this data is shared by the thread(s) */
int size, avg, maxi, mini; /*size for integer values, mean, max, and min*/
int *values; /*pointer to an integer*/

void *mean(void *param); /* the threads functions */
void *min(void *param);
void *max(void *param);

int main(int argc, char *argv[])
{
    pthread_t meanPt;       /* the thread identifiers */
    pthread_t maxPt;
    pthread_t minPt;
    pthread_attr_t attr; /* set of attributes for the thread */
    size_t i; 

    /*check the validity*/
    if (argc < 2)
    {
        fprintf(stderr, "usage: a.out <integer values...>\n");
        /*exit(1);*/
        return -1;
    }

    /*modify below because we only need inputs from argv[1] */
    --argc; 
    ++argv; 

    /*allocate size to the pointer*/
    // values = calloc(argc, sizeof(int)); // calloc initializes all values to zero
    values = malloc(sizeof(int) * argc); // malloc is fater than calloc

    for (i = 0; i < argc; i++)
    {
        values[i] = atoi(argv[i]);
        size++;
    }

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the threads */
    pthread_create(&meanPt, &attr, mean, values);
    pthread_create(&maxPt, &attr, max, values);
    pthread_create(&minPt, &attr, min, values);

    /* now wait for the thread to exit */
    pthread_join(meanPt, NULL);
    pthread_join(maxPt, NULL);
    pthread_join(minPt, NULL);

    printf("mean = %d \t max = %d \t min = %d\n", avg, maxi, mini);
}

/**
 * The threads will begin control in below functions
 */
void *mean(void *param)
{
    int i = 0;
    int s = 0;
    int *val;

    val = param;

    for (i = 0; i < size; i++)
    {
        // printf("%d \n", val[i]);
        s += val[i];
    }
    avg = s / (double) size;

    pthread_exit(0);
}

void *max(void *param)
{
    int i = 0;
    int *val;
    val = param;

    maxi = val[0];

    for (i = 0; i < size ; i++)
    {
        if (val[i] > maxi)
        {
            maxi = val[i];
        }
    }
    pthread_exit(0);

}

void *min(void *param)
{
    int i = 0;
    int *val;
    val = param;

    mini = val[0];

    for (i = 0; i < size ; i++)
    {
        if (val[i] < mini)
        {
            mini = val[i];
        }
    }
    pthread_exit(0);

}