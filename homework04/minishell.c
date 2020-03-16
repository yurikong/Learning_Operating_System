/*
   The parser piece for the minishell assignment
   Replace the part that prints the parsed command
   with you code that does the fork, exec, wait.
*/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
  char line[40];
  char *nargs[10];
  char *space = " ";
  char *where;
  while(1)
  {
    int i = 0;
    printf("Your command please: ");
    fgets(line,sizeof(line),stdin);
    line[strlen(line)-1] = '\0';
    where = strtok(line,space);
    nargs[i++] = where;
    while (where != NULL)
    {
      where = strtok(NULL,space);
      nargs[i++] = where;
    }
    /* Print the results */
   // printf("Your parsed command was:\n");
   // for (i = 0 ; nargs[i] != NULL; i++)
   // {
   //   printf(" %s\n", nargs[i]);
   // }
    if(fork()==0) {
      execvp(nargs[0], nargs);
    }
    else {
      wait3(0, 0, NULL);
    }
  }
}
