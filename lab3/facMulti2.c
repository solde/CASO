#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/sysinfo.h>
#include <sched.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>

int r=1;
pthread_mutex_t mutex;

int number;
int *n;

void* f(void *x) {
    pthread_mutex_lock(&mutex);
    r = r * *(int *)x;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* f2(void *cpu_id){
    int i = *(int*)cpu_id;
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(i, &mask);
    if(sched_setaffinity(0, sizeof(mask), &mask) < 0){
        printf("sched_setaffinity error\n");
    }
    pthread_mutex_lock(&mutex);
    printf("%d\n", i);
    while(i < number){
        r = r * n[i];
        i += get_nprocs_conf();
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int NTHREADS = get_nprocs_conf();
    int i;
    pthread_t *pth;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutex,NULL);
    number=atoi(argv[1]);
    /* BEGIN codi a editar
    * demaneu memoria pel vector N de NTHREADS enters
    */
    n = malloc(sizeof(int)*number);
    // demaneu memoria pel vector pth de NTHREADS pthread_ts
    pth = malloc(sizeof(pthread_t)*NTHREADS);
    /* END codi a editar*/
    for (i=0;i<number;i++){
        n[i]=i+1;
    }
    int aux[NTHREADS];
    for (i=0;i<NTHREADS;i++){
        /* BEGIN codi a editar
        * feu la crida per crear un thread que executi la funci´o f*/
        aux[i] = i;
        if(pthread_create(&pth[i], &attr, f2, &aux[i]) < 0){
            printf("pthread_created error: %d\n", i);
        }
        // END codi a editar*/
    }
    for(i=0;i<NTHREADS;i++){
        /* BEGIN codi a editar
        * feu crida per esperar a un thread*/
        if(pthread_join(pth[i], NULL) < 0){
            printf("phtread_join error: %d\n", i);
        }
        //* END codi a editar*/
    }
    printf("factorial de %d = %d\n",number,r);
    return 0;
}