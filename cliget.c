#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void send_command(int sock, const char* command, const char* value) {
    char buffer[1024] = {0};

    // Kirim perintah ke server
    write(sock, command, strlen(command));
    sleep(1); // Beri waktu untuk server memproses
    
    if (strcmp(command, "POST") == 0 && value != NULL) {
        // Kirim angka jika perintahnya POST
        write(sock, value, strlen(value));
    }

    // Terima respons dari server
    read(sock, buffer, 1024);
    printf("%s", buffer);
}

int main(int argc, char const* argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Membuat socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket gagal dibuat\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Mengonversi alamat IP
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Alamat tidak valid\n");
        return -1;
    }

    // Menghubungkan ke server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Koneksi gagal\n");
        return -1;
    }

    // Contoh penggunaan: mengirim perintah POST dan GET
    send_command(sock, "POST", "5");
    send_command(sock, "POST", "3");
    send_command(sock, "GET", NULL);

    close(sock);
    return 0;
}