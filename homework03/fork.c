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
  pid = fork();  /* create a new process */
  for (i=1; i<=10 ; i++) {
    sum += i;
    if(pid) {
      printf("P sum is %d\n", sum);
      sleep(1);
    }
    else {
      printf("C sum is %d\n", sum);
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
