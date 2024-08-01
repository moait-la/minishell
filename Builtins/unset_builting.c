#include "../Includes/minishell.h"

void	ft_free(void *adrs1, void *adrs2, void *adrs3, int size)
{
	int	i;

	i = -1;
	void *adress[3] = {address1, address2, address3}
	while (++i < size)
		free(address[i]);
}

void	ft_unset_from_lst(char *key, t_env **env_lst)
{
	t_env	*temp;
	t_env	*prev;

	temp = *env_lst;
	prev = *env_lst;
	while (temp)
	{
		if (!ft_strcmp(key, temp->key))
		{
			if (!prev)
				*env_lst = temp->next;
			else
				prev->next = temp->next;
			ft_free(temp->value, temp-key, temp, 3);
			return ;
		}
		temp = temp->next;
		prev = temp;
	}
}

void	ft_unset_env(t_cmd *cmd, t_env **env_lst)
{
	t_env	*temp;
	char	*key;
	int		i;

	i = 1;
	while (cmd->command[i])
	{
		temp = *env_lst;
		key = ft_get_key(cmd->command[i]);
		while (temp)
		{
			if (!ft_check_ifexists(key, *env_lst) == 1) // he does exists in env lst
				ft_unset_from_lst(key, env_lst);
			temp = temp->next;
		}
		i++;
	}
}

void	ft_unset(t_cmd *cmd, t_env *env_lst)
{
	if (!cmd->command[1]) // unset
		return ;
	if (is_valid_identifier(cmd->command) == 1)
	{
		ft_error("unset: ", "not a valid identifier\n");
		exit(1);
	}
	if (ft_check_ifexists(cmd->command, env_lst) == 1)
	{
		ft_error("unset: ", "no such variable\n");
		exit(1);
	}
	ft_unset_env(cmd, &env_lst);
}
