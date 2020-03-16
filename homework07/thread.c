#include <stdlib.h> /* Needed for fork */
#include <stdio.h> /* Needed for printf. */
#include <stdbool.h> /* Needed for bool, optional. */
#include <unistd.h> /* Needed for fork and exit. */
#include <sys/wait.h>
#include <pthread.h>

int sum;
int i;
void ChildThread() {
  int i;
  for (i=1; i<=10; i++) {
    sum += i;
    int c = getchar();
    printf("C sum is %d %c\n", sum, c);
    //sleep(2);
    fflush(stdout);
  }
}

int main (int argc, char* argv[]) {
  sum = 0;
  printf("sleeping\n");
  sleep(5);
  pthread_t th;
  pthread_attr_t ta;
  (void) pthread_attr_init(&ta);
  (void) pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
  pthread_create(&th, &ta, (void * (*)(void *))ChildThread, NULL);
  for (i=1; i<=10 ; i++) {
    sum += i;
    int c = getchar();
    printf("P sum is %d %c\n", sum, c);
    //sleep(1);
    fflush(stdout);
    //if(argv[1] && pid) {
      //wait3(&pid, 0, NULL);
    //}
  }
  /*
  if(argc > 1 && pid) {
    wait3(&pid, 0, NULL);
  }
  */
  return 0;
}