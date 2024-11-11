#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket gagal\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Alamat tidak valid\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Koneksi gagal\n");
        return -1;
    }

    while (1) {
        printf("Masukkan perintah (POST/GET): ");
        fgets(message, sizeof(message), stdin);

        send(sock, message, strlen(message), 0);

        valread = read(sock, buffer, sizeof(buffer));
        buffer[valread] = '\0';
        printf("%s", buffer);

        memset(buffer, 0, sizeof(buffer)); // Bersihkan buffer
    }

    close(sock);
    return 0;
}
