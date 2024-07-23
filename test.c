#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd); // Close the original fd

        // This will be written to output.txt
        printf("This is the child process\n");
        exit(EXIT_SUCCESS);
    } else { // Parent process
        // close(fd); // Close the original fd in the parent

        // This will still be written to the terminal
        printf("This is the parent process\n");

        // Wait for the child to finish
        wait(NULL);
    }
    return 0;
}
