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

#include <string.h>
#include <ctype.h>
#include <unistd.h>

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
 * Echos the requested input, if the user wants to view the HOME or PATH 
 * variables, those values are printed out. If the second arguemtn is not
 * one of these variables, all subsequent arguments are printed out
 *
 * @param argCount - the number of arguments inputed for this command
 * @param args - the list of arguments inputed for this command
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
 * @param args - the list of arguments inputed for this command
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
 * Execute the function with its arguments
 *
 * @param args - the list of arguments inputed for this command
 */
void execute(char** args, int argCount) {
  // If we're not in absolute path format, search the PATH variable
  if(args[0][0] != '.') {


    printf("quash: %s: command not found...\n", args[0]);
  }
  else {
    printf("quash: %s: No such file or directory\n", args[0]);
  }  
}

/**
 * Handles the input command by tokenizing the string and executing functions
 * based on the input
 *
 * @param cmdstr the input from the command line
 */
void handle_cmd(char* cmd) {
  int argCount = 0;
  char** args = parse_cmd(cmd, &argCount);

  if(argCount == 0)
    return;

  // Main handler
  // Print working directory (with args)
  if(!strcmp(args[0], "pwd")) {
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
  // Quit/Exit
  else if(!strcmp(args[0], "exit") || !strcmp(args[0], "quit")) {
    puts("Exiting...");
    terminate(); // Exit quash
  }
  // Else, try to execute it
  else {
    execute(args, argCount);
  }
}

/**
 * Trims leading and trailing whitespace from the input string
 *
 * @param input - the string to be trimmed
 */
void trim(char* cmd) {
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
  }

  return EXIT_SUCCESS;
}
