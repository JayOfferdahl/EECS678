/**
 * @file quash.c
 *
 * Quash's main file
 */

/**************************************************************************
 * Included Files
 **************************************************************************/ 
#include "quash.h" // Putting this above the other includes allows us to ensure
                   // this file's headder's #include statements are self
                   // contained.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <readline/readline.h>

static int numJobs;
static struct job jobList[100];

/**************************************************************************
 * Private Variables
 **************************************************************************/
/**
 * Keep track of whether Quash should request another command or not.
 */
// NOTE: "static" causes the "running" variable to only be declared in this
// compilation unit (this file and all files that include it). This is similar
// to private in other languages.
static bool running;

/**************************************************************************
 * Private Functions 
 **************************************************************************/
/**
 * Start the main loop by setting the running flag to true
 */
static void start() {
  running = true;
}

/**************************************************************************
 * Public Functions 
 **************************************************************************/
bool is_running() {
  return running;
}

void terminate() {
  running = false;
}

/**
 * Parses the input string by tokenizing it
 *
 * @param cmdstr the input from the command line
 * @return the number of arguments included in the command
 */
char** parse_cmd(char* cmd, int* numCmds) {
  char** parsed = NULL;
  char* current;
  int spaces = 0;

  current = strtok(cmd, " ");

  while(current) {
    parsed = realloc(parsed, sizeof(char*) * ++spaces);

    if(current == NULL) {
      current++;
      sprintf(current, "%s%s%s", current, " ", strtok(NULL, "\""));
    }

    parsed[spaces - 1] = current;
    *numCmds = (*numCmds) + 1;

    current = strtok(NULL, " =");
  }

  parsed = realloc(parsed, sizeof(char*) * (spaces + 1));
  parsed[spaces] = 0;

  return parsed;
}

/**
 * Prints the current working directory
 */
void pwd() {
  char cwd[1024];
  getcwd(cwd, 1024);

  printf("%s\n", cwd);
}

/**
 * Changes the current working directory to the input directory, or
 * HOME if no directory is given
 *
 * @param target - the input to change directories to
 */
void cd(char* target) {
  // Change directory to the HOME variable
  if(!target) {
    if(chdir(getenv("HOME")) == -1)
      printf("quash: cd: %s: No such file or directory\n", getenv("HOME"));

    pwd();
  }
  else if(chdir(target) == -1)
    printf("quash: cd: %s: No such file or directory\n", target);
    // Print out the current working directory
  else
    pwd();
}

/**
 * Flushes out all the jobs in the job list if they're terminated
 */
void flush_jobs() {
  int i;

  for(i = 0; i < numJobs; i++) {
  	// wait for the process with no hang
  	waitpid(jobList[i].pid, NULL, WNOHANG);
  }
}

/**
 * Prints out all the jobs in the job list
 */
void print_jobs() {
  int i;

  // Flush current jobs
  flush_jobs();

  // Print out all jobs in the job list
  for(i = 0; i < numJobs; i++) {
  	// If the process exists
  	if(kill(jobList[i].pid, 0) == 0)
    	printf("[%i] %i %s\n", jobList[i].jobid, jobList[i].pid, jobList[i].cmd);
  }
}

/**
 * Echos the requested input, if the user wants to view the HOME or PATH 
 * variables, those values are printed out. If the second arguemtn is not
 * one of these variables, all subsequent arguments are printed out
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void echo(char** args, int argCount) {
  int i;
  char* string;

  for(i = 1; i < argCount; i++) {
    if(!strcmp(args[i], "$HOME"))
      string = getenv("HOME");
    else if(!strcmp(args[i], "$PATH"))
      string = getenv("PATH");
    else
      string = args[i];

    printf("%s ", string);
  }

  printf("\n");
}

/**
 * Sets the system variables HOME or PATH to the desired location
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void set(char** args, int argCount) {
  if(argCount < 3)
      printf("quash: set: Too few arguments: Expected 3, received %i\n", argCount);
  else {
    char* var = args[1];
    char* value = args[2];

    // Check for valid input
    if(strcmp(var, "PATH") && strcmp(var, "HOME")) {
      printf("quash: set: %s: Invalid system variable.\nProper usage: set HOME=/.../ or set PATH=/.../\n", var);
      return;
    }

    // Check to make sure it was set correctly
    if(!strcmp(getenv(var), value))
      printf("%s is already set to %s\n", var, value);
    else {
      setenv(var, value, 1);
      printf("%s was set to %s\n", var, value);
    }
  }
}

/**
 * Returns a string of the executable file, should that executable be found
 * within one of the path locations in the PATH system variable.
 *
 * @param cmd - the input argument for this command
 * @return a string of the path to the executable, NULL if not found
 */
char* get_path_exec(char* cmd) { 
  char pathstr[strlen(getenv("PATH"))];
  strcpy(pathstr, getenv("PATH"));

  // Parse out the path variable into strings
  char** PATH = NULL;
  char* current;
  int colons = 0, numPaths = 0;

  current = strtok(pathstr, ":");

  while(current) {
    PATH = realloc(PATH, sizeof(char*) * ++colons);

    PATH[colons - 1] = current;
    numPaths++;

    current = strtok(NULL, ":");
  }

  PATH = realloc(PATH, sizeof(char*) * (colons + 1));
  PATH[colons] = 0;

  char* buffer = (char *) malloc(sizeof(char) * 1024);
  int i;

  for(i = 0; i < sizeof(PATH); i++) {
    sprintf(buffer, "%s/%s", PATH[i], cmd);
    if(access(buffer, X_OK) == 0)
      return buffer;
  }
  return NULL;
}

/**
 * Execute the input function and pipes its output to the input
 * of another function.
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void pipe_exec(char* cmd, int argCount) {
  char **args = NULL;
  char  *path, *rhs;

  // Tokenize the string up until the pipe, and then after it
  path = strtok(cmd, "|");
  rhs = strtok(NULL, "");
  
  // Store the arguments in an array
  args = parse_cmd(path, &argCount);

  // Store the paths of the executables
  path = get_path_exec(args[0]);

  if(!path) {
    if(access(args[0], X_OK) == 0) {
      // Path works (absolute path format)
      path = args[0];
    }
    else {
      printf("quash: %s: command not found...\n", args[0]);
      return;
    }
  }

  pid_t pid_1, pid_2;

  // Setup pipeline
  int fd[2], status;
  pipe(fd);

  pid_1 = fork();
  if(pid_1 == 0) {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);

    // Execute the program
    if(execv(path, args) < 0)
      fprintf(stderr, "\nError executing the first command. ERROR#%d\n", errno);

    exit(0);
  }

  pid_2 = fork();
  if(pid_2 == 0) {
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);

    fflush(stdout);

    // Execute the program
    handle_cmd(rhs);

    exit(0);
  }

  close(fd[0]);
  close(fd[1]);

  if((waitpid(pid_1, &status, 0)) == -1) {
    fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
  }
  if((waitpid(pid_2, &status, 0)) == -1) {
    fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
  }
}

/**
 * Execute the function with its arguments in the background, while keeping track
 * of the job id, process id, and command in a job struct
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments included in args
 */
void execute_in_background(char* cmd, char** args, int argCount) {
  // Chop the "&" off the back of the string
  cmd[strlen(cmd) - 1] = 0;

  pid_t pid;
  pid = fork();

  // Child process
  if(pid == 0) {
    printf("\n[%i] %d\n", numJobs + 1, getpid());

    // Execute the function normally
    handle_cmd(cmd);

    printf("\n[%i] %d finished %s\n", numJobs + 1, getpid(), args[0]);

    exit(0);
  }
  // Parent process
  else {
    struct job newJob = {
      .pid = pid,
      .jobid = numJobs + 1,
      .cmd = args[0]
    };

    jobList[++numJobs - 1] = newJob;
    
    // Try to flush here
    while(waitpid(pid, NULL, WEXITED | WNOHANG) > 0) {}
  }
}

/**
 * Execute the function with its arguments
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void execute(char** args, int argCount) {
  int status;
  char* buffer = NULL;

  // If in absolute path format, try this directory
  if(args[0][0] == '.') {
    // If the file is executable here, put it in the buffer
    if(access(args[0], X_OK) == 0)
      buffer = args[0];
  }
  // If we're not in absolute path format, search the PATH variable
  else {
    // Look for accessible executables in the PATH
    buffer = get_path_exec(args[0]);
  }
    
  if(buffer) {
    pid_t pid;
    pid = fork();

    if(pid == 0) {
        if(execv(buffer, args) < 0) {
          fprintf(stderr, "\nError executing funtion. ERROR#%d\n", errno);
        }
    }
    else if((waitpid(pid, &status, 0)) == -1) {
      fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    }
  }
  else
    printf("quash: %s: command not found...\n", args[0]); 
}

/**
 * Implements I/O redirection to write or read command output to/from a file
 *
 * @param cmd - the command string inputted for this command
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void ioRedirect(char* cmd, char ** args, int argCount)
{
  if (strchr(cmd, '<') != NULL && strchr(cmd, '>') != NULL)
  {
    int status;

    // Get fully qualified path to file to be written to 
    char inFileName[1024];
    getcwd(inFileName, 1024);
    strcat(inFileName, "/");

    char outFileName[1024];
    getcwd(outFileName, 1024);
    strcat(outFileName, "/");

    char *tokenizedCmd;
    tokenizedCmd = strtok(cmd, " ");

    char temp[1024] = "";

    while (tokenizedCmd != NULL)
    {
      if (strcmp(tokenizedCmd, "<") == 0)
      {
        tokenizedCmd = strtok(NULL, " ");
        strcat (inFileName, tokenizedCmd);
        tokenizedCmd = strtok(NULL, " ");
        tokenizedCmd = strtok(NULL, " ");
        strcat(outFileName, tokenizedCmd);
        tokenizedCmd = NULL;
      }
      else
      {
        strcat(temp, tokenizedCmd);
        strcat(temp, " ");
        tokenizedCmd = strtok(NULL, " ");
      }
    }

    pid_t pid;
    pid = fork();

    // child process
    if (pid == 0)
    {
      // File is open to write to
      freopen(outFileName, "w", stdout);

      freopen(inFileName, "r", stdin);

      handle_cmd(temp);

      fclose(stdin);

      // Close stdout to file
      fclose(stdout);
      
      exit(0);
    }
    else if((waitpid(pid, &status, 0)) == -1) {
      fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    }

  }
  // Redirect standard input from a file
  else if (strchr(cmd, '<') != NULL)
  {
    int status;

    // Get fully qualified path to file to be written to 
    char fileName[1024];
    getcwd(fileName, 1024);
    strcat(fileName, "/");
    
    // Get number of args on LHS of the carrot
    char *tokenizedCmd;
    tokenizedCmd = strtok(cmd, " ");

    char temp[1024] = "";

    while (tokenizedCmd != NULL)
    {
      if (strcmp(tokenizedCmd, "<") == 0)
      {
        tokenizedCmd = strtok(NULL, " ");
        strcat (fileName, tokenizedCmd);
        tokenizedCmd = NULL;
      }
      else
      {
        strcat(temp, tokenizedCmd);
        strcat(temp, " ");
        tokenizedCmd = strtok(NULL, " ");
      }
    }

    pid_t pid;
    pid = fork();

    // child process
    if (pid == 0)
    {
      // File is open to write to
      freopen(fileName, "r", stdin);

      handle_cmd(temp);

      // Close stdin
      fclose(stdin);
      exit(0);
    }
    else if((waitpid(pid, &status, 0)) == -1) {
      fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    }
  }
  // Redirect standard output to a file
  else
  {
    int status;
    
    // Get fully qualified path to file to be written to 
    char fileName[1024];
    getcwd(fileName, 1024);
    strcat(fileName, "/");
    
    // Get number of args on LHS of the carrot
    char *tokenizedCmd;
    tokenizedCmd = strtok(cmd, " ");

    char temp[1024] = "";

    while (tokenizedCmd != NULL)
    {
      if (strcmp(tokenizedCmd, ">") == 0)
      {
        tokenizedCmd = strtok(NULL, " ");
        strcat (fileName, tokenizedCmd);
        tokenizedCmd = NULL;
      }
      else
      {
        strcat(temp, tokenizedCmd);
        strcat(temp, " ");
        tokenizedCmd = strtok(NULL, " ");
      }
    }

    pid_t pid;
    pid = fork();

    // child process
    if (pid == 0)
    {
      // File is open to write to
      freopen(fileName, "w", stdout);

      handle_cmd(temp);

      // Close stdout to file
      fclose(stdout);
      exit(0);

    }
    else if((waitpid(pid, &status, 0)) == -1) {
      fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
    }
  }
}

/**
 * Kills the selected job if it exists in the background
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments included in args
 */
void killProcess(char** args, int argCount) {
  int i;
  char buffer[256];

  // Print out all jobs in the job list
  for(i = 0; i < numJobs; i++) {
    sprintf(buffer, "%i", jobList[i].jobid);
    if(!strcmp(args[2], buffer)) {
      // If the process exists
      if(kill(jobList[i].pid, 0) == 0) {
        if(kill(jobList[i].pid, strtoumax(args[1], NULL, 10)) == -1)
          fprintf(stderr, "Killing encountered an error: ERROR%d\n", errno);
        else
      	  printf("Killed process %i (jobid: %i)\n", jobList[i].pid, jobList[i].jobid);
      }
      else
      	printf("The requested job does not exist.\n");
    }
  }
}

/**
 * Handles the input command by tokenizing the string and executing functions
 * based on the input
 *
 * @param cmd the input from the command line
 */
void handle_cmd(char* cmd) {
  // Copy the command string so cmd stays intact.
  char* temp = (char *) malloc(sizeof(char *) * strlen(cmd));
  strcpy(temp, cmd);

  // Parse the string for the argument list
  int argCount = 0;
  char** args = parse_cmd(temp, &argCount);

  if(argCount == 0)
    return;

  // Main handler
  // Quit/Exit
  if(!strcmp(args[0], "exit") || !strcmp(args[0], "quit")) {
    puts("Exiting...");
    terminate(); // Exit quash
  }
  // If the last argument is &, run this in the background
  else if(!strcmp(args[argCount - 1], "&")) {
    if(argCount == 1)
      printf("quash: & must be used after a command\n");
    else
      execute_in_background(cmd, args, argCount);
  }
  // File I/O redirection
  else if(strchr(cmd, '<') != NULL || strchr(cmd, '>') != NULL)
  {
    ioRedirect(cmd, args, argCount);
  }
  // Search for pipes
  else if(strchr(cmd, '|') != NULL) {
    if(argCount <= 2)
      printf("quash: cannot pipe to null\n");
    else
      pipe_exec(cmd, argCount);
  }
  // Print working directory (with args)
  else if(!strcmp(args[0], "pwd")) {
    pwd();
  }
  // Change directory (with/without args)
  else if(!strcmp(args[0], "cd")) {
    cd(args[1]);
  }
  // Echo 
  else if(!strcmp(args[0], "echo")) {
    echo(args, argCount);
  }
  // Set home/path variables
  else if(!strcmp(args[0], "set")) {
    set(args, argCount);
  }
  // Print out all current jobs
  else if(!strcmp(args[0], "jobs")) {
    print_jobs();
  }
  // Kill the input process id
  else if(!strcmp(args[0], "kill")) {
    if(argCount != 3)
      printf("quash: kill: invalid number of arguments: 3 expected, %i received\n", argCount);
    else
      killProcess(args, argCount);
  }
  // Else, try to execute it
  else {
    execute(args, argCount);
  }
}

/**
 * Trims leading and trailing whitespace from the input string
 *
 * @param cmd - the string to be trimmed
 */
void trim(char* cmd) {
  if(cmd) {
    // Move pointer from front of string to first legit character
    while(isspace(*cmd))
      cmd++;

    // The string is empty
    if(*cmd == 0)
      return;

    // Find the back of the string
    char* back = cmd + strlen(cmd) - 1;

    // Move back pointer from back of string to last legit character
    while(isspace(*back) && back > cmd) 
      back--;

    // Terminate the string
    *(back + 1) = 0;
  }
}

/**
 * Quash entry point
 *
 * @param argc argument count from the command line
 * @param argv argument vector from the command line
 * @return program exit status
 */
int main(int argc, char** argv) {
  start();

  char* cmd, line[1024];
  numJobs = 0;
  
  puts("\nWelcome to Quash!");
  puts("Type \"exit\" or \"quit\" to quit\n");
  // Main execution loop
  while (is_running()) {
    snprintf(line, sizeof(line), "quash$> ");

    cmd = readline(line);
    if(*cmd) {
      // Trim down the command (leading & trailng white space)
      trim(cmd);
      handle_cmd(cmd);
    }

    // Flush current jobs
  	flush_jobs();
  }

  return EXIT_SUCCESS;
}
