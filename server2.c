#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10

int main() {
    int server_fd, new_socket, epoll_fd, event_count;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    
    // Membuat socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Mengatur socket option
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Menyiapkan alamat server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Mengikat socket ke alamat
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Mendengarkan koneksi
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Membuat epoll instance
    if ((epoll_fd = epoll_create1(0)) == -1) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    // Menambahkan server socket ke epoll
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl failed");
        exit(EXIT_FAILURE);
    }

    // Loop utama untuk memantau event
    struct epoll_event events[MAX_EVENTS];
    while (1) {
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        
        if (event_count == -1) {
            perror("epoll_wait failed");
            exit(EXIT_FAILURE);
        }

        // Menangani setiap event yang terdeteksi
        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == server_fd) {
                // Jika server_fd siap, menerima koneksi masuk
                new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                if (new_socket < 0) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }

                // Menambahkan soket baru ke epoll untuk pemantauan lebih lanjut
                event.events = EPOLLIN;
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("epoll_ctl failed");
                    exit(EXIT_FAILURE);
                }

                printf("New connection accepted\n");
            } else if (events[i].events & EPOLLIN) {
                // Jika soket yang diawasi siap dibaca
                valread = read(events[i].data.fd, buffer, 1024);
                if (valread == 0) {
                    // Jika klien menutup koneksi, hapus dari epoll
                    close(events[i].data.fd);
                    printf("Connection closed\n");
                } else {
                    // Menampilkan data yang diterima
                    printf("Received: %s\n", buffer);
                    // Mengirim pesan kembali ke klien
                    send(events[i].data.fd, hello, strlen(hello), 0);
                    printf("Hello message sent\n");
                }
            }
        }
    }

    close(server_fd);
    return 0;
}