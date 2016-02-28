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
#include <readline/readline.h>

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
 * Parses the input string by tokenizing it
 *
 * @param cmdstr the input from the command line
 * @return the number of arguments included in the command
 */
char** parse_cmd(char* cmd, int* numCmds);

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
 * Echos the requested input, if the user wants to view the HOME or PATH 
 * variables, those values are printed out. If the second arguemtn is not
 * one of these variables, all subsequent arguments are printed out
 *
 * @param argCount - the number of arguments inputed for this command
 * @param args - the list of arguments inputed for this command
 */
void echo(char** args, int argCount);

/**
 * Sets the system variables HOME or PATH to the desired location
 *
 * @param args - the list of arguments inputed for this command
 */
void set(char** args, int argCount);

/**
 * Returns a string of the executable file, should that executable be found
 * within one of the path locations in the PATH system variable.
 *
 * @param args - the list of arguments inputed for this command
 */
char* get_path_exec(char* cmd);

/**
 * Execute the function with its arguments
 *
 * @param args - the list of arguments inputed for this command
 */
void execute(char** args, int argCount);

/**
 * Handles the input command by tokenizing the string and executing functions
 * based on the input
 *
 * @param cmdstr the input from the command line
 */
void handle_cmd(char* cmd);

/**
 * Trims leading and trailing whitespace from the input string
 *
 * @param input - the string to be trimmed
 */
void trim(char* cmd);

#endif // QUASH_H
