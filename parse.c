#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

char ** parse_args( char * line ){
  char** rgs = calloc(strlen(line), sizeof(char**));
  for(int i = 0; line; i++) {

    rgs[i] = strsep(&line, " ");

  }

  return rgs;
}
