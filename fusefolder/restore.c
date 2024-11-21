#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void restoreFileNames(const char *directoryPath) {
    struct dirent *entry;
    DIR *dir = opendir(directoryPath);

    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }

    char oldPath[1024], newPath[1024];
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Only process regular files
            snprintf(oldPath, sizeof(oldPath), "%s/%s", directoryPath, entry->d_name);

            // Reverse the file name
            char reversedName[1024];
            strcpy(reversedName, entry->d_name);
            reverseString(reversedName);

            snprintf(newPath, sizeof(newPath), "%s/%s", directoryPath, reversedName);
            if (rename(oldPath, newPath) == 0) {
                printf("Renamed: %s -> %s\n", oldPath, newPath);
            } else {
                perror("Failed to rename file");
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    restoreFileNames(argv[1]);
    return 0;
}