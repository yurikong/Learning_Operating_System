#include <stdlib.h> /* Needed for fork */
#include <stdio.h> /* Needed for printf. */
#include <stdbool.h> /* Needed for bool, optional. */
#include <unistd.h> /* Needed for fork and exit. */
#include <sys/wait.h>

int sum;
int main (int argc, char* argv[]) {
  int i;
  sum = 0;
  int pid;
  printf("sleeping\n");
  sleep(5);
  pid = fork();  /* create a new process */
  for (i=1; i<=10 ; i++) {
    sum += i;
    if(pid) {
      int c = getchar();
      printf("P sum is %d %c\n", sum, c);
      sleep(1);
    }
    else {
      int c = getchar();
      printf("C sum is %d %c\n", sum, c);
      sleep(2);
    }
    fflush(stdout);
    //if(argv[1] && pid) {
      //wait3(&pid, 0, NULL);
    //}
  }
  if(argc > 1 && pid) {
    wait3(&pid, 0, NULL);
  }
  return 0;
}
