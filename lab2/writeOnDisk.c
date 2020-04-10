#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define GB 1000000000

int main(){
    FILE *ptr;
    //ptr = fopen("/mnt/usb/tmpFile.dat", "w+");
    ptr = fopen("/tmp/tmpFIle.dat", "w");
    if(ptr == NULL){
        printf("ptr==NULL\n");
        return 0;
    }

    for(double j = 0.5; j < 1; ++j){
        struct timeval tp1;
        struct timeval tp2;
        int time1 = gettimeofday(&tp1, NULL);
        
        for(unsigned int i = 0; i < GB*j; ++i){
            //printf("i == %d", i);
            fprintf(ptr,"%c", 'a');
        }

        int time2 = gettimeofday(&tp2, NULL);
        int sec = (tp2.tv_sec * 1000000 + tp2.tv_usec) - (tp1.tv_sec * 1000000 + tp1.tv_usec);
        double BW = (j)/(sec/1000000);
        printf("%fGB: %d - %d = %d \t BW=%fGB/s\n", j, tp2.tv_sec * 1000000 + tp2.tv_usec, tp1.tv_sec * 1000000 + tp1.tv_usec, sec, BW);

        if(j == 0.5)j=0;
    }
    fclose(ptr);
    system("rm -rf /tmp/tmpFile.dat");
    return 0;
}
