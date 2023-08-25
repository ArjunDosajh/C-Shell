#ifndef SEEK_H
#define SEEK_H

void get_name_without_extension(const char *filename, char *name);
int search_directory(const char *dir_name, const char *search, int search_files, int search_dirs, int exec_flag, char *single_file_path);

#endif