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
        // char home_directory[1024], host_name[1024], system_name[1024], current_working_direcotry[1024];

        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        tokeniseCommands(input);
    }
}