#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <rpc/des_crypt.h>
int main (int argc, char* argv[]) {
  if(argc != 4)
  {
    printf("Invalid arguments.");
    return -1;
  }
  char *key = "secretkey";
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
  dest_fd = open(argv[2],O_WRONLY|O_CREAT,0777); // opens(creates) the file for the first time
  if(dest_fd < 0)
  {
    printf("Destination file already exists.\n");
    return -1;
  }
  while (( readSize = read(src_fd, filebuf, sizeof(filebuf) - 1)) > 0)
  {
    // I turn it into a string only because I'm using printf;
    // making a copy using "write" would not need this.
    //filebuf[readSize] = '\0';
    if(argv[3][0]=='e') {
      ecb_crypt(key,filebuf,256,DES_ENCRYPT);
    }
    if(argv[3][0]=='d') {
      ecb_crypt(key,filebuf,256,DES_DECRYPT);
    }
    // printf("%s",filebuf);
    write(dest_fd, filebuf, readSize);
  }
  close(src_fd);
  close(dest_fd);
  return 0;
}
