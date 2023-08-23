#include "headers.h"

// char **all_commands_list = (char **)(malloc)(sizeof(char *) * 100);
int number_of_commands = 1;

int execute_command(char *command, int isAnd) {
    char *strtok_state;
    char delimiters[] = " ";
    char *token = strtok_r(command, delimiters, &strtok_state);
    int hasPastEvents = 0;
    for(int i = 0; i < number_of_commands; i++) {
        if(strcmp(token, "sleep") == 0) {
            Sleep(strtok_state);
        } else if(strcmp(token, "echo") == 0) {
            Echo(strtok_state);
        } else if(strcmp(token, "warp") == 0) {
            Warp(strtok_state);
        } else if(strcmp(token, "clear" )== 0) {
            Clear();
        } else if(strcmp(token, "peek") == 0) {
            Peek(strtok_state);
        } else if(strcmp(token, "pastevents") == 0) {
            hasPastEvents = 1;
            token = strtok_r(NULL, " ", &strtok_state);
            if(token == NULL) {
                printPastEvents(strtok_state);
            }
            else if(strcmp(token, "purge") == 0) {
                // clear the file
                char past_events_file_path[PATH_MAX];
                strcpy(past_events_file_path, home_directory);
                strcat(past_events_file_path, "/.pastevents.txt");
                FILE *fp = fopen(past_events_file_path, "w");
                fclose(fp);
            } else if(strcmp(token, "execute") == 0) {
                executePastEvent(strtok_state);
            } else {
                printf("Invalid command for pastevents\n");
            }
        } else if(strcmp(token, "seek") == 0) {
            Seek(strtok_state);
        }
        else {
            // ERR: not in the list of all commands
            printf("Invalid command: %s\n", token);
            return -1;
        }
    }
    return hasPastEvents;
}

void executePastEvent(char *strtok_state) {
    char *token = strtok_r(NULL, " ", &strtok_state);
    if(token == NULL) {
        printf("Please give command number to execute!\n");
        return;
    }
    int command_no = atoi(token);
    if (command_no == 0) {
        printf("Command number should be an integer between 1 and 15 (inclusive)!\n");
        return;
    }

    char past_events_file_path[PATH_MAX];
    strcpy(past_events_file_path, home_directory);
    strcat(past_events_file_path, "/.pastevents.txt");
    FILE *file = fopen(past_events_file_path, "r");

    int number_of_lines = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            number_of_lines++;
        }
    }

    if (command_no <= 0 || command_no > number_of_lines) {
        printf("Command number should be an integer between 1 and 15 (inclusive)!\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_SET);
    int targetLine = number_of_lines - command_no + 1;
    for (int j = 1; j < targetLine; j++)
    {
        while ((c = fgetc(file)) != EOF && c != '\n');
    }

    char line[1024];
    if (fgets(line, sizeof(line), file) != NULL) {
        tokeniseCommands(line);
    }

    fclose(file);
}

void printPastEvents(char *strtok_state) {
    char past_events_file_path[PATH_MAX];
    strcpy(past_events_file_path, home_directory);
    strcat(past_events_file_path, "/.pastevents.txt");

    // open the file using open()
    int fd = open(past_events_file_path, O_CREAT | O_RDWR, 0644);
    if(fd == -1) {
        perror("Error");
        return;
    }

    // read the contents of the file
    char buffer[4096];
    int num_bytes_read = read(fd, buffer, 4096);
    if(num_bytes_read == -1) {
        perror("Error");
        return;
    }

    // if the file is empty, print "No past events"
    if(num_bytes_read == 0) {
        printf("No past events\n");
        return;
    }

    int num_lines = 0;
    for(int i = 0; i < num_bytes_read; i++) {
        if(buffer[i] == '\n') num_lines++;
    }

    // print the last 15 commands
    int start = 0, end = num_bytes_read - 1;
    int num_commands_to_print = 15;
    if(num_lines < 15) num_commands_to_print = num_lines;
    int num_commands_printed = 0;
    for(int i = num_bytes_read - 1; i >= 0; i--) {
        if(buffer[i] == '\n') {
            num_commands_printed++;
            if(num_commands_printed > num_commands_to_print) {
                start = i + 1;
                break;
            }
        }
    }
    for(int i = start; i < num_bytes_read; i++) {
        printf("%c", buffer[i]);
    }
    close(fd);
}

void Seek(char *strtok_state) {
    char *token = strtok_r(NULL, " ", &strtok_state);
    char search[1024];
    int search_files = 1, search_dirs = 1, exec_flag = 0;
    char *target_directory = ".";  // default to current directory
    int readFind = 0, readTarget = 0;

    // Parse flags
    while (token != NULL) {
        if (token[0] == '-') {
            if (strcmp(token, "-d") == 0) {
                search_dirs = 1;
                search_files = 0;
            } else if (strcmp(token, "-f") == 0) {
                search_files = 1;
                search_dirs = 0;
            } else if (strcmp(token, "-e") == 0) {
                exec_flag = 1;
            } else {
                // ERR: unknown flag
                printf("Unknown flag: %s\n", token);
                return;
            }
        } else if (readFind == 0) {
            printf("find is: %s\n", token);
            // search term
            strcpy(search, token);
            readFind = 1;
        } else if (readTarget == 0) {
            printf("target is: %s\n", token);
            // target directory
            if (token[0] == '~') {
                strcpy(target_directory, home_directory);
                strcat(target_directory, token + 1);
            } else if (token[0] == '-') {
                // previous directory
                strcpy(target_directory, previous_working_directory);
            } else if (token[0] == '.' && token[1] == '.') {
                // parent directory
                char temp[1024];
                strcpy(temp, current_working_direcotry);
                char *last_slash = strrchr(temp, '/');
                *last_slash = '\0';
                strcpy(target_directory, temp);
            } else if (token[0] == '.' && token[1] == '\0') {
                // current directory
                strcpy(target_directory, current_working_direcotry);
            } else if (token[0] == '/') {
                // absolute path
                strcpy(target_directory, token);
            } else {
                if (token[0] == '.' && token[1] == '/') token += 2;
                // relative path
                char temp[1024];
                strcpy(temp, current_working_direcotry);
                if (temp[strlen(temp) - 1] != '/') strcat(temp, "/");
                strcat(temp, token);
                strcpy(target_directory, temp);
                printf("target directory is: %s\n", target_directory);
            }
            readTarget = 1;
        }
        token = strtok_r(NULL, " ", &strtok_state);
    }

    if (!search_files && !search_dirs) {
        // ERR: invalid flags
        printf("Invalid flags!\n");
        return;
    }

    printf("Yayy");

    // search_directory(target_directory, search, search_files, search_dirs, exec_flag);
}

void Peek(char *strlok_state) {
    // peek is similar to the ls command in terminal
    // it takes in flags (-a and -l) and path as arguments and prints the contents of the directory
    // if no path is specified, it prints the contents of the current directory

    char *token = strtok_r(NULL, " ", &strlok_state);
    int showHidden = 0, showList = 0;
    while(1) {
        if(token && token[0] == '-') {
            // flags can be -a -l, -l -a, -a, -l, -al, -la

            if(strlen(token) == 2) {
                if(token[1] == 'a') showHidden = 1;
                else if(token[1] == 'l') showList = 1;
                else {
                    // ERR: invalid flag
                    printf("Invalid flag: %s\n", token);
                    return;
                }
            } else if(strlen(token) == 3) {
                if(token[1] == 'a' && token[2] == 'l') {
                    showHidden = 1;
                    showList = 1;
                } else if(token[1] == 'l' && token[2] == 'a') {
                    showHidden = 1;
                    showList = 1;
                } else {
                    // ERR: invalid flag
                    printf("Invalid flag: %s\n", token);
                    return;
                }
            } else {
                // ERR: invalid flag
                printf("Invalid flag: %s\n", token);
                return;
            }
        } else {
            // path is present
            // check if the path is valid
            // if valid, print the contents of the directory
            // if invalid, print error
            // if no path is specified, print the contents of the current directory
            char path[PATH_MAX];
            if(token == NULL) {
                strcpy(path, current_working_direcotry);
            } else if(token[0] == '~') {
                strcpy(path, home_directory);
                strcat(path, token + 1);
                printf("%s\n", path);
            } else if(token[0] == '-') {
                // previous directory
                strcpy(path, previous_working_directory);
            }
            else if(token[0] == '.' && token[1] == '.') {
                // parent directory
                char temp[PATH_MAX];
                strcpy(temp, current_working_direcotry);
                char *last_slash = strrchr(temp, '/');
                *last_slash = '\0';
                strcpy(path, temp);
            } else if(token[0] == '.' && token[1] == '\0') {
                // current directory
                strcpy(path, current_working_direcotry);
            } else if(token[0] == '/') {
                // absolute path
                strcpy(path, token);
            } else{
                if(token[0] == '.' && token[1] == '/') token += 2;
                // relative path
                char temp[PATH_MAX];
                strcpy(temp, current_working_direcotry);
                if(temp[strlen(temp) - 1] != '/') strcat(temp, "/");
                strcat(temp, token);
                strcpy(path, temp);
            }
            struct stat st;
            int result = stat(path, &st);

            if (result == 0) {
                if (S_ISDIR(st.st_mode)) {
                    struct dirent **namelist;
                    int n;
                    char* dirname = path; // default to current directory

                    if (showHidden) {
                        n = scandir(dirname, &namelist, NULL, comparator);
                    } else {
                        n = scandir(dirname, &namelist, filter, comparator);
                    }

                    if (n == -1) {
                        perror("scandir");
                        exit(EXIT_FAILURE);
                    }

                    for (int i = 0; i < n; i++) {
                        if (showList) {
                            display_long_format(dirname, namelist[i]);
                        } else {
                            // print files in white, dir in blue and execs in green
                            struct stat st;
                            char filepath[512];
                            snprintf(filepath, sizeof(filepath), "%s/%s", dirname, namelist[i]->d_name);

                            if (stat(filepath, &st) != 0) {
                                perror("stat");
                                exit(EXIT_FAILURE);
                            }

                            if (S_ISDIR(st.st_mode)) {
                                printf("\e[0;34m%s\033[0m\n", namelist[i]->d_name);
                            } else if (st.st_mode & S_IXUSR) {
                                printf("\e[0;32m%s\033[0m\n", namelist[i]->d_name);
                            } else {
                                printf("%s\n", namelist[i]->d_name);
                            }
                        }
                        free(namelist[i]);
                    }
                    free(namelist);
                    return;
                } else {
                    printf("The path is not a directory.\n");
                }
            } else {
                perror("Error");
            }
            break;
        }
        token = strtok_r(NULL, " ", &strlok_state);
    }
}

void Clear() {
    printf("\033[H\033[J");
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

    if(token == NULL) {
        strcpy(new_path, home_directory);

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
    }
    
    while(token) {
        if(token[0] == '~') {
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
        } else if(token[0] == '.' && token[1] == '\0') {
            // current directory
            strcpy(new_path, current_working_direcotry);
        } else if(token[0] == '/') {
            // absolute path
            strcpy(new_path, token);
        } else{
            if(token[0] == '.' && token[1] == '/') token += 2;
            // relative path
            char temp[PATH_MAX];
            strcpy(temp, current_working_direcotry);
            if(temp[strlen(temp) - 1] != '/') strcat(temp, "/");
            strcat(temp, token);
            strcpy(new_path, temp);
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

void saveInPastEvents(char *inputCopy2) {
    char past_events_file_path[PATH_MAX];
    strcpy(past_events_file_path, home_directory);
    strcat(past_events_file_path, "/.pastevents.txt");
    FILE *fp = fopen(past_events_file_path, "r");

    char *lines[4096];
    int num_lines = 0;
    
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        lines[num_lines] = strdup(buffer);
        num_lines++;
    }
    fclose(fp);

    if (num_lines > 0 && strcmp(lines[num_lines-1], inputCopy2) == 0) {
        for (int i = 0; i < num_lines; i++) {
            free(lines[i]);
        }
        return;
    }

    lines[num_lines++] = strdup(inputCopy2);

    if (num_lines > 15) {
        free(lines[0]);
        for (int i = 0; i < num_lines - 1; i++) {
            lines[i] = lines[i + 1];
        }
        num_lines--;
    }

    fp = fopen(past_events_file_path, "w");
    for (int i = 0; i < num_lines; i++) {
        fprintf(fp, "%s", lines[i]);
        free(lines[i]);
    }
    fclose(fp);
}

void tokeniseCommands(char *input) {
    // example input = "this;is&a;new&command"

    char inputCopy2[4096];
    strcpy(inputCopy2, input);
    inputCopy2[strlen(inputCopy2) - 1] = '\n';
    input[strlen(input) - 1] = ';';
    char inputCopy[4096];
    strcpy(inputCopy, input);

    char delimiters[] = ";&";
    char *commandToken = strtok(input, delimiters);

    int commandEndPosition = 0, numTokens = 0, hasPastEvents = 0;
    
    // executing the commands, one by one
    while(commandToken != NULL) {
        int isAnd = 0;
        numTokens++;
        commandEndPosition += strlen(commandToken);
        char delimiter = inputCopy[commandEndPosition];
        // printf("Command: %s, Delimiter: %c, tokenLen: %lu\n", token, delimiter, strlen(token));
        if(delimiter == '&') {
            isAnd = 1;
        }
        hasPastEvents += execute_command(commandToken, isAnd);
        commandToken = strtok(NULL, delimiters);
        commandEndPosition++;
    }

    // only store if pastevents was not present in the command
    if(hasPastEvents == 0 || hasPastEvents == -1) {
        saveInPastEvents(inputCopy2);
    }
}