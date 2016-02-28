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

bool get_command(command_t* cmd, FILE* in) {
  if (fgets(cmd->cmdstr, MAX_COMMAND_LENGTH, in) != NULL) {
    size_t len = strlen(cmd->cmdstr);
    char last_char = cmd->cmdstr[len - 1];

    if (last_char == '\n' || last_char == '\r') {
      // Remove trailing new line character.
      cmd->cmdstr[len - 1] = '\0';
      cmd->cmdlen = len - 1;
    }
    else
      cmd->cmdlen = len;
    
    return true;
  }
  else
    return false;
}

/**
 * Parses the input string and splits it up based on whitespace
 *
 * @param cmdstr the input from the command line
 * @return the number of arguments included in the command
 */
int parse_cmd(command_t* cmd, char** args, int numCmds) {
  int length = cmd->cmdlen, i, argStart, argEnd, endOfString, argCount;
  argStart = argEnd = endOfString = argCount = 0;

  char* string = (char *) malloc(length);

  // Temporarily store the string
  strcpy(string, cmd->cmdstr);

  // Empty command
  if(cmd->cmdlen == 0)
    return argCount;

  // Allow a max of 10 arguments
  for(i = 0; i < numCmds && !endOfString; i++) {
    while(!isspace(*string)) {
      // If at the end of the string, break out
      if(*string == 0) {
        endOfString = true;
        break;
      }
      argEnd++;
      string++;
    }

    // If there's something to add
    if(argEnd != argStart) {
      length = argEnd - argStart;

      // Allocate memory in the input array
      args[i] = malloc(length + 1);

      strncpy(args[i], cmd->cmdstr + argStart, length);
      args[i][length] = 0;
      argCount++;
    }
    string++;
    argEnd++;
    argStart = argEnd;
  }
  if(!endOfString)
    argCount++;
  return argCount;
}

/**
 * Command handler
 *
 * @param cmdstr the input from the command line
 */
void handle_cmd(command_t cmd) {
  int NUM_CMDS = 20;
  char *args[NUM_CMDS];
  int argCount = parse_cmd(&cmd, args, NUM_CMDS);

  if(argCount > NUM_CMDS) {
    printf("You've entered too many arguments. Please try again.\n");
    return;
  }
  else if(argCount == 0) {
    return;
  }
  // Main handler

  if(strcmp(args[0], "")) {

  }
}

/**
 * Trims leading and trailing whitespace from the input string
 *
 * @param input - the string to be trimmed
 */
void trim(command_t* cmd) {
  int commandLength = cmd->cmdlen;

  // Move pointer from front of string to first legit character
  while(isspace(*cmd->cmdstr)) {
    cmd->cmdstr++;
    cmd->cmdlen--;
  }

  // The string is empty
  if(*cmd->cmdstr == 0)
    return;

  // Find the back of the string
  char* back = cmd->cmdstr + commandLength - 1;

  // Move back pointer from back of string to last legit character
  while(isspace(*back) && back > cmd->cmdstr) {
    back--;
    cmd->cmdlen--;
  }

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
  // #TODO --> on first program run, if a string longer than 25 characters is entered, seg fault... why?
  command_t cmd; //< Command holder argument

  start();
  
  puts("\nWelcome to Quash!");
  puts("Type \"exit\" or \"quit\" to quit\n");
  printf("quash$> ");
  // Main execution loop
  while (is_running() && get_command(&cmd, stdin)) {
    // Trim down the command (leading & trailng white space)
    trim(&cmd);

    // Terminate this program if the user wants to exit
    if (!strcmp(cmd.cmdstr, "exit") || !strcmp(cmd.cmdstr, "quit")) {
      puts("Exiting...");
      terminate(); // Exit Quash
    }
    else {
      // If we didn't terminate, parse the command
      handle_cmd(cmd);
      printf("quash$> ");
    }
  }

  return EXIT_SUCCESS;
}
