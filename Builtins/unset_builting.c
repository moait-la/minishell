#include "../Includes/minishell.h"

void	ft_unset_env(char *key, t_env **env_lst)
{}

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
	ft_unset_env(cmd->command, &env_lst);
}
