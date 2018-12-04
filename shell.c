#include "parse_and_run.h"

static void sighandler(int signo){

  if(signo == SIGINT) {
     int fd = open("record.txt", O_WRONLY | O_CREAT, 0644);
     char error[] =  "Exit due to SIGINT \n";
     int wf = write(fd, error, sizeof(error));
     close(fd);
     exit(0);
  }
}



int main() {
    printf("\e[33mWe sell C-shells by the C-shore: Welcome to Josh's shell!!!! \n");
    signal(SIGINT, sighandler);

    while (1) {

        //Prints out new input line with current working directory
        char cwd[PATH_MAX]; //4096 bytes allocated to cwd
        getcwd(cwd, sizeof(cwd));
        printf("\e[34m%s\e[31m(C-Shell)\e[37m$ ", cwd);
        fflush(stdout);

        char *line = malloc(1000);
        char *start = line;
        char *iter_cmd;
        fgets(line, 1000, stdin);
        line[strlen(line) - 1] = '\0';

        iterate_commands_semicolon(line, start, iter_cmd);
    }
    return 0;
}
