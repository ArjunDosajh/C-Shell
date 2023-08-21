#include "headers.h"

// char **all_commands_list = (char **)(malloc)(sizeof(char *) * 100);
char *all_commands_list[] = {
    "sleep",
    "echo"
};
int number_of_commands = 1;

void execute_command(char *command, int isAnd) {
    char *strtok_state;
    char delimiters[] = " ";
    char *token = strtok_r(command, delimiters, &strtok_state);
    for(int i = 0; i < number_of_commands; i++) {
        if(strcmp(token, "sleep") == 0) {
            printf("Valid command: %s\n", token);
            Sleep(strtok_state);
        } else if(strcmp(token, "echo") == 0) {
            printf("Valid command: %s\n", token);
            Echo(strtok_state);
        } else if(strcmp(token, "warp") == 0) {
            // printf("Valid command: %s\n", token);
            Warp(strtok_state);
        }
        else {
            // ERR: not in the list of all commands
            printf("Invalid command: %s\n", token);
            return;
        }
    }
}

void Sleep(char *strtok_state) {
    char *token = strtok_r(NULL, " ", &strtok_state);
    if(token == NULL) {
        // ERR: no time specified
        printf("No time specified\n");
        return;
    }
    int sleep_time = atoi(token);
    printf("Sleeping for %d seconds\n", sleep_time);
    fflush(stdout);
    sleep(sleep_time);
}

void Echo(char *strtok_state) {
    char *token = strtok_r(NULL, " ", &strtok_state);
    if(token == NULL) {
        // ERR: no string specified
        printf("No string specified\n");
        return;
    }
    printf("%s\n", token);
}

void Warp(char *strtok_state) {
    char *token = strtok_r(NULL, " ", &strtok_state);
    char new_path[PATH_MAX];

    while(token) {
        if(token == NULL) {
            // home directory
            strcpy(new_path, home_directory);
        } else if(token[0] == '~') {
            strcpy(new_path, home_directory);
            strcat(new_path, token + 1);
        } else if(token[0] == '-') {
            // previous directory
            strcpy(new_path, previous_working_directory);
        }
         else if(token[0] == '.' && token[1] == '.') {
            // parent directory
            char temp[PATH_MAX];
            strcpy(temp, current_working_direcotry);
            char *last_slash = strrchr(temp, '/');
            *last_slash = '\0';
            strcpy(new_path, temp);
        } else if(token[0] == '.') {
            // current directory
            strcpy(new_path, current_working_direcotry);
        } else if(token[0] != '/' && token[0] != '.') {
            // relative path
            char temp[PATH_MAX];
            strcpy(temp, current_working_direcotry);
            strcat(temp, "/");
            strcat(temp, token);
            strcpy(new_path, temp);
        } else {
            // absolute path
            strcpy(new_path, token);
        }
        struct stat st;
        int result = stat(new_path, &st);

        if (result == 0) {
            if (S_ISDIR(st.st_mode)) {
                printf("%s\n", new_path);
                strcpy(previous_working_directory, current_working_direcotry);
                strcpy(current_working_direcotry, new_path);
            } else {
                printf("The path is not a directory.\n");
            }
        } else {
            perror("Error");
        }

        token = strtok_r(NULL, " ", &strtok_state);
    }
}

void tokeniseCommands(char *input) {
    // example input = "this;is&a;new&command"
    // remove the \n at the end of input 
    input[strlen(input) - 1] = ';';
    char inputCopy[4096];
    strcpy(inputCopy, input);

    char delimiters[] = ";&";
    char *commandToken = strtok(input, delimiters);

    int commandEndPosition = 0, numTokens = 0;
    while(commandToken != NULL) {
        int isAnd = 0;
        numTokens++;
        commandEndPosition += numTokens == 1 ? strlen(commandToken) : strlen(commandToken) + 1;
        char delimiter = inputCopy[commandEndPosition];
        // printf("Command: %s, Delimiter: %c, tokenLen: %lu\n", token, delimiter, strlen(token));
        if(delimiter == '&') {
            isAnd = 1;
        }
        execute_command(commandToken, isAnd);
        commandToken = strtok(NULL, delimiters);
    }
}