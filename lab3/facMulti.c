#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int r=1;
pthread_mutex_t mutex;

void* f(void *x) {
    pthread_mutex_lock(&mutex);
    r = r * *(int *)x;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int NTHREADS;
    int *n;
    int i;
    pthread_t *pth;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutex,NULL);
    NTHREADS=atoi(argv[1]);
    /* BEGIN codi a editar
    * demaneu memoria pel vector N de NTHREADS enters
    */
    n = malloc(sizeof(int)*NTHREADS);
    // demaneu memoria pel vector pth de NTHREADS pthread_ts
    pth = malloc(sizeof(pthread_t)*NTHREADS);
    /* END codi a editar*/
    for (i=0;i<NTHREADS;i++){
        n[i]=i+1;
    }
    for (i=0;i<NTHREADS;i++){
        /* BEGIN codi a editar
        * feu la crida per crear un thread que executi la funci´o f*/
        if(pthread_create(&pth[i], &attr, f, &n[i]) < 0){
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
    printf("factorial de %d = %d\n",NTHREADS,r);
    return 0;
}