#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
  int s;
  char message[80];
  struct sockaddr_in      srv_addr;
  /* Get a socket. */
  s = socket(PF_INET, SOCK_STREAM, 0);
  /* Put server's address into a socket structure */
  //memset(&srv_addr, 0, sizeof(srv_addr));

  // change server address from 134.139.248.2 to 134.139.248.3
  srv_addr.sin_addr.s_addr = inet_addr("134.139.248.3");
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(7); // change port from 13 to 7
  /* Request the connection to the server */
  connect(s, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
  /*Get time from serve*/
  // set the message
  strcpy(message, "This is at least 16 characters long.\n");
  write(s, message, 80); // write the message to the echo server
  read(s, message, 80);
  printf("%s",message);
  close(s);
  return 0;
}
