#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *args[] = { "cat", "<" , "main.c", NULL };
    char *envp[] = { NULL };

    // Execute the command using execve
    if (execve("/bin/cat", args, envp) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // This line will not be reached if execve is successful
    return 0;
}
