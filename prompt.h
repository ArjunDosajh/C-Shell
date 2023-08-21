#ifndef __PROMPT_H
#define __PROMPT_H

char home_directory[PATH_MAX], host_name[PATH_MAX], system_name[PATH_MAX], current_working_direcotry[PATH_MAX], previous_working_directory[PATH_MAX];

int init_terminal();
void prompt();

#endif