#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

int main() {
  int toParent[2], toChild[2];
  pipe(toParent); pipe(toChild);
  if(fork()==0) {
    close(toParent[0]);
    close(toChild[1]);
    for(int i = 0; i < 5; i++) {
      int x,y;
      read(toChild[0],&x,sizeof(int));
      read(toChild[0],&y,sizeof(int));
      int r=x*y;
      write(toParent[1],&r,sizeof(int));
    }
  } else {
    close(toChild[0]);
    close(toParent[1]);
    for(int i = 0; i < 5; i++) {
      int x,y,num;
      cin >> x;
      write(toChild[1],&x,sizeof(int));
      cin >> y;
      write(toChild[1],&y,sizeof(int));
      read(toParent[0],&num,sizeof(int));
      cout << num << endl;
    }
  }
  return 0;
}
