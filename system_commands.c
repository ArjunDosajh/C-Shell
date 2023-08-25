#include "headers.h"

typedef struct {
    pid_t pid;
    char commandName[1024];
} BackgroundProcess;

BackgroundProcess bgProcesses[100];
int bgIndex = 0;

const char *tempFileName = "/tmp/bg_output_all";

void displayOutputAndCleanup() {
    char buffer[1024];
    FILE *f = fopen(tempFileName, "r");

    if (f) {
        while (fgets(buffer, sizeof(buffer), f)) {
            printf("%s", buffer);
        }
        fclose(f);
    }
    remove(tempFileName);
}

void checkBackgroundProcesses() {
    int anyCompleted = 0;

    for (int i = 0; i < bgIndex; i++) {
        int status;
        pid_t result = waitpid(bgProcesses[i].pid, &status, WNOHANG);

        if (result != 0) {
            displayOutputAndCleanup();

            if (WIFEXITED(status)) {
                printf("%s exited normally (%d)\n", bgProcesses[i].commandName, bgProcesses[i].pid);
            } else if (WIFSIGNALED(status)) {
                printf("%s exited due to signal (%d)\n", bgProcesses[i].commandName, bgProcesses[i].pid);
            }

            for (int j = i; j < bgIndex - 1; j++) {
                bgProcesses[j] = bgProcesses[j + 1];
            }
            bgIndex--;
            i--;
        }
    }
}

void executeSystemCommand(char *command[], int isBackground) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        if (isBackground) {
            int file = open(tempFileName, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
            dup2(file, STDOUT_FILENO);
            dup2(file, STDERR_FILENO);
            close(file);
        }

        if (execvp(command[0], command) == -1) {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    } else {
        if (isBackground) {
            printf("%d\n", pid);
            bgProcesses[bgIndex].pid = pid;
            strncpy(bgProcesses[bgIndex].commandName, command[0], 1024 - 1);
            bgProcesses[bgIndex].commandName[1024 - 1] = '\0';
            bgIndex++;
        } else {
            time_t start_time, end_time;
            start_time = time(NULL);
            waitpid(pid, NULL, 0);
            end_time = time(NULL);
            if(difftime(end_time, start_time) > 2) {
                long_process_time = (int) difftime(end_time, start_time);
                strcpy(long_process, command[0]);
            } else {
                long_process_time = 0;
            }
        }
    }
}