#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDR "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Membuat socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Menghubungkan ke server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Client connected with file descriptor: %d\n", sock);

    // Mengirim data ke server
    const char *message = "Hello from client";
    send(sock, message, strlen(message), 0);
    printf("Sent data to server: %s\n", message);

    // Menerima respons dari server
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate string
        printf("Received response from server: %s\n", buffer);
    }

    close(sock);
    return 0;
}