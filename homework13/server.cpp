#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main () {
  int result;
  int fd;
  int x, y, ans;
  result = mkfifo("fifo.pipe", 0600);
  while(1) {
    fd = open("fifo.pipe", O_RDONLY);
    read(fd, &x, sizeof(int));
    read(fd, &y, sizeof(int));
    ans = x * y;
    close(fd);
    fd = open("fifo.pipe", O_WRONLY);
    write(fd, &ans, sizeof(int));
    close(fd);
  }
  return 0;
}
