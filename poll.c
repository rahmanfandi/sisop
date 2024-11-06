#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <arpa/inet.h>  // Untuk inet_pton()

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
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

    struct pollfd fds[2];
    fds[0].fd = 0;        // Monitor input dari stdin (keyboard)
    fds[0].events = POLLIN;

    fds[1].fd = sockfd;   // Monitor soket untuk data yang masuk dari server
    fds[1].events = POLLIN;

    while (1) {
        int ret = poll(fds, 2, -1); // Timeout = -1 berarti menunggu selamanya

        if (ret < 0) {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // Jika ada input dari stdin
        if (fds[0].revents & POLLIN) {
            fgets(buffer, sizeof(buffer), stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }

        // Jika ada data dari server
        if (fds[1].revents & POLLIN) {
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