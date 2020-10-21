#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define SIZE 10

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

struct Memory
{
    // Flag = 1 when the client request is outstanding.
    // Flag = -1 when the client issues the quit command
    int client_flag;

    // Flags;
    // flag = 0 : in use by server, with no value in slot
    // flag = 1 : ready to read by client
    // flag = 2 : factorising is complete and/or slot is unused
    int server_flag[SIZE];

    // Holds the number of threads completed
    // for a given slot (-1 if slot is unused)
    int threads_complete[SIZE];

    uint32_t slot[SIZE];

    // user input number
    uint32_t number;
};

#endif