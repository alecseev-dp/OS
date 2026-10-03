#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    int reader_id = atoi(argv[1]);
    int sem_idx = SHM_COUNT + reader_id; // Семафоры 6..10
    int target_shm = reader_id % SHM_COUNT;

    int shmId, semId;
    key_t keySem = ftok("creator.cpp", 2);
    key_t keyShm = ftok("creator.cpp", 1);
    char *addr1 = NULL;
    struct sembuf check_sem[1];

    check_sem[0].sem_num = sem_idx;
    check_sem[0].sem_op = -1;
    check_sem[0].sem_flg = 0;

    semId = semget(keySem, TOTAL_SEMS, 0);
    shmId = shmget(keyShm + target_shm, SHM_SIZE, 0);

    if (semId == -1 || shmId == -1) {
        perror("SemReader IPC error");
        return 1;
    }

    // Ожидание открытия семафора чтения (6..10)
    if (semop(semId, check_sem, 1) == -1) {
        perror("SemReader semop error");
        return 1;
    }

    addr1 = (char*)shmat(shmId, 0, SHM_RDONLY);
    if (addr1 != (char*)-1) {
        printf("[SEM READER %d] Прочитал из SHM %d по семафору %d: \"%s\"\n",
               reader_id, target_shm, sem_idx, addr1);
        shmdt(addr1);
    }

    return 0;
}
