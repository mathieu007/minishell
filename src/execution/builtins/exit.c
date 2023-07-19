/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/16 13:06:25 by math             ###   ########.fr       */
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
		printf("exit: too many arguments");
		exit(1);
	}
}
