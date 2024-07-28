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
# include <string.h>
# include <errno.h>

typedef struct s_env {
    char    *key;
    char    *value;
    int     has_printed; // for srted env lst
    int     exported_with_value; // to list in the export 1 lst or env lst 0
    struct s_env *next;
}	t_env;

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
void    ft_init_in_out(t_cmd *cmd);
void    ft_init_pipe(t_cmd *cmd, t_open_fds **open_fds);
void	add_to_open_fds(t_open_fds **open_fds, int fd);
char    *ft_getenv(char *value, t_env *env_lst);
void	ft_error(char *s1, char *s2);

//builtiins
void    ft_echo(t_cmd *cmd);
void	ft_cd(t_cmd *cmd, t_env *env_lst);
void    ft_env(t_env *env);


//env
t_env *parse_envp(char **envp);

// builtins utils
int     is_valid_identifier(char **command);
void	ft_add_env(t_env **env_lst, int exported, char *key, char *value);

#endif