#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

// Fungsi untuk membalikkan string
void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// Fungsi untuk membalikkan nama file (tanpa ekstensi)
void reverse_filename(const char *filename, char *reversed) {
    const char *dot = strrchr(filename, '.'); // Cari posisi titik terakhir
    if (dot) {
        int name_len = dot - filename;
        strncpy(reversed, filename, name_len); 
        reversed[name_len] = '\0'; // Terminasi null
        reverse_string(reversed);
        strcat(reversed, dot); // Tambahkan ekstensi
    } else {
        strcpy(reversed, filename);
        reverse_string(reversed);
    }
}

// Fungsi untuk memproses direktori
void process_directory(const char *path) {
    DIR *dp = opendir(path);
    if (!dp) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        if (snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name) >= sizeof(full_path)) {
            fprintf(stderr, "Path too long: %s/%s\n", path, entry->d_name);
            continue;
        }

        struct stat statbuf;
        if (stat(full_path, &statbuf) == -1) {
            perror("Unable to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // Rekursif jika folder diawali "Clem_"
            if (strncmp(entry->d_name, "Clem_", 5) == 0)
                process_directory(full_path);
        } else if (S_ISREG(statbuf.st_mode)) {
            char reversed_name[PATH_MAX];
            reverse_filename(entry->d_name, reversed_name);

            char new_path[PATH_MAX];
            if (snprintf(new_path, sizeof(new_path), "%s/%s", path, reversed_name) >= sizeof(new_path)) {
                fprintf(stderr, "New path too long: %s/%s\n", path, reversed_name);
                continue;
            }

            if (rename(full_path, new_path) == 0)
                printf("Renamed: %s -> %s\n", full_path, new_path);
            else
                perror("Error renaming file");
        }
    }
    closedir(dp);
}

int main() {
    char user_documents_path[PATH_MAX];
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: HOME environment variable is not set.\n");
        return 1;
    }

    if (snprintf(user_documents_path, sizeof(user_documents_path), "%s/Documents", home) >= sizeof(user_documents_path)) {
        fprintf(stderr, "Path to Documents is too long.\n");
        return 1;
    }

    process_directory(user_documents_path);
    return 0;
}