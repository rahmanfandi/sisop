#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Fungsi tugas1
void tugas1() {
    for (int i = 0; i < 5; i++) {
        printf("Tugas1  -  Iterasi: %d\n", i);
        sleep(1);  // Delay 1 detik
    }
  
}

// Fungsi tugas2
void tugas2() {
    for (int i = 0; i < 5; i++) {
        printf("Tugas2 -  Iterasi: %d\n", i);
        sleep(1);  // Delay 1 detik
    }
  
}

int main() {
tugas1();
tugas2();

    printf("Semua thread selesai.\n");
    return 0;
}