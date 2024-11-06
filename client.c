#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>  // Menambahkan header ini untuk inet_pton

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // Membuat soket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Mengonversi alamat IP
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Mengonfigurasi soket untuk terhubung ke server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds); // Memantau input dari terminal (stdin)
        FD_SET(sockfd, &readfds); // Memantau data yang datang dari server

        // Menunggu data yang siap dibaca
        int activity = select(sockfd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Jika ada data dari stdin (input user)
        if (FD_ISSET(0, &readfds)) {
            fgets(buffer, sizeof(buffer), stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }

        // Jika ada data dari server
        if (FD_ISSET(sockfd, &readfds)) {
            int valread = read(sockfd, buffer, sizeof(buffer));
            if (valread == 0) {
                printf("Server disconnected\n");
                close(sockfd);
                exit(0);
            } else {
                buffer[valread] = '\0';
                printf("Server: %s\n", buffer);
            }
        }
    }

    return 0;
}