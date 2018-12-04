# C-shell by the C-shore
### Joshua Weiner
Shell project01 for Systems Level Programming with Mr. DW

## Functionality:
  - Successfully forks commands
  - Displays users current directory
  - Allows for changing of directory through the cd command
  - Can exit the shell or any of its processes using exit
  - Simple pipes work
  - Redirects work using <, >
  - SIGINT works
  - Parses multiple commands delineated by semicolons
  - Fancy text coloring because what else should I be concerned with early in the morning?
  - Child processes spawned with "&" symbol

## Issues / Not achieved
  - Using arrow keys to maneuver among text inputted by the user
  - Had one issue where in the redirect out test, the shell would not exit the process. This created a several-gigabyte sized file, and then would eat up resources when using cat on the file. Issue seems to have been resolved.
  - Was not able to resolve double redirects
  - For whatever reason, cd commands into directories will be executed successfully, then spawned as a child process and fail.
  - Will throw errors with multiple semicolons. This function WAS working in an earlier version, yet for whatever reason it no longer works. Was unable to diagnose the issue.

### Sources Used for Reference
  - [C Tutorial: Pipes - Rutgers University](https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html)
  - [Using dup2 for I/O Redirection and Pipes](http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html)

### Functions
  - `static void sighandler(int signo);`
    - Takes signal in the form of an integer (SIGINT in this case)
    - Exits process
  - `void child_process(char ** rgs);`
    - Runs child process of parent
    - Waits until child process completes or fails
  - `void exec_args(int pipes[], int infileno, char * cmd, char ** rgs, int i, int file_descriptor);`
    - Executes the arguments in array rgs
    - First tests if there is a valid command
    - Then tests if there is a valid file descriptor (if there is a piping function, for example)
  - `void redirect_out(char * item, int * fd);`
    - Redirects output from the shell to a file
    - File name specified in `item`, file descriptor specified in `fd`
  - `void redirect_in(char * item, int * fd);`
    - Redirects input from a file into the shell
    - File name specified in `item`, file descriptor specified in `fd`
  - `void piping(char * command, int pipes[], int file_descriptor, int infileno);`
    - Creates and executes pipes with the file given by `file_descriptor`
    - Reliant on STDIN_FILENO passed by `infileno`
    - Writes from `pipes[1]` to read from `pipes[0]`
  - `void iterate_commands_semicolon(char * line, char * address_of_line, char * iter_cmd);`
    - Iterates through shell commands as delineated by semicolons
    - (Used to) but does not work with multiple semicolons in a row, throws a Segmentation Fault error
