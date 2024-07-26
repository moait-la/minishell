#include "../Includes/minishell.h"

// Function to create a new node for the linked list
t_env *create_env_node(const char *key, const char *value) {
    t_env *node = (t_env *)malloc(sizeof(t_env));
    if (node) {
        node->key = strdup(key);
        node->value = strdup(value);
        node->next = NULL;
    }
    return node;
}

// Function to free memory allocated for the linked list
void free_env_list(t_env *head) {
    while (head) {
        t_env *temp = head;
        head = head->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

t_env *parse_envp(char **envp) {
    t_env *head = NULL;
    t_env *current = NULL;

    // Iterate through envp array until NULL terminator
    for (int i = 0; envp[i] != NULL; i++) {
        // Duplicate the environment variable to avoid modifying the original
        char *env_var = strdup(envp[i]);
        if (env_var == NULL) {
            // Handle memory allocation failure if needed
            free_env_list(head); // Free already allocated nodes
            return NULL;
        }

        char *equals_sign = strchr(env_var, '=');
        if (equals_sign) {
            *equals_sign = '\0'; // Split at '='
            char *key = env_var;
            char *value = equals_sign + 1; // Points to the character after '='

            // Skip the creation of a node if the key is OLDPWD
            if (strcmp(key, "OLDPWD") == 0) {
                free(env_var); // Free duplicated environment variable
                continue;
            }

            // Create a new node for the linked list
            t_env *node = create_env_node(key, value);
            if (node == NULL) {
                // Handle memory allocation failure if needed
                free(env_var); // Free duplicated environment variable
                free_env_list(head); // Free already allocated nodes
                return NULL;
            }

            // Append the node to the linked list
            if (head == NULL) {
                head = node;
                current = node;
            } else {
                current->next = node;
                current = node;
            }
        }

        free(env_var); // Free duplicated environment variable
    }

    return head;
}
