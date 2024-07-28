#include <stdio.h>
#include "Includes/minishell.h"

char	*ft_get_key(char *cmd)
{
	int		i;
	char	*key;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
		{
			key = ft_substr(cmd, 0, i);
			return (key);
		}
		i++;
	}
    return (ft_strdup(cmd));
}

char	*ft_get_value(char *cmd)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = ft_strlen(cmd);
	while (cmd[i])
	{
		if (cmd[i] == '=')
		{
			value = ft_substr(cmd, i + 1, len);
            return (value);
		}
		i++;
	}
	return (NULL);
}

int main()
{
    char *value = ft_get_value("AA=");
    if (*value == '\0')
        printf("'%s'\n", value);
}
