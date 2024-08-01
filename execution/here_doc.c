#include "../Includes/minishell.h"

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

void	here_doc(t_cmd *cmd, char *eof)
{
	int	fds[2];

	if (pipe(fds) == -1)
		exit(456);
	cmd->in = fds[0];
	while (1)
	{
		char *line = readline("\033[37mhere_doc > \033[0m");
		if (!line || !ft_strcmp(line, "eof"))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fds[1]);
		ft_putstr_fd("\n", fds[1]);
	}
	close(fds[1]);
}

int main()
{
	t_cmd *cmd;

	cmd = malloc(sizeof(t_cmd));
	cmd->command = malloc(2 * sizeof(char *));
	char *command1 = ft_strdup("cat");

	cmd->command[0] = command1;
	cmd->command[1] = NULL;
	here_doc(cmd, "eof");
}
