#include "../Includes/minishell.h"
int main() {
    t_cmd *head;
    t_cmd *cmd2;
    char **command;
    char **command2;

    // Command used is: cat < file1.txt | grep "data" > result.txt | tee log.txt
    char *input = "cat < file1.txt | grep \"data\" > result.txt | tee log.txt";

    // ---- command 1 (cat) ----
    command = (char **)malloc(sizeof(char *) * (1 + 1));
    if (!command) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command[0] = ft_strdup("cat");
    command[1] = NULL;

    // ---- command 2 (grep "data") ----
    command2 = (char **)malloc(sizeof(char *) * (2 + 1));
    if (!command2) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command2[0] = ft_strdup("grep");
    command2[1] = ft_strdup("data");
    command2[2] = NULL;

    // ---- command 3 (tee log.txt) ----
    char **command3 = (char **)malloc(sizeof(char *) * (2 + 1));
    if (!command3) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command3[0] = ft_strdup("tee");
    command3[1] = ft_strdup("log.txt");
    command3[2] = NULL;

    // Allocate and initialize head
    head = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->command = command;
    head->redirections = (char **)malloc(sizeof(char *) * (1 + 1));
    if (!head->redirections) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->redirections[0] = ft_strdup("file1.txt");
    head->redirections[1] = NULL;
    head->next = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head->next) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->command = command2;
    head->next->redirections = (char **)malloc(sizeof(char *) * (1 + 1));
    if (!head->next->redirections) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->redirections[0] = ft_strdup("result.txt");
    head->next->redirections[1] = NULL;
    head->next->next = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head->next->next) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->next->command = command3;
    head->next->next->redirections = NULL;
    head->next->next->next = NULL;

    // Initialize default redirections
    head->in = 0;
    head->out = 1;
    head->next->in = 0;
    head->next->out = 1;
    head->next->next->in = 0;
    head->next->next->out = 1;

    // Execute the command with redirections
    ft_execute(head, input);

    // Free allocated memory (if not done in ft_execute)
    free(head->redirections[0]);
    free(head->redirections);
    free(head->next->redirections[0]);
    free(head->next->redirections);
    for (int i = 0; command[i] != NULL; i++) {
        free(command[i]);
    }
    free(command);
    for (int i = 0; command2[i] != NULL; i++) {
        free(command2[i]);
    }
    free(command2);
    for (int i = 0; command3[i] != NULL; i++) {
        free(command3[i]);
    }
    free(command3);
    free(head->next->next);
    free(head->next);
    free(head);

    return 0;
}
