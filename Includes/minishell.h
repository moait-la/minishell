# ifndef MINISHELL_H
# define MINISHELL_H

# define startcolor "\033[1;33m"
# define endcolor   "\033[0m"
# define true 1
# define false 0

# include "../libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_open_fds
{
    int     fd;
    struct  s_open_fds *next;
}   t_open_fds;


typedef	struct s_cmd
{
	int		in;
	int		out;
	char	**command;
	char    **redirections;
    // struct s_open_fds *opened_fds; // to store all opned file descriptors
    struct	s_cmd *next;
}				t_cmd;

typedef struct s_data
{
    char    *line;
    char    **arr;
}       t_data;

// parsing
void	create_array(t_data *data);

// errors
void	ft_allocation_error(void);

//utils
char	*ft_get_path(char *cmd);
int     open_file(char *name, int nbr);

void	ft_execute(t_cmd *cmd, char *input);

void    ft_init_in_out(t_cmd *cmd);
void    ft_init_pipe(t_cmd *cmd, t_open_fds **open_fds);

void	add_to_open_fds(t_open_fds **open_fds, int fd);

#endif