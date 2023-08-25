#include "headers.h"

typedef struct {
    pid_t pid;
    char commandName[1024];
} BackgroundProcess;

BackgroundProcess bgProcesses[100];
int bgIndex = 0;

void checkBackgroundProcesses() {
    for (int i = 0; i < bgIndex; i++) {
        int status;
        pid_t result = waitpid(bgProcesses[i].pid, &status, WNOHANG);

        if (result != 0) {
            if (WIFEXITED(status)) {
                printf("%s exitted normally (%d)\n", bgProcesses[i].commandName, bgProcesses[i].pid);
            } else if (WIFSIGNALED(status)) {
                printf("%s was terminated by a signal (%d)\n", bgProcesses[i].commandName, bgProcesses[i].pid);

            }

            // Shift the array to remove the completed process info
            for (int j = i; j < bgIndex - 1; j++) {
                bgProcesses[j] = bgProcesses[j + 1];
            }
            bgIndex--;
            i--; // Adjust index after shifting elements
        }
    }
}

void executeSystemCommand(char *command[], int isBackground) {
    pid_t pid = fork();
    time_t start_time, end_time;
    start_time = time(NULL);
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        if (isBackground) {
            int devnull = open("/dev/null", O_WRONLY);
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            close(devnull);
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
            bgProcesses[bgIndex].commandName[1024 - 1] = '\0';  // Ensure null termination
            bgIndex++;
        } else {
            waitpid(pid, NULL, 0);
            end_time = time(NULL);
            double difference = difftime(end_time, start_time);
            if(difference > 2.00) {
                long_process_time = (int) difference;
                strcpy(long_process, command[0]);
            } else {
                long_process_time = difference;
            }
        }
    }
}