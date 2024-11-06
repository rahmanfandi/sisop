#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAX_SOCKETS 10 // Maksimal soket yang ingin dipantau

int main() {
    int server_fd, client_fd, max_fd, num_clients = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    fd_set readfds;
    struct timeval timeout;
    int client_fds[MAX_SOCKETS] = {0}; // Menyimpan FD klien

    // Membuat socket untuk server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Menyiapkan alamat untuk server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    // Mengikat socket ke alamat server
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    // Mendengarkan koneksi masuk
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    // Menyiapkan fd_set
    FD_ZERO(&readfds);  // Kosongkan fd_set
    FD_SET(server_fd, &readfds);  // Masukkan server_fd ke dalam readfds
    max_fd = server_fd;  // Set fd tertinggi

    printf("Server is listening on port 12345...\n");

    while (1) {
        // Menetapkan waktu timeout (5 detik)
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // Memanggil select untuk memonitor soket
        int activity = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

        if (activity == -1) {
            perror("select error");
            close(server_fd);
            return 1;
        }

        // Memeriksa apakah ada koneksi baru pada server_fd
        if (FD_ISSET(server_fd, &readfds)) {
            addr_len = sizeof(client_addr);
            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
            if (client_fd < 0) {
                perror("Accept failed");
                continue;
            }

            printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Tambahkan soket klien ke dalam fd_set
            if (num_clients < MAX_SOCKETS) {
                client_fds[num_clients++] = client_fd;
                FD_SET(client_fd, &readfds);
                if (client_fd > max_fd) {
                    max_fd = client_fd;  // Update max_fd
                }
            } else {
                printf("Max number of clients reached.\n");
                close(client_fd);
            }
        }

        // Memeriksa apakah ada data pada soket klien yang ada
        for (int i = 0; i < num_clients; i++) {
            if (FD_ISSET(client_fds[i], &readfds)) {
                char buffer[1024];
                int n = recv(client_fds[i], buffer, sizeof(buffer), 0);
                if (n <= 0) {
                    if (n == 0) {
                        printf("Client disconnected: %d\n", client_fds[i]);
                    } else {
                        perror("recv failed");
                    }
                    close(client_fds[i]);
                    FD_CLR(client_fds[i], &readfds);  // Hapus dari fd_set
                    client_fds[i] = client_fds[--num_clients];  // Pindahkan FD terakhir ke posisi kosong
                } else {
                    buffer[n] = '\0';  // Pastikan null-terminated string
                    printf("Received from client %d: %s\n", client_fds[i], buffer);
                }
            }
        }

        // Memeriksa apakah terjadi timeout
        if (activity == 0) {
            printf("Timeout reached, no activity detected.\n");
        }
    }

    close(server_fd);
    return 0;
}