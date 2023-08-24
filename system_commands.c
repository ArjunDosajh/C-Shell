#include "headers.h"

pid_t backgroundProcesses[100];

void checkBackgroundProcesses() {
    for (int i = 0; i < bgIndex; i++) {
        int status;
        pid_t result = waitpid(backgroundProcesses[i], &status, WNOHANG);

        if (result != 0) {
            if (WIFEXITED(status)) {
                printf("Background process with PID %d completed\n", backgroundProcesses[i]);
            } else if (WIFSIGNALED(status)) {
                printf("Background process with PID %d was terminated by a signal\n", backgroundProcesses[i]);
            }

            // Shift the array to remove the completed PID
            for (int j = i; j < bgIndex - 1; j++) {
                backgroundProcesses[j] = backgroundProcesses[j + 1];
            }
            bgIndex--;
            i--; // Adjust index after shifting elements
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
            printf("Process started in background with PID: %d\n", pid);
            backgroundProcesses[bgIndex++] = pid;
        } else {
            waitpid(pid, NULL, 0);
        }
    }
}