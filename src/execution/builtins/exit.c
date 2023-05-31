/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/31 11:13:16 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_cmd(t_cmd  *cmd)
{
	if (cmd->args[0] == NULL)
		exit(0);
	else if (cmd->args[0] != NULL)
		exit(ft_atoi(cmd->args[0]));
	else
	{
		printf("exit: too many arguments");
		return(1);
	}
}
