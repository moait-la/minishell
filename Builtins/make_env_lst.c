#include "../Includes/minishell.h"

// Function to create a new node for the linked list
t_env *create_env_node(const char *key, const char *value) {
    t_env *node = (t_env *)malloc(sizeof(t_env));
    if (node)
    {
        node->key = strdup(key);
        node->value = strdup(value);
        node->has_printed = 1;  // Initialize has_printed to 1
        node->exported_with_value = 0;
        node->next = NULL;
    }
    return node;
}

// The rest of the file remains the same

t_env *parse_envp(char **envp) {
    t_env *head = NULL;
    t_env *current = NULL;

    for (int i = 0; envp[i] != NULL; i++) {
        char *env_var = strdup(envp[i]);
        if (env_var == NULL) {
            free_env_list(head);
            return NULL;
        }

        char *equals_sign = strchr(env_var, '=');
        if (equals_sign) {
            *equals_sign = '\0';
            char *key = env_var;
            char *value = equals_sign + 1;

            if (strcmp(key, "OLDPWD") == 0) {
                free(env_var);
                continue;
            }

            t_env *node = create_env_node(key, value);
            if (node == NULL) {
                free(env_var);
                free_env_list(head);
                return NULL;
            }

            if (head == NULL) {
                head = node;
                current = node;
            } else {
                current->next = node;
                current = node;
            }
        }

        free(env_var);
    }

    return head;
}
