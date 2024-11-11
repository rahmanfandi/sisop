#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX_NUMBERS 100

int numbers[MAX_NUMBERS];
int count = 0;

void handle_client(int new_socket) {
    char buffer[1024] = {0};
    int valread;

    while ((valread = read(new_socket, buffer, sizeof(buffer))) > 0) {
        buffer[valread] = '\0';

        if (strncmp(buffer, "POST", 4) == 0) {
            int num;
            sscanf(buffer + 5, "%d", &num); // Ambil angka setelah 'POST '

            if (count < MAX_NUMBERS) {
                numbers[count++] = num;
                send(new_socket, "Penambahan angka berhasil\n", strlen("Penambahan angka berhasil\n"), 0);
            } else {
                send(new_socket, "Storage penuh\n", strlen("Storage penuh\n"), 0);
            }

        } else if (strncmp(buffer, "GET", 3) == 0) {
            char response[1024] = {0};
            for (int i = 0; i < count; i++) {
                char num_str[12];
                sprintf(num_str, "%d ", numbers[i]);
                strcat(response, num_str);
            }
            strcat(response, "\n");
            send(new_socket, response, strlen(response), 0);

        } else {
            send(new_socket, "Bad request\n", strlen("Bad request\n"), 0);
        }

        memset(buffer, 0, sizeof(buffer)); // Bersihkan buffer
    }

    close(new_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket gagal");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind gagal");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen gagal");
        exit(EXIT_FAILURE);
    }

    printf("Server berjalan di port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept gagal");
            exit(EXIT_FAILURE);
        }

        handle_client(new_socket);
    }

    return 0;
}
