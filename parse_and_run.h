#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

static void sighandler(int signo);
// char ** parse_args( char * line );
void child_process(char ** rgs);
void exec_args(int pipes[], int infileno, char * cmd, char ** rgs, int i, int file_descriptor);
void redirect_out(char * item, int * fd);
void redirect_in(char * item, int * fd);
void piping(char * command, int pipes[], int file_descriptor, int infileno);
void iterate_commands_semicolon(char * line, char * address_of_line, char * iter_cmd);
