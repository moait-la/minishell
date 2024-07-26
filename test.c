#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char *path, *oldpath;

    // Get and print PATH and OLDPATH before chdir
    path = getenv("PWD");
    oldpath = getenv("OLDPWD");
    printf("Before chdir:\n");
    printf("PATH: %s\n", path ? path : "Not set");
    printf("OLDPATH: %s\n\n", oldpath ? oldpath : "Not set");

    // Change directory to "."
    if (chdir(".") != 0) {
        perror("chdir failed");
        return 1;
    }

    // Get and print PATH and OLDPATH after chdir
    path = getenv("PWD");
    oldpath = getenv("OLDPWD");
    printf("After chdir:\n");
    printf("PATH: %s\n", path ? path : "Not set");
    printf("OLDPATH: %s\n", oldpath ? oldpath : "Not set");

    return 0;
}
