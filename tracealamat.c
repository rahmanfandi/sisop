#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 256  // Ukuran setiap halaman (bytes)
#define FRAME_SIZE 256 // Ukuran setiap frame (bytes)
#define MAX_SEGMENTS 4 // Jumlah maksimum segmen
#define MAX_PAGES 8    // Jumlah maksimum halaman per segmen
#define PHYSICAL_MEMORY_SIZE 1024 // Ukuran memori fisik (bytes)

// Struktur untuk tabel halaman
typedef struct {
    int frameNumber; // Frame di mana halaman berada
    int isValid;     // Apakah halaman valid (sudah dimuat)
} PageTableEntry;

// Struktur untuk tabel segmen
typedef struct {
    PageTableEntry pageTable[MAX_PAGES]; // Tabel halaman untuk segmen
    int pageCount;                       // Jumlah halaman dalam segmen
} SegmentTable;

// Fungsi untuk melacak alamat memori fisik
int translateAddress(SegmentTable segments[], int segmentNumber, int virtualAddress) {
    // Hitung page number dan offset
    int pageNumber = virtualAddress / PAGE_SIZE;
    int pageOffset = virtualAddress % PAGE_SIZE;

    // Validasi segment
    if (segmentNumber >= MAX_SEGMENTS || segments[segmentNumber].pageCount <= pageNumber) {
        printf("Invalid segment or page number.\n");
        return -1;
    }

    // Ambil entry dari tabel halaman
    PageTableEntry entry = segments[segmentNumber].pageTable[pageNumber];

    if (!entry.isValid) {
        printf("Page fault! Segment %d, Page %d not loaded.\n", segmentNumber, pageNumber);
        return -1;
    }

    // Hitung alamat fisik
    int physicalAddress = (entry.frameNumber * FRAME_SIZE) + pageOffset;
    return physicalAddress;
}

int main() {
    // Inisialisasi memori fisik
    int physicalMemory[PHYSICAL_MEMORY_SIZE] = {0};

    // Inisialisasi tabel segmen
    SegmentTable segments[MAX_SEGMENTS];
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        segments[i].pageCount = rand() % MAX_PAGES + 1; // Acak jumlah halaman per segmen
        for (int j = 0; j < segments[i].pageCount; j++) {
            segments[i].pageTable[j].frameNumber = rand() % (PHYSICAL_MEMORY_SIZE / FRAME_SIZE); // Acak frame
            segments[i].pageTable[j].isValid = 1; // Tandai halaman valid
        }
    }

    // Tampilkan konfigurasi tabel segmen dan halaman
    printf("Segment Table Configuration:\n");
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        printf("Segment %d: %d pages\n", i, segments[i].pageCount);
        for (int j = 0; j < segments[i].pageCount; j++) {
            printf("  Page %d -> Frame %d (Valid: %d)\n", j, segments[i].pageTable[j].frameNumber,
                   segments[i].pageTable[j].isValid);
        }
    }

    // Simulasi pelacakan alamat memori
    printf("\nSimulating address translation:\n");
    int segmentNumber, virtualAddress;

    while (1) {
        printf("\nEnter Segment Number (-1 to exit): ");
        scanf("%d", &segmentNumber);
        if (segmentNumber == -1) break;

        printf("Enter Virtual Address: ");
        scanf("%d", &virtualAddress);

        int physicalAddress = translateAddress(segments, segmentNumber, virtualAddress);
        if (physicalAddress != -1) {
            printf("Physical Address: %d\n", physicalAddress);
        }
    }

    return 0;
}