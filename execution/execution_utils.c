#include "../Includes/minishell.h"

void	ft_init_pipe(t_cmd *cmd, t_open_fds **open_fds)
{
	int	fds[2];

	if (cmd->next != NULL)
	{
		if (pipe(fds) == -1) // ERROR
			exit(42);
		cmd->out = fds[1];
		cmd->next->in = fds[0];
		ft_addto_openfds(open_fds, fds[1]);
		ft_addto_openfds(open_fds, fds[0]);
	}
}

void	ft_init_in_out(t_minishell *t_mini)
{
	while (t_mini->cmd)
	{
		t_mini->cmd->in = 0;
		t_mini->cmd->out = 1;
		t_mini->cmd = t_mini->cmd->next; 
	}
}

int	open_file(char *name, int nbr)
{
	int	fd;

	fd = 0;
	if (nbr == 1)
		fd = open(name, O_RDONLY, 0666);
	else if (nbr == 2)
		fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0666);
	else if (nbr == 3)
		fd = open(name, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (fd == -1 && nbr == 1)
	{
		ft_putstr_fd("Error: opening infile\n", 1);
		exit(1);
	}
	else if (fd == -1 && (nbr == 2 || nbr == 3))
	{
		ft_putstr_fd("Error: opening outfile\n", 1);
		exit(1);
	}
	return (fd);
}

char	*ft_get_path(char *cmd)
{
	char	*path;
	char	**all_paths;
	char	*cmd_path;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	else
	{
		path = getenv("PATH");
		all_paths = ft_split(path, ':');
		i = -1;
		while (all_paths[++i] != NULL)
		{
			path = ft_strjoin(all_paths[i], "/");
			cmd_path = ft_strjoin(path, cmd);
			free(path);
			if (access(cmd_path, X_OK) == 0)
				return (ft_free(all_paths), cmd_path);
			if (all_paths[i + 1] != NULL)
				free(cmd_path);
		}
	}
	return (perror("Command not found"), cmd_path);
}

char	*ft_getenv(char *key, t_env *env_lst)
{
	while (env_lst)
	{
		if (!ft_strcmp(key, env_lst->key))
			return (env_lst->value);
		env_lst = env_lst->next;
	}
	return (NULL);
}

void	ft_free(char **ptr)
{
	char	**temp;

	if (ptr == NULL)
		return ;
	temp = ptr;
	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}
	free(temp);
}

int	ft_pipes_count(char *input)
{
	int	count;

	count = 0;
	while (*input)
	{
		if (*input == '|')
			count++;
		input++;
	}
	return (count);
}
