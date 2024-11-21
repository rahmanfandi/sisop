#include <stdio.h>
#include <stdlib.h>
int main() {
    // Ukuran masing-masing partisi
    size_t partition_sizes[] = {512, 1024, 2048, 4096};  // Ukuran dalam byte
    int num_partitions = sizeof(partition_sizes) / sizeof(partition_sizes[0]);
    // Alokasi memori untuk setiap partisi dengan ukuran berbeda
    void *partitions[num_partitions];
    for (int i = 0; i < num_partitions; i++) {
        partitions[i] = malloc(partition_sizes[i]);
        if (partitions[i] == NULL) {
            printf("Memory allocation failed for partition %d!\n", i + 1);
            return 1;
        }
        printf("Partition %d: Allocated %zu bytes at address %p\n", 
               i + 1, partition_sizes[i], partitions[i]);
    }
    // Bebaskan memori setelah digunakan
    for (int i = 0; i < num_partitions; i++) {
        free(partitions[i]);
    }
    return 0;
}
