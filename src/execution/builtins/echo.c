/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/21 15:12:49 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args && cmd->args[i] && cmd->args[i][0] != '\0')
		i++;
	return (i);
}

static int32_t	skip_options(t_cmd *cmd)
{
	int	i;
	int	arg_i;

	arg_i = 1;
	if (!cmd->args[arg_i] || !cmd->args[arg_i][0] || cmd->args[arg_i][0] != '-')
		return (1);
	while (cmd->args[arg_i] && cmd->args[arg_i][0] == '-')
	{
		i = 1;
		while (cmd->args[arg_i][i])
		{
			if (cmd->args[arg_i][i] != 'n')
				return (arg_i);
			i++;
		}
		arg_i++;
	}
	return (arg_i);
}

int	echo_cmd(t_cmd *cmd)
{
	int		j;
	int		count;
	bool	has_option_n;

	has_option_n = false;
	count = count_args(cmd);
	if (count == 1)
		return (ft_printf("\n"), 0);
	j = skip_options(cmd);
	if (j > 1)
		has_option_n = true;
	while (j < count)
	{
		ft_printf("%s", cmd->args[j]);
		if (cmd->args[j + 1])
			ft_printf(" ");
		j++;
	}
	if (!has_option_n)
		ft_printf("\n");
	return (0);
}
