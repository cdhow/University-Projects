/* ----------------------------------------------------------------- */
/* PROGRAM  client.c                                                 */
/* ----------------------------------------------------------------- */

#include "../header.h"
#include <ncurses.h>

// For Time I can just have a continuous clock, and store the begin time
// of a query and when the query ends just conitunous_clock - clock[i]

#define BUFF 100

// Array to store the time of when a query begins
// we can use this to calculate the time it takes
// to complete the query
time_t init_time[SIZE];

/**
 * Takes the shared memory struct and prints each query and
 * its completion percentage.
 **/
void displayQueries(struct Memory *ShmPTR)
{

     // Queries print string max size is 100
     for (int i = 0; i < BUFF; i++)
          printw("\b");

     // Print active queries and thread completion status
     for (int i = 0; i < 10; i++)
     {
          if (ShmPTR->threads_complete[i] != -1)
          {
               float per = ((float)ShmPTR->threads_complete[i] / 32.0) * 100.0;
               printw("Q%d:%-3d%%  ", i + 1, (int)per);
          }
     }
}

/**
 * This function is continuously called by the run function,
 * it checks if the server has placed a factor in a given slot and
 * prints the factor and query number if a factor is present.
 * It also checks the server has finished factorising an input completely
 * and prints an appropriate message.
 **/
void receiveServerData(struct Memory *ShmPTR)
{
     // Check the server flags,
     for (int i = 0; i < SIZE; i++)
     {

          if (ShmPTR->server_flag[i] == 1)
          {

               // Flag is 1, factorisation is ongoing and factor is ready to read
               for (int i = 0; i < BUFF; i++)
                    printw("\b");
               // Print Query number and factor received
               printw("Q%d; Factor : %lu\n", i + 1, ShmPTR->slot[i], ShmPTR);
               ShmPTR->server_flag[i] = 0;
          }

          if (ShmPTR->threads_complete[i] > 31)
          {
               // Flag is 1 and threads completed > 31, factorisation is complete
               ShmPTR->threads_complete[i] = -1; // set threads_complete to -1 (query to idle)
               // Calculate the time
               time_t now;
               time(&now);
               double seconds = difftime(now, init_time[i]);
               for (int i = 0; i < BUFF; i++)
                    printw("\b");
               attron(A_STANDOUT);
               printw("Query %d has completed in %.f seconds..\n", i + 1, seconds);
               attroff(A_STANDOUT);
          }
     }
}

/**
 * This function is called when the user inputs a message,
 *  it takes the shared memory structure and the user input
 *  number. It sends the number to the client, then it retrieves
 *  and returns the slot number for which the factors of that
 *  number will be placed.
 **/
int sendNumber(struct Memory *ShmPTR, uint32_t number)
{
     // Wait until the server is ready to accept
     while (ShmPTR->client_flag != 0)
          ;
     // Send number to server set client_flag = 1
     ShmPTR->number = number;
     ShmPTR->client_flag = 1;
     // Wait for server response
     while (ShmPTR->client_flag != 0)
          ;

     // Return slot number
     return ShmPTR->number;
}

/**
 * Takes the shared memory pointer and runs a continuous loop,
 *  handles the non-blocking user input  and calls all the
 *  above functions.
 **/
void run(struct Memory *ShmPTR)
{
     char input[BUFF] = {0};
     int n = 0;

     initscr();             // Start curses mode
     noecho();              // Don't echo() while we do getch
     cbreak();              // So we dont print a \n when user presses enter
     nodelay(stdscr, TRUE); // enable non-blocking input
     scrollok(stdscr, 1);   // ensure the window scrolls when full

     char ch;
     while (1)
     {
          if ((ch = getch()) == ERR)
          {
               displayQueries(ShmPTR);
               // Continuously check if data is available from the server
               receiveServerData(ShmPTR);
               // user hasn't responded
          }
          else
          {
               // user has pressed a key ch
               if (ch == '\n')
               {
                    // Check if the input is "quit"
                    if (strcmp(input, "quit") == 0)
                    {
                         ShmPTR->client_flag = -1;
                         break;
                    }
                    // We have the number
                    char *strP;
                    uint32_t num = strtoul(input, &strP, 10);
                    // Send the number to the server
                    int slot = sendNumber(ShmPTR, num);
                    // Begin counting completed threads if there is an available slot
                    if (slot == 11)
                    {
                         for (int i = 0; i < BUFF; i++)
                              printw("\b");
                         attron(A_STANDOUT);
                         addstr("*** WARNING: Server busy.. wait for available query ***\n");
                         attroff(A_STANDOUT);
                         // printw("*** WARNING: Server busy.. wait for available query ***\n");
                    }
                    else
                    {
                         ShmPTR->threads_complete[slot] = 0;
                         time(&(init_time[slot])); // Begin the clock
                    }

                    memset(input, 0, BUFF);
                    n = 0;
               }
               else
               {
                    input[n++] = ch;
               }
          }
     }

     refresh();

     endwin(); // End curses mode
}

int main()
{
     key_t ShmKEY;
     int ShmID;
     struct Memory *ShmPTR;

     ShmKEY = ftok("..", 'x');
     ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);
     if (ShmID < 0)
     {
          printf("*** shmget error (client) ***\n");
          exit(1);
     }
     printf("client has received a shared memory...\n");

     ShmPTR = (struct Memory *)shmat(ShmID, NULL, 0);
     if ((int)ShmPTR == -1)
     {
          printf("*** shmat error (client) ***\n");
          exit(1);
     }
     printf("client has attached the shared memory...\n");
     sleep(2);

     // Initialise the flags
     ShmPTR->client_flag = 0;
     for (int i = 0; i < SIZE; i++)
          ShmPTR->server_flag[i] = 2;
     for (int i = 0; i < SIZE; i++)
          ShmPTR->threads_complete[i] = -1;

     // Program Loop
     run(ShmPTR);

     printf("client has detected the completion of its server...\n");
     shmdt((void *)ShmPTR);
     printf("client has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("client has removed its shared memory...\n");
     printf("client exits...\n");

     return 0;
}