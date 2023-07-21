/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 15:13:01 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_cmd(t_cmd *cmd)
{
	char	args[32];

	if (!cmd->args || cmd->args[0] == NULL)
	{
		free_all();
		exit(0);
	}
	else if (cmd->args && cmd->args[0] != NULL)
	{
		ft_strlcpy(&args[0], cmd->args[0], 32);
		free_all();
		exit(ft_atoi(args));
	}
	else
	{
		ft_printf("exit: too many arguments");
		exit(1);
	}
}
