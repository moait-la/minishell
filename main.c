#include "./Includes/minishell.h"

void	ft_run(t_data *data)
{
	// data->line = add_spaces(data->line); // later // make sure to remove spaces from start and end of the command

	create_array(data);
	if (!data->arr)
		return(ft_allocation_error());

	// ft_syntax_handle(data->arr);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	while (1)
	{
		data.line = readline(startcolor"minishell » "endcolor);
		if (data.line)
		{
			ft_run(&data);
		}
		else
			break;
	}
	return (0);
}
