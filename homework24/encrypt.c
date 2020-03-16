#include <rpc/des_crypt.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
  char *key = "secretkey";
  char data[50];
  int result;
  int i;
  int datasize;
  strcpy(data, "My message");
  datasize = strlen(data);
  result = ecb_crypt(key,data,datasize,DES_ENCRYPT);
  printf("%d\n",result);
  for (i = 0 ; i < datasize ; i ++)
    printf("%x",data[i]);
  printf("\n");

  result = ecb_crypt(key,data,datasize,DES_DECRYPT);
  printf("%d\n",result);
  for (i = 0 ; i < datasize ; i ++)
    printf("%c",data[i]);
  printf("\n");


  return 0;
}
