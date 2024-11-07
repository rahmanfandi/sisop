#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10

int main(int argc, char const *argv[]) {
    struct sockaddr_in serv_addr;
    int sock = 0, valread, epoll_fd;
    struct epoll_event event, events[MAX_EVENTS];
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    // Membuat socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    // Menghubungkan socket ke server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Membuat epoll instance
    if ((epoll_fd = epoll_create1(0)) == -1) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    // Menambahkan socket ke epoll untuk pemantauan
    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event) == -1) {
        perror("epoll_ctl failed");
        exit(EXIT_FAILURE);
    }

    // Mengirim pesan ke server
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // Loop utama untuk menunggu event
    while (1) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        if (event_count == -1) {
            perror("epoll_wait failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == sock && (events[i].events & EPOLLIN)) {
                // Jika socket siap dibaca
                valread = read(sock, buffer, 1024);
                if (valread == 0) {
                    // Jika server menutup koneksi
                    printf("Server closed the connection\n");
                    close(sock);
                    exit(0);
                }
                printf("Received from server: %s\n", buffer);
            }
        }
    }

    close(sock);
    return 0;
}