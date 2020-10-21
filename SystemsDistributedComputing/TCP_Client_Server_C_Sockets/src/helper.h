/* This file contains all the helper functions used by the Client and the Server */
#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>


// Prints an error message and exits the program
void error(char*);


// Takes two parameters, file_path and content
// and writes the content string to the file_path file.
// Returns -1 if it failed to write to the file.
int writeToFile(char*, char*);


// Takes a paramter file_path and returns the contents of
// the file or an error message.
char* getFileContent(char*);


// Takes two parameters, a directory name and a flag.
// The function creates a new directory.
// Returns 0 if the directory was successfully created.
// Returns -1 if the directory already exists and overwrite flag
// was not provided.
int allocateDirectory(char*, int);


// Takes a command as a paramter and runs the command using popen().
// Returns the result of the system command call or an error message.
char* runSystemCommand(char*);


// Takes a directory name as a paramter, checks the directories .c files
// and updates the directories makefile accordingly.
// Returns -1 if it fails
int updateDirMakefile(char*);

#endif 