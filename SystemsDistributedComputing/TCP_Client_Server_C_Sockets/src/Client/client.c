#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <time.h> 

#include "../helper.h"

#define BUFF_SIZE 4096
#define MAX 80 
#define PORT 9500 
#define SA struct sockaddr

// ------------------------ COMMAND HANDLING FUNCTIONS -----------------------------

/*
* Handles all functionality of the put command; puts client files to
* the server and handles errors.
*/
void putCommand(int sockfd, char client_buff[BUFF_SIZE], char input_arr[MAX][MAX], int len)
{
    // Error handling.
    if (len < 3) {
        printf("ERROR: put command requires minimum 3 arguments.\n");
        return;
    }
    if (len >= 3 && strcmp(input_arr[len-2], "-f") == 0) {
        printf("ERROR: no sourcefile provided to put command .\n");
        return;
    } 

    // Check if all the files exist on the client.
    for (int i=2; i<len; i++) {
        if (strcmp(input_arr[len-1], "-f") == 0)
            break;
        if (access( input_arr[i], F_OK ) == -1 ) {
            printf("ERROR: file does not exist on the client: %s\n", input_arr[i]);
            return;
        }
    }

    // Send the command to the server. The server will respond with 0
    // if the progname allocation was successful, otherwise it will 
    // respond with -1 if the file exists and the "-f" flag was not provided.
    write(sockfd, client_buff, BUFF_SIZE);
    
    int res;
    read(sockfd, &res, sizeof(res));
    int res_val = ntohl(res); // stores the response from the server.
    
    if (res_val == -1) {
        // Directory exists and the -f flag was not provided, or the server failed to allocate
        // the directory.
        printf("PERMISSION DENIED: cannot overwrite directory on server,\n");
        printf("to overwrite directory use the flag -f.\n");
        return;
    }

    // We can now ignore the "-f" value.
    if (strcmp(input_arr[len-1], "-f") == 0) len -= 1; 

    // Send the file contents for each file.
    for (int i=2; i<len; i++) {
        char *contents = getFileContent(input_arr[i]); // get file contents.
        memset(client_buff, 0, BUFF_SIZE);
        strcpy(client_buff, contents);
        free(contents);
        write(sockfd, client_buff, BUFF_SIZE); // send contents to the server.
    }
}

/*
* Handles all functionality of the get command; gets server file
* content and handles errors.
*/
void getCommand(int sockfd, char client_buff[BUFF_SIZE], int len)
{
    // Error handling
    if (len < 3) {
        printf("ERROR: get command requires 3 arguments. Only received %d.\n", len);
        return;
    }

    char server_msg[BUFF_SIZE] = {0}; // message from the server.

    // Send the command to the server.
    write(sockfd, client_buff, BUFF_SIZE);

    // Receive the file contents or an error message from the server.
    read(sockfd, server_msg, BUFF_SIZE);

    // Split the content by lines so we can print
    // to console 40 lines at a time.
    char *tk;
    tk = strtok(server_msg, "\n");
    int count = 0;
    while (tk !=NULL)
    {
        count++;
        printf("%2d| %s\n", count, tk); // print line number and the line.
        tk = strtok(NULL, "\n");
        // If the count is 40, wait for user input and repeat.
        if (count % 40 == 0) {
            printf("Press ENTER to continue...");
            getchar(); // wait for the user to press enter
            printf("\033[A\33[2K\r"); // remove the "press Enter.."
        }
    }
}


/*
* Handles all functionality of the run command; compiles and executes source code
* on the server, displays results to console or prints to file, and handles errors.
*/
void runCommand(int sockfd, char client_buff[BUFF_SIZE], char input_arr[MAX][MAX], int len)
{
    int fileFlag = 0; // flag for writing to file

    // Error handling
    if (len < 2) {
        printf("ERROR: run commnad requires minimum 2 arguments. Only received %d.\n", len);
        return;
    }

    char server_msg[BUFF_SIZE] = {0}; // message from the server.

    // Send the command to the server.
    write(sockfd, client_buff, BUFF_SIZE);

    // Get the response: program output or an error message.
    read(sockfd, server_msg, BUFF_SIZE);
    if (strcmp(input_arr[len-2], "-f") == 0)
        writeToFile(input_arr[len-1], server_msg);  // print to file
    else
        printf("%s\n", server_msg); // print to console

}

/*
* Handles all functionality of the list command; retrieves list information from 
* server directories or the server itself, also handles errors.
*/
void listCommand(int sockfd, char client_buff[BUFF_SIZE])
{
    char server_msg[BUFF_SIZE] = {0}; // message from the server.

    // Send the command to the server.
    write(sockfd, client_buff, BUFF_SIZE);

    // Get the response: list infomation or an error message.
    read(sockfd, server_msg, BUFF_SIZE);

    // Print response
    printf("%s\n", server_msg);

}

/*
* Handles all the sys command; retrieves operating system type and version, 
* and CPU type of the server, from the server.
*/
void sysCommand(int sockfd, char client_buff[BUFF_SIZE])
{
    char server_msg[BUFF_SIZE] = {0}; // message from the server.

    // Send the command to the server.
    write(sockfd, client_buff, BUFF_SIZE);

    // Get the response: sys infomation or an error message.
    read(sockfd, server_msg, BUFF_SIZE);

    // Print response
    printf("%s\n", server_msg);

}


// ------------------------ PROGAM FUNCTIONS -----------------------------

/* 
* Client loop, parses the user input and communicates 
* with the server accordingly.
*/ 
void clientLoop(int sockfd)
{
    clock_t t; 
    char client_buff[BUFF_SIZE]; // stores the user input.
    char input_arr[MAX][MAX]; // stores the split user input string words.
    while (1)
    {
        memset(client_buff, 0, BUFF_SIZE);
        memset(input_arr, 0, sizeof(input_arr));

        printf("> "); // ready for user input.

        // Get user input 
        int n = 0;
        while ((client_buff[n++] = getchar()) != '\n'); 
        client_buff[n-1] = 0; // remove the newline character.

        // Split the user input
        char tokenized_buff[BUFF_SIZE];
        strcpy(tokenized_buff, client_buff);
        char *token = strtok(tokenized_buff, " ");
        int len = 0;
        while (token !=NULL) {    
            strcpy(input_arr[len++], token); // store words in input_arr.
            token = strtok(NULL, " ");
        }

        // Record the time it takes to execute a command
        t = clock();

        // Execute command.
        if (strcmp(input_arr[0], "quit") == 0) 
        {
            printf("Disconnecting...\n");
            return;
        } 
        else if (strcmp(input_arr[0], "put") == 0) 
        {
            putCommand(sockfd, client_buff, input_arr, len);
        } 
        else if (strcmp(input_arr[0], "get") == 0) 
        {
            getCommand(sockfd, client_buff, len);
        } 
        else if (strcmp(input_arr[0], "run") == 0) // TODO: fix file issue
        {
            runCommand(sockfd, client_buff, input_arr, len);
        } 
        else if (strcmp(input_arr[0], "list") == 0) 
        {
            listCommand(sockfd, client_buff);
        } 
        else if (strcmp(input_arr[0], "sys") == 0) 
        {
            sysCommand(sockfd, client_buff);
        }
        else 
        {
            printf("ERROR: command from client is not recognised. Please try again.\n");
        }

        // Print the time
        t = clock() - t;
        printf("Time taken to execute command: %f.\n\n", ((double)t)/CLOCKS_PER_SEC);
    
    }
}

/*
* Main function, handles the connection to the server.
*/
int main(int argc, char **argv) 
{ 
    // Ensure an IP address is provided as a cmd line argument
     if(argc < 2)
        error("ERROR: expected IP address as second argument, received nothing.\n");
    
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1)
        error("ERROR: Socket could not be created.\n");
    printf("Socket successfully created..\n"); 

    // Assign IP, PORT. 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); // "127.0.0.1"
    servaddr.sin_port = htons(PORT); 
  
    // Connect the client socket to server socket.
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
        error("ERROR: connecting to the server failed.\n");
    else
        printf("connected to the server..\n"); 


    // Main client loop.
    clientLoop(sockfd);

    // Close the socket. 
    close(sockfd); 

} 