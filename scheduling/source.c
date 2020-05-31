#define HAVE_STRUCT_TIMESPEC
#define _CRT_SECURE_NO_WARNINGS

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define NUM_THREADS 5

typedef struct{
    int class_num;
    int pid;
    int prior;
    int cpu_time;
}data;

int times[5];
int turn = 0;
int process_num = 5;
data data1[100];

void StartNext(int tid)     //choose next thread to run
{
    int i;
    for (i = (tid + 1) % 5; times[i] == 0; i = (i + 1) % 5)
        if (i == tid)    //every thread finish
            return;
    turn = i;
}

void* RR(void* pid)    //the thread function
{
    int quan = 3;
    int i = (int)pid;
    int get_time = 0;
    while (times[i] != 0)
    {
        int temp = quan;
        while (turn != i);   //busy waiting till it is its turn
        if (times[i] > quan)
        {
            for (int k = 0; k < quan; k++)
                printf("%d", i+1);
            printf("\n");
            Sleep(3000);   //sleep is to simulate the actual running time
            times[i] -= quan;
            
        }
        else if (times[i] > 0 && times[i] <= quan)      //this thread will have finished after this turn
        {
            for (int k = 0; k < (int)times[i]; k++)
                printf("%d", i+1);
            printf("\n");
            Sleep(times[i]*1000);
            temp = times[i];
            times[i] = 0;
        }
        StartNext(i);   //choose the next thread to run
    }
    pthread_exit(0);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int i, status;
    int count = 0;

    for (int i = 0; i < process_num; i++) {
        scanf("%d %d %d %d", &data1[i].class_num, &data1[i].pid, &data1[i].prior, &data1[i].cpu_time);
        times[i] = data1[i].cpu_time;
        count++;
    }
    if (count == 5)
    {
        printf("\nRound Robin Scheduling\n");
        for (i = 0; i < 5; i++)
        {
            data dat = data1[i];
            status = pthread_create(&threads[i], NULL, RR, (void*) i);    //Create threads
            if (status != 0)
            {
                printf("While creating thread %d, pthread_create returned error code %d\n", i, status);
                exit(-1);
            }    //Join threads
        }
        for (i = 0; i < 5; i++) {
            pthread_join(threads[i], 0);
        }
    }
    return 0;
}