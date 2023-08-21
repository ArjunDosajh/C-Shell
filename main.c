#include "headers.h"

int main()
{
    if(init_terminal() == 1) {
        printf("init_terminal() error");
        return 1;
    }
    // Keep accepting commands
    while (1)
    {
        // SPEC 1 --> Print appropriate prompt with username, systemname and directory before accepting input
        // char home_directory[PATH_MAX], host_name[PATH_MAX], system_name[PATH_MAX], current_working_direcotry[PATH_MAX];

        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        tokeniseCommands(input);
    }
}