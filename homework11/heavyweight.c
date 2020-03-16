#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

static struct sembuf HeavyWait = {0, -3, SEM_UNDO};
static struct sembuf HeavySignal = {0, 3, SEM_UNDO};

int main(int argc, char* argv[]) {
  key_t key;
  int semid;
  key = ftok(".cshrc", 'x');
  printf("%d\n", key);
  if((semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600)) != -1)
    semctl(semid, 0, SETVAL, 5);
  else
    semid = semget(key, 1, 0600);
  printf("semid: %d\n", semid);
  for(int i = 0; i < 5; i++) {
    if(semop(semid, &HeavyWait, 1) != -1) {
      printf("HeavyWeight Starting\n");
      sleep(4);
      printf("HeavyWeight Ending\n");
      semop(semid, &HeavySignal, 1);
      sleep(8);
    }
  }
  return 0;
}
