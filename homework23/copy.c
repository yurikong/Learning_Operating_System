#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <string.h>
int main (int argc, char* argv[]) {
  if(argc != 3)
  {
    printf("Invalid arguments.");
    return -1;
  }
  /* Digest variables */
  EVP_MD_CTX md_context;
  const EVP_MD* md = EVP_md5();
  unsigned int md_length;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  /* Initialize the digest */
  EVP_DigestInit(&md_context, md);
  int src_fd;
  int dest_fd;
  char filebuf[64]; /* a small buffer used to copy the file */
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
    memset(filebuf, '0', readSize);
    filebuf[readSize] = '\0';
    // printf("%s",filebuf);
    write(dest_fd, filebuf, readSize);
    /* Feed data into the digest */
    EVP_DigestUpdate(&md_context, filebuf, readSize);
  }
  close(src_fd);
  close(dest_fd);
  /* Get the digest and it's length */
  EVP_DigestFinal(&md_context, md_value, &md_length);
  /* Print the result */
  for (int i = 0; i < md_length; i++)
  {
    printf("%x",md_value[i]);
  }
  printf("\n");
  return 0;
}
