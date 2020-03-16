#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
int main (int argc, char* argv[]) {
  printf("uid: %d\n", getuid());
  printf("euid: %d\n", geteuid());
  if(argc != 2)
  {
    printf("Invalid arguments.");
    return -1;
  }
  int src_fd;
  int dest_fd;
  char filebuf[256]; /* a small buffer used to copy the file */
  ssize_t readSize;

  src_fd = open(argv[1],O_RDONLY);
  if(src_fd < 0)
  {
    printf("Source file does not exist.\n");
    return -1;
  }
  dest_fd = open("/home/017547307/xfer.tmp",O_WRONLY|O_CREAT,0777); // opens(creates) the file for the first time
  if(dest_fd < 0)
  {
    printf("Destination file already exists.\n");
    return -1;
  }
  while (( readSize = read(src_fd, filebuf, sizeof(filebuf) - 1)) > 0)
  {
    // I turn it into a string only because I'm using printf;
    // making a copy using "write" would not need this.
    filebuf[readSize] = '\0';
//    printf("%s",filebuf);
    write(dest_fd, filebuf, readSize);
  }
  close(src_fd);
  close(dest_fd);
  return 0;
}
