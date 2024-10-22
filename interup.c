#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Fungsi handler untuk menangani interupsi SIGINT
void interrupt_handler(int signal) {
    if (signal == SIGINT) {
        printf("\nInterrupt (SIGINT) received. Program stopped by user.\n");
        exit(0);  // Mengakhiri program dengan kode sukses
    }
}

int main() {
    // Mengatur handler untuk SIGINT
    signal(SIGINT, interrupt_handler);

    // Program berjalan dalam loop tak hingga
    printf("Program is running. Press Ctrl+C to interrupt.\n");

    while (1) {
        printf("Working...\n");
        sleep(1);  // Tunggu selama 1 detik setiap iterasi
    }

    return 0;
}
