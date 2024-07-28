#include "../Includes/minishell.h"

void	ft_error(char *s1, char *s2)
{
	ft_putstr_fd("minishell: ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
		ft_putstr_fd(s2, 2);
}

void	ft_allocation_error(void)
{
	printf("Allocation Failed!\n");
	exit(1);
}
