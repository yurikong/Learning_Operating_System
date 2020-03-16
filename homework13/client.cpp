#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main () {
  int result;
  int fd;
  int x, y, ans;
  result = mkfifo("fifo.pipe",0600);
  for(int i = 0; i < 5; i++) {
    fd = open("fifo.pipe", O_WRONLY);
    cin >> x;
    write(fd, &x, sizeof(int));
    cin >> y;
    write(fd, &y, sizeof(int));
    close(fd);
    fd = open("fifo.pipe", O_RDONLY);
    read(fd, &ans, sizeof(int));
    printf("%d\n", ans);
    close(fd);
  }
  return 0;
}
