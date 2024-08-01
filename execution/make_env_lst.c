#include "../Includes/minishell.h"

char	*ft_get_value(char *cmd)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = ft_strlen(cmd);
	while (cmd[i])
	{
		if (cmd[i] == '=')
		{
			value = ft_substr(cmd, i + 1, len);
            return (value);
		}
		i++;
	}
	return (NULL);
}

char	*ft_get_key(char *cmd)
{
	int		i;
	char	*key;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
		{
			key = ft_substr(cmd, 0, i);
			return (key);
		}
		i++;
	}
    return (ft_strdup(cmd));
}

t_env	*ft_create_node(char *env)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	node->key = ft_get_key(env);
	node->value = ft_get_value(env);
	node->has_printed = 1;
	node->exported_with_value = 0;
	node->next = NULL;
	return (node);
}

t_env	*ft_parse_env(char **envp)
{
	t_env	*env_lst;
	t_env	*temp;
	int 	i;

	i = 0;
	env_lst = ft_create_node(envp[0]);
	temp = env_lst;
	while (envp[++i])
	{
		temp->next = ft_create_node(envp[i]);
		temp = temp->next;
		temp->next = NULL;
	}
	return (env_lst);
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_env *env_list = parse_env(envp);
    t_env *current = env_list;

    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }

    // Don't forget to free the allocated memory
    current = env_list;
    while (current != NULL)
    {
        t_env *next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }

    return 0;
}
