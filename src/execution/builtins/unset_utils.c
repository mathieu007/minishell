/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 18:37:52 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_not_valid_identifier(int export_or_unset, char *identifier)
{
	int	i;

	i = 0;
	if (export_or_unset == 0)
	{
		write(2, "export: `", 10);
		while (identifier[i])
			write(2, &identifier[i++], 1);
		write_err(1, "': not a valid identifier\n");
	}
	if (export_or_unset == 1)
	{
		write(2, "unset: `", 9);
		while (identifier[i])
			write(2, &identifier[i++], 1);
		write_err(1, "': not a valid identifier\n");
	}
	free(identifier);
}

int	is_valid_identifier(char *identifier)
{
	int	i;

	if (!identifier)
		return (0);
	if (!identifier[0])
		return (0);
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
