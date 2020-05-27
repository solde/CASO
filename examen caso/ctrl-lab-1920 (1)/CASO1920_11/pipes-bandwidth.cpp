#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE (1024*64)

#define TOTAL_INFO (1024*1024*16)


// global buffers

char buffer[BUFFER_SIZE] __attribute__((aligned(4096)));
char recvbuf[BUFFER_SIZE] __attribute__((aligned(4096)));

int main (int argc, char * argv [])
{
   pid_t pid;
   int res;

   int pipe_pare_fill[2];
   int pipe_fill_pare[2];

   int total_info = TOTAL_INFO;

   if (argc>1) total_info = atoi(argv[1]);
   if (argc>2) {
      fprintf(stderr, "Us: %s [datasize]\n", argv[0]);
      exit(1);
   }

   // mlock the buffers, checking for errors appropriately







   // end of your code


   res = pipe(pipe_pare_fill);
   if (res < 0) { perror("pipe"); exit(1); }
   res = pipe(pipe_fill_pare);
   if (res < 0) { perror("pipe"); exit(1); }

   pid = fork();
    //
    //          |                       ^ 
    //          |                       |
    //       ---v---                 ---|---
    //       |     |     child       |     |
    //       --- ----------------------- ---
    //         |a| pipe_fill_pare[1]   |a| pipe_pare_fill[0]
    //         |l|     |               |l|    ^
    //         |o|     |               |o|    |
    //         |h|     v               |h|    |
    //         | |                     | |
    //         | | pipe_fill_pare[0]   | | pipe_pare_fill[1]
    //       --- ----------------------- ---
    //       |     |     parent      |  ^  |
    //       ---|---                 ---|---
    //          - > > > > > > > > > > > -

   if (pid < 0) { perror ("fork"); exit(1); }
   else if (pid==0) { // child
      int arg;


      close(pipe_fill_pare[0]);
      close(pipe_pare_fill[1]);

      // start measuring time to compute bandwidth
      struct timeval tv0, tv1;
      res = gettimeofday(&tv0, NULL);
      if (res < 0) perror("gettimeofday");
      
      for (arg = 0; arg<total_info; arg += BUFFER_SIZE) {
         // fill buffer
         int c;
         for (c = 0; c < BUFFER_SIZE; c++)
            buffer[c] = (char) (arg+c);
#ifdef DEBUG
         printf("writing %d %d\n", arg+c, c);
#endif

         res = write(pipe_fill_pare[1], buffer, BUFFER_SIZE);
         if (res < 0) { perror ("write"); break; }
         else if (res==0) printf ("EOF1\n");
         else if (res != BUFFER_SIZE) 
            { fprintf (stderr, "child, write truncated\n"); }
         res = read(pipe_pare_fill[0], recvbuf, BUFFER_SIZE);
         if (res < 0) { perror ("read"); break; }
         else if (res==0) printf ("EOF2\n");
         else if (res != BUFFER_SIZE) 
            { fprintf (stderr, "child, read truncated\n"); }
         
         for (c = 0; c < BUFFER_SIZE; c++)
            if (recvbuf[c] != (char) (arg+c))
               printf ("Check error %d %d %d %d\n", arg, c, 
                             recvbuf[c], (char) (arg+c));
      }

      // stop measuring time to compute bandwidth
      res = gettimeofday(&tv1, NULL);
      if (res < 0) perror("gettimeofday");
      tv1.tv_sec -= tv0.tv_sec;
      tv0.tv_sec =  0;
      double t = tv1.tv_sec*1000000.0 + tv1.tv_usec - tv0.tv_usec;
      fprintf (stderr, "time:       %lf us\n", t);
      fprintf (stderr, "bandwidth:  %lf MBytes/s\n", (double) total_info/t);

#ifdef DEBUG
      printf ("Finishing child\n");
#endif
         
      exit(0);
   }
   else { // parent

      close(pipe_fill_pare[1]);
      close(pipe_pare_fill[0]);

      int arg;
      for (arg = 0; arg < total_info; arg += BUFFER_SIZE) {
         // receive buffer
         res = read(pipe_fill_pare[0], buffer, BUFFER_SIZE);
         if (res < 0) { perror("parent, read"); break; }
         else if (res != BUFFER_SIZE) 
            { fprintf (stderr, "parent, read truncated\n"); }
#ifdef DEBUG
         printf ("received %d\n", buffer[0]);
#endif
         // resend buffer
         res = write(pipe_pare_fill[1], buffer, BUFFER_SIZE);
         if (res < 0) { perror("parent, write"); break; }
         else if (res != BUFFER_SIZE) 
            { fprintf (stderr, "parent, write truncated\n"); }
      }
   }

#ifdef DEBUG
   printf ("Waiting for child\n");
#endif

   int status = 0;
   res = waitpid(pid,&status,0);
   if (res >= 0) {
      printf ("Child process ended with status: %d\n", WEXITSTATUS(status));
   } 
   else 
      perror ("waitpid");

   return 0;
}  
