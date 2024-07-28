#include "../Includes/minishell.h"

void	ft_sorted_envlst(t_cmd *cmd, t_env *env_lst)
{
    t_env   *current;
    t_env   *smallest;
    int     biggest_char;

    while (1)
    {
        smallest = NULL;
        biggest_char = 127;
		current = env_lst;
        while (current)
        {
            if (current->key[0] < biggest_char && current->has_printed != 0)
			{
				biggest_char = current->key[0];
				smallest = current;
			}
			current = current->next;
        }
		if (smallest == NULL)
			break ;
		smallest->has_printed = 0;
		printf("%s=%s\n", smallest->key, smallest->value);
    }
}

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

char	*ft_get_key(char *cmd, int *equal_sign)
{
	int		i;
	char	*key;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
		{
			*equal_sign = 0;
			key = ft_substr(cmd, 0, i);
			return (key);
		}
		i++;
	}
    return (ft_strdup(cmd));
}

int	ft_check_ifexists(char *key, t_env *env_lst)
{
	while (env_lst)
	{
		if (!ft_strcmp(key, env_lst->key))
			return (0); // exists in the list
		env_lst = env_lst->next;
	}
	return (1); // does not exists in the list
}

void	ft_update_export_lst(t_env *env_lst, char *key, char *value)
{
	while (env_lst)
	{
		if (!ft_strcmp(env_lst->key, key))
		{
			free(env_lst->value);
			env_lst->value = ft_strdup(value);
			break ;
		}
		env_lst = env_lst->next;
	}
}

void	ft_export(t_cmd *cmd, t_env *env_lst)
{
	if (!cmd->command[1]) // export
	{
		ft_sorted_envlst(cmd, env_lst);
		return ;
	}
	if (is_valid_identifier(cmd->command) == 1)
	{
		ft_error("export: ", "not a valid identifier\n");
		exit(1);
	}
	int i = 1;
	while (cmd->command[i])
	{
		int	equal_sign = 1;
		char *key = ft_get_key(cmd->command[i], &equal_sign);
		char *value = ft_get_value(cmd->command[i]);
		int exists = ft_check_ifexists(key, env_lst);
		if (exists == 1) // he does not exits he will be added
		{
			if (equal_sign == 0) // equal sign exits so he will be added to env lst
				ft_add_env(&env_lst, 0, key, value);
			else // will be added to export lst
				ft_add_env(&env_lst, 1, key, value);
		}
		else if (!exists && !equal_sign)  // if he exists in the env lst and needs update check if = cause we dont need to update export AA for example need to as stay as it was
			ft_update_export_lst(env_lst, key, value);
		i++;
	}
}

int main()
{
    // Initialize the environment list
    t_env *env_lst = NULL;
    
    // Create a sample environment
    ft_add_env(&env_lst, 0, "PATH", "/usr/bin:/bin");
    ft_add_env(&env_lst, 0, "HOME", "/home/user");

    // Create a command structure
    t_cmd cmd;
    char *command[] = {"export", "AA", "!A=med", "BB=" ,NULL};
    cmd.command = command;

    // Call the export function
    ft_export(&cmd, env_lst);

    // Print the updated environment list
    printf("Updated environment list:\n");
    t_env *current = env_lst;
    while (current)
    {
        printf("%s=%s %d\n", current->key, current->value, current->exported_with_value);
        current = current->next;
    }

    // Free the environment list
    while (env_lst)
    {
        t_env *temp = env_lst;
        env_lst = env_lst->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }

    return 0;
}

