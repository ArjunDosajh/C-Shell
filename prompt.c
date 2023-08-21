#include "headers.h"

int init_terminal()
{
    // this function gets the data for the initial prompt of terminal
    if (getcwd(home_directory, PATH_MAX) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    strncpy(current_working_direcotry, home_directory, PATH_MAX);
    strncpy(previous_working_directory, home_directory, PATH_MAX);

    if (gethostname(host_name, PATH_MAX) == -1)
    {
        perror("gethostname() error");
        return 1;
    }

    if (getlogin_r(system_name, PATH_MAX) == -1)
    {
        perror("getlogin_r() error");
        return 1;
    }

    return 0;
}

void prompt() {
    // Do not hardcode the prmopt
    // check if the home_directory is a substring of the current_working_direcotry
    if(strstr(current_working_direcotry, home_directory) != NULL) {
        char relative_path[PATH_MAX];
        strncpy(relative_path, current_working_direcotry + strlen(home_directory), PATH_MAX);
        printf("<\e[0;36m%s\033[0m@\e[0;33m%s\033[0m:\e[0;35m~%s\033[0m> ", host_name, system_name, relative_path);
        return;
    } else {
        printf("<\e[0;36m%s\033[0m@\e[0;33m%s\033[0m:\e[0;35m~%s\033[0m> ", host_name, system_name, current_working_direcotry);
    }
}
