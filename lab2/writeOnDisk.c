#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define GB 1000000000

int main(){
    FILE *ptr;
    ptr = fopen("/tmp/tmpFile.dat", "w+");
    if(ptr == NULL){
        printf("ptr==NULL\n");
        return 0;
    }

    for(double j = 0.5; j < 5; ++j){
        struct timeval tp1;
        struct timeval tp2;
        int time1 = gettimeofday(&tp1, NULL);
        
        for(unsigned int i = 0; i < GB*j; ++i){
            //printf("i == %d", i);
            fprintf(ptr,"%c", 'a');
        }

        int time2 = gettimeofday(&tp2, NULL);
        int sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
        printf("%fGB: %d - %d = %d\n", j, tp2.tv_sec * 1000000 + tp2.tv_usec, tp1.tv_sec * 1000000 + tp1.tv_usec, sec);

        if(j == 0.5)j=0;
    }
    
    fclose(ptr);
    return 0;
}

/*
0.500000GB: 1584896288 - 1584896287 = 1
1.000000GB: 1584896292 - 1584896288 = 4
2.000000GB: 1584896299 - 1584896292 = 7
3.000000GB: 1584896310 - 1584896299 = 11
4.000000GB: 1584896324 - 1584896310 = 14
*/