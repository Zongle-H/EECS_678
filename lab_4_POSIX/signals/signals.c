#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
int got_response = 0;
#define CTRL_C_THRESHOLD  5

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;
  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];

    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");

    //instantiate alarm
    alarm(5);

    fflush(stdout);
    got_response= fgets(answer, sizeof(answer), stdin);

    if ((answer[0] == 'n' || answer[0] == 'N') ) {
      printf("\nContinuing\n");
      fflush(stdout);
      /*
       * Reset Ctrl-C counter
       */
      ctrl_c_count = 0;
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}

/* the Timeout handler */
void catch_alrm(int sig_num) {

  if(got_response != 0) {
    printf("\nUser taking too long to respond. Exiting...");
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  struct sigaction sa;
  sigset_t mask_set;  /* used to set a signal masking set. */

  sa.sa_mask = mask_set;
  sa.sa_sigaction = NULL;

  /* setup mask_set */
  sigfillset(mask_set);
  sigdelset(mask_set, SIGALRM);

  // 2 - SIGINT = Crtl+C
  // 9 - SIGKILL
  // 14 - SIGALRM
  // 18, 20, 24 - Ctrl-Z

  /* set signal handlers */
  sa.sa_handler = catch_int;
  sigaction(2, &sa, NULL);

  sa.sa_handler = catch_tstp;
  sigaction(18, &sa, NULL);

  sa.sa_handler = catch_alrm;
  sigaction(14, &sa, NULL);

  //sigaction
  while(1) {
    pause();
  }

  return 0;
}
