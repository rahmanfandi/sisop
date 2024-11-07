#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    fd_set read_fds;
    char buffer[BUFFER_SIZE];

    // Membuat socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);

    while (1) {
        fd_set temp_fds = read_fds;
        select(server_fd + 1, &temp_fds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &temp_fds)) {
            client_fd = accept(server_fd, NULL, NULL);
            FD_SET(client_fd, &read_fds);
        }

        for (int i = 0; i <= server_fd; i++) {
            if (FD_ISSET(i, &temp_fds) && i != server_fd) {
                int bytes_received = recv(i, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    close(i);
                    FD_CLR(i, &read_fds);
                } else {
                    send(i, "Hello", 5, 0);
                }
            }
        }
    }

    return 0;
}