#include "../Includes/minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("Error: getcwd()\n", 1);
		exit(1);
	}
	ft_putstr_fd(pwd, cmd->out);
	ft_putchar_fd('\n', cmd->out);
}
