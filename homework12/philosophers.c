/* You need to switch from 2 to 5 semaphores.
   Initialize the 5 semaphores to unlocked.
   You still only need two semaphores in the OpList.
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
int main () {
  struct sembuf Wait[2]
   = { {0,-1,SEM_UNDO}, {1,-1,SEM_UNDO} };
  struct sembuf Signal[2]
   = { {0,1,SEM_UNDO}, {1,1,SEM_UNDO} };
  int ch, i, pid;
  key_t mykey;
  int semid;
  union wait waitstatus;
  struct sembuf OpList[2];

  mykey = ftok("./a.out",'a');
  semid = semget(mykey, 2, IPC_CREAT | 0600 );
  semctl(semid,0,SETVAL,0);
  semctl(semid,1,SETVAL,0);
  if (fork()==0) { /* child */
    OpList[0] = Wait[0];
    OpList[1] = Wait[1];
    printf("Waiting\n");
    semop(semid,OpList,2);
    printf("Unlocked\n");
  } else { /* parent*/
    sleep(3);
    OpList[0] = Signal[0];
    OpList[1] = Signal[1];
    semop(semid,OpList,2);
    wait3(&ch,0,NULL);
  }
  for (i = 0 ; i < 5 ; i++ )
  {
    pid = fork();
    if (pid == 0) break;
  }
  if (pid == 0)
  {
    pid = getpid();
    /* do the philosopher thing */
    return 0;
  }
  else
  {
    for (i = 0 ; i < 5 ; i++){
      wait3(&ch,0,NULL);
    }
    /* remove the semaphore */
    return 0;
  }
}