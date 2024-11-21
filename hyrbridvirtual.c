#include <stdio.h>
#include <stdlib.h>

#define SEGMENT_COUNT 3  // Jumlah segmen
#define PAGE_SIZE 4      // Ukuran halaman
#define FRAME_COUNT 8    // Jumlah frame dalam memori fisik
#define MAX_PAGES 5      // Jumlah maksimum halaman per segmen

// Struktur untuk merepresentasikan halaman dalam memori fisik
typedef struct {
    int segmentNumber;
    int pageNumber;
    int isOccupied;
} Frame;

// Struktur untuk merepresentasikan tabel halaman setiap segmen
typedef struct {
    int pages[MAX_PAGES];
    int pageCount;
} SegmentTable;

// Fungsi untuk mencetak status frame dalam memori fisik
void printFrames(Frame frames[], int frameCount) {
    printf("Physical memory: ");
    for (int i = 0; i < frameCount; i++) {
        if (frames[i].isOccupied) {
            printf("[S%d P%d] ", frames[i].segmentNumber, frames[i].pageNumber);
        } else {
            printf("[ ] ");
        }
    }
    printf("\n");
}

int main() {
    SegmentTable segments[SEGMENT_COUNT];
    Frame frames[FRAME_COUNT];
    int nextFrameToReplace = 0;  // Untuk mengganti frame (FIFO)

    // Inisialisasi segmen
    for (int i = 0; i < SEGMENT_COUNT; i++) {
        segments[i].pageCount = rand() % MAX_PAGES + 1;  // Jumlah halaman acak untuk setiap segmen
        for (int j = 0; j < segments[i].pageCount; j++) {
            segments[i].pages[j] = -1;  // Awalnya halaman tidak dimuat
        }
    }

    // Inisialisasi frame
    for (int i = 0; i < FRAME_COUNT; i++) {
        frames[i].isOccupied = 0;
    }

    printf("Segments and page tables:\n");
    for (int i = 0; i < SEGMENT_COUNT; i++) {
        printf("Segment %d: %d pages\n", i, segments[i].pageCount);
    }
    printf("\n");

    // Simulasi akses memori
    printf("Simulating memory access:\n");
    for (int step = 0; step < 10; step++) {
        int segment = rand() % SEGMENT_COUNT;
        int page = rand() % segments[segment].pageCount;

        printf("Accessing S%d P%d -> ", segment, page);

        // Cek apakah halaman sudah ada di memori fisik
        int isPageHit = 0;
        for (int i = 0; i < FRAME_COUNT; i++) {
            if (frames[i].isOccupied && frames[i].segmentNumber == segment && frames[i].pageNumber == page) {
                isPageHit = 1;
                break;
            }
        }

        if (!isPageHit) {
            // Page fault - muat halaman ke memori fisik
            printf("Page Fault! ");
            frames[nextFrameToReplace].segmentNumber = segment;
            frames[nextFrameToReplace].pageNumber = page;
            frames[nextFrameToReplace].isOccupied = 1;
            nextFrameToReplace = (nextFrameToReplace + 1) % FRAME_COUNT;
        } else {
            printf("Page Hit! ");
        }

        // Cetak status memori fisik
        printFrames(frames, FRAME_COUNT);
    }

    return 0;
}