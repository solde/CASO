#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

int main(){
    struct timeval tp1;
    struct timeval tp2;

    int time1 = gettimeofday(&tp1, NULL);

    int pid;
    for(int i = 0; i < 200; ++i) pid = getpid();

    int time2 = gettimeofday(&tp2, NULL);
    int sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
    printf("%d = getpid(): %d - %d = %d\n", pid, tp2.tv_usec, tp1.tv_usec, sec);

    time1 = gettimeofday(&tp1, NULL);

    for(int i = 0; i < 200; ++i) sbrk(0);

    time2 = gettimeofday(&tp2, NULL);
    sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
    printf("sbrk(0): %d - %d = %d\n", tp2.tv_usec, tp1.tv_usec, sec);

    time1 = gettimeofday(&tp1, NULL);

    for(int i = 0; i < 200; ++i) sbrk(5);

    time2 = gettimeofday(&tp2, NULL);
    sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
    printf("sbrk(5): %d - %d = %d\n", tp2.tv_usec, tp1.tv_usec, sec);

    time1 = gettimeofday(&tp1, NULL);

    for(int i = 0; i < 200; ++i) sched_yield();

    time2 = gettimeofday(&tp2, NULL);
    sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
    printf("sched_yield: %d - %d = %d\n", tp2.tv_usec, tp1.tv_usec, sec);

    time1 = gettimeofday(&tp1, NULL);
    for(int i = 0; i < 200; ++i){
        pid = fork();
        if(pid == 0){
            //printf("Hello, my pid is: %d\n", pid);
            return 0;
        }
        int log;
        waitpid(pid, &log, 0);
    }

    time2 = gettimeofday(&tp2, NULL);
    sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
    printf("fork() + waitpid: %d - %d = %d\n", tp2.tv_usec, tp1.tv_usec, sec);
}