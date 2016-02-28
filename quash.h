/**
 * @file quash.h
 *
 * Quash essential functions and structures.
 */

#ifndef QUASH_H
#define QUASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Specify the maximum number of characters accepted by the command string
 */
#define MAX_COMMAND_LENGTH (1024)

/**
 * Holds information about a command.
 */
typedef struct command_t {
  char *cmdstr; 				   ///< character buffer to store the
                                   ///< command string. You may want
                                   ///< to modify this to accept
                                   ///< arbitrarily long strings for
                                   ///< robustness.
  size_t cmdlen;                   ///< length of the cmdstr character buffer

  // Extend with more fields if needed
} command_t;

/**
 * Query if quash should accept more input or not.
 *
 * @return True if Quash should accept more input and false otherwise
 */
bool is_running();

/**
 * Causes the execution loop to end.
 */
void terminate();

/**
 *  Read in a command and setup the #command_t struct. Also perform some minor
 *  modifications to the string to remove trailing newline characters.
 *
 *  @param cmd - a command_t structure. The #command_t.cmdstr and
 *               #command_t.cmdlen fields will be modified
 *  @param in - an open file ready for reading
 *  @return True if able to fill #command_t.cmdstr and false otherwise
 */
bool get_command(command_t* cmd, FILE* in);

/**
 * Parses the input string and splits it up based on whitespace
 *
 * @param cmdstr the input from the command line
 * @return the number of arguments included in the command
 */
int parse_cmd(command_t* cmd, char** args, int numCmds);

/**
 * Prints the current working directory
 */
void pwd();

/**
 * Changes the current working directory to the input directory, or
 * HOME if no directory is given
 *
 * @param target - the input to change directories to
 */
void cd(char* target);

/**
 * Handles the input command by tokenizing the string and executing functions
 * based on the input
 *
 * @param cmdstr the input from the command line
 */
void handle_cmd(command_t cmd);

/**
 * Trims leading and trailing whitespace from the input string
 *
 * @param input - the string to be trimmed
 */
void trim(command_t* cmd);

#endif // QUASH_H
