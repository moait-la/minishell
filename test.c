#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Path to the `wc` command
    const char *pathname = "/bin/cat";

    // Arguments for the `wc -l file1 file2` command
    char *argv[] = {
        "cat",   // argv[0] - the command to execute
        "fake",   // argv[2] - first file
        "test.c",   // argv[3] - second file
        NULL       // Null-terminated array
    };

    // Environment variables (NULL to inherit the current environment)
    char *envp[] = { NULL };
    // Execute the `wc -l file1 file2` command
    if (execve(pathname, argv, envp) == -1) {
        // perror("execve");  // Print error if execve fails
        exit(1);
    }
    // This point is never reached if execve is successful
    return 0;
}
