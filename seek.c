#include "headers.h"

void get_name_without_extension(const char *filename, char *name) {
    strcpy(name, filename);
    char *dot = strrchr(name, '.');
    if (dot) {
        *dot = '\0';  // Truncate the name at the last dot
    }
}

void search_directory(const char *dir_name, const char *search, int search_files, int search_dirs, int exec_flag) {
    DIR *dir;
    struct dirent *entry;
    struct stat entry_stat;
    char path[1024];
    char name_without_extension[1024];
    int found = 0;

    if (!(dir = opendir(dir_name))) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        get_name_without_extension(entry->d_name, name_without_extension);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

            // Check if the directory name matches the search term
            if (search_dirs && strcmp(search, name_without_extension) == 0) {
                printf("\033[34m%s\033[0m\n", path);  // print in blue
                found = 1;
                if (exec_flag) {
                    if (chdir(path) == 0) {
                        closedir(dir);
                        return;
                    } else {
                        printf("Missing permissions for task!\n");
                        closedir(dir);
                        return;
                    }
                }
            }
            search_directory(path, search, search_files, search_dirs, exec_flag);
        } else {
            if (search_files && strcmp(search, name_without_extension) == 0) {
                snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
                stat(path, &entry_stat);
                if (exec_flag) {
                    if (entry_stat.st_mode & S_IRUSR) {
                        printf("\033[32m%s\033[0m\n", path);  // print in green
                        found = 1;
                        FILE *fp = fopen(path, "r");
                        char ch;
                        while ((ch = fgetc(fp)) != EOF) {
                            putchar(ch);
                        }
                        fclose(fp);
                        closedir(dir);
                        return;
                    } else {
                        printf("Missing permissions for task!\n");
                        closedir(dir);
                        return;
                    }
                } else {
                    printf("\033[32m%s\033[0m\n", path);  // print in green
                    found = 1;
                }
            }
        }
    }
    closedir(dir);
    // if (!found) {
    //     printf("No match found!\n");
    // }
}