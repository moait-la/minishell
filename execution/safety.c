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
        close(fds[0]);
        if (cmd->in != 0)
        {
            dup2(cmd->in, STDIN_FILENO);
            close(cmd->in);
        }
		execve(path, cmd->command, NULL);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
        close(fds[1]);
	}
}

void    ft_last_command(t_cmd *cmd, int childp_nbr)
{
    pid_t   pid;
    int     i;
    char    *path;

    i = 0;
    pid = fork();
    if (pid < 0)
        exit(789);
    if (pid == 0)
    {
        if (cmd->out != 1)
        {
            dup2(cmd->out, STDOUT_FILENO);
            close(cmd->out);
        }
        path = ft_get_path(cmd->command[0]);
        if (execve(path, cmd->command, NULL) == -1)
			exit(645);
    }
    else
    {
        while (i < childp_nbr + 1)
        {
            wait(NULL);
            i++;
        }
    }
}

void	ft_execute(t_cmd *cmd, char *input)
{
	int	nbr_pipes;

	// ----- !! this need to be init for each node
	cmd->in = 0;
	cmd->out = 1;
	// -----
	nbr_pipes = ft_pipes_count(input);
	ft_redirect_in_out_oprtrs(cmd, input);
	int	i = 0; 
	int	childp_count = 0;
	if (nbr_pipes == 0)
	{
		ft_execute_without_pipes(cmd, input);
	}
	else if (nbr_pipes > 0)
	{
		while (i <= nbr_pipes - 1) // -1 to not excute last command
		{

                char *path = ft_get_path(cmd->command[0]);
                ft_child_process(cmd, path);
                free(path);
                i++;
                childp_count++;
                cmd = cmd->next;
		}
	}
    ft_last_command(cmd, childp_count);
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


/*

cmd_1 | cmd_2
built_in | cmd_2
cmd_1 | built_in | cmd_3

in while loop of nbr of pipes <= nbr of pipes - 1
i will create a pipe to communicate between processes
and init stdout of cmd_1 will be pipe[1] 
and stdin of cmd_2 will be pipe[0]
like this :
    cmd->out = pipe[1];
    cmd->next->in = pipe[0];
after that i will call the redirection function to open the file as needed
    if its the first command he will be < or 0
    if its the last command he will be > or 1
after that i will need to create a child process
    here i will check if the command is a built_in or not

after first command is executed
i will go to the next command after pipe
and repeat the process

1 | 2 | 3 | 4


*/