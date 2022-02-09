#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ALIVE 35
#define DEAD 32
#define N 25
#define M 80

void createPlace(int timer);
int simulation(char **place, char **placeOld);
int getCellsCount(char **placeOld, int i, int j);
void updateOldPlace(char **place, char **placeOld);
void updateDislplay(char **place);
int getHashSum(char **place);
int getHashKey(int countI, int countJ, int countN);
int hashCheck(int *hashTable, int currentHashKey);

int main(int argc, char **argv) {
    int timer = 1;

    if (argc >= 1) sscanf(argv[1], "%d", &timer);

    createPlace(timer);
}

void createPlace(int timer) {
    int isRepeat = 0;
    char *place[N];
    char *placeOld[N];
    int hashTable[2500];

    for (int i = 0; i < 100; i++) hashTable[i] = -1;
    for (int i = 0; i < N; i++) {
        place[i] = (char*) malloc(M * sizeof(char));
        placeOld[i] = (char*) malloc(M * sizeof(char));
        for (int j = 0; j < M; j++) {
            int digitASCII;

            scanf("%d", &digitASCII);
            place[i][j] = digitASCII;
        }
    }

    updateOldPlace(place, placeOld);

    while (isRepeat == 0) {
        system("clear");
        updateDislplay(place);

        isRepeat = hashCheck(hashTable, simulation(place, placeOld));

        usleep(timer * 1000);
    }

    for (int countFree = 0; countFree < N; countFree++) {
        free(place[countFree]);
        free(placeOld[countFree]);
    }
}

int simulation(char **place, char **placeOld) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int cellsCount = getCellsCount(placeOld, i, j);

            if (placeOld[i][j] == DEAD) {
                if (cellsCount == 3) { place[i][j] = ALIVE; }
            } else {
                if (cellsCount < 2 || cellsCount > 3) { place[i][j] = DEAD; }
            }
        }
    }
    updateOldPlace(place, placeOld);

    return getHashSum(place);
}

int getCellsCount(char **placeOld, int i, int j) {
    int count = 0;
    int y = i - 1, x = j - 1;

    if (y < 0) { y += N; }
    if (x < 0) { x += M; }

    for (int i1 = y; i1 < y + 3; ++i1) {
        for (int j1 = x; j1 < x + 3; ++j1) {
            if (placeOld[i1 % N][j1 % M] == ALIVE && !((i1 % N) == i && (j1 % M) == j)) { count++ };
        }
    }

    return count;
}

void updateOldPlace(char **place, char ** placeOld) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) { placeOld[i][j] = place[i][j]; }
    }
}

void updateDislplay(char **place) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            ((i + j) % 2 == 0) ? printf("\033[1;33m") : printf("\033[1;31m");
            printf("%c", place[i][j]);
        }
        puts("");
    }
    printf("\033[0m");
}

int getHashSum(char **place) {
    int countI = 0, countJ = 0, countN = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (place[i][j] == ALIVE) {
                countI += (i * i);
                countJ += (j * j);
                countN++;
            }
        }
    }

    return getHashKey(countI, countJ, countN);
}

int getHashKey(int countI, int countJ, int countN) {
    const int keyI = 1087, keyJ = 371, keyN = 57;

    return ((countI * keyI) + (countJ * keyJ) + (countN * keyN));
}

int hashCheck(int *hashTable, int currentHashKey) {
    int isRepeat = 0;

    for (int i = 0; i < 2499; i++) {
        if (hashTable[i] == currentHashKey) { isRepeat++; }
    }

    if (isRepeat == 0) {
        for (int j = 2499; j > 0; j--) {
            hashTable[(j - 1)] = hashTable[j];
        }

        hashTable[2498] = currentHashKey;
    }

    return isRepeat;
}
