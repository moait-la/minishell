#include "../Includes/minishell.h"

// the **command in t_cmd struct should have all the command with the flags 
// alos it need to be expaned and should remove all spaces in parsing

void    ft_echo(t_cmd *cmd)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	if (!cmd->command[i] || !ft_strcmp(cmd->command[i], "-n"))
	{
		nl_flag = 0;
		if (!ft_strcmp(cmd->command[i], "-n"))
			i++;
	}
	while (cmd->command[i])
	{
		ft_putstr_fd(cmd->command[i], cmd->out);
		if (cmd->command[i + 1])
			ft_putchar_fd(' ', cmd->out);
		i++;
	}
	if (nl_flag == 1)
		ft_putchar_fd('\n', cmd->out);
}
