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
    fgets(answer, sizeof(answer), stdin);

    if ((answer[0] == 'n' || answer[0] == 'N') ) {
      got_response = 1;
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

  if(got_response == 0) {
    printf("\nUser taking too long to respond. Exiting...");
    exit(0);
  }

  got_response = 0;
  fflush(stdout);
}

  // 2 - SIGINT = Crtl+C
  // 9 - SIGKILL
  // 14 - SIGALRM
  // 18, 20, 24 - Ctrl-Z

int main(int argc, char* argv[])
{
  struct sigaction sa;
  struct sigaction sa_2;
  struct sigaction sa_alarm;
  sigset_t mask_set;  /* used to set a signal masking set. */

    /* set signal handlers */
  sa.sa_handler = catch_int;
  sa_2.sa_handler = catch_tstp;
  sa_alarm.sa_handler = catch_alrm;

  /* setup mask_set */
  sigfillset(&sa.sa_mask);
  sigfillset(&sa_2.sa_mask);
  sigfillset(&sa_alarm.sa_mask);

  /* ensure that alarm signals are executed if recieved */
  sigdelset(&sa.sa_mask, SIGALRM);
  //sigdelset(&sa_2.sa_mask, SIGALRM);
  //sigdelset(&sa_alarm.sa_mask, SIGALRM);

  //assign sigactions to handled specifc signal numbers
  sigaction(2, &sa, NULL);
  sigaction(18, &sa_2, NULL);
  sigaction(14, &sa_alarm, NULL);

  while(1) {
    pause();
  }

  return 0;
}
