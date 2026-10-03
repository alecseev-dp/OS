#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SHM_COUNT 6          // A = 6 областей памяти
#define WRITER_COUNT 8       // B = 8 писателей
#define SEM_READERS_COUNT 5  // C = 5 читателей по семафорам
#define TOTAL_SEMS 12        // 6 семафоров записи + 6 семафоров чтения

#define SHM_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#endif
