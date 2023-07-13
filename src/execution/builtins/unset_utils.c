#include "minishell.h"

void	print_not_valid_identifier(int export_or_unset, char *identifier)
{
	int	i;

	i = -1;
	if (export_or_unset == 0)
	{
		write(2, "Minishell: export: `", 20);
		while (identifier[++i])
			write(2, &identifier[i], 1);
		write(2, ": not a valid identifier\n", 25);
	}
	if (export_or_unset == 1)
	{
		write(2, "Minishell: unset: `", 19);
		while (identifier[++i])
			write(2, &identifier[i], 1);
		write(2, ": not a valid identifier\n", 25);
	}
}

int	is_valid_identifier(char *identifier)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	if (!identifier)
		return (res);
	while (identifier[i])
	{
		if (identifier[0])
			if (ft_isalpha(identifier[0]) == 1 || identifier[0] == '_')
				res = 1;
		if (ft_isalnum(identifier[i]) != 1 && identifier[i] != '_'
			&& identifier[i] != '=' && identifier[i] != '\0')
			res = 0;
		i++;
	}
	return (res);
}

int	is_valid_identifier_unset(char *identifier)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	if (!identifier)
		return (res);
	while (identifier[i])
	{
		if (identifier[0])
			if (ft_isalpha(identifier[0]) == 1 || identifier[0] == '_')
				res = 1;
		if (ft_isalnum(identifier[i]) != 1 && identifier[i] != '_'
			&& identifier[i] != '\0')
			res = 0;
		if (identifier[i] == '=')
		{
			res = 0;
			break ;
		}
		i++;
	}
	return (res);
}
