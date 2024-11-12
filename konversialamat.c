#include <stdio.h>

#define PAGE_SIZE 4096   // Ukuran page dalam byte (4 KB)
#define NUM_PAGES 5      // Jumlah halaman dalam page table

// Page table sederhana yang memetakan page number ke frame number
int page_table[NUM_PAGES] = {3, 5, 1, 4, 2};  // Page 0 ada di Frame 3, Page 1 ada di Frame 5, dst.

int convert_physical_to_logical(int physical_address) {
    int frame_number = physical_address / PAGE_SIZE;
    int offset = physical_address % PAGE_SIZE;
    
    int page_number = -1;
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i] == frame_number) {
            page_number = i;
            break;
        }
    }

    if (page_number == -1) {
        printf("Frame not found in page table.\n");
        return -1;
    }

    int logical_address = page_number * PAGE_SIZE + offset;
    printf("Physical Address: %d --> Logical Address: Page %d, Offset %d (Logical Address: %d)\n", 
           physical_address, page_number, offset, logical_address);
    
    return logical_address;
}

int main() {
    int physical_address;
    
    printf("Enter the physical address to convert: ");
    scanf("%d", &physical_address);

    int logical_address = convert_physical_to_logical(physical_address);
    
    if (logical_address != -1) {
        printf("The logical address is: %d\n", logical_address);
    }

    return 0;
}