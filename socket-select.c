#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int server_fd, client_fd, max_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    fd_set readfds;
    struct timeval timeout;

    // Membuat socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Menyeting alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    // Mengikat socket ke alamat
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

    // Menetapkan waktu timeout (5 detik)
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    printf("Server is listening on port 12345...\n");

    while (1) {
        // Memanggil select untuk memonitor socket
        int activity = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

        if (activity == -1) {
            perror("select error");
            close(server_fd);
            return 1;
        }

        printf("select() activity = %d\n", activity);

        // Memeriksa apakah ada activity dalam readfds
        if (FD_ISSET(server_fd, &readfds)) {
            // Jika server socket siap untuk menerima koneksi baru
            addr_len = sizeof(client_addr);
            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
            if (client_fd < 0) {
                perror("Accept failed");
                continue;
            }

            printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            FD_SET(client_fd, &readfds);  // Masukkan client socket ke dalam readfds
            if (client_fd > max_fd) {
                max_fd = client_fd;  // Update max_fd jika client_fd lebih besar
            }
        }

        // Memeriksa aktivitas pada socket lain
        for (int i = server_fd + 1; i <= max_fd; i++) {
            if (FD_ISSET(i, &readfds)) {
                char buffer[1024];
                int bytes_read = read(i, buffer, sizeof(buffer));
                if (bytes_read == 0) {
                    // Klien menutup koneksi
                    printf("Closing connection on socket %d\n", i);
                    close(i);
                    FD_CLR(i, &readfds);  // Hapus socket dari fd_set
                } else {
                    // Data diterima dari klien
                    buffer[bytes_read] = '\0';  // Menambahkan null-terminator
                    printf("Received data from socket %d: %s\n", i, buffer);
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