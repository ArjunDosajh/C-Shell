#include "headers.h"
#ifndef PEEK_H
#define PEEK_H

int filter(const struct dirent *dir);
int comparator(const struct dirent **a, const struct dirent **b);
void display_permissions(mode_t mode);
void display_long_format(const char* dirname, struct dirent *entry);

#endif