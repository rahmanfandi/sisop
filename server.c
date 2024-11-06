#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 5

int main() {
    int server_fd, new_socket, max_sd, sd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];
    fd_set readfds;
    int client_sockets[MAX_CLIENTS] = {0}; // Menyimpan soket client yang aktif

    // Membuat soket server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind soket ke alamat dan port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Mendengarkan koneksi masuk
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds); // Menambahkan server socket ke set

        max_sd = server_fd;

        // Menambahkan soket client yang aktif ke dalam set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Menunggu data pada soket yang siap untuk dibaca
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Select error");
        }

        // Koneksi masuk baru
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }
            printf("New connection, socket fd: %d\n", new_socket);

            // Menambahkan soket client yang baru ke dalam daftar
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // Memeriksa apakah ada data yang masuk pada soket client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds)) {
                int valread = read(sd, buffer, sizeof(buffer));
                if (valread == 0) {
                    // Client disconnect
                    close(sd);
                    client_sockets[i] = 0;
                    printf("Client disconnected, socket fd: %d\n", sd);
                } else {
                    // Menampilkan pesan dari client
                    buffer[valread] = '\0';
                    printf("Message from client: %s\n", buffer);
                    send(sd, "Message received\n", strlen("Message received\n"), 0);
                }
            }
        }
    }

    return 0;
}