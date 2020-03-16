#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

static struct sembuf Wait[4] = {{0, -1, SEM_UNDO}, {1, -1, SEM_UNDO}, {2, -1, SEM_UNDO}, {3, -1, SEM_UNDO}};
static struct sembuf Signal[4] = {{0, 1, SEM_UNDO}, {1, 1, SEM_UNDO}, {2, 1, SEM_UNDO}, {3, 1, SEM_UNDO}};
static struct sembuf OpList[2];

int main(int argc, char* argv[]) {
  key_t key;
  int semid;
  int counter;
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
    OpList[1] = Wait[2];
    if(semop(semid, OpList, 2) != -1) {
      counter = semctl(semid, 3, GETVAL, 0);
      if(counter == 0) {
        OpList[0] = Wait[0];
        semop(semid, OpList, 1);
      }
      counter++;
      semctl(semid, 3, SETVAL, counter);
      OpList[0] = Signal[2];
      OpList[1] = Signal[1];
      semop(semid, OpList, 2);
    }
    printf("Reading\n");
    sleep(2);
    printf("Done reading\n");
    OpList[0] = Wait[2];
    if(semop(semid, OpList, 1) != -1) {
      counter = semctl(semid, 3, GETVAL, 0);
      counter--;
      semctl(semid, 3, SETVAL, counter);
      if(counter == 0) {
        OpList[0] = Signal[0];
        semop(semid, OpList, 1);
      }
      OpList[0] = Signal[2];
      semop(semid, OpList, 1);
    }
    sleep(4);
  }
  return 0;
}
