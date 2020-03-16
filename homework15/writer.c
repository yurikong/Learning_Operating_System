#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>

static struct sembuf Wait[2] = {{0, -1, SEM_UNDO}, {1, -3, SEM_UNDO}};
static struct sembuf Signal[2] = {{0, 1, SEM_UNDO}, {1, 3, SEM_UNDO}};
static struct sembuf OpList[2];

int main(int argc, char* argv[]) {
  key_t key;
  int semid, shmid;
  char *mem;
  key = ftok(".cshrc", 'x');
  printf("key: %d\n", key);
  if((semid = semget(key, 4, IPC_CREAT | IPC_EXCL | 0600)) != -1) {
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 3);
    semctl(semid, 2, SETVAL, 1);
    semctl(semid, 3, SETVAL, 0);
  }
  else
    semid = semget(key, 4, 0600);
  if((shmid = shmget(key, 3, IPC_CREAT | IPC_EXCL | 0600)) != -1) {
    mem = shmat(shmid, 0, 0);
    mem[0] = mem[1] = mem[2] = 1;
  } else {
    shmid = shmget(key, 3, IPC_CREAT | 0600);
    mem = shmat(shmid, 0, 0);
  }
  for(int i = 0; i < 5; i++) {
    OpList[0] = Wait[1];
    OpList[1] = Wait[0];
    if(semop(semid, OpList, 2) != -1) {
      mem[0] += 1;
      sleep(1);
      mem[1] += 1;
      sleep(1);
      mem[2] += 1;
      sleep(3);
      OpList[0] = Signal[0];
      OpList[1] = Signal[1];
      semop(semid, OpList, 2);
    }
  }
  shmdt(mem);
  shmctl(shmid, IPC_RMID, 0);
  return 0;
}
