#include <stdio.h>
#include <windows.h>  // Windows에서 Sleep 함수 사용

void printCars(int car1, int car2) {
    printf("CAR #1: ");
    for (int i = 0; i < car1; i++) printf("*");
    printf("\n");

    printf("CAR #2: ");
    for (int i = 0; i < car2; i++) printf("*");
    printf("\n");

    printf("-----------------------------------\n");
}

int main() {
    int car1[] = { 2, 7, 13, 17, 22, 26, 30 };
    int car2[] = { 3, 4, 9, 16, 21, 27, 30 };
    int steps = sizeof(car1) / sizeof(car1[0]);

    for (int i = 0; i < steps; i++) {
        printCars(car1[i], car2[i]);
        Sleep(500);  // 0.5초 대기 (Windows 전용)
    }

    return 0;
}
