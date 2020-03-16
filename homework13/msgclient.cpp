#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

int main () {
  int myqueue;
  key_t mykey;
  int msgsz = sizeof(int);
  struct Mybuf {
    long mtype;
    int mtext;
  };
  struct Mybuf buf;
  int x;
  mykey = ftok( ".cshrc" , 'a' );
  myqueue = msgget(mykey, IPC_CREAT | 0600 );
  printf("qid: %d\n", myqueue);
  if ( -1 == myqueue) {
    printf("error in msgget");
    return 1;
  }
  for(int i = 0; i < 5; i++) {
    cin >> buf.mtext;
    if (msgsnd(myqueue, (struct msgbuf *)&buf, msgsz, 0) == -1) {
      printf("error in msgsnd %s\n", strerror(errno));
      return 1;
    }
    cin >> buf.mtext;
    if(msgsnd(myqueue, (struct msgbuf *)&buf, msgsz, 0) == -1) {
      printf("error in msgsnd %s\n", strerror(errno));
      return 1;
    }
    if(msgrcv(myqueue, (struct msgbuf *)&buf, msgsz, 0, 0) == -1) {
      printf("error in msgrcv\n");
      return 1;
    }
    printf("%d\n", buf.mtext);
  }
  return 0;
}