#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <signal.h>

#include "../helper.h"

#define BUFF_SIZE 4096
#define MAX 80 
#define PORT 9500
#define SA struct sockaddr 


// ---------------- COMMAND HANDLING FUNCTIONS --------------------------

/*
* Handles all functionality of the put command; sends the client -1 if 
* directory exists an "-f" is not provided, otherwise accepts file content
* from the client
*/
void putCommand(int sockfd, char msg_arr[MAX][MAX], int len)
{
    char client_msg[BUFF_SIZE];

    // Check if the overwrite flag is present.
    int flag = (strcmp(msg_arr[len-1], "-f") == 0) ? 1 : 0;
    if (flag) len -= 1; // we can now ignore the "-f" value.

    // Try to create the directory.
    // (res will equal -1 if the directory exists and -f is not present).
    int res = allocateDirectory(msg_arr[1], flag);
    int net_res = htonl(res);

    // Send the result to client so it knows whether an error
    // has occurred, or whether the server is read to receive the content.
    write(sockfd, &net_res, sizeof(net_res));

    // If the directory allocation was successful, send the content
    if (res == 0) {
        // Directory was created or it exists and the -f flag was present.
        // Receive the file contents from the client and create/update
        // the file, for each file.
        for (int i=2; i<len; i++) {
            memset(client_msg, 0, BUFF_SIZE);
            read(sockfd, client_msg, BUFF_SIZE); // get the content from client.
            char file_path[MAX] = {0}; // construct the filepath for the file.
            sprintf(file_path, "./%s/%s", msg_arr[1], msg_arr[i]); 
            writeToFile(file_path, client_msg); // write content to file.
        }
    }
}

/*
* Handles all functionality of the get command; reads a file's content
* and sends the content to the client.
*/
void getCommand(int sockfd, char msg_arr[MAX][MAX])
{
    char server_buff[BUFF_SIZE] = {0};

    // Construct the file path.
    char file_path[MAX] = {0};
    sprintf(file_path, "./%s/%s", msg_arr[1], msg_arr[2]);

    // Get the file content and send it to the server
    // (An error message is returned if an error occurs).
    char *contents = getFileContent(file_path);
    strcpy(server_buff, contents);
    free(contents);

    // Send the contents to the client.
    write(sockfd, server_buff, BUFF_SIZE);
}

/*
* Handles all functionality of the run command; compiles and executes source code
* in the directory "progname", sends results or any errors to the client.
*/
void runCommand(int sockfd, char msg_arr[MAX][MAX], int len)
{
    char server_buff[BUFF_SIZE] = {0};

    // Update the directory's makefile.
    updateDirMakefile(msg_arr[1]);

    // Execute makefile.
    char cmd[MAX] = {0};
    sprintf(cmd, "make -C ./%s", msg_arr[1]);
    char* result = runSystemCommand(cmd);
    free(result);

    // Get the programs arguments (if there are any).
    char args[MAX] = {0};
    for (int i=2; i<len; i++) {
        if (strcmp(msg_arr[i], "-f") == 0) // next elem is the outputfile, break.
            break;
        sprintf(args + strlen(args), "%s ", msg_arr[i]); // update the args string.
    }

    // Execute the program and get the results (or errors).
    memset(cmd, 0, MAX);
    sprintf(cmd, "./%s/output %s", msg_arr[1], args);
    result = runSystemCommand(cmd);
    strcpy(server_buff, result);
    free(result);

    // Send the results (or errors) to the client.
    write(sockfd, server_buff, BUFF_SIZE);     
}

/*
* Handles all functionality of the list command; retrieves list information from 
* a directory or local and sends the information to the client.
*/
void listCommand(int sockfd, char msg_arr[MAX][MAX], int len)
{
    char server_buff[BUFF_SIZE] = {0};
    char cmd[MAX] = {0};

    // Construct the command with "ls".
    switch (len) {
        case 1 :
            strcpy(cmd, "ls"); 
            break;
        case 2 : 
            sprintf(cmd, "ls %s", msg_arr[1]); 
            break;
        case 3 :
            sprintf(cmd, "ls %s %s", msg_arr[1], msg_arr[2]); 
            break;
    }

    // Call command and get result.
    char* output = runSystemCommand(cmd);
    strcpy(server_buff, output);
    free(output);

    // Send result to the client.
    write(sockfd, server_buff, BUFF_SIZE);
}

/*
* Handles all the sys command; retrieves operating system type and version, 
* and CPU type of the server and sends the information to the client.
*/
void sysCommand(int sockfd)
{
    char server_buff[BUFF_SIZE] = {0};

    // Get the CPU info from proc/cpuinfo.
    char* cpu_info = runSystemCommand("grep '^model name' /proc/cpuinfo | head -1");
    // Get the OS info from proc/version.
    char* os_info = runSystemCommand("cat /proc/version");

    // Combine system details into a single string.
    sprintf(server_buff, "Operating System: %s\nCPU Type: %s\n", os_info, cpu_info);
    free(os_info);
    free(cpu_info);

    // Send the system details to the client.
    write(sockfd, server_buff, BUFF_SIZE);
}

// ------------------------ PROGAM FUNCTIONS -----------------------------

/* 
* Server loop, parses the client message and communicates 
* with the client accordingly.
*/ 
void serverLoop(int sockfd)
{
    char client_msg[BUFF_SIZE]; // stores command sent from the client
    char msg_arr[MAX][MAX]; // stores the split client message into words
    while (1)
    {
        memset(client_msg, 0, BUFF_SIZE);
        memset(msg_arr, 0, sizeof(msg_arr));

        // Get the command from the client, we can assume that
        // the syntax of the commad is correct as syntax issues are
        // handled on the client side.
        read(sockfd, client_msg, BUFF_SIZE);

        // Split the client message.
        char *token;
        token = strtok(client_msg, " ");
        int len = 0;
        while (token !=NULL) {
            strcpy(msg_arr[len++], token); // store the words in msg_arr.
            token = strtok(NULL, " ");
        }

        // Execute command
        if (strcmp(msg_arr[0], "put") == 0) 
        {
            putCommand(sockfd, msg_arr, len);
        } 
        else if (strcmp(msg_arr[0], "get") == 0) 
        {
            getCommand(sockfd, msg_arr);
        } 
        else if (strcmp(msg_arr[0], "run") == 0) 
        {
            runCommand(sockfd, msg_arr, len);
        } 
        else if (strcmp(msg_arr[0], "list") == 0) 
        {
            listCommand(sockfd, msg_arr, len);
        } 
        else if (strcmp(msg_arr[0], "sys") == 0) 
        {
            sysCommand(sockfd);
        }

    }
}


/* Kills zombie processes. */
void ZombieKill(int sig)
{
    int status;
    waitpid(-1, &status, WNOHANG);
}

/*
* Main function, handles the connection to multiple clients.
*/
int main() 
{
    int sockfd, connfd, len, pid; 
    struct sockaddr_in servaddr, cli; 

    signal(SIGCHLD, ZombieKill); // kill zombie processes
  
    // Create socket and verifying.
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1)
        error("ERROR: socket could not be created.\n");
    printf("Socket successfully created..\n"); 

    // Assign IP, PORT.
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT); 

    // Binding the server socket to given IP and verifying.
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
        error("ERROR: socket bind failed.\n");
    printf("Socket successfully binded..\n"); 
  
    // The server is ready to listen, and verifying.
    if ((listen(sockfd, 5)) != 0)
        error("ERROR: listen failed.\n"); 
    printf("Server is listening..\n"); 
   
  
    // Accept the data packet from a client and verify success
    // Run in loop with fork() so the server can handle multiple
    // non blocking clients.
    len = sizeof(cli);  
    while (1) {
        connfd = accept(sockfd, (SA*)&cli, &len); 
        if (connfd < 0)
            error("ERROR: server accept failed.\n"); 

        pid = fork();
        if (pid < 0)
            error("ERROR on fork.");
        else if (pid == 0)  {
            close(sockfd);
            serverLoop(connfd); // main serverloop
            exit(0);
        }
        else 
            close(connfd);// close socket
    }

} 