#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
  int fd, pos;
  char *filePtr, ch;
  fd=open("mmap.txt",O_RDWR);
  filePtr=mmap(0,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  printf("Position(0-99): ");
  scanf("%d",&pos);
  getchar();
  printf("New character: ");
  scanf("%c",&ch);
  filePtr[pos] = ch;
  munmap(filePtr,100);
  close(fd);
  return 0;
}
