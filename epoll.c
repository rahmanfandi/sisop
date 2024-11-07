#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_EVENTS 10
#define PORT 8080

int main() {
    int epoll_fd, server_fd, new_socket;
    struct epoll_event event, events[MAX_EVENTS];
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Membuat socket server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    // Membuat epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Tambahkan server_fd ke epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    printf("Epoll monitoring started\n");

    while (1) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_fd) {
                new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                event.events = EPOLLIN;
                event.data.fd = new_socket;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event);
                printf("New connection accepted\n");
            } else {
                char buffer[1024];
                int client_fd = events[i].data.fd;
                int bytes_read = read(client_fd, buffer, sizeof(buffer));
                if (bytes_read > 0) {
                    printf("Data received: %s\n", buffer);
                    send(client_fd, buffer, bytes_read, 0);
                } else {
                    close(client_fd);
                    printf("Connection closed\n");
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}