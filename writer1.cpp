#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    int writer_id = atoi(argv[1]);
    int target_shm_idx = writer_id % SHM_COUNT;

    int shmId, semId;
    key_t keySem, keyShm;
    char *addr1, buff[64];

    // Структуры для блокировки и разблокировки семафора
    struct sembuf lock_sem = { (unsigned short)target_shm_idx, -1, 0 };
    struct sembuf unlock_sem = { (unsigned short)target_shm_idx, 1, 0 };

    keySem = ftok("creator.cpp", 2);
    keyShm = ftok("creator.cpp", 1);

    snprintf(buff, sizeof(buff), " writer%d str", writer_id);

    semId = semget(keySem, TOTAL_SEMS, 0);
    shmId = shmget(keyShm + target_shm_idx, SHM_SIZE, 0);

    if (semId == -1 || shmId == -1) {
        perror("Writer: Ошибка получения IPC ресурса");
        return 1;
    }

    addr1 = (char*)shmat(shmId, 0, 0);
    if (addr1 == (char*)-1) {
        perror("Writer shmat error");
        return 1;
    }

    for (int j = 0; j < 2; j++) {
        if (semop(semId, &lock_sem, 1) == -1) {
            perror("Writer semop lock error");
            break;
        }

        printf("[WRITER %d] Запись в SHM %d: %s %d\n", writer_id, target_shm_idx, buff, j);
        size_t current_len = strlen(addr1);
        if (current_len < SHM_SIZE - 1) {
            snprintf(addr1 + current_len, SHM_SIZE - current_len, "%s%d", buff, j);
        }

        // 2. Освобождаем семафор (+1), чтобы читатель (или другой писатель) мог войти
        if (semop(semId, &unlock_sem, 1) == -1) {
            perror("Writer semop unlock error");
            break;
        }

        // Небольшая задержка, чтобы уступить квант времени другим процессам
        usleep(1000);
    }

    shmdt(addr1);
    return 0;
}
