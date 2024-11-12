#include <stdio.h>
#include <stdlib.h>

// Ukuran partisi yang tersedia
size_t partition_sizes[] = {30, 20, 10, 50,60, 70};
int num_partitions = sizeof(partition_sizes) / sizeof(partition_sizes[0]);

// Ukuran data yang ingin dialokasikan
size_t data_sizes[] = {40, 10, 25, 60, 50};

int num_data = sizeof(data_sizes) / sizeof(data_sizes[0]);

// Array untuk menandai partisi yang sudah terpakai
int used[7];  // Ukuran array sesuai dengan jumlah partisi

// Fungsi untuk mereset status partisi
void reset_used() {
    for (int i = 0; i < num_partitions; i++) {
        used[i] = 0;
    }
}

// Fungsi Best Fit
void best_fit() {
    printf("\nBest Fit:\n");
    for (int d = 0; d < num_data; d++) {
        int best_index = -1;
        for (int i = 0; i < num_partitions; i++) {
            if (!used[i] && partition_sizes[i] >= data_sizes[d]) {
                if (best_index == -1 || partition_sizes[i] < partition_sizes[best_index]) {
                    best_index = i;
                }
            }
        }
        if (best_index != -1) {
            used[best_index] = 1; // Tandai partisi sebagai terpakai
            printf("Data %zu bytes ditempatkan di partisi %d dengan %zu bytes\n", data_sizes[d], best_index + 1, partition_sizes[best_index]);
        } else {
            printf("Data %zu bytes tidak dapat dialokasikan\n", data_sizes[d]);
        }
    }
    reset_used(); // Reset array used setelah algoritma selesai
}

// Fungsi First Fit
void first_fit() {
    printf("\nFirst Fit:\n");
    for (int d = 0; d < num_data; d++) {
        int allocated = 0;
        for (int i = 0; i < num_partitions; i++) {
            if (!used[i] && partition_sizes[i] >= data_sizes[d]) {
                used[i] = 1; // Tandai partisi sebagai terpakai
                printf("Data %zu bytes ditempatkan di partisi %d dengan %zu bytes\n", data_sizes[d], i + 1, partition_sizes[i]);
                allocated = 1;
                break; // Cukup satu partisi yang cukup besar
            }
        }
        if (!allocated) {
            printf("Data %zu bytes tidak dapat dialokasikan\n", data_sizes[d]);
        }
    }
    reset_used(); // Reset array used setelah algoritma selesai
}

// Fungsi Next Fit
void next_fit() {
    printf("\nNext Fit:\n");
    int last_index = 0;
    for (int d = 0; d < num_data; d++) {
        int allocated = 0;
        for (int i = 0; i < num_partitions; i++) {
            int index = (last_index + i) % num_partitions; // Mulai dari partisi terakhir yang dialokasikan
            if (!used[index] && partition_sizes[index] >= data_sizes[d]) {
                used[index] = 1; // Tandai partisi sebagai terpakai
                last_index = index; // Update partisi terakhir yang terpakai
                printf("Data %zu bytes ditempatkan di partisi %d dengan %zu bytes\n", data_sizes[d], index + 1, partition_sizes[index]);
                allocated = 1;
                break;
            }
        }
        if (!allocated) {
            printf("Data %zu bytes tidak dapat dialokasikan\n", data_sizes[d]);
        }
    }
    reset_used(); // Reset array used setelah algoritma selesai
}

int main() {
    // Eksekusi masing-masing algoritma
    printf("=============================================\n");
    best_fit();
    printf("=============================================\n");
    first_fit();
    printf("=============================================\n");
    next_fit();
    return 0;
}
