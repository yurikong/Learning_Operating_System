#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void catch(int sig)
{
  printf("Got signal %d\n",sig);
}

void ring(int sig)
{
  printf("Ring Ring\n");
}

int main ()
{
  int i;
  //signal(SIGINT, catch);
  struct sigaction cact;
  cact.sa_handler = catch;
  sigaction(SIGINT, &cact, NULL);
  //signal(SIGALRM, ring);
  struct sigaction ract;
  ract.sa_handler = ring;
  sigaction(SIGALRM, &ract, NULL);
  alarm(6);
  for (i=0;i<20;i++) {
    printf("%d\n",i);
    sleep(1);
  }
  return 0;
}

/* control c prints Got signal 2 */
/* after 6 seconds the program stops (Alarm clock) */