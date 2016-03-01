/**
 * @file quash.h
 *
 * Quash essential functions and structures.
 */

#ifndef QUASH_H
#define QUASH_H

#include <stdbool.h> 
#include <ctype.h>
#include <sys/types.h>

/**
 * Struct to hold background processes
 */
struct job {
  int jobid;  // The job ID stored in this job
  pid_t pid;  // The process ID stored in this job
  char *cmd;  // The command being run by this job
};

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
 * Flushes out all the jobs in the job list if they're terminated
 */
void flush_jobs();

/**
 * Prints out all the jobs in the job list
 */
void print_jobs();

/**
 * Echos the requested input, if the user wants to view the HOME or PATH 
 * variables, those values are printed out. If the second arguemtn is not
 * one of these variables, all subsequent arguments are printed out
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void echo(char** args, int argCount);

/**
 * Sets the system variables HOME or PATH to the desired location
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void set(char** args, int argCount);

/**
 * Returns a string of the executable file, should that executable be found
 * within one of the path locations in the PATH system variable.
 *
 * @param cmd - the input argument for this command
 * @return a string of the path to the executable, NULL if not found
 */
char* get_path_exec(char* cmd);

/**
 * Execute the input function and pipes its output to the input
 * of another function.
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void pipe_exec(char* args, int argCount);

/**
 * Execute the function with its arguments in the background, while keeping track
 * of the job id, process id, and command in a job struct
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments included in args
 */
void execute_in_background(char* cmd, char** args, int argCount);

/**
 * Execute the function with its arguments
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void execute(char** args, int argCount);

/**
 * Implements I/O redirection to write or read command output to/from a file
 *
 * @param cmd - the command string inputted for this command
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments inputted for this command
 */
void ioRedirect(char *cmd, char **args, int argCount);

/**
 * Kills the selected job if it exists in the background
 *
 * @param args - the list of arguments inputted for this command
 * @param argCount - the number of arguments included in args
 */
void killProcess(char** args, int argCount);

/**
 * Handles the input command by tokenizing the string and executing functions
 * based on the input
 *
 * @param cmd the input from the command line
 */
void handle_cmd(char* cmd);

/**
 * Trims leading and trailing whitespace from the input string
 *
 * @param cmd - the string to be trimmed
 */
void trim(char* cmd);



#endif // QUASH_H
