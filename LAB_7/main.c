#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4
sem_t cond1;
sem_t cond23;

void * job (void * id)
{
    long long tid = ((long long) id) + 1;
    switch (tid)
    {
        case 1:
            while(1)
            {
                int value;
                sem_getvalue(&cond1, &value);
                if(value == 2)
                {
                    printf("Volte sempre!\n");
                    break;
                }

            }

            break;

        case 2:
            sem_wait(&cond23);
            printf("Fique a vontade.\n");
            sem_post(&cond23);
            sem_post(&cond1);
            break;

        case 3:
            sem_wait(&cond23);
            printf("Sente-se por favor.\n");
            sem_post(&cond23);
            sem_post(&cond1);
            break;

        case 4:
            printf("Seja bem-vindo!\n");
            sem_post(&cond23);
            break;

        default:
            printf("Error\n");
            break;
    }
    pthread_exit(NULL);


}

int main()
{
    pthread_t *tid;
    sem_init(&cond23, 0, 0);
    sem_init(&cond1,0,0);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
    if (tid == NULL)
    {
        fprintf(stderr, "ERROR-malloc\n");
        return 2;
    }
    //create threads
    for(long long i = 0; i<NTHREADS; i++)
    {
        if(pthread_create(tid+i, NULL, job, (void *) i))
        {
            fprintf(stderr, "ERROR-pthread_create\n");
            return 3;
        }
    }

    //wait for the thread's end
    for(long int i = 0; i < NTHREADS; i++) //prioritizes the lower values to sum first to minimize the error
    {
        if(pthread_join(*(tid+i), NULL))
        {
            fprintf(stderr, "ERROR-pthread_join\n");
            return 3;
        }
    }
    free(tid);
    pthread_exit(NULL);
    return 0;
}
