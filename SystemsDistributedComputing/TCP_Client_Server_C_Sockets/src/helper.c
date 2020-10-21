#define MAX 80
#define BUFF_SIZE 1024

#include "helper.h"

void error(char *msg)
{
    perror(msg); 
    exit(1); 
}

/**
 * This function is used by the server’s put handling function. It takes a directory name 
 * and an overwrite flag and creates a new directory with the provided name. 
 * If the directory exists and the overwrite flag is false (0) then the function returns -1,
 * else it returns 0 for success.
 * @param dir_name - directory name.
 * @param overwrite - whether the directory should be overwritten if it exists.
 * @return - 0 if success, -1 if could not overwrite.
 **/
int allocateDirectory(char *dir_name, int overwrite)
{
    // Construct the directory name
    char buff[MAX] = {0};
    sprintf(buff, "./%s", dir_name);

    // Check if the directory exists
    struct stat st = {0};
    if (stat(buff, &st) == -1) {
        // Does not exist, create directory
        mkdir(buff, 0700);
        return 0;
    }
    // Directory exists
    if (!overwrite)
        return -1; // overwrite was not flagged
    
    // Delete all content in directory
    remove(buff);
    mkdir(buff, 0700);

    return 0;
}

/**
 * Takes a file path and and content string and writes the content to the file
 *  located at file path.
 * @param file_path - where to write the content.
 * @param content - the data to be written to the file.
 * @return - 0 if successful, else -1.
 **/
int writeToFile(char* file_path, char* content)
{
    FILE *fp = fopen(file_path, "wb");
    if (fp == NULL)
        return -1;

    // Write the content to the file
    fputs(content, fp); 

    fclose(fp);

    return 0;
}

/** Loads the content of a file into a buffer and returns the buffer. 
* @param file_path - the file to be read from.
* @return - the file's content.
**/
char* getFileContent(char* file_path)
{
    char *error_msg = calloc(MAX, sizeof(char));

    FILE *fp = fopen(file_path, "rb");
    
    if(fp == NULL) {
        strcpy(error_msg, "ERROR: File does not exist.\n");
        return error_msg; // Return failed signal
    }
    free(error_msg);

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = malloc(fsize + 1);
    fread(content, 1, fsize, fp);
    fclose(fp);

    content[fsize] = 0;

    return content;
}

/**
 * Takes a system command and uses popen to run the command. The results of
 * the command, or any errors that occurred from running the system command
 * are returned from the function.
 * @param command - the command to be run on the system
 * @return - either the results of the command or any errors caused by running the command.
 **/
char* runSystemCommand(char* command)
{
    char *error_msg = calloc(MAX, sizeof(char));

    // Run the command using popen to write the results to a temp file.
    FILE *cmd_fp = popen(command, "r");

        if (cmd_fp == NULL) {
        strcpy(error_msg, "ERROR: failed to execute system command..\n");
        return error_msg;
    }

    size_t fsize;
    char buff[500];

    // Get the result content
    if ((fsize = fread(buff, 1, sizeof(buff)-1, cmd_fp)) <= 0) {
        strcpy(error_msg,"ERROR: failed to execute system command..\n");
        return error_msg;
    }
    free(error_msg);

    pclose(cmd_fp);

    buff[fsize] = 0;

    char* content = calloc(fsize, sizeof(int));

    strcpy(content, buff);

    return content;
}

/**
 * Takes a directory name as a parameter. The function scans the directory for any .c files,
 * then proceeds to create a makefile for the directory that includes all c files from that
 * directory. It returns -1 if any errors occurred, else it returns 0.
 * @param dir_name - the directory where the makefile will be created.
 * @return - 0 if success, -1 if errors occurred.
 **/
int updateDirMakefile(char* dir_name)
{
    char dir_content[MAX][MAX] = {0};
    int len = 0;
    // Get all filenames that are in the directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_name)) != NULL) {
        // Load the directory content into the dir_content array
        while ((ent = readdir (dir)) != NULL) {
            strcpy(dir_content[len++], ent->d_name);
        }
    closedir (dir);
    } else {
        printf("ERROR: failed to open directory"); 
        printf("%s while updating makefile..\n", dir_name);
        return -1;
    }

    char c_files[MAX][MAX] = {0};  // array of .c filenames

    // Extract the .c files into a separate array
    int c_len = 0;
    for (int i=0; i<len; i++)
        if (dir_content[i][strlen(dir_content[i])-1] == 'c')
            strcpy(c_files[c_len++], dir_content[i]);
    
    // Each section of the makefile is seperated into lines.
    // On the first line of 2 we have the exe target "output" and it's 
    // corresponding compilation data.
    // Each line of two after that contains a .o target with it's 
    // relevant compilation data.

    // Construct the make file
    char makefile[MAX] = {0};
    sprintf(makefile, "./%s/makefile", dir_name);
    FILE *fp = fopen(makefile, "wb");
    if(fp == NULL) {
        printf("ERROR: failed to open directory "); 
        printf("%s while updating makefile..\n", dir_name);
        return -1;
    }

    // First two lines
    for (int j=0; j<2; j++) {
        if (j == 0)
            fprintf(fp, "output: ");
        else
            fprintf(fp, "\tgcc ");
        for (int i=0; i<c_len; i++) {
            // Create the .o string
            int len = strlen(c_files[i]);
            char obj_file[len];
            strcpy(obj_file, c_files[i]);
            obj_file[len-1] = 'o';
            // Print the .o to the file
            fprintf(fp, "%s ", obj_file);   
        }
        if (j == 0)
            fprintf(fp, "\n");
        else
            fprintf(fp, "-o output\n");
    }

    // Every two lines (.o targets) after the first two
    for (int i=0; i<c_len; i++) {
        // Create the .o string
        int len = strlen(c_files[i]);
        char obj_file[len];
        strcpy(obj_file, c_files[i]);
        obj_file[len-1] = 'o';

        fprintf(fp, "%s: %s\n", obj_file, c_files[i]); // first line
        fprintf(fp, "\tgcc -c %s\n", c_files[i]); // second line
    }
        
    fclose(fp);

    return 0;

}