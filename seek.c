#include "headers.h"

#include "headers.h"

void get_name_without_extension(const char *filename, char *name) {
    strcpy(name, filename);
    char *dot = strrchr(name, '.');
    if (dot) {
        *dot = '\0';  // Truncate the name at the last dot
    }
}

int search_directory(const char *dir_name, const char *search, int search_files, int search_dirs, int exec_flag, char *single_file_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat entry_stat;
    char path[1024];
    char name_without_extension[1024];
    int match_count = 0;

    if (!(dir = opendir(dir_name))) {
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        get_name_without_extension(entry->d_name, name_without_extension);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

            if (search_dirs && strcmp(search, name_without_extension) == 0) {
                printf("\033[34m%s\033[0m\n", path);  // print in blue
                match_count++;
                if (match_count == 1) {
                    strcpy(single_file_path, path);
                }
            }
            match_count += search_directory(path, search, search_files, search_dirs, exec_flag, single_file_path);
        } else {
            if (search_files && strcmp(search, name_without_extension) == 0) {
                snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
                printf("\033[32m%s\033[0m\n", path);  // print in green
                match_count++;
                if (match_count == 1) {
                    strcpy(single_file_path, path);
                }
            }
        }
    }
    closedir(dir);

    return match_count;
}