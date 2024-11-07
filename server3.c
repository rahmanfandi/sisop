#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10  // Maksimum jumlah client yang bisa dilayani

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;  // Jumlah awal file descriptor (hanya server socket)

    // Membuat socket server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Konfigurasi alamat server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind dan listen pada port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Menambahkan server_fd ke poll set
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;  // Memantau untuk koneksi masuk

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Panggil poll() untuk memonitor event pada file descriptor
        int poll_count = poll(fds, nfds, -1);
        if (poll_count < 0) {
            perror("Poll failed");
            exit(EXIT_FAILURE);
        }

        // Loop melalui file descriptor yang aktif
        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                // Jika ada koneksi baru pada server socket
                if (fds[i].fd == server_fd) {
                    client_fd = accept(server_fd, NULL, NULL);
                    if (client_fd < 0) {
                        perror("Accept failed");
                        continue;
                    }
                    printf("New client connected with file descriptor: %d\n", client_fd);

                    // Tambahkan client baru ke poll set
                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } else {
                    // Menerima data dari client yang ada
                    int bytes_received = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0);
                    if (bytes_received <= 0) {
                        printf("Closing connection with file descriptor: %d\n", fds[i].fd);
                        close(fds[i].fd);
                        fds[i] = fds[nfds - 1];  // Menghapus fd dari array
                        nfds--;
                    } else {
                        buffer[bytes_received] = '\0';  // Null-terminate string
                        printf("Received data from file descriptor %d: %s\n", fds[i].fd, buffer);

                        // Mengirim respons ke client
                        const char *response = "Hello from server";
                        send(fds[i].fd, response, strlen(response), 0);
                        printf("Sent response to file descriptor %d: %s\n", fds[i].fd, response);
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}