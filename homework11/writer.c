#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

static struct sembuf Wait[2] = {{0, -1, SEM_UNDO}, {1, -3, SEM_UNDO}};
static struct sembuf Signal[2] = {{0, 1, SEM_UNDO}, {1, 3, SEM_UNDO}};
static struct sembuf OpList[2];

int main(int argc, char* argv[]) {
  key_t key;
  int semid;
  key = ftok(".cshrc", 'x');
  printf("%d\n", key);
  if((semid = semget(key, 4, IPC_CREAT | IPC_EXCL | 0600)) != -1) {
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 3);
    semctl(semid, 2, SETVAL, 1);
    semctl(semid, 3, SETVAL, 0);
  }
  else
    semid = semget(key, 4, 0600);
  for(int i = 0; i < 5; i++) {
    OpList[0] = Wait[1];
    OpList[1] = Wait[0];
    if(semop(semid, OpList, 2) != -1) {
      printf("Writing\n");
      sleep(4);
      printf("Done writing\n");
      OpList[0] = Signal[0];
      OpList[1] = Signal[1];
      semop(semid, OpList, 2);
      sleep(8);
    }
  }
  return 0;
}