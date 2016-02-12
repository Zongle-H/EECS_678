#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>

#define BSIZE 256

#define BASH_EXEC  "/bin/bash"
#define FIND_EXEC  "/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
  int status;
  pid_t pid_1, pid_2, pid_3, pid_4;

  if (argc != 4) {
    printf("usage: finder DIR STR NUM_FILES\n");
    exit(0);
  }

  // declare pipes and make the pipe() system call before
   int fd_1[2];
   int fd_2[2];
   int fd_3[2];

   /*
   if(pipe(fd_1) < 0) {
     printf("Couldn't make first pipe\n");
     return -1;
   }

   if(pipe(fd_2) < 0){
     printf("Couldn't make second pipe\n");
   };

   if(pipe(fd_3) < 0){
     printf("Couldn't make third pipe\n");
   };
   set up pipes */

  pipe(fd_1);
  pid_1 = fork();
  if (pid_1 == 0) {

    /* First Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s %s -name \'*\'.[ch]", FIND_EXEC, argv[1]);

    //assign write end of first pipe to be STD_OUT destination
    dup2(fd_1[WRITE_END], STDOUT_FILENO);

    //close READ_END of pipe 1
    close(fd_1[READ_END]);

    if( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char*) 0)) < 0) {
      fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
      return EXIT_FAILURE;
    }

    exit(0);
  }

  //close write end of first pipe
  close(fd_1[WRITE_END]);

  pipe(fd_2);
  pid_2 = fork();
  if (pid_2 == 0) {
    /* Second Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s %s -c %s", XARGS_EXEC, GREP_EXEC, argv[2]);

    //assign read end of first pipe to be desination of STD_IN
    dup2(fd_1[READ_END], STDIN_FILENO);

    //assign write end of second pipe to be destination of STD_OUT
    dup2(fd_2[WRITE_END], STDOUT_FILENO);

    if( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char*) 0)) < 0) {
      fprintf(stderr, "\nError execing xargs grep. ERROR#%d\n", errno);
      return EXIT_FAILURE;
    }
    exit(0);
  }

  close(fd_1[READ_END]);
  close(fd_2[WRITE_END]);

  pipe(fd_3);
  pid_3 = fork();
  if (pid_3 == 0) {
    /* Third Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s -t: +1.0 -2.0 --numeric --reverse", SORT_EXEC);

    dup2(fd_2[READ_END], STDIN_FILENO);
    dup2(fd_3[WRITE_END], STDOUT_FILENO);

    if( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char*) 0)) < 0) {
      fprintf(stderr, "\nError execing xargs grep. ERROR#%d\n", errno);
      return EXIT_FAILURE;
    }

    exit(0);
  }

  close(fd_2[READ_END]);
  close(fd_3[WRITE_END]);

  pid_4 = fork();
  if (pid_4 == 0) {
    /* Fourth Child */
    char cmdbuf[BSIZE];
    bzero(cmdbuf, BSIZE);
    sprintf(cmdbuf, "%s --lines=%s", HEAD_EXEC, argv[3]);

    dup2(fd_3[READ_END], STDIN_FILENO);

    if( (execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char*) 0)) < 0) {
      fprintf(stderr, "\nError execing xargs grep. ERROR#%d\n", errno);
      return EXIT_FAILURE;
    }

    exit(0);
  }

  close(fd_3[READ_END]);

  if ((waitpid(pid_1, &status, 0)) == -1) {
    fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_2, &status, 0)) == -1) {
    fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_3, &status, 0)) == -1) {
    fprintf(stderr, "Process 3 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }
  if ((waitpid(pid_4, &status, 0)) == -1) {
    fprintf(stderr, "Process 4 encountered an error. ERROR%d", errno);
    return EXIT_FAILURE;
  }

  return 0;
}
