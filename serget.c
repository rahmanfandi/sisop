#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_NUMBERS 100

int numbers[MAX_NUMBERS];
int count = 0;

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    int num;
    
    // Baca perintah dari client
    read(client_socket, buffer, sizeof(buffer));

    if (strncmp(buffer, "POST", 4) == 0) {
        // Jika perintah adalah POST, terima angka
        read(client_socket, buffer, sizeof(buffer));
        num = atoi(buffer);

        if (count < MAX_NUMBERS) {
            numbers[count++] = num;
            write(client_socket, "Penambahan angka berhasil\n", strlen("Penambahan angka berhasil\n"));
        } else {
            write(client_socket, "Array penuh\n", strlen("Array penuh\n"));
        }
    } 
    else if (strncmp(buffer, "GET", 3) == 0) {
        // Jika perintah adalah GET, kirim semua angka yang tersimpan
        char result[1024] = {0};
        for (int i = 0; i < count; i++) {
            char num_str[10];
            sprintf(num_str, "%d ", numbers[i]);
            strcat(result, num_str);
        }
        strcat(result, "\n");
        write(client_socket, result, strlen(result));
    } 
    else {
        // Perintah tidak dikenal
        write(client_socket, "Bad request\n", strlen("Bad request\n"));
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Membuat socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket gagal");
        exit(EXIT_FAILURE);
    }

    // Mengatur opsi socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt gagal");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket ke port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind gagal");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Mulai mendengarkan koneksi
    if (listen(server_fd, 3) < 0) {
        perror("Listen gagal");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server berjalan di port %d...\n", PORT);

    // Loop untuk menerima koneksi dari client
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept gagal");
            continue;
        }

        handle_client(client_socket);
    }

    return 0;
}