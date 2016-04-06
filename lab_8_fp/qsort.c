#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

/*
 * Job struct
 */
typedef struct {
  int id;
  int arrival_time;
  int prio;
} job;

/*
 * Global Variables
 */
 job job_arr[10];

/*
 * File reading
 */
 void readFile(char* myfile){
   FILE *fr;
   char line[80];
   int id, arrive, prio;
   int counter = 0;

   fr = fopen(myfile, "r");
   while(fgets(line,80,fr) != NULL){
     if (fscanf(fr,"%d %d %d",&id,&arrive,&prio)){
       job_arr[counter].id = id;
       job_arr[counter].arrival_time = arrive;
       job_arr[counter].prio = prio;
     }
   }
   fclose(fr);

   return job_arr;
 }

int main(int argc, char **argv)
{
  printf("Test\n");
  readFile("process.txt");
  job testjob = job_arr[1];
  printf("%d\n",job_arr[1].id);
  return 0;
}
