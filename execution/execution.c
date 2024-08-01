#include "../Includes/minishell.h"

int	ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (0);
	else if (!ft_strcmp(cmd, "cd"))
		return (0);
	else if (!ft_strcmp(cmd, "pwd"))
		return (0);
	else if (!ft_strcmp(cmd, "export"))
		return (0);
	else if (!ft_strcmp(cmd, "unset"))
		return (0);
	else if (!ft_strcmp(cmd, "env"))
		return (0);
	else if (!ft_strcmp(cmd, "exit"))
		return (0);
	return (1);
}

void	ft_execute_builtin(t_minishell *t_mini, t_env env_lst)
{
	if (!ft_strcmp(t_mini->cmd->command[0], "echo"))
		ft_echo(t_mini);
	else if (!ft_strcmp(t_mini->cmd->command[0], "cd"))
		ft_cd(t_mini, env_lst);
	else if (!ft_strcmp(t_mini->cmd->command[0], "env"))
		ft_env(t_mini, env_lst);
	else if (!ft_strcmp(t_mini->cmd->command[0], "pwd"))
		ft_pwd(t_mini);
	else if (!ft_strcmp(t_mini->cmd->command[0], "export"))
		ft_export(t_mini);
	else if (!ft_strcmp(t_mini->cmd->command[0], "unset"))
		ft_unset(t_mini);
	// else if (!ft_strcmp(t_mini->cmd->command[0], "exit"))
	// 	ft_exit(t_mini);
	if (t_mini->cmd->out != 1) // always close the write end of pipe after writing to it ola ghadi ib9a m3la9
		close(t_mini->cmd->out);
}

void	ft_redirect_opertrs(t_minishell *t_mini, t_open_fds **open_fds, int *offset)
{
	int	i;

	i = -1; // index of redirection file name
	while (t_mini->str[*offset] && t_mini->str[*offset] != '|')
	{
		if (t_mini->str[*offset] == '<')
		{
			if (t_mini->cmd->in != 0)
				close(t_mini->cmd->in);
			t_mini->cmd->in = open_file(t_mini->cmd->redirections[i], 1);
			ft_add_to_open_fds(open_fds, t_mini->cmd->in);
			i++;
		}
		else if (t_mini->str[*offset] == '>')
		{
			if (t_mini->cmd->out != 1)
				close(t_mini->cmd->out);
			t_mini->cmd->out = open_file(t_mini->cmd->redirections[i], 2);
			ft_add_to_open_fds(open_fds, t_mini->cmd->out);
			i++;
		}
		else if (!ft_strncmp(&t_mini->str[*offset], ">>", 2))
		{
			if (t_mini->cmd->out != 1)
				close(t_mini->cmd->out);
			t_mini->cmd->out = open_file(t_mini->cmd->redirections[i], 3);
			ft_add_to_open_fds(open_fds, t_mini->cmd->out);
			(*offset)++;
			i++;
		}
		else if (!ft_strncmp(&t_mini->str[*offset], "<<", 2))
		{
			(*offset)++;
			i++;
		}
		(*offset)++;
	}
	if (t_mini->str[*offset] == '|')
		(*offset)++;
}

void	ft_child_process(t_cmd *cmd, char *path)
{
	pid_t	pid;

	pid = fork;
	if (pid < 0)
		exit(42); // ERROR
	else if (pid == 0)
	{
		if (cmd->in != 0)
		{
			dup2(cmd->in, STDIN_FILENO); // ERROR
			closecmd->in);
		}
		if (cmd->out != 1)
		{
			dup2(cmd->out, STDOUT_FILENO); // ERROR
			close(cmd->out);
		}
		if (execve(path, cmd->command, NULL) == -1)
			exit(42); // ERROR
	}
	else
	{
		if (cmd->out != 1)
			close(cmd->out)
	}
}

void	ft_execute_single_cmd(t_minishell *t_mini, t_env *env_lst, t_open_fds **open_fds)
{
	char	*path;
	int		offset;

	offset = 0;
	if (!ft_is_builtin(t_mini->t_cmd->command[0]))
	{
		ft_execute_builtin(t_mini, env_lst);
		return ;
	}
	ft_redirect_opertrs(t_mini, open_fds, &offset);
	path = ft_get_path(t_mini->cmd->command[0]);
	ft_child_process(t_mini->cmd, path);
	wait(NULL);
}

void	ft_execute_multi_cmd(t_minishell *t_mini, t_env *env_lst, t_open_fds **open_fds, int pipe_count)
{
	int	i;
	int	offset;

	i = -1;
	offset = 0;
	while (++i <= pipe_count)
	{
		ft_init_pipe(t_mini->cmd, &open_fds);
		ft_redirect_oprtrs(t_mini, open_fds, &offset);
		if (!ft_is_builtin(t_mini->cmd->command[0]))
			ft_execute_builtin(t_mini->cmd, env_lst);
		else
		{
			path = ft_get_path(t_mini->cmd->command[0]);
			ft_child_process(t_mini->cmd, path);
			free(path);
			childp_count++;
		}
		t_mini->cmd = t_mini->cmd->next;
	}
	i = -1;
	while (++i < childp_count)
		wait(NULL);
}

void	ft_execute(t_minishell	*t_mini, char **envp)
{
	int			pipe_count;
	int			nbr_childp;
	t_env		*env_lst;
	t_open_fds	*open_fds;

	open_fds = NULL;
	env_lst	= ft_parse_envp(envp);
	pipe_count = ft_pipes_count(t_mini->str); // provide input
	ft_init_in_out(t_mini);
	if (!pipe_count)
		ft_execute_single_cmd(t_mini, env_lst, &open_fds);
	else
		ft_execute_multi_cmd(t_mini, env_lst, &open_fds, pipe_count);
	while (open_fds)
	{
		close(open_fds->fd); // i need to check first if fd is linked with terminal
		open_fds = open_fds->next;
	}
}
