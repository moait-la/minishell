# ifndef MINISHELL_H
# define MINISHELL_H

# define startcolor "\033[1;33m"
# define endcolor   "\033[0m"
# define true 1
# define false 0

# include "../libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>

typedef struct s_data
{
    char    *line;
    char    **arr;
}       t_data;

// parsing
void	create_array(t_data *data);

// errors
void	ft_allocation_error(void);

#endif