#include "../Includes/minishell.h"
#include <fcntl.h>

int	ft_pipes_count(char *input)
{
	int nbr = 0;

	while (*input)
	{
		if (*input == '|')
			nbr++;
		input++;
	}
	return (nbr);
}

void	add_to_open_fds(t_open_fds	**open_fds, int fd)
{
	t_open_fds	*new;

	new = malloc(sizeof(t_open_fds));
	new->fd = fd;
	new->next = NULL;
	if (*open_fds == NULL)
	{
		*open_fds = new;
		return ;
	}
	t_open_fds	*tmp;
	tmp = *open_fds;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	tmp->next = new;
}

void	ft_redirect_in_out_oprtrs(t_cmd *cmd, char *input, int *offset, t_open_fds **open_fds)
{
	int	j;

	j = 0;

		while (input[*offset] && input[*offset] != '|')
		{
			if (input[*offset] == '<')
			{
                close(cmd->in); // same here
				cmd->in = open_file(cmd->redirections[j], 1);
				add_to_open_fds(open_fds, cmd->in);
				j++;
			}
			else if (input[*offset] == '>')
			{
                close(cmd->out); // this is really important as if we update the fd
                //we need first to close the old opned pipe fds
				cmd->out = open_file(cmd->redirections[j], 2);
				add_to_open_fds(open_fds, cmd->out);
				j++;
			}
			(*offset)++;
		}
		if (input[*offset] == '|')
		{
			(*offset)++;
		}
}

void	ft_execute_without_pipes(t_cmd	*cmd, char *input)
{
	char	*path;

	if (cmd->in != 0)
	{
		dup2(cmd->in, STDIN_FILENO);
		close(cmd->in);
	}
    if (cmd->out != 1)
	{
		dup2(cmd->out, STDOUT_FILENO);
		close(cmd->out);
	}
	path = ft_get_path(cmd->command[0]);
	if (execve(path, cmd->command, NULL) == -1)
	{
		perror("execve failed");
		exit(1);
	}
}

void	ft_child_process(t_cmd *cmd, char *path)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perror("400");
	if (pid == 0)
	{
        if (cmd->in != 0)
        {
            if (dup2(cmd->in, STDIN_FILENO) == -1)
                perror("dup issue");
            close(cmd->in);
        }
		if (cmd->out != 1)
		{
			if (dup2(cmd->out, STDOUT_FILENO) == -1)
                perror("dup issue");
			close(cmd->out);
		}
        if (execve(path, cmd->command, NULL) == -1)
            perror("execve failed!");
	}
	else
	{
		if (cmd->out != 1)
			close(cmd->out);
	}
}

int is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (0);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (1);
}

void	excute_builtin(t_cmd *cmd, t_env *env_lst)
{
	if (ft_strcmp(cmd->command[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->command[0], "cd") == 0)
		ft_cd(cmd, env_lst);
	else if (ft_strcmp(cmd->command[0], "env") == 0)
		ft_env(env_lst);
	else if (ft_strcmp(cmd->command[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->command[0], "export") == 0)
		ft_export(cmd->command);
	// else if (ft_strcmp(cmd->command[0], "unset") == 0)
	// 	ft_unset(cmd->command);
	// else if (ft_strcmp(cmd->command[0], "exit") == 0)
	// 	ft_exit(cmd->command);
	if (cmd->out != 1) // always close the write end of pipe after writing to it ola ghadi ib9a m3la9
			close(cmd->out);
}

void	ft_execute(t_cmd *cmd, char *input, char **envp)
{
	t_open_fds	*open_fds;
	int			nbr_pipes;
	int			i = 0;
	char		*path;

	open_fds = NULL;
    t_env *env_lst = parse_envp(envp);
	ft_init_in_out(cmd);
	nbr_pipes = ft_pipes_count(input);
	int	childp_count = 0;
	int input_offset = 0;
	if (nbr_pipes == 0)
	{
        if (!is_builtin(cmd->command[0]))
		{
			excute_builtin(cmd, env_lst);
		}
        else
        {
            ft_redirect_in_out_oprtrs(cmd, input, &input_offset, &open_fds);
            ft_execute_without_pipes(cmd, input);
        }
	}
	else if (nbr_pipes > 0)
	{
		while (i <= nbr_pipes)
		{
			ft_init_pipe(cmd, &open_fds);
			ft_redirect_in_out_oprtrs(cmd, input, &input_offset, &open_fds);
			if (!is_builtin(cmd->command[0]))
			{
				excute_builtin(cmd, env_lst);
			}
			else
			{
				path = ft_get_path(cmd->command[0]);
				ft_child_process(cmd, path);
				free(path);
				childp_count++;
			}
            i++;
            cmd = cmd->next;
		}
	}
	i = 0;
	while (i < childp_count)
	{
		wait(NULL);
		i++;
	}
	while (open_fds != NULL)
	{
		close(open_fds->fd);
		open_fds = open_fds->next;
	}
}

// int main(int ac, char **av, char **envp)
// {
//     t_cmd *head;
//     char **command;

//     char *input = "cd -";

//     // Command: cd -
//     command = (char **)malloc(sizeof(char *) * 3);
//     command[0] = ft_strdup("cd");
//     command[1] = ft_strdup("-");
//     command[2] = NULL;

//     // Allocate and initialize command
//     head = (t_cmd *)malloc(sizeof(t_cmd));
//     head->command = command;
//     head->next = NULL;

//     // No redirections for cd
//     head->redirections = NULL;

//     // Initialize in and out
//     head->in = 0;
//     head->out = 1;

//     // Execute the command
//     ft_execute(head, input, envp);

//     // Free allocated memory (implementation omitted for brevity)

//     return 0;
// }

