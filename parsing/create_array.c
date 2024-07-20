#include "../Includes/minishell.h"

int	skip_word(char *line, int i)
{
	while (line[i] && line[i] != 32 && !(line[i] >= 9 && line[i] <= 13))
	{
		i++;
	}
	return (i);
}

int	find_second_quote(char *line, int i)
{
	char	quote;

	quote = line[i];
	while (line[i])
	{
		i++;
		while (line[i] && line[i] != quote)
			i++;
		if (!line[i])
			return (-1); // means quote not closed
		else
			return (++i);
	}
	return (999);
}

int	words_nbr(char *line)
{
	int	nbr;
	int	i;

    i = 0;
	nbr = 0;
	while (line[i])
	{
        while (line[i] == 32 || (line[i] >= 9 && line[i] <= 13))
			i++;
		if (line[i] && (line[i] == '\"' || line[i] == '\''))
		{
			i = find_second_quote(line, i);
			if (i == -1) // quote is not closed
				return (++nbr);
			nbr++;
		}
        else if (line[i] && line[i] != 32 && !(line[i] >= 9 && line[i] <= 13))
		{
			i = skip_word(line, i);
			nbr++;
		}
	}
	return (nbr);
}

static void	ft_insert_word(t_data *data, int len, int i, int j)
{
	int	k;

	k = 0;
	while (len > 0)
	{
		data->arr[j][k] = data->line[i - len];
		len--;
		k++;
	}
	data->arr[j][k] = '\0';
	printf("[%s]\n", data->arr[j]);
}

static void	ft_get_word_len(t_data *data, int *len, int *i)
{
	int	start;

	start = 0;
	if (data->line[*i] && data->line[*i] == '\'' || data->line[*i] == '\"')
		{
			start = *i;
			*i = find_second_quote(data->line, *i);
			if (*i == -1)
				*i = ft_strlen(data->line);	
			*len = *i - start;
		}
		else if (data->line[*i] && data->line[*i] != 32 && !(data->line[*i] >= 9 && data->line[*i] <= 13)
					&& data->line[*i] != '\'' || data->line[*i] != '\"')
		{
			start = *i;
			*i = skip_word(data->line, *i);
			*len = *i - start;
		}
}

static void	ft_insert(t_data *data)
{
	int	i;
	int	j;
	int	start;
	int	len;

	i = 0;
	j = 0;
	while (data->line[i])
	{
		len = 0;
		while (data->line[i] == 32 || (data->line[i] >= 9 && data->line[i] <= 13))
			i++;
		ft_get_word_len(data, &len, &i);
		data->arr[j] = (char *)malloc(sizeof(char *) * (len + 1));
		if (!data->arr[j])
			return (ft_allocation_error());
			ft_insert_word(data, len, i, j);
		j++;
	}
	data->arr[j] = NULL;
}

void	create_array(t_data *data)
{
	int	nbr;
	int	i;

	nbr = words_nbr(data->line);
	data->arr = (char **)malloc(sizeof(char *) * (nbr + 1));
	if (!data->arr)
		return (ft_allocation_error());
	ft_insert(data);
}
