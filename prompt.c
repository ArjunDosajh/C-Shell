#include "headers.h"

int init_terminal()
{
    if (getcwd(home_directory, 1024) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    char pastevents_path[1024];
    strncpy(pastevents_path, home_directory, 1024);
    strcat(pastevents_path, "/.pastevents.txt");
    int fd = open(pastevents_path, O_CREAT | O_RDWR, 0644);
    if (fd == -1)
    {
        perror("open() error");
        return 1;
    }

    strncpy(current_working_direcotry, home_directory, 1024);

    if (gethostname(host_name, 1024) == -1)
    {
        perror("gethostname() error");
        return 1;
    }

    if (getlogin_r(system_name, 1024) == -1)
    {
        perror("getlogin_r() error");
        return 1;
    }

    return 0;
}

void prompt() {
    if(strstr(current_working_direcotry, home_directory) != NULL) {
        char relative_path[1024];
        strncpy(relative_path, current_working_direcotry + strlen(home_directory), 1024);
        if(long_process_time > 2) {
            printf("<\e[0;36m%s\033[0m@\e[0;33m%s\033[0m:\e[0;35m~%s\033[0m \e[0;34m%s : %ds\033[0m> ", host_name, system_name, relative_path, long_process, long_process_time);
            long_process_time = 0;
        } else {
            printf("<\e[0;36m%s\033[0m@\e[0;33m%s\033[0m:\e[0;35m~%s\033[0m > ", host_name, system_name, relative_path);
        }
        return;
    } else {
        if(long_process_time > 2) {
            printf("<\e[0;36m%s\033[0m@\e[0;33m%s\033[0m:\e[0;35m%s\033[0m %s : \e[0;34m%ds\033[0m> ", host_name, system_name, current_working_direcotry, long_process, long_process_time);
            long_process_time = 0;
        } else {
            printf("<\e[0;36m%s\033[0m@\e[0;33m%s\033[0m:\e[0;35m%s\033[0m> ", host_name, system_name, current_working_direcotry);
        }
    }
}
