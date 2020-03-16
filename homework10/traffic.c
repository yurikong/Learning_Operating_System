#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
//#include <sys/wait.h>

static struct sembuf Wait = {0, -1, SEM_UNDO};
static struct sembuf Signal = {0, 1, SEM_UNDO};

int main(int argc, char* argv[]) {
  key_t mykey = ftok(".cshrc",'x');
  int semid;
  if((semid = semget(mykey, 1, IPC_CREAT | IPC_EXCL | 0600)) != -1)
    semctl(semid, 0, SETVAL, 1);
  else
    semid = semget(mykey, 1, 0600);
  printf("semid: %d\n", semid);
  int count = 0;
  if(fork() == 0) {
    while(count < 5) {
      semop(semid, &Wait, 1);
      printf("N/S car entering intersection\n");
      sleep(1);
      printf("N/S car leaving intersection\n");
      semop(semid, &Signal, 1);
      count++;
    }
  }
  else {
    while(count < 5) {
      semop(semid, &Wait, 1);
      printf("E/W car entering intersection\n");
      sleep(1);
      printf("E/W car leaving intersection\n");
      semop(semid, &Signal, 1);
      count++;
    }
    //wait3(0, 0, NULL);
  }
  return 0;
}
