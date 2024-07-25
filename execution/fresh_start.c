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

void	ft_execute(t_cmd *cmd, char *input)
{
	t_open_fds	*open_fds = NULL;

	int	nbr_pipes;
	// ----- !! this need to be init for each node stdin and out
	ft_init_in_out(cmd);
	nbr_pipes = ft_pipes_count(input);
	int	i = 0;
	int	childp_count = 0;
	int input_offset = 0;
	if (nbr_pipes == 0)
	{
		ft_redirect_in_out_oprtrs(cmd, input, &input_offset, &open_fds);
		ft_execute_without_pipes(cmd, input);
	}
	else if (nbr_pipes > 0)
	{
		while (i <= nbr_pipes)
		{
			ft_init_pipe(cmd, &open_fds);
			ft_redirect_in_out_oprtrs(cmd, input, &input_offset, &open_fds);
            
            char *path = ft_get_path(cmd->command[0]);
            ft_child_process(cmd, path);
            free(path);
            i++;
            childp_count++;
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

int main() {
    t_cmd *head;
    t_cmd *cmd2;
    t_cmd *cmd3;
    char **command;
    char **command2;
    char **command3;
    char **redirections;

    // Command used is: cat < file < main < makefile | grep "x" | wc -c
    char *input = "cat < file < main < makefile | grep \"x\" | wc -c";

    // ---- command 1 (cat) ----
    command = (char **)malloc(sizeof(char *) * (1 + 1));
    if (!command) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command[0] = ft_strdup("cat");
    command[1] = NULL;

    // ---- command 2 (grep "x") ----
    command2 = (char **)malloc(sizeof(char *) * (2 + 1));
    if (!command2) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command2[0] = ft_strdup("grep");
    command2[1] = ft_strdup("x");
    command2[2] = NULL;

    // ---- command 3 (wc -c) ----
    command3 = (char **)malloc(sizeof(char *) * (2 + 1));
    if (!command3){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command3[0] = ft_strdup("wc");
    command3[1] = ft_strdup("-l");
    command3[2] = NULL;

    // Allocate and initialize head
    head = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->command = command;
    head->redirections = NULL;
    head->next = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head->next) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->command = command2;
    head->next->redirections = NULL;
    head->next->next = (t_cmd *)malloc(sizeof(t_cmd));
    if (!head->next->next) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->next->command = command3;
    head->next->next->redirections = NULL;
    head->next->next->next = NULL;

    // ----- redirections -----
    char *input_file_1 = "file";
    char *input_file_2 = "main";
    char *input_file_3 = "makefile";
    char *output_file_1 = "outfile_1";
    char *output_file_2 = "outfile_2";

    // Redirections for head (cat)
    head->redirections = (char **)malloc(sizeof(char *) * (4 + 1));
    if (!head->redirections) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->redirections[0] = ft_strdup(input_file_1);
    head->redirections[1] = ft_strdup(input_file_2);
    head->redirections[2] = ft_strdup(input_file_3);
    head->redirections[3] = NULL; // Ensure NULL termination

    head->next->redirections = (char **)malloc(sizeof(char *) * (1 + 1));
    if (!head->next->redirections) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->redirections[0] = NULL; // No input files for grep
    head->next->redirections[1] = NULL; // Ensure NULL termination

    // Redirections for cmd3 (wc -c)
    head->next->next->redirections = (char **)malloc(sizeof(char *) * (2 + 1));
    if (!head->next->next->redirections) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next->next->redirections[0] = ft_strdup(output_file_1);
    head->next->next->redirections[1] = ft_strdup(output_file_2);
    head->next->next->redirections[2] = NULL; // Ensure NULL termination

    head->next->in = 0;
    head->next->out = 1;
    head->next->next->in = 0;
    head->next->next->out = 1;

    // Execute the command with redirections
    ft_execute(head, input);

    // Free allocated memory (if not done in ft_execute)
    for (int i = 0; head->redirections[i] != NULL; i++) {
        free(head->redirections[i]);
    }
    free(head->redirections);

    for (int i = 0; head->next->redirections[i] != NULL; i++) {
        free(head->next->redirections[i]);
    }
    free(head->next->redirections);

    for (int i = 0; head->next->next->redirections[i] != NULL; i++) {
        free(head->next->next->redirections[i]);
    }
    free(head->next->next->redirections);

    for (int i = 0; command[i] != NULL; i++) {
        free(command[i]);
    }
    free(command);

    for (int i = 0; command2[i] != NULL; i++) {
        free(command2[i]);
    }
    free(command2);

    for (int i = 0; command3[i] != NULL; i++) {
        free(command3[i]);
    }
    free(command3);

    free(head->next->next);
    free(head->next);
    free(head);
    
    return 0;
}
