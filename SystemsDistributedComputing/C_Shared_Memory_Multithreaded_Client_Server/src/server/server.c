/* ----------------------------------------------------------------- */
/* PROGRAM  server.c                                                 */
/* ----------------------------------------------------------------- */

#include "../header.h"
#include <pthread.h>
#include <semaphore.h>

struct Memory *ShmPTR; // Shared memory between server/client

sem_t mutex_fact[SIZE]; // semaphore access for the factorisation threads to a given slot

struct thread_data
{
    int slot_num;
    uint32_t number;
};

/**
 * This function takes a number and a rotate_degree,
 *  it it rotates the binary representation of the number
 *  to the right by a degree specified in the second parameter.
 *  It returns the uint resulting from the rotation.
 **/
uint32_t rotate(uint32_t number, int rotate_degree)
{
    if (rotate_degree == 0)
        return number;

    int bit_width = sizeof(number) * 8;
    // Rotating 32 bits on a 32-bit integer is the same as rotating 0 bits;
    //   33 bits -> 1 bit; etc.
    rotate_degree = rotate_degree % bit_width;

    uint32_t temp = number;

    // Rotate input to the right
    number = number >> rotate_degree;

    // Build mask for carried over bits
    temp = temp << (bit_width - rotate_degree);

    return number | temp;
}

void *factorise(void *data)
{
    uint32_t number = ((struct thread_data *)data)->number;
    int slot_num = ((struct thread_data *)data)->slot_num;
    uint32_t f = 2;
    uint32_t prev_f = 0;

    // Trial Division
    while (number > 1)
    {
        // Calculate factor
        if (number % f == 0)
        {
            if (prev_f != f)
            {
                // Wait
                sem_wait(&(mutex_fact[slot_num]));
                // Critical section

                // ensure client is ready
                while (ShmPTR->server_flag[slot_num] != 0)
                    ;
                // Send factor
                ShmPTR->slot[slot_num] = f;
                ShmPTR->server_flag[slot_num] = 1;
                prev_f = f;

                msleep(2000);

                // Signal
                sem_post(&(mutex_fact[slot_num]));
            }

            number /= f;
        }
        else
        {
            f++;
        }
    }

    sem_wait(&(mutex_fact[slot_num]));
    // Thread has finished
    ShmPTR->threads_complete[slot_num]++;
    printf("Query: %d, Thread %d complete\n", slot_num + 1, ShmPTR->threads_complete[slot_num]);

    sem_post(&(mutex_fact[slot_num]));

    pthread_exit(NULL);
}

void initThreads(uint32_t number, int slot_num)
{
    // Create a thread for each 32bit rotation of number
    pthread_t tid[32];
    struct thread_data td[32]; // passed to the factorisation threads

    for (int i = 0; i < 32; i++)
    {
        td[i].number = rotate(number, i);
        td[i].slot_num = slot_num;
        pthread_create(&(tid[i]), NULL, factorise, (void *)&(td[i]));
    }
}

// Returns the slot number (11 if no free slots)
uint32_t getFreeSlot()
{
    // Look for a free slot
    uint32_t slot_num = 11;
    for (uint32_t i = 0; i < SIZE; i++)
    {
        if (ShmPTR->threads_complete[i] == -1)
        {
            slot_num = i;
            ShmPTR->server_flag[i] = 0;
            break;
        }
    }
    return slot_num;
}

void run()
{
    // So threads do not read/write to a slot simultaneously
    for (int i = 0; i < SIZE; i++)
        sem_init(&(mutex_fact[i]), 0, 1);

    while (1)
    {
        // Check for the quit command
        if (ShmPTR->client_flag == -1)
            break; // quit

        // Check if the server has sent a number
        if (ShmPTR->client_flag == 1)
        {
            // Read number from client
            uint32_t number = ShmPTR->number;

            // Respond to client with slot number
            uint32_t slot_num = getFreeSlot();
            ShmPTR->number = slot_num;

            ShmPTR->client_flag = 0;

            // Create 32 threads for 32 rotatations of number
            initThreads(number, slot_num);

            // We dont need pthread join because threads exit when using pthread_exit,
            // join is simply used when we want to wait
        }
    }
}

int main()
{
    key_t ShmKEY;
    int ShmID;

    ShmKEY = ftok("..", 'x');
    ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
    if (ShmID < 0)
    {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("   Server has received a shared memory with client...\n");

    ShmPTR = (struct Memory *)shmat(ShmID, NULL, 0);
    if ((int)ShmPTR == -1)
    {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }
    printf("   server has attached the shared memory...\n");

    run();

    shmdt((void *)ShmPTR);
    printf("   server has detached its shared memory...\n");
    printf("   server exits...\n");

    return 0;
}
