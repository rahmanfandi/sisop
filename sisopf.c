#include <stdio.h>
#include <unistd.h>

int main() {
    // Membuat child process pertama
					printf("tes 1 \n");
    pid_t pid1 = fork();
					printf("tes 2 \n");
    if (pid1 < 0) {
        printf("Fork pertama gagal!\n");
        return 1;
    } else if (pid1 == 0) {
        // Ini adalah child process pertama
        printf("Ini adalah child process pertama. PID: %d\n", getpid());
        
        // Membuat child process kedua dari child process pertama
                            printf("tes 3 \n");
        pid_t pid2 = fork();
        					printf("tes 4 \n");
        if (pid2 < 0) {
            printf("Fork kedua gagal!\n");
            return 1;
        } else if (pid2 == 0) {
            // Ini adalah child process kedua
            printf("Ini adalah child process kedua. PID: %d\n", getpid());
        }
    } else {
        // Ini adalah parent process
        printf("Ini adalah parent process. PID: %d\n", getpid());
    }


printf("end ------------------------------------------------------ \n");
    return 0;
}
