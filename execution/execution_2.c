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
/*
	cat < file < main < makefile | grep "zeb" | wc -c
	cat < file < main < makefile | grep "zeb"
	cat < file < main < makefile
	ls
*/


void	ft_redirect_in_out_oprtrs(t_cmd *cmd, char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && input[i] != '|')
		{
			if (input[i] == '<')
			{
				cmd->in = open_file(cmd->redirections[j], 1);
				j++;
			}
			else if (input[i] == '>')
			{
				cmd->out = open_file(cmd->redirections[j], 2);
				j++;
			}
			i++;
		}
		if (input[i] == '|')
		{
			cmd = cmd->next;
			j = 0;
			i++;
		}
	}	
}

void	ft_child_process(t_cmd *cmd, char *path)
{
	int		fds[2];
	pid_t	pid;

	if (pipe(fds) == -1)
		exit(3);
	pid = fork();
	if (pid < 0)
		exit (4);
	else if (pid == 0)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execve(path, cmd->command, NULL);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
}

void	ft_execute_without_pipes(t_cmd	*cmd, char *input)
{
	char	*path;

	printf("%d\n", cmd->in);
	if (cmd->in != 0)
	{
		dup2(cmd->in, STDIN_FILENO);
		close(cmd->in);
	}
	path = ft_get_path(cmd->command[0]);
	if (execve(path, cmd->command, NULL) == -1)
	{
		perror("execve failed");
		exit(1);
	}
}

void	ft_execute(t_cmd *head, char *input)
{
	int	nbr_pipes;

	// ----- !! this need to be init for each node
	head->in = 0;
	head->out = 1;
	// -----
	nbr_pipes = ft_pipes_count(input);
	ft_redirect_in_out_oprtrs(head, input);
	int	i = 0;
	int	j = 0;
	int	childp_count = 0;
	if (nbr_pipes == 0)
	{
		ft_execute_without_pipes(head, input);
	}
	else if (nbr_pipes > 0)
	{
		while (i < nbr_pipes)
		{
			char *path = ft_get_path(head->command[j]);
			ft_child_process(head, path);
			free(path);
			i++;
			childp_count++;
		}
	}
}

int main() {
    t_cmd *head;
    char **command;
    char **redirections;

    // Command used is: grep x < main < file < Makefile > outfile_1 > outfile_2
    char *input = "grep x < main < file < Makefile > outfile_1 > outfile_2";

    // ---- command ----
    command = (char **)malloc(sizeof(char *) * (2 + 1));
    if (!command) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command[0] = ft_strdup("grep");
    command[1] = ft_strdup("x");
    command[2] = NULL;

    // Allocate and initialize head
    head = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->command = command;
    head->redirections = NULL;
    head->next = NULL;

    // ----- redirections -----
    char *input_file_1 = "Makefile";
    char *input_file_2 = "file";
    char *input_file_3 = "main";
    char *output_file_1 = "outfile_1";
    char *output_file_2 = "outfile_2";

    redirections = (char **)malloc(sizeof(char *) * (5 + 1));
    if (!redirections) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    redirections[0] = ft_strdup(input_file_1);
    redirections[1] = ft_strdup(input_file_2);
    redirections[2] = ft_strdup(input_file_3);
    redirections[3] = ft_strdup(output_file_1);
    redirections[4] = ft_strdup(output_file_2);
    redirections[5] = NULL;

    head->redirections = redirections;

    // Execute the command with redirections
    ft_execute(head, input);

    // Free allocated memory (if not done in ft_execute)
    for (int i = 0; redirections[i] != NULL; i++) {
        free(redirections[i]);
    }
    free(redirections);

    for (int i = 0; command[i] != NULL; i++) {
        free(command[i]);
    }
    free(command);
    free(head);

    return 0;
}

// int main() {
//     t_cmd *head;
//     char **command;
//     char **redirections;

//     // !! command used is >>>>> cat < Makefile < main < file | wc -l
// 	char *input = "cat < Makefile < main < file | wc -l";
//     // ---- command ----
//     command = (char **)malloc(sizeof(char *) * (1 + 1));
//     command[0] = ft_strdup("cat");
//     command[1] = NULL;

//     char **command_2 = (char **)malloc(sizeof(char *) * (2 + 1));
//     command_2[0] = ft_strdup("wc");
//     command_2[1] = ft_strdup("-l");
//     command_2[2] = NULL;

//     // Allocate and initialize head
//     head = (t_cmd *)malloc(sizeof(t_cmd));
//     head->command = command;
//     head->redirections = NULL;
//     head->next = NULL;

//     // Allocate and initialize head->next
//     head->next = (t_cmd *)malloc(sizeof(t_cmd));
//     head->next->command = command_2;
//     head->next->redirections = NULL;
//     head->next->next = NULL;

//     // ----- redirections -----
//     char *word_1 = "Makefile";
//     char *word_2 = "main";
//     char *word_3 = "file";

//     redirections = (char **)malloc(sizeof(char *) * (3 + 1));
//     redirections[0] = ft_strdup(word_1);
//     redirections[1] = ft_strdup(word_2);
//     redirections[2] = ft_strdup(word_3);
//     redirections[3] = NULL;

//     head->redirections = redirections;

//     ft_execute(head, input);

//     return 0;
// }


