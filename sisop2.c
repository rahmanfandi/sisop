#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid1 = fork();

    if (pid1 < 0) {
        printf("Fork pertama gagal!\n");
        return 1;
    } else if (pid1 == 0) {
        printf("Ini adalah child process pertama. PID: %d\n", getpid());
        
        pid_t pid2 = fork();
        
        if (pid2 < 0) {
            printf("Fork kedua gagal!\n");
            return 1;
        } else if (pid2 == 0) {
            printf("Ini adalah child process kedua. PID: %d\n", getpid());
        }
    } else {
        printf("Ini adalah parent process. PID: %d\n", getpid());
    }

    return 0;
}
