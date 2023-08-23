#include "headers.h"

// Filter function for scandir: only displays non-hidden files
int filter(const struct dirent *dir) {
    return dir->d_name[0] != '.';
}

// Comparison function for scandir: sorts in lexicographical order
int comparator(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}

void display_permissions(mode_t mode) {
    char permissions[11];

    strcpy(permissions, "----------");
    if (S_ISDIR(mode)) permissions[0] = 'd';
    if (mode & S_IRUSR) permissions[1] = 'r';
    if (mode & S_IWUSR) permissions[2] = 'w';
    if (mode & S_IXUSR) permissions[3] = 'x';
    if (mode & S_IRGRP) permissions[4] = 'r';
    if (mode & S_IWGRP) permissions[5] = 'w';
    if (mode & S_IXGRP) permissions[6] = 'x';
    if (mode & S_IROTH) permissions[7] = 'r';
    if (mode & S_IWOTH) permissions[8] = 'w';
    if (mode & S_IXOTH) permissions[9] = 'x';

    printf("%s ", permissions);
}

void display_long_format(const char* dirname, struct dirent *entry) {
    struct stat st;
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s", dirname, entry->d_name);
    
    if (stat(filepath, &st) != 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    display_permissions(st.st_mode);
    printf("%hu ", st.st_nlink);

    struct passwd *pwd = getpwuid(st.st_uid);
    if (pwd) {
        printf("%s ", pwd->pw_name);
    } else {
        printf("%d ", st.st_uid);
    }

    struct group *grp = getgrgid(st.st_gid);
    if (grp) {
        printf("%s ", grp->gr_name);
    } else {
        printf("%d ", st.st_gid);
    }

    printf("%lld ", st.st_size);

    char timebuf[64];
    struct tm *timeinfo = localtime(&st.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
    printf("%s ", timebuf);

    // if entry is a file, display it in white, if it is a directory display it in blue, if it is a executable display it in green
    if (S_ISDIR(st.st_mode)) {
        printf("\e[0;34m%s\033[0m\n", entry->d_name);
    } else if (st.st_mode & S_IXUSR) {
        printf("\e[0;32m%s\033[0m\n", entry->d_name);
    } else {
        printf("%s\n", entry->d_name);
    }
}