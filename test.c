#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fds[2];

    if (pipe(fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Close the write end of the pipe
    // close(fds[1]);

    // Redirect the read end of the pipe to stdin
    dup2(fds[0], STDIN_FILENO);

    // Close the original read end of the pipe
    close(fds[0]);

    char *command[3] = {"wc", "-l", NULL};
    if (execve("/usr/bin/wc", command, NULL) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    return 0;
}
