#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

void* check_prime(void* arg) {
    int n = *(int*)arg;
    bool is_prime = true;

    // Jika angka kurang dari 2, maka bukan prima
    if (n <= 1) {
        is_prime = false;
    } else {
        // Cek dari 2 hingga sqrt(n)
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                is_prime = false;
                break;
            }
        }
    }

    // Cetak hasil
    if (is_prime)
        printf("%d prima\n", n);
    else
        printf("%d bukan prima\n", n);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <num1> <num2> ... <numN>\n", argv[0]);
        return 1;
    }

    int num_count = argc - 1;
    pthread_t threads[num_count];
    int numbers[num_count];

    // Membuat thread untuk setiap angka
    for (int i = 0; i < num_count; i++) {
        numbers[i] = atoi(argv[i + 1]);
        pthread_create(&threads[i], NULL, check_prime, (void*)&numbers[i]);
    }

    // Menunggu semua thread selesai
    for (int i = 0; i < num_count; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}