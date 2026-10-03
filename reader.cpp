#include "common.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    int shm_idx = atoi(argv[1]);

    int shmId;
    key_t keyShm = ftok("creator.cpp", 1);
    char *addr1 = NULL;

    shmId = shmget(keyShm + shm_idx, SHM_SIZE, 0);
    if (shmId == -1) {
        perror("DirectReader shmget error");
        return 1;
    }

    addr1 = (char*)shmat(shmId, 0, SHM_RDONLY);
    if (addr1 == (char*)-1) {
        perror("DirectReader shmat error");
        return 1;
    }

    // Бесконечный цикл, чтобы отслеживать появление записей
    while (1) {
        if (strlen(addr1) > 0) {
            printf("[DIRECT READER %d] SHM %d обновилась: \"%s\"\n", shm_idx, shm_idx, addr1);
            // break;
        } else {

        }

        usleep(500000); // Спим 0.5 секунды перед следующей проверко
    }

    shmdt(addr1);
    return 0;
}
