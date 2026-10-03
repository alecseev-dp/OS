#include "common.h"

int main() {
    int shmIds[SHM_COUNT];
    int semId;
    key_t keySem, keyShm;

    // Генерация ключей по образцу методологии
    keySem = ftok("creator.cpp", 2);
    keyShm = ftok("creator.cpp", 1);

    if (keySem == -1 || keyShm == -1) {
        perror("Ошибка ftok (убедитесь, что файл creator.cpp существует)");
        return 1;
    }

    printf("keySem=%d, keyShm=%d\n", keySem, keyShm);

    // 1. Создаем 6 областей памяти
    for (int i = 0; i < SHM_COUNT; i++) {
        shmIds[i] = shmget(keyShm + i, SHM_SIZE, IPC_CREAT | 0606);
        if (shmIds[i] == -1) {
            perror("shmget error");
            return 1;
        }
        printf("shmId[%d]=%d\n", i, shmIds[i]);

        // Инициализируем пустой строкой
        char *addr = (char*)shmat(shmIds[i], 0, 0);
        if (addr != (char*)-1) {
            addr[0] = '\0';
            shmdt(addr);
        }
    }

    // 2. Создаем набор из 12 семафоров (6 для записи, 6 для чтения)
    semId = semget(keySem, TOTAL_SEMS, IPC_CREAT | 0606);
    if (semId == -1) {
        perror("semget error");
        return 1;
    }
    printf("semId=%d (всего 12 семафоров)\n", semId);

    // Устанавливаем все 12 семафоров в 0 (блокирующие)
    unsigned short ray[TOTAL_SEMS];
    for (int i = 0; i < TOTAL_SEMS; i++) ray[i] = 0;

    union semun ar;
    ar.array = ray;
    semctl(semId, 0, SETALL, ar);


    printf("Инициализация успешно завершена.\n");
    return 0;
}
