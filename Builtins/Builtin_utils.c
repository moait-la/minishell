#include "../Includes/minishell.h"

void	ft_add_env(t_env **env_lst, int exported, char *key, char *value);
int     ft_equal_sign(char *cmd);

int	is_valid_identifier(char **command)
{
	int i;
	int	j;

	i = 0;
	while (command[i])
	{
		if (ft_isdigit(command[i][0]) || (!ft_isalpha(command[i][0]) && command[i][0] != '_'))
			return (1);
		j = 0;
		while (command[i][j])
		{
			if (!ft_isalnum(command[i][j]) && command[i][j]!= '_')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	ft_add_env(t_env **env_lst, int exported, char *key, char *value)
{
    t_env *new_node;
    t_env *temp;

    new_node = malloc(sizeof(t_env));
    new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    new_node->has_printed = 1;
    new_node->exported_with_value = exported;
    new_node->next = NULL;

    if (*env_lst == NULL)
    {
        *env_lst = new_node;
    }
    else
    {
        temp = *env_lst;
        while (temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
}

int	ft_equal_sign(char *cmd)
{
	while (*cmd)
	{
		if (*cmd == '=')
			return (0);
		cmd++;
	}
	return (1);
}
