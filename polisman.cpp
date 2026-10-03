#include "common.h"

int main() {
    int dr, semId;
    key_t keySem, keyShm;
    keySem = ftok("creator.cpp", 2);
    keyShm = ftok("creator.cpp", 1);

    printf("keySem=%d, keyShm=%d\n", keySem, keyShm);

    semId = semget(keySem, TOTAL_SEMS, 0);
    if (semId == -1) {
        perror("Polisman: error semget");
        return 1;
    }

    printf("semId=%d\n", semId);
    printf("нажмите -1 для завершения работы и очистки IPC\n");
    printf("нажмите 9 для вывода заблокированных процессов\n");
    printf("нажмите 0 для открытия семафора Писателей (0..5)\n");
    printf("нажмите 1 для открытия семафора Читателей (6..11)\n");

    for (;;) {
        printf("\nВведите команду: ");
        if (scanf("%d", &dr) != 1) break;

        if (dr == 9) {
            printf("\n--- СОСТОЯНИЕ 12 СЕМАФОРОВ ---\n");
            printf("ПИСАТЕЛИ (0..5): ");
            for (int i = 0; i < SHM_COUNT; i++) {
                int cnt = semctl(semId, i, GETNCNT, NULL);
                printf("[%d]:%d(ждут:%d) ", i, semctl(semId, i, GETVAL, NULL), cnt);
            }
            printf("\nЧИТАТЕЛИ (6..11): ");
            for (int i = SHM_COUNT; i < TOTAL_SEMS; i++) {
                int cnt = semctl(semId, i, GETNCNT, NULL);
                printf("[%d]:%d(ждут:%d) ", i, semctl(semId, i, GETVAL, NULL), cnt);
            }
            printf("\n");
        }

        if (dr == -1) {
            semctl(semId, 0, IPC_RMID, NULL);
            for (int i = 0; i < SHM_COUNT; i++) {
                int shmId = shmget(keyShm + i, SHM_SIZE, 0);
                if (shmId != -1) shmctl(shmId, IPC_RMID, NULL);
            }
            printf("Очистка IPC и завершение работы.\n");
            exit(0);
        }

        if (dr == 0 || dr == 1) {
            int num;
            int min_idx = (dr == 0) ? 0 : SHM_COUNT;
            int max_idx = (dr == 0) ? SHM_COUNT - 1 : TOTAL_SEMS - 1;

            printf("Введите номер семафора (%d..%d): ", min_idx, max_idx);
            if (scanf("%d", &num) == 1 && num >= min_idx && num <= max_idx) {
                struct sembuf check_sem[1];
                check_sem[0].sem_num = num;
                check_sem[0].sem_op = 1; // Открываем семафор
                check_sem[0].sem_flg = 0;


                if (semop(semId, check_sem, 1) == -1) {
                    perror("semop error");
                } else {
                    printf("Семафор %d успешно открыт!\n", num);
                }
            } else {
                printf("Неверный номер семафора!\n");
            }
        }
    }
    return 0;
}
