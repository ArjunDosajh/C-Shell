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
        checkBackgroundProcesses();
        prompt();
        char input[4096];
        fgets(input, 4096, stdin);
        tokeniseCommands(input);
    }
}