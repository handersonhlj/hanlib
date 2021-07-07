#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

sem_t op_sem, ap_sem;

pthread_t sem_event;
int cnt = 0;
int flag = 0;

void wait_semaphore(int timeout_sec, const char *error_msg)
{
    int a;
    int run = 1;
    while (run)
    {
        sem_init(&ap_sem, 0, 0);
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += timeout_sec;
        if (sem_timedwait(&ap_sem, &timeout) != 0)
        {
            printf("%s********\n\n", error_msg);
            cnt = 0;
            run = 0;
        }
        else
        {
            printf("other\n");
        }
    }
}

void *sem_thread()
{
    sem_init(&op_sem, 0, 0);
    printf("sem_thread\n");
    while (1)
    {
        printf("start\n");
        sem_wait(&op_sem);
        wait_semaphore(5, "han\n");
    }
}

void wait_semaphore1(int timeout_sec, const char *error_msg)
{
    int a;
    int run = 1;
    while (run)
    {
        sem_init(&ap_sem, 0, 0);
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += timeout_sec;
        if (sem_timedwait(&ap_sem, &timeout) != 0)
        {
            printf("%s********\n\n", error_msg);
            cnt = 0;
            run = 0;
        }
        else
        {
            printf("other\n");
        }
    }
}


void *sem_thread1()
{
    sem_init(&op_sem, 0, 0);
    printf("sem_thread\n");
    while (1)
    {
        printf("start\n");
        sem_wait(&op_sem);
        int tmpflag = true;
        while (tmpflag)
        {
            sem_init(&op_sem, 0, 0);
            struct timespec timeout;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += 2;
            if (sem_timedwait(&op_sem, &timeout) != 0)
            {
                printf("time out********\n\n");
                tmpflag = false;
            }
            else
            {
                printf("other\n");
            }
        }
    }
}



// void main(void)
// {
//     int a;
//     int ret = pthread_create(&sem_event, 0, sem_thread, 0);
//     while (1)
//     {
//         scanf("%d", &a);
//         if (a == 1)
//         {
//             sem_post(&ap_sem);
//         }
//         if (a == 2)
//         {
//             if (cnt == 0)
//                 sem_post(&op_sem);
//             cnt++;
//             if (cnt > 1)
//             {
//                 flag = 1;
//                 sem_post(&ap_sem);
//             }
//         }
//     }
// }

void main(void)
{
    int a;
    int ret = pthread_create(&sem_event, 0, sem_thread1, 0);
    while (1)
    {
        scanf("%d", &a);
        if (a == 2)
        {
            sem_post(&op_sem);
        }
    }
}