#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char * argv[])
{
   struct stat fileinfo;
   char buffer[100];
   int res;


   if (argc < 2) {
     fprintf (stderr, "Us: %s source-filename target-filename\n", argv[0]);
     exit(1);
   }

   res = stat (argv[2], &fileinfo);
   if (res < 0) { 
      if (errno != ENOENT) { perror("stat"); exit(2); }
   }
   else {
      fprintf(stderr, 
          "target file \"%s\" exists. It will not be overwritten\n", argv[2]);
      exit(3);
   }

   FILE * fi = fopen(argv[1], "r");
   if (fi==NULL) {
      perror ("fopen(r) -- source file");
      exit(1);
   }
   FILE * fo = fopen(argv[2], "w");
   if (fo==NULL) {
      perror ("fopen(w) -- target file");
      exit(1);
   }

   res = fread(buffer, 1, 100, fi);
   if (res > 0) { printf ("Reading .", res); fflush(stdout); }
   while (res > 0) {
      res = fwrite(buffer, 1, res, fo);
      if (res < 0) { perror ("fwrite"); exit(2); }
      sleep(1);
    res = fread(buffer, 1, 100, fi);
    if (res > 0) { printf (".", res); fflush(stdout); }
   }
   printf ("\n");

   return 0;
}
