#include "parse_and_run.h"

// ----------- sighandler -------------- //
// Takes signal in the form of an integer (SIGINT in this case)
// Exits the process
// -----------            -------------- //
static void sighandler(int signo){
  if(signo == SIGINT) {
     int fd = open("record.txt", O_WRONLY | O_CREAT, 0644);
     char error[] =  "Exit due to SIGINT \n";
     int wf = write(fd, error, sizeof(error));
     close(fd);
     exit(0);
  }
}

// ----------- child_process -------------- //
// Runs child process of parent
// Waits until child process completes or fails
// -----------            -------------- //
void child_process(char ** rgs) {
  int status;
  wait(&status);
  if (WEXITSTATUS(status)) {
    printf("Process %s failed\n", rgs[0]);
  }
  // exit(1);
}

// ----------- exec_args -------------- //
// Executes the arguments in array rgs
// First tests if there is a valid command
// Then tests if there is a valid file descriptor (if there is a piping function, for example)
// -----------            -------------- //
void exec_args(int pipes[], int infileno, char * cmd, char ** rgs, int i, int file_descriptor) {
  if (!fork()) {
      if (cmd) {
        dup2(pipes[1], STDOUT_FILENO);
      }
      close(pipes[0]);
      if(!file_descriptor) {
        dup2(infileno, STDIN_FILENO);
      }
      execvp(rgs[0], rgs);
      exit(1);
  }
  signal(SIGINT, sighandler);
  if (strcmp(rgs[i - 1], "&")) {
    child_process(rgs);
  }
  signal(SIGINT, sighandler);
  close(pipes[1]);
  infileno = pipes[0];
}

// ----------- redirect_out -------------- //
// Redirects output from the shell to a file
// File name specified in `item`, file descriptor specified in `fd`
// -----------            -------------- //
void redirect_out(char * item, int * fd) {
  *fd = open(item, O_CREAT| O_WRONLY, 0777);
  dup2(*fd, STDOUT_FILENO);
  close(0);
}

// ----------- redirect_in -------------- //
// Redirects input from a file into the shell
// File name specified in `item`, file descriptor specified in `fd`
// -----------            -------------- //
void redirect_in(char * item, int * fd) {
  // printf("%d", *fd);
  *fd = open(item, O_RDONLY, 0700);
  dup2(*fd, STDIN_FILENO);
  close(*fd);
}

// EXITS PROCESS
// HAD TO COMMENT OUT BECAUSE IT WAS FAULTY FOR SOME REASON
// void exit_function(char * rgs, char * address_of_line, int pipes[]) {
//  free(address_of_line);
//  free(rgs);
//  close(pipes[0]);
//  close(pipes[1]);
//  exit(0);
// }

// ----------- piping -------------- //
// Creates and executes pipes with the file given by `file_descriptor`
// Reliant on STDIN_FILENO passed by `infileno`
// Writes from `pipes[1]` to read from `pipes[0]`
// -----------            -------------- //
void piping(char * command, int pipes[], int file_descriptor, int infileno) {
  if (command) {
   dup2(pipes[1], STDOUT_FILENO);
  }
  close(pipes[0]);

  if(!file_descriptor) {
    dup2(infileno, STDIN_FILENO);
  }
}

// ----------- iterate_commands_semicolon -------------- //
// Iterates through shell commands as delineated by semicolons
// (Used to) but does not work with multiple semicolons in a row, throws a Segmentation Fault error
// Was helped out in person by Crystal Wang, she showed me some useful items to add and explained some key functionality. Especially helpful because of my lacking a partner. Noted in the function where she gave the most input
// -----------            -------------- //
void iterate_commands_semicolon(char * line, char * address_of_line, char * iter_cmd) {
  // Testing passed line
  // printf("Line: %s", line);
  while ((iter_cmd = strsep(&line, ";"))) {
    //Testing strsep --> printing iterated command
    // printf("command %s", iter_cmd);
      char *cmd = iter_cmd;

      int pipes[2];
      int infileno = STDIN_FILENO;
      //Iterate through pipies
      while ((iter_cmd = strsep(&cmd, "|"))) {
          pipe(pipes);
          char **rgs = calloc(100, sizeof(char *));
          int fd = 0;
          int i = 0;
          // Duplicate filedescriptors, will allow us to write to and read from files successfully
          int fdout = dup(STDOUT_FILENO);
          int fdin = dup(STDIN_FILENO);
          int fderr = dup(STDERR_FILENO);
          //Iterate theough the commands
          for (; iter_cmd; i ++) {
              //Ignore extra whitespace
              while (*iter_cmd == ' ') {
                iter_cmd++;
              }
              //TESTING BASIC REDIRECTS
              rgs[i] = strsep(&iter_cmd, " "); //splits command by spaces
              // printf("%s", iter_cmd);
              //output to file
              if (i && !strcmp(rgs[i - 1], ">")) {
                redirect_out(rgs[i], &fd);
                rgs[i - 1] = NULL;
              }
              //input to file
              else if (i && !strcmp(rgs[i - 1], "<")) {
                redirect_in(rgs[i], &fd);
                rgs[i - 1] = NULL;
              }
          } //endfor

          if (!*(rgs[i - 1])) {
            rgs[(i--) - 1] = NULL;
          }
          //Ignore blank commands
          //This allows for iteration over sequential semicolons or commas, for example.
          if (!*(rgs[0])) {
            continue;
          } //Thank you to Crystal Wang for explaining these two to me, I did not understand their necessity but she really helped me out

          //exit program --> THIS WAS ORIGINALLY A SEPARATE FUNCTION YET I RAN INTO ISSUES (DETAILED IN THE README)
          if (!strcmp(rgs[0], "exit")) {
            exit(0);
          }

          //change directory
          if (!strcmp(rgs[0], "cd")) {
            chdir(rgs[1]);
          }
          //Execute arguments. Same issue that I had earlier with exiting the process I had with creating a separate function for execution. Was able to successfulyl create a separate piping function though.

          else {
            // exec_args(pipes, infileno, cmd, rgs, i, fd);
            // ^ no longer worked, so commented out
            int child = fork();
            //fork process
            if (!child) {
                // piping function
                piping(cmd, pipes, fd, infileno);
                execvp(rgs[0], rgs);
                exit(1);
            }

            signal(SIGINT, sighandler);
            //spawns a child process if there is an & symbol, so that operations can continue in the shell
            if (strcmp("&", rgs[i - 1])) {
              child_process(rgs);
            }

            signal(SIGINT, sighandler);
            close(pipes[1]);
            infileno = pipes[0];
          }
          close(fd);
          //Redirect console output
          dup2(fdout, STDOUT_FILENO);
          dup2(fdin, STDIN_FILENO);
          dup2(fderr, STDERR_FILENO);
      }
  }
  return;
  }
