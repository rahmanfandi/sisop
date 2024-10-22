#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Fungsi tugas1
void* tugas1(void* thread_id) {
    for (int i = 0; i < 5; i++) {
        printf("Tugas1 - Thread ID: %ld, Iterasi: %d\n", (long)thread_id, i);
        sleep(1);  // Delay 1 detik
    }
    pthread_exit(NULL);
}

// Fungsi tugas2
void* tugas2(void* thread_id) {
    for (int i = 0; i < 5; i++) {
        printf("Tugas2 - Thread ID: %ld, Iterasi: %d\n", (long)thread_id, i);
        sleep(1);  // Delay 1 detik
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    // Membuat thread pertama yang menjalankan tugas1
    pthread_create(&thread1, NULL, tugas1, (void *)1);

    // Membuat thread kedua yang menjalankan tugas2
    pthread_create(&thread2, NULL, tugas2, (void *)2);

    // Menunggu kedua thread selesai
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Semua thread selesai.\n");
    return 0;
}