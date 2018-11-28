#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <errno.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include "parse.h"

static void sighandler(int signo) {
  if(signo == SIGINT) {
     // int fd = open("record.txt", O_WRONLY | O_CREAT, 0644);
     // char error[] =  "Exit due to SIGINT \n";
     // int wf = write(fd, error, sizeof(error));
     // close(fd);
     exit(0);
   }
}


int main(int argc, char *argv[]) {
  printf("We sell C-shells by the C-shore \n");
  signal(SIGINT, sighandler);

  while (1) {

    char cwd[100];
    getwd(cwd);
    printf("%s$", cwd);
    fflush(stdout);

    char * line = malloc(1000);
    fgets(line, 1000, stdin);

    char * last;
    while (last == strsep(&line, ";")) {
      printf("%s", last);
      char ** rgs = parse_args(line);
      execvp(getwd(cwd), rgs);
      printf("%s", rgs);
      if (errno) {
          printf("Error: %s \n", strerror(errno));
      }
      last ++;
    }


  }
  return 0;
}
