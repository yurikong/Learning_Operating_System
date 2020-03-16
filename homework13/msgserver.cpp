#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

int main () {
  int myqueue;
  key_t mykey;
  int msgsz = sizeof(int);
  struct Mybuf {
    long mtype;
    int mtext;
  };
  struct Mybuf buf;
  int x, y;
  mykey = ftok( ".cshrc" , 'a' );
  myqueue = msgget(mykey, IPC_CREAT | 0600 );
  printf("qid: %d\n", myqueue);
  if ( -1 == myqueue) {
    printf("error in msgget\n");
    return 1;
  }
  for(int i = 0; i < 5; i++) {
    if(msgrcv(myqueue, (struct msgbuf *)&buf, msgsz, 0, 0) == -1) {
      printf("error in msgrcv\n");
      return 1;
    }
    x = buf.mtext;
    if(msgrcv(myqueue, (struct msgbuf *)&buf, msgsz, 0, 0) == -1) {
      printf("error in msgrcv\n");
      return 1;
    }
    y = buf.mtext;
    buf.mtext = x * y;
    if(msgsnd(myqueue, (struct msgbuf *)&buf, msgsz, 0) == -1) {
      printf("error in msgsnd\n");
      return 1;
    }
  }
  return 0;
}
