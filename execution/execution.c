#include "../Includes/minishell.h"
#include <fcntl.h>

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

void	ft_find_what_redir(t_cmd *cmd, int *i, int *j)
{
	char	*input = "cat < Makefile < main < file | wc -l";
	int		fd;

	while (input[*i] && input[*i] != '|')
	{
		if (input[*i] == '<')
		{
			cmd->in = open_file(cmd->redirections[*j], 1);
            (*j)++;
			(*i)++;
		}
		else if (input[*i] == '>')
		{
			cmd->out = open_file(cmd->redirections[*j], 2);
            (*j)++;
			(*i)++;
		}
        else
		    (*i)++;
	}
}

void	ft_excute_command(t_cmd	*cmd, int pipe_bool)
{
	char	*path;
	int		fds[2];
	pid_t	pid_1;

	dup2(cmd->in, STDIN_FILENO);
	close(cmd->in);
    // dup2(cmd->out, STDOUT_FILENO);
	// if (pipe_bool == 1)
	// 	close(cmd->out);
	if (pipe(fds) == -1)
	{
		printf("pipe failed\n");
		exit(1);
	}
	if (pipe_bool == 1)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	path = ft_get_path(cmd->command[0]);
	if (!path)
	{
		printf("path not found\n");
		exit(1);
	}
	pid_1 = fork();
	if (pid_1 < 0)
	{
		printf("failed forking!");
		exit(1);
	}
	else if (pid_1 == 0)
	{
		execve(path, cmd->command, NULL);
		printf("execve failed!\n");
		exit(1);
	}
	else // main process
	{
		wait(NULL);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
}

void	ft_execute(t_cmd *cmd)
{
	char	*input = "cat < Makefile < main < file | wc -l";

	int i = 0;
    int j = 0;
	int	fd;
	cmd->in = 0;
	cmd->out = 1;
	cmd->next->in = 0;
	cmd->next->out = 1;
	while (input[i])
	{
		ft_find_what_redir(cmd, &i, &j); // i is init to where it found | or end of str
		if (input[i] == '|') // means it stoped cause it found the pipe
		{
			// excute prev command
			ft_excute_command(cmd, 1);
			cmd = cmd->next; // go to next command in the list
			i++;
            j++;
		}
		else	// there is no pipe in command
		{
			ft_excute_command(cmd, 0);
			break ;
		}
	}
}

int main() {
    t_cmd *head;
    char **command;
    char **redirections;

    // !! command used is >>>>> cat < Makefile < main < file | wc -l

    // ---- command ----
    command = (char **)malloc(sizeof(char *) * (1 + 1));
    command[0] = ft_strdup("cat");
    command[1] = NULL;

    char **command_2 = (char **)malloc(sizeof(char *) * (2 + 1));
    command_2[0] = ft_strdup("wc");
    command_2[1] = ft_strdup("-l");
    command_2[2] = NULL;

    // Allocate and initialize head
    head = (t_cmd *)malloc(sizeof(t_cmd));
    head->command = command;
    head->redirections = NULL;
    head->next = NULL;

    // Allocate and initialize head->next
    head->next = (t_cmd *)malloc(sizeof(t_cmd));
    head->next->command = command_2;
    head->next->redirections = NULL;
    head->next->next = NULL;

    // ----- redirections -----
    char *word_1 = "Makefile";
    char *word_2 = "main";
    char *word_3 = "file";

    redirections = (char **)malloc(sizeof(char *) * (3 + 1));
    redirections[0] = ft_strdup(word_1);
    redirections[1] = ft_strdup(word_2);
    redirections[2] = ft_strdup(word_3);
    redirections[3] = NULL;

    head->redirections = redirections;

    ft_execute(head);

    return 0;
}

/*


--- tested tests ---
with in < redirection >>SUCCESS<< 
        wc -l < Makefile < main < file

with out > redirections >>SUCCESS<<
        wc -l < Makefile < main < file > outfile > outfile_2

with one pipe |
        cat < Makefile < main < file | wc -l
*/