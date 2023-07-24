/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 10:35:45 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		write(2, "': not a valid identifier\n", 27);
	}
	if (export_or_unset == 1)
	{
		write(2, "Minishell: unset: `", 20);
		while (identifier[++i])
			write(2, &identifier[i], 1);
		write(2, "': not a valid identifier'\n", 27);
	}
}

int	is_valid_identifier(char *identifier)
{
	int	res;
	int	i;

	res = 0;
	if (!identifier)
		return (res);
	if (identifier[0])
		if (!(ft_isalpha(identifier[0]) == 1 || identifier[0] == '_'))
			return (0);
	i = 1;
	while (identifier[i])
	{
		if (!(ft_isalnum(identifier[i]) == 1 || identifier[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
